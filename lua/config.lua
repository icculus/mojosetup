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

Setup.Install
{
    product = "mygame",
    desc = "My Game",
    version = "1.0",
    splash = "splash.jpg",
    splashpos = "top",
    superuser = false,
    appbundle = true,
    reinstall = true,
    nouninstall = true,

    -- Things named Setup.Something are internal functions we supply.
    --  Generally these return the table you pass to them, but they
    --  may sanitize the values, add defaults, and verify the data.

    -- End User License Agreement(s). You can specify multiple files.
    --  Also, Note the "translate" call.
    Setup.Eula
    {
        name = "My Game License",
        ui_stdio = MojoSetup.translate("MyGame_EULA.txt"),
        ui_gnome = MojoSetup.translate("MyGame_EULA.html"),
        ui_macosx = MojoSetup.translate("MyGame_EULA.html"),
        ui_generic = MojoSetup.translate("MyGame_EULA.mtf"),
    },

    Setup.Eula
    {
        name = "Punkbuster License",
        ui_stdio = MojoSetup.translate("PunkBuster_EULA.txt"),
        ui_gnome = MojoSetup.translate("PunkBuster_EULA.html"),
        ui_macosx = MojoSetup.translate("PunkBuster_EULA.html"),
        ui_generic = MojoSetup.translate("PunkBuster_EULA.mtf"),
    },

    -- README file(s) to show and install.
    Setup.Readme
    {
        name = "My Game README",
        ui_stdio = MojoSetup.translate("README.txt"),
        ui_gnome = MojoSetup.translate("README.html"),
        ui_macosx = MojoSetup.translate("README.html"),
        ui_generic = MojoSetup.translate("README.mtf"),
    },

    -- Specify media (discs) we may need for this install and how to find them.
    Setup.Media
    {
        id = "cd1",
        name = "MyGame CD 1",
        uniquefile = "Sound/blip.wav"
    },

    Setup.Media
    {
        id = "cd2",
        name = "MyGame CD 2",
        uniquefile = "Maps/town.map"
    },

    -- Specify chunks to install...optional or otherwise.
    Setup.Option
    {
        required = true,
        size = "600M",
        description = "Base Install",

        -- File(s) to install.
        Setup.File
        {
            cdromid = "cd1",
            dst = "MyGame/MyGame.app",
            unpackarchives = true,
            src = { "Maps/Maps.zip", "Sounds/*.wav", "Graphics/*" },

            -- You can optionally assign a lua function...we'll call this for
            --  each file to see if we should avoid installing it.
            filter = function(fn) return fn == "Graphics/dontinstall.jpg" end
        },
    },
}

-- end of config.lua ...

