-- MojoSetup; a portable, flexible installation application.
--
-- Please see the file LICENSE.txt in the source's root directory.
--
--  This file written by Ryan C. Gordon.

-- Lines starting with "--" are comments in this file.
--
-- NOTE: If you care about Unicode or ASCII chars above 127, this file _MUST_
--  be UTF-8 encoded! If you think you're using a certain high-ascii codepage,
--  you're wrong!
--
-- Most of the MojoSetup table isn't set up when this code is run, so you
--  shouldn't rely on any of it. For most purposes, you should treat this
--  file more like a data description language and less like a turing-complete
--  scripting language.
--
-- The format of an entry looks like this:
--
--  ["Hello"] = {
--    es = "Hola",
--    de = "Hallo",
--    fr = "Bonjour",
--  };
--
-- So you just fill in the translation of the English for your language code.
--  Note that locales work, too:
--
--  ["Color"] = {
--    en_GB = "Colour",
--  };
--
-- Specific locales are favored, falling back to specific languages, eventually
--  ending up on the untranslated version (which is technically en_US).
--
-- Whenever you see a %x sequence, that is replaced with a string at runtime.
--  So if you see, ["Hello, %0, my name is %1."], then this might become
--  "Hello, Alice, my name is Bob." at runtime. If your culture would find
--  introducing yourself second to be rude, you might translate this to:
--  "My name is %1, hello %0." If you need a literal '%' char, write "%%":
--  "Operation is %0%% complete" might give "Operation is 3% complete."
--  All strings, from your locale or otherwise, are checked for formatter
--  correctness at startup. This is to prevent the installer working fine
--  in all reasonable tests, then finding out that one guy in Ghana has a
--  crashing installer because his localization forgot to add a %1 somewhere.
--
-- You should leave the existing strings here. They aren't hurting anything,
--  and most are used by MojoSetup itself. Add your own, if needed, though.
--
-- The table you create here goes away shortly after creation, as the relevant
--  parts of it get moved somewhere else. You should call MojoSetup.translate()
--  to get the proper translation for a given string.
--
-- Questions about the intent of a specific string can go to Ryan C. Gordon
--  (icculus@icculus.org).

