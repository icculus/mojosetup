-- This is where most of the magic happens. Everything is initialized, and
--  the user's config script has successfully run. This Lua chunk drives the
--  main application code.

-- This is just for convenience.
local _ = MojoSetup.translate

-- This dumps the table built from the user's config script using logdebug,
--  so it only spits out crap if debug-level logging is enabled.
MojoSetup.dumptable("MojoSetup.installs", MojoSetup.installs)


local function do_install(install)
    -- !!! FIXME: try to sanity check everything we can here
    -- !!! FIXME:  (unsupported URLs, bogus media IDs, etc.)
    -- !!! FIXME:  I would like everything possible to fail here instead of
    -- !!! FIXME:  when a user happens to pick an option no one tested...

    -- This is to save us the trouble of a loop every time we have to
    --  find media by id...
    MojoSetup.media = {}
    if install.medias ~= nil then
        for k,v in pairs(install.medias) do
            if MojoSetup.media[v.id] ~= nil then
                MojoSetup.fatal(_("Config bug: duplicate media id"))
            end
            MojoSetup.media[v.id] = v
        end
    end

    -- Build a bunch of functions into a linear array...this lets us move
    --  back and forth between stages of the install with customized functions
    --  for each bit that have their own unique params embedded as upvalues.
    -- So if there are three EULAs to accept, we'll call three variations of
    --  the EULA function with three different tables that appear as local
    --  variables, and the driver that calls this function will be able to
    --  skip back and forth based on user input. This is a cool Lua thing.
    local stages = {}

    -- First stage: Make sure installer can run. Always fails or steps forward.
    if install.precheck ~= nil then
        stages[#stages+1] = function ()
            MojoSetup.gui.precheck()
            local errstr = install.precheck()
            if errstr ~= nil then
                MojoSetup.fatal(errstr)
            end
        end
    end

    -- Next stage: accept all EULAs. Never lets user step back, so they
    --  either accept or reject and go to the next EULA or stage. Rejection
    --  of any EULA is considered fatal.
    for k,eula in pairs(install.eulas) do
        local desc = eula.description
        local fname = eula.source

        -- (desc) and (fname) become an upvalues in this function.
        stages[#stages+1] = function (thisstage, maxstage)
            if not MojoSetup.gui.readme(desc, fname, thisstage, maxstage) then
                return false
            end

            if not MojoSetup.promptyn(desc, _("Accept this license?")) then
                MojoSetup.fatal(_("You must accept the license before you may install"))
            end
            return true
        end
    end

    -- Next stage: show any READMEs.
    for k,readme in pairs(install.readmes) do
        local desc = readme.description
        local fname = readme.source
        -- (desc) and (fname) become upvalues in this function.
        stages[#stages+1] = function(thisstage, maxstage)
            return MojoSetup.gui.readme(desc, fname, thisstage, maxstage)
        end
    end

    -- Next stage: let user choose install destination.
    --  The config file can force a destination if it has a really good reason
    --  (system drivers that have to go in a specific place, for example),
    --  but really really shouldn't in 99% of the cases.
    MojoSetup.destination = install.destination
    if MojoSetup.destination ~= nil then
        MojoSetup.logdebug("Install dest: '" .. install.destination .. "'")
    else
        local recommend = install.recommended_destinations
        -- (recommend) becomes an upvalue in this function.
        stages[#stages+1] = function(thisstage, maxstage)
            local x = MojoSetup.gui.destination(recommend, thisstage, maxstage)
            if x == nil then
                return false   -- go back
            end
            MojoSetup.destination = x
            MojoSetup.logdebug("Install dest: '" .. x .. "'")
            return true
        end
    end

    -- Next stage: let user choose install options.
    if install.options ~= nil or install.optiongroups ~= nil then
        -- (install) becomes an upvalue in this function.
        stages[#stages+1] = function(thisstage, maxstage)
            -- This does some complex stuff with a hierarchy of tables in C.
            return MojoSetup.gui.options(install, thisstage, maxstage)
        end
    end


    -- Next stage: Make sure source list is sane.
    --  This is not a GUI stage, it just needs to run between them.
    --  This gets a little hairy.
    stages[#stages+1] = function(thisstage, maxstage)
        local process_file = function(option, file)
            -- !!! FIXME: do this in schema?
            if type(file.source) == "string" then
                file.source = { file.source }
            end

            -- !!! FIXME: what happens if a file shows up in multiple options?
            for k,v in pairs(file.source) do
                local prot,host,path = string.match(v, "^(.+://)(.-)/(.*)");
                if prot == nil then  -- included content?
                    MojoSetup.sources.included[v] = option
                elseif prot == "media://" then
                    -- !!! FIXME: make sure media id is valid.
                    if MojoSetup.sources.media[host] == nil then
                        MojoSetup.sources.media[host] = {}
                    end
                    MojoSetup.sources.media[host][path] = option
                else
                    -- !!! FIXME: make sure we can handle this URL...
                    MojoSetup.sources.downloads[v] = option
                end
            end
        end

        -- Sort an option into tables that say what sort of install it is.
        --  This lets us batch all the things from one CD together,
        --  do all the downloads first, etc.
        local process_option = function(option)
            -- !!! FIXME: check for nil in schema?
            if option.files ~= nil then
                for k,v in pairs(option.files) do
                    process_file(option, v);
                end
            end
        end

        local build_source_tables = function(opts)
            local options = opts['options']
            if options ~= nil then
                for k,v in pairs(options) do
                    if v.value then
                        process_option(v)
                    end
                end
            end

            local optiongroups = opts['optiongroups']
            if optiongroups ~= nil then
                for k,v in pairs(optiongroups) do
                    if not v.disabled then
                        build_source_tables(optiongroups)
                    end
                end
            end
        end

        MojoSetup.sources = {}
        MojoSetup.sources.downloads = {}
        MojoSetup.sources.media = {}
        MojoSetup.sources.included = {}
        build_source_tables(install)

        -- This dumps the sources tables using logdebug,
        --  so it only spits out crap if debug-level logging is enabled.
        MojoSetup.dumptable("MojoSetup.sources", MojoSetup.sources)

        return true   -- always go forward from non-GUI stage.
    end

    -- Next stage: Download external packages.
    stages[#stages+1] = function(thisstage, maxstage)
--        MojoSetup.gui.startdownload()
-- !!! FIXME
return true
--        MojoSetup.gui.enddownload()
    end

    -- Next stage: actual installation.
    stages[#stages+1] = function(thisstage, maxstage)
        MojoSetup.gui.startinstall()
        -- Do stuff on media first, so the user finds out he's missing
        --  disc 3 of 57 as soon as possible...

        -- Since we sort all things to install by the media that contains
        --  the source data, you should only have to insert each disc
        --  once, no matter how they landed in the config file.

        -- !!! FIXME: sort these so they show up in a reasonable order...
        -- !!! FIXME:  ...disc 1 before disc 2, etc...

        for mediaid,srcs in pairs(MojoSetup.sources.media) do
            local media = MojoSetup.media[mediaid]
            local basepath = MojoSetup.findmedia(media.uniquefile)
            while basepath == nil do
                basepath = MojoSetup.findmedia(media.uniquefile)
                if not MojoSetup.gui.insertmedia(media.description) then
                    MojoSetup.fatal(_("User cancelled."))  -- !!! FIXME: don't like this.
                end
            end

            -- Media is ready, install everything from this one...
            for src,option in pairs(srcs) do
                -- !!! FIXME: write me.   install_file(basepath, src, option)
            end
        end
        MojoSetup.gui.endinstall()
    end

    -- Next stage: show results gui
        -- On failure, back out changes (make this part of fatal()).

    -- Now make all this happen.
    if not MojoSetup.gui.start(install.description, install.splash) then
        MojoSetup.fatal(_("GUI failed to start"))
    end

    -- Make the stages available elsewhere.
    MojoSetup.stages = stages

    MojoSetup.installed_files = {}

    local i = 1
    while MojoSetup.stages[i] ~= nil do
        local stage = MojoSetup.stages[i]
        local go_forward = stage(i, #MojoSetup.stages)
        if go_forward then
            i = i + 1
        else
            if i == 1 then
                MojoSetup.logwarning("Stepped back over start of stages")
                MojoSetup.fatal(_("Internal error"))
            else
                i = i - 1
            end
        end
    end

    -- Don't let future errors delete files from successful installs...
    MojoSetup.installed_files = nil

    MojoSetup.gui.stop()

    -- Done with these things. Make them eligible for garbage collection.
    MojoSetup.stages = nil
    MojoSetup.sources = nil
    MojoSetup.media = nil
    MojoSetup.destination = nil
end

local saw_an_installer = false
for installkey,install in pairs(MojoSetup.installs) do
    saw_an_installer = true
    do_install(install)
    MojoSetup.collectgarbage()  -- nuke all the tables we threw around...
end

if not saw_an_installer then
    MojoSetup.fatal(_("Nothing to do!"))
end

-- end of mojosetup_mainline.lua ...

