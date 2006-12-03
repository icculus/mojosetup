-- NOTE: If you care about Unicode, this file _MUST_ be UTF-8 encoded!
--
--  Most of the MojoSetup table isn't set up when this code is run, so you
--  shouldn't rely on any of it. For most purposes, you should treat this
--  file more like a data description language and less like a turing-complete
--  scripting language.
--
-- You should leave the existing strings here. They aren't hurting anything,
--  and most are used by MojoSetup itself. Add your own, though.

MojoSetup.translations = {
    ["README.txt"] = {
        es = "README_spanish.txt";
        l33t = "README_l33t.txt";
    };

    ["Required for play"] = {
        es = "Requerido para el juego";
        l33t = "r3qu1r3d 4 pl4y";
    };

    ["blahblahblah"] = {
        es = "blaho blaho blaho";
        l33t = "bl4hbl4hbl4h";
    };
}

-- end of translations.lua ...