MojoSetup.localization = {
    -- zlib error message
    ["need dictionary"] = {
    };

    -- zlib error message
    ["data error"] = {
    };

    -- zlib error message
    ["memory error"] = {
    };

    -- zlib error message
    ["buffer error"] = {
    };

    -- zlib error message
    ["version error"] = {
    };

    -- zlib error message
    ["unknown error"] = {
    };

    -- stdio GUI plugin says this for msgboxes.
    ["NOTICE: %0\n[hit enter]"] = {
    };

    -- stdio GUI plugin says this for yes/no prompts that default to yes.
    ["%0\n[Y/n]: "] = {
    };

    -- stdio GUI plugin says this for yes/no prompts that default to no.
    ["%0\n[y/N]: "] = {
    };

    -- stdio GUI plugin says this for yes/no/always/never prompts.
    ["%0\n[y/n/Always/Never]: "] = {
    };

    -- This is utf8casecmp()'d for "yes" answers in stdio GUI's promptyn().
    ["Y"] = {
    };

    -- This is utf8casecmp()'d for "no" answers in stdio GUI's promptyn().
    ["N"] = {
    };

    -- This is shown when using stdio GUI's built-in README pager (printf format).
    ["(%0-%1 of %2 lines, see more?)"] = {
    };

    -- This is utf8casecmp()'d for "always" answers in stdio GUI's promptyn().
    ["Always"] = {
    };

    -- This is utf8casecmp()'d for "never" answers in stdio GUI's promptyn().
    ["Never"] = {
    };

    ["Type '%0' to go back."] = {
    };

    -- This is the string used for the '%0' in the above string.
    ["back"] = {
    };

    -- This is the prompt in the stdio driver when user input is expected.
    ["> "] = {
    };

    ["NOTICE: %0\n[hit enter]"] = {
    };

    -- That's meant to be the name of an item (%0) and the percent done (%1).
    ["%0: %1%%"] = {
    };

    ["%0 (total progress: %1%%)\n"] = {
    };

    ["Accept this license?"] = {
    };

    -- This is a GTK+ button label. The '_' comes after the hotkey character.
    ["_Always"] = {
    };

    ["Archive not found."] = {
    };

    ["Are you sure you want to cancel installation?"] = {
    };

    ["Back"] = {
    };

    -- This is a GTK+ button label. The '_' comes after the hotkey character.
    ["B_rowse..."] = {
    };

    -- "bytes per second"
    ["B/s"] = {
    };

    ["BUG: '%0' is not a valid permission string"] = {
    };

    ["BUG: Invalid format() string"] = {
    };

    ["BUG: stage returned wrong type."] = {
    };

    ["BUG: stage returned wrong value."] = {
    };

    ["BUG: stepped back over start of stages"] = {
    };

    ["BUG: Unhandled data type"] = {
    };

    -- "tar" is a proper name in this case.
    ["BUG: Can't duplicate tar inputs"] = {
    };

    -- Buggy config elements:
    -- This is supposed to be a config element ($0) and something that's wrong
    --  with it ($1), such as "BUG: Config Package::description not a string"
    -- The grammar can be imperfect here; this is a developer error, not an
    --  end-user error, so we haven't made this very flexible.
    ["BUG: Config $0 $1."] = {
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be explicitly specified"] = {
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be string or table of strings"] = {
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be a string or number"] = {
    };

    -- This is an error string for a buggy config element. See notes above.
    ["can't be empty string"] = {
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have protocol"] = {
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have host"] = {
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have path"] = {
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL protocol is unsupported"] = {
    };

    -- This is an error string for a buggy config element. See notes above.
    ["Permission string is invalid"] = {
    };

    -- This is an error string for a buggy config element. See notes above.
    ["is not a valid property"] = {
    };

    -- This is an error string for a buggy config element. See notes above.
    --  $0 is a data type name (string, number, table, etc).
    ["must be $0"] = {
    };

    -- Data types for "must be $0" above...
    ["string"] = {
    };

    ["boolean"] = {
    };

    ["number"] = {
    };

    ["function"] = {
    };

    ["table"] = {
    };



    ["bzlib triggered an internal error: %0"] = {
    };

    ["Cancel"] = {
    };

    ["Cancel installation"] = {
    };

    ["Can't enumerate archive"] = {
    };

    ["Can't open archive."] = {
    };

    ["Choose install destination by number (hit enter for #1), or enter your own."] = {
    };

    ["Choose number to change."] = {
    };

    ["Config bug: duplicate media id"] = {
    };

    ["Config bug: no options"] = {
    };

    -- As in "two different files want to use the same name"
    ["Conflict!"] = {
    };

    ["Couldn't backup file for rollback"] = {
    };

    ["Couldn't create manifest"] = {
    };

    ["Couldn't enumerate archive."] = {
    };

    ["Couldn't open archive."] = {
    };

    ["Couldn't restore some files. Your existing installation is likely damaged."] = {
    };

    ["Deletion failed!"] = {
    };

    ["Destination"] = {
    };

    ["Downloading"] = {
    };

    ["Enter path where files will be installed."] = {
    };

    ["failed to load file '%0'"] = {
    };

    ["Fatal error"] = {
    };

    ["File creation failed!"] = {
    };

    ["File download failed!"] = {
    };

    ["File '$0' already exists! Replace?"] = {
    };

    ["Finish"] = {
    };

    ["GUI failed to start"] = {
    };

    ["Incomplete installation. We will revert any changes we made."] = {
    };

    ["Installation location"] = {
    };

    ["Installation was successful."] = {
    };

    ["Installing"] = {
    };

    ["Install options:"] = {
    };

    ["(I want to specify a path.)"] = {
    };

    -- "kilobytes per second"
    ["KB/s"] = {
    };

    -- Download rate when we don't know the goal (can't report time left).
    -- This is a number ($0) followed by the localized "KB/s" or "B/s" ($1).
    ["$0 $1"] = {
    };

    -- Download rate when we know the goal (can report time left).
    -- This is a number ($0) followed by the localized "KB/s" or "B/s" ($1),
    --  then the hours ($2), minutes ($3), and seconds ($4) remaining
    ["$0 $1, $2:$3:$4 remaining"] = {
    };

    -- download rate when download isn't progressing at all.
    ["stalled"] = {
    };

    -- Download progress string: filename ($0), percent downloaded ($1),
    --  download rate determined in one of the above strings ($2).
    ["$0: $1%% ($2)"] = {
    };

    ["Media change"] = {
    };

    ["Directory creation failed"] = {
    };

    ["need dictionary"] = {
    };

    -- This is a GTK+ button label. The '_' comes after the hotkey character.
    ["N_ever"] = {
    };

    ["Next"] = {
    };

    ["No"] = {
    };

    ["No networking support in this build."] = {
    };

    ["Not Found"] = {
    };

    ["Nothing to do!"] = {
    };

    ["OK"] = {
    };

    ["Options"] = {
    };

    ["PANIC"] = {
    };

    ["Please insert '%0'"] = {
    };

    ["Press enter to continue."] = {
    };

    ["Serious problem"] = {
    };

    ["Shutting down..."] = {
    };

    ["Symlink creation failed!"] = {
    };

    ["The installer has been stopped by the system."] = {
    };

    ["The installer has crashed due to a bug."] = {
    };

    -- This is a button label in the ncurses ui.
    ["Toggle"] = {
    };

    ["Unknown file type in archive"] = {
    };

    ["Yes"] = {
    };

    ["You must accept the license before you may install"] = {
    };

    ["Metadata"] = {
    };
};

-- end of localization.lua ...

