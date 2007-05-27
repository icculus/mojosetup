-- MojoSetup; a portable, flexible installation application.
--
-- Please see the file LICENSE.txt in the source's root directory.
--
--  This file written by Ryan C. Gordon.


-- Compatibility with loki_setup and related tools.
--
-- You can cut-and-paste this file into your config script, or call this at
--  the start of it:  MojoSetup.runfile("loki_setup_compat")
-- This code is optional, and not otherwise referenced by MojoSetup. It could
--  be useful for those that need to integrate with loki_update, etc.
--
-- Using this code requires you to build with the Lua "io" runtime library.
--  (for now, at least).

MojoSetup.Loki = {}


-- !!! FIXME: some of these are currently-undocumented MojoSetup.* methods...

function MojoSetup.Loki.manifest(update_url)
    local install = MojoSetup.install
    local manifestdir = MojoSetup.destination .. "/.manifest"
    MojoSetup.installed_files[#MojoSetup.installed_files+1] = manifestdir
    if not MojoSetup.platform.mkdir(dest, perms) then
        -- !!! FIXME: formatting
        MojoSetup.logerror("Failed to create dir '" .. manifestdir .. "'")
        MojoSetup.fatal(_("mkdir failed"))
    end

    local manifestfile = manifestdir .. "/" .. install.id .. ".xml"
    MojoSetup.installed_files[#MojoSetup.installed_files+1] = manifestfile
    local f, err = io.open(manifestfile, "a")
    if f == nil then
        MojoSetup.logerror("Couldn't create manifest: " .. err)
        MojoSetup.fatal(_("Couldn't create manifest"))
    end

    if update_url ~= nil then
        update_url = 'update_url="' .. update_url .. '"'
    else
        update_url = ''
    end

    -- !!! FIXME: check i/o errors
    f:write('<?xml version="1.0" encoding="UTF-8"?>\n')
    f:write('<product name="' .. install.id .. '" desc="' ..
            install.description .. '" xmlversion="1.6" root="'
            .. MojoSetup.destination .. '" ' .. update_url .. '>\n');
    f:write('  <component name="Default" version="' .. install.version ..
            '" default="yes">\n')

    local destlen = string.length(MojoSetup.destination) + 2
    for option,items in pairs(MojoSetup.manifest) do
        f:write('    <option name="' .. option.description .. '">\n')
        for i,item in ipairs(items) do
            local type = item.type
            if type == "dir" then
                type = "directory"
            end

            local path = item.path
            if path ~= nil then
                path = string.sub(path, destlen)  -- make it relative.
            end

            local xml = '      <' .. type

            if type == "file" then
                for k,v in pairs(item.checksums) do
                    xml = xml .. ' ' .. k .. '="' .. v .. '"'
                end
                xml = xml .. ' mode="' .. item.mode .. '"'
            elseif type == "directory" then
                xml = xml .. ' mode="' .. item.mode .. '"'
            elseif type == "symlink" then
                xml = xml .. ' dest="' .. item.linkdest .. '" mode="0777"'
            end

            xml = xml .. '>' .. path .. "</" .. type .. ">\n"
            f:write(xml)
        end
    end

    f:write('  </component>\n')
    f:write('</product>\n\n')

    -- !!! FIXME: check i/o errors
    f:close()
end

-- end of loki_setup_compat.lua ...

