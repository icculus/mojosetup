require("schema");

-- This is a setup file. Lines that start with "--" are comments.
--  This config file is actually just lua code. Even though you'll only
--  need a small subset of the language, there's a lot of flexibility
--  available to you if you need it.   http://www.lua.org/
--
-- Case is significant! Functions we supply are CamelCased, everything
--  else should be lowercased.
--
-- You can use lua as a turing-complete scripting language right inside
--  the config file...here's an example of localizing your install for
--  english (the default), Spanish, and "l33t speak". We set up a table
--  with the translations, and a function called "translate" that we can
--  call at runtime to produce the right information.

translations = {
    es = {
        ["README.txt"] = "README_spanish.txt";
        ["Required for play"] = "Requerido para el juego";
        ["blahblahblah"] = "blaho blaho blaho";
    };

    l33t = {
        ["README.txt"] = "README_l33t.txt";
        ["Required for play"] = "r3qu1r3d 4 pl4y";
        ["blahblahblah"] = "bl4hbl4hbl4h";
    };
}

translation = translations[GetLanguage()]
function translate(origStr)
    return (translation and translation[origStr]) or origStr
end




-- So here's the actual configuration...we used loki_setup's xml schema
--  as a rough guideline.

Install
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
    Eula { keepdirs = true, filename = "MyGame/MyGame_EULA.txt" };
    Eula { keepdirs = true, filename = "MyGame/PunkBuster_EULA.txt" };

    -- README file(s) to show and install. Note the "translate" call.
    Readme { filename = translate("README.txt") };

    -- Specify media (discs) we may need for this install and how to find them.
    Media { id = "cd1", name = "MyGame CD 1", uniquefile = "Sound/blip.wav" };
    Media { id = "cd2", name = "MyGame CD 2", uniquefile = "Maps/town.map" };

    -- Specify chunks to install...optional or otherwise.
    Option
    {
        required = true;
        size = "600M";
        description = "Base Install";

        -- File(s) to install.
        File
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

print("_installs = {")
    _dumptable(_installs,1)
print("}")

-- end of setup.lua ...

