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
      nb = "trenger ordbok",
    };

    -- zlib error message
    ["data error"] = {
      nb = "datafeil",
    };

    -- zlib error message
    ["memory error"] = {
      nb = "minnefeil",
    };

    -- zlib error message
    ["buffer error"] = {
      nb = "bufferfeil",
    };

    -- zlib error message
    ["version error"] = {
      nb = "versjonsfeil",
    };

    -- zlib error message
    ["unknown error"] = {
      nb = "ukjent feil",
    };

    -- stdio GUI plugin says this for msgboxes.
    ["NOTICE: %0\n[hit enter]"] = {
      nb = "NB: %0\n[trykk enter]",
    };

    -- stdio GUI plugin says this for yes/no prompts that default to yes.
    ["%0\n[Y/n]: "] = {
      nb = "%0\n[J/n]: ",
    };

    -- stdio GUI plugin says this for yes/no prompts that default to no.
    ["%0\n[y/N]: "] = {
      nb = "%0\n[j/N]: ",
    };

    -- stdio GUI plugin says this for yes/no/always/never prompts.
    ["%0\n[y/n/Always/Never]: "] = {
      nb = "%0\n[j/n/Alltid/Aldri]: ",
    };

    -- This is utf8casecmp()'d for "yes" answers in stdio GUI's promptyn().
    ["Y"] = {
      nb = "J",
    };

    -- This is utf8casecmp()'d for "no" answers in stdio GUI's promptyn().
    ["N"] = {
      nb = "N",
    };

    -- This is shown when using stdio GUI's built-in README pager (printf format).
    ["(%0-%1 of %2 lines, see more?)"] = {
      nb = "(%0-%1 av %2 linjer, se mer?)",
    };

    -- This is utf8casecmp()'d for "always" answers in stdio GUI's promptyn().
    ["Always"] = {
      nb = "Alltid",
    };

    -- This is utf8casecmp()'d for "never" answers in stdio GUI's promptyn().
    ["Never"] = {
      nb = "Aldri",
    };

    ["Type '%0' to go back."] = {
      nb = "Skriv '%0' for å gå tilbake.",
    };

    -- This is the string used for the '%0' in the above string.
    ["back"] = {
      nb = "tilbake",
    };

    -- This is the prompt in the stdio driver when user input is expected.
    ["> "] = {
    };

    ["NOTICE: %0\n[hit enter]"] = {
      nb = "NB: %0\n[trykk enter]", --DUP?
    };

    -- That's meant to be the name of an item (%0) and the percent done (%1).
    ["%0: %1%%"] = {
      nb = "%0: %1%%",
    };

    ["%0 (total progress: %1%%)\n"] = {
      nb = "%0 (totalt: %1%%)\n",
    };

    ["Accept this license?"] = {
      nb = "Akseptere denne lisensen?",
    };

    -- This is a GTK+ button label. The '_' comes after the hotkey character.
    ["_Always"] = {
      nb = "_Alltid",
    };

    ["Archive not found."] = {
      nb = "Fant ikke arkiv.",
    };

    ["Are you sure you want to cancel installation?"] = {
      nb = "Er du sikker på at du vil avbryte installasjonen?",
    };

    ["Back"] = {
      nb = "Tilbake",
    };

    -- This is a GTK+ button label. The '_' comes after the hotkey character.
    ["B_rowse..."] = {
      nb = "B_la gjennom...",
    };

    -- "bytes per second"
    ["B/s"] = {
    };

    ["BUG: '%0' is not a valid permission string"] = {
      nb = "FEIL: '%0' er ikke en gyldig rettighetsstreng",
    };

    ["BUG: Invalid format() string"] = {
      nb = "FEIL: Ugyldig format()-streng",
    };

    ["BUG: stage returned wrong type."] = {
      nb = "FEIL: nivå returnerte feil type.",
    };

    ["BUG: stage returned wrong value."] = {
      nb = "FEIL: nivå returnerte feil verdi.",
    };

    ["BUG: stepped back over start of stages"] = {
      nb = "FEIL: Gikk tilbake forbi startnivå",
    };

    ["BUG: Unhandled data type"] = {
      nb = "FEIL: Uhåndtert datatype",
    };

    -- "tar" is a proper name in this case.
    ["BUG: Can't duplicate tar inputs"] = {
      nb = "FEIL: Kan ikke duplisere innfiler for tar",
    };

    -- Buggy config elements:
    -- This is supposed to be a config element ($0) and something that's wrong
    --  with it ($1), such as "BUG: Config Package::description not a string"
    -- The grammar can be imperfect here; this is a developer error, not an
    --  end-user error, so we haven't made this very flexible.
    ["BUG: Config $0 $1."] = {
      nb = "FEIL: Konfigurasjon $0 $1.",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be explicitly specified"] = {
      nb = "må gis eksplisitt",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be string or table of strings"] = {
      nb = "må være streng eller tabell av strenger",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be a string or number"] = {
      nb = "må være streng eller nummer",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["can't be empty string"] = {
      nb = "kan ikke være tom streng",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have protocol"] = {
      nb = "URL har ikke protokoll",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have host"] = {
      nb = "URL har ikke vert",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have path"] = {
      nb = "URL har ikke sti",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL protocol is unsupported"] = {
      nb = "URL-protokoll er ikke støttet",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["Permission string is invalid"] = {
      nb = "Rettighetsstreng er ugyldig",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["is not a valid property"] = {
      nb = "er ikke en gyldig egenskap",
    };

    -- This is an error string for a buggy config element. See notes above.
    --  $0 is a data type name (string, number, table, etc).
    ["must be $0"] = {
      nb = "må være $0",
    };

    -- Data types for "must be $0" above...
    ["string"] = {
      nb = "streng",
    };

    ["boolean"] = {
      nb = "boolsk verdi",
    };

    ["number"] = {
      nb = "nummer",
    };

    ["function"] = {
      nb = "funksjon",
    };

    ["table"] = {
      nb = "tabell",
    };



    ["bzlib triggered an internal error: %0"] = {
      nb = "intern feil i bzlib: %0",
    };

    ["Cancel"] = {
      nb = "Avbryt",
    };

    ["Cancel installation"] = {
      nb = "Avbryt installasjonen",
    };

    ["Can't enumerate archive"] = {
      nb = "Kan ikke enumerere arkiv",
    };

    ["Can't open archive."] = {
      nb = "Kan ikke åpne arkiv.",
    };

    ["Choose install destination by number (hit enter for #1), or enter your own."] = {
      nb = "Velg installasjonssti etter nummer (trykk enter for #1), eller skriv din egen.",
    };

    ["Choose number to change."] = {
      nb = "Velg nummer som skal endres.",
    };

    ["Config bug: duplicate media id"] = {
      nb = "Konfigurasjonsfeil: duplisert media-id",
    };

    ["Config bug: no options"] = {
      nb = "Konfigurasjonsfeil: ingen valg",
    };

    -- As in "two different files want to use the same name"
    ["Conflict!"] = {
      nb = "Konflikt!",
    };

    ["Couldn't backup file for rollback"] = {
      nb = "Kunne ikke sikkerhetskopiere fil for tilbakerulling",
    };

    ["Couldn't create manifest"] = {
      nb = "Kunne ikke lage manifest",
    };

    ["Couldn't enumerate archive."] = {
      nb = "Kunne ikke enumerere arkiv.",
    };

    ["Couldn't open archive."] = {
      nb = "Kunne ikke åpne arkiv.",
    };

    ["Couldn't restore some files. Your existing installation is likely damaged."] = {
      nb = "Noen filer kunne ikke tilbakestilles. Den eksisterende installasjonen er sannsynligvis skadet.",
    };

    ["Deletion failed!"] = {
      nb = "Kunne ikke slette!",
    };

    ["Destination"] = {
      nb = "Destinasjon",
    };

    ["Downloading"] = {
      nb = "Laster ned",
    };

    ["Enter path where files will be installed."] = {
      nb = "Skriv inn destinasjonssti for installasjonen.",
    };

    ["failed to load file '%0'"] = {
      nb = "kunne ikke laste fil '%0'",
    };

    ["Fatal error"] = {
      nb = "Fatal feil",
    };

    ["File creation failed!"] = {
      nb = "Kunne ikke lage fil!",
    };

    ["File download failed!"] = {
      nb = "Kunne ikke laste ned fil!",
    };

    ["File '$0' already exists! Replace?"] = {
      nb = "Filen '$0' eksisterer allerede! Skrive over?",
    };

    ["Finish"] = {
      nb = "Ferdig",
    };

    ["GUI failed to start"] = {
      nb = "Kunne ikke starte grafisk grensesnitt",
    };

    ["Incomplete installation. We will revert any changes we made."] = {
      nb = "Installasjonen ble ikke ferdig. Vi vil tilbakestille alle endringer som ble gjort.",
    };

    ["Installation location"] = {
      nb = "Installasjonssti",
    };

    ["Installation was successful."] = {
      nb = "Installasjonen var en suksess.",
    };

    ["Installing"] = {
      nb = "Installerer",
    };

    ["Install options:"] = {
      nb = "Installasjonsvalg:",
    };

    ["(I want to specify a path.)"] = {
      nb = "(Jeg vil skrive min egen sti.)",
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
      nb = "$0 $1, $2:$3:$4 igjen",
    };

    -- download rate when download isn't progressing at all.
    ["stalled"] = {
      nb = "står fast",
    };

    -- Download progress string: filename ($0), percent downloaded ($1),
    --  download rate determined in one of the above strings ($2).
    ["$0: $1%% ($2)"] = {
    };

    ["Media change"] = {
      nb = "Mediaendring",
    };

    ["Directory creation failed"] = {
      nb = "Kunne ikke lage katalog",
    };

    ["need dictionary"] = {
      nb = "trenger ordbok",
    };

    -- This is a GTK+ button label. The '_' comes after the hotkey character.
    ["N_ever"] = {
      nb = "Al_dri",
    };

    ["Next"] = {
      nb = "Neste",
    };

    ["No"] = {
      nb = "Nei",
    };

    ["No networking support in this build."] = {
      nb = "Nettverksstøtte er ikke aktivert i denne versjonen.",
    };

    ["Not Found"] = {
      nb = "Ikke funnet",
    };

    ["Nothing to do!"] = {
      nb = "Ingenting å gjøre!",
    };

    ["OK"] = {
      nb = "OK",
    };

    ["Options"] = {
      nb = "Valg",
    };

    ["PANIC"] = {
      nb = "PANIKK",
    };

    ["Please insert '%0'"] = {
      nb = "Sett inn '%0'",
    };

    ["Press enter to continue."] = {
      nb = "Trykk enter for å fortsette.",
    };

    ["Serious problem"] = {
      nb = "Alvorlig problem",
    };

    ["Shutting down..."] = {
      nb = "Avslutter...",
    };

    ["Symlink creation failed!"] = {
      nb = "Kunne likke lage symbolsk lenke!",
    };

    ["The installer has been stopped by the system."] = {
      nb = "Installasjonsprogrammet ble stoppet av systemet.",
    };

    ["The installer has crashed due to a bug."] = {
      nb = "Installasjonsprogrammet kræsjet pga. en feil.",
    };

    -- This is a button label in the ncurses ui.
    ["Toggle"] = {
      nb = "Inverter valg",
    };

    ["Unknown file type in archive"] = {
      nb = "Ukjent filtype i arkivet",
    };

    ["Yes"] = {
      nb = "Ja",
    };

    ["You must accept the license before you may install"] = {
      nb = "Lisensen må godkjennes før du kan installere",
    };

    ["Metadata"] = {
      nb ="Metadata",
    };
};

-- end of localization.lua ...

