-- This is a setup file. Lines that start with "--" are comments.
--  This config file is actually just Lua code. Even though you'll only
--  need a small subset of the language, there's a lot of flexibility
--  available to you if you need it.   http://www.lua.org/
--
-- All functionality supplied by the installer is encapsulated in either the
--  "Setup" or "MojoSetup" table, so you can use any other symbol name without
--  namespace clashes, assuming it's not a Lua keyword or a symbol supplied
--  by the standard lua libraries.
--
-- So here's the actual configuration...we used loki_setup's xml schema
--  as a rough guideline.

-- You can define functions like in any other Lua script. For example, here's
--  a function that figures out how many bytes are in a megabyte, so you don't
--  have to list exact values in Setup.Option's "bytes" attribute.
local function megabytes(num)
    return num * 1024 * 1024
end

-- And naturally, functions can build on others. It's a programming language,
--  after all! But if you don't want to screw with programming, you can treat
--  this strictly as a config file. This just gives you flexibility if you
--  need it.
local function gigabytes(num)
    return megabytes(num) * 1024
end

Setup.Package
{
    id = "mygame",
    description = "My Game",
    version = "1.0",
    splash = "splash.jpg",
    superuser = false,
    destination = "/usr/local/bin",
    recommended_destinations = { "/opt/games", "/usr/local/games" },

    -- Things named Setup.Something are internal functions we supply.
    --  Generally these return the table you pass to them, but they
    --  may sanitize the values, add defaults, and verify the data.

    -- End User License Agreement(s). You can specify multiple files.
    --  Also, Note the "translate" call.
    Setup.Eula
    {
        description = "My Game License",
        source = MojoSetup.translate("MyGame_EULA.html")
    },

    Setup.Eula
    {
        description = "Punkbuster License",
        source = MojoSetup.translate("PunkBuster_EULA.html"),
    },

    -- README file(s) to show and install.
    Setup.Readme
    {
        description = "My Game README",
        source = MojoSetup.translate("README.html"),
    },

    -- Specify media (discs) we may need for this install and how to find them.
    Setup.Media
    {
        id = "cd1",
        description = "MyGame CD 1",
        uniquefile = "Sound/blip.wav"
    },

    Setup.Media
    {
        id = "cd2",
        description = "MyGame CD 2",
        uniquefile = "Maps/town.map"
    },

    -- Specify chunks to install...optional or otherwise.
    Setup.Option
    {
        value = true,
        required = true,
        disabled = false,
        bytes = megabytes(600),
        description = "Base Install",

        -- File(s) to install.
        Setup.File
        {
            -- source can be a directory, an archive, or a supported URL.
            --  You can use "media://" to get data from a disc that the user
            --  will be prompted to insert. Everything in the source will
            --  be installed, but the "wildcards" and "filter" attributes
            --  can be used to cull the archive's contents.
            source = "media://cd1/Maps/m.zip",

            -- This is a directory where files will be installed from this
            --  source. If this isn't specified, the directory tree structure
            --  in the source will be recreated for the installation.
            -- You can change the destination on a per-file basis using
            --  the filter attribute instead. It overrides this attribute,
            --  but the parameter passed to the filter will use this value
            --  if it exists.
            destination = "MyGame/MyGame.app",

            -- Files in here need to match at least one wildcard to be
            --  installed. If they pass here, they go to the filter function.
            -- This can be a single string or a table of strings.
            wildcards = { "Single/*/*.map", "Multi/*/*.map" },

            -- You can optionally assign a lua function...we'll call this for
            --  each file to see if we should avoid installing it. It returns
            --  nil to drop the file from the install list, or a string of
            --  the new install destination...you can return the original
            --  string to just pass it through for installation, or
            --  use this opportunity to rename a file on the fly.
            filter = function(fn)
                if fn == "Single/x/dontinstall.map" then return nil end
                return fn
            end
        },

        -- Radio buttons.
        Setup.OptionGroup
        {
            disabled = false,
            description = "Language",
            Setup.Option
            {
                value = string.match(MojoSetup.info.locale, "^en_") ~= nil,
                bytes = megabytes(10),
                description = "English",
                Setup.File { source = "base:///Lang/English.zip" },
            },
            Setup.Option
            {
                value = string.match(MojoSetup.info.locale, "^fr_") ~= nil,
                bytes = megabytes(10),
                description = "French",
                Setup.File { source = "base:///Lang/French.zip" },
            },
            Setup.Option
            {
                value = string.match(MojoSetup.info.locale, "^de_") ~= nil,
                bytes = megabytes(10),
                description = "German",
                Setup.File { source = "base:///Lang/German.zip" },
            },
        },
    },

    Setup.Option
    {
        value = true,
        required = false,
        disabled = false,
        bytes = 19384292,
        description = "Downloadable extras",

        -- File(s) to install.
        Setup.File
        {
            destination = "MyGame/MyGame.app",
            source = "http://hostname.dom/extras/extras.zip",
        },
    },
}

-- end of config.lua ...

