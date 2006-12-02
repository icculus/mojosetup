require("schema");

-- This is a setup file. Lines that start with "--" are comments.
--  This config file is actually just lua code. Even though you'll only
--  need a small subset of the language, there's a lot of flexibility
--  available to you if you need it.   http://www.lua.org/
--
-- All functionality supplied by the installer is encapsulated in the
--  "MojoSetup" table, so you can use any other symbol name without
--  clashes.
--
-- So here's the actual configuration...we used loki_setup's xml schema
--  as a rough guideline.

MojoSetup.Install
{
    product = "mygame";
    desc = "My Game";
    version = "1.0";
    splash = "splash.jpg";
    splashpos = "top";
    superuser = false;
    appbundle = true;
    reinstall = true;
    nouninstall = true;

    -- Things that are Capitalized are internal functions we supply.
    --  Generally these return the table you pass to them, but they
    --  may sanitize the values, add defaults, and verify the data.

    -- End User License Agreement(s). You can specify multiple files.
    MojoSetup.Eula { keepdirs = true, filename = "MyGame/MyGame_EULA.txt" };
    MojoSetup.Eula { keepdirs = true, filename = "MyGame/PunkBuster_EULA.txt" };

    -- README file(s) to show and install. Note the "translate" call.
    MojoSetup.Readme { filename = MojoSetup.translate("README.txt") };

    -- Specify media (discs) we may need for this install and how to find them.
    MojoSetup.Media { id = "cd1", name = "MyGame CD 1", uniquefile = "Sound/blip.wav" };
    MojoSetup.Media { id = "cd2", name = "MyGame CD 2", uniquefile = "Maps/town.map" };

    -- Specify chunks to install...optional or otherwise.
    MojoSetup.Option
    {
        required = true;
        size = "600M";
        description = "Base Install";

        -- File(s) to install.
        MojoSetup.File
        {
            cdromid = "cd1";
            dst = "MyGame/MyGame.app";
            unpackarchives = true;
            src = { "Maps/Maps.zip", "Sounds/*.wav", "Graphics/*" };

            -- You can optionally assign a lua function...we'll call this for
            --  each file to see if we should avoid installing it.
            filter = function(fn) return fn == "Graphics/dontinstall.jpg" end;
        };
    };
};

-- dumptable("_installs", MojoSetup.installs);

-- end of setup.lua ...

