-- MojoSetup; a portable, flexible installation application.
--
-- Please see the file LICENSE.txt in the source's root directory.
--
--  This file written by Ryan C. Gordon.

-- Lines starting with "--" are comments in this file.
--
-- You should add your installer's strings to app_localization.lua, not this
--  file. This file is for strings internal to MojoSetup, and we change it all
--  the time. Your app can override any individual string in this file, though.
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
-- Occasionally you might see a "\n" ... that's a newline character. "\t" is
--  a tab character, and "\\" turns into a single "\" character.
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
        de = "Wörterbuch benötigt",
        nb = "trenger ordbok",
        se = "behöver ordbok",
    };

    -- zlib error message
    ["data error"] = {
        de = "Datenfehler",
        nb = "datafeil",
        se = "datafel",
    };

    -- zlib error message
    ["memory error"] = {
        de = "Speicherfehler",
        nb = "minnefeil",
        se = "minnesfel",
    };

    -- zlib error message
    ["buffer error"] = {
        de = "Pufferfehler",
        nb = "bufferfeil",
        se = "bufferfel",
    };

    -- zlib error message
    ["version error"] = {
        de = "Versionsfehler",
        nb = "versjonsfeil",
        se = "versionsfel",
    };

    -- zlib error message
    ["unknown error"] = {
        de = "Unbekannter Fehler",
        nb = "ukjent feil",
        se = "okänt fel",
    };

    -- stdio UI plugin says this for "OK"-only msgboxes. "%0" is the message
    --  box's text content.
    ["NOTICE: %0\n[hit enter]"] = {
        de = "HINWEIS: %0\n[Drücken Sie Enter]",
        nb = "NB: %0\n[trykk enter]",
        se = "NB: %0\n[tryck enter]",
    };

    -- stdio UI plugin says this for yes/no prompts that default to yes.
    -- "%0" is the question the user is being asked to respond to.
    ["%0\n[Y/n]: "] = {
        de = "%0\n[J/n]",
        nb = "%0\n[J/n]: ",
        se = "%0\n[J/n]: ",
    };

    -- stdio UI plugin says this for yes/no prompts that default to no.
    -- "%0" is the question the user is being asked to respond to.
    ["%0\n[y/N]: "] = {
        de = "%0\n[j/N]",
        nb = "%0\n[j/N]: ",
        se = "%0\n[j/N]: ",
    };

    -- stdio UI plugin says this for yes/no/always/never prompts.
    -- "%0" is the question the user is being asked to respond to.
    ["%0\n[y/n/Always/Never]: "] = {
        de = "%0\n[j/n/Immer/Niemals]",
        nb = "%0\n[j/n/Alltid/Aldri]: ",
        se = "%0\n[j/n/Alltid/Aldrig]: ",
    };

    -- This is used for "yes" in stdio UI's yes/no prompts (case insensitive).
    ["Y"] = {
        de = "J",
        nb = "J",
        se = "J",
    };

    -- This is used for "no" in stdio UI's yes/no prompts (case insensitive).
    ["N"] = {
        de = "N",
        nb = "N",
        se = "N",
    };

    -- This is used for "always" in stdio UI's yes/no/always/never prompts
    --  (case insensitive).
    ["Always"] = {
        de = "Immer",
        nb = "Alltid",
        se = "Alltid",
    };

    -- This is used for "never" in stdio UI's yes/no/always/never prompts
    --  (case insensitive).
    ["Never"] = {
        de = "Niemals",
        nb = "Aldri",
        se = "Aldrig",
    };

    -- This is shown when using stdio UI's built-in README pager, to
    --  show what range of lines of text are being displayed (%0 is first
    --  line, %1 is last line, %2 is the total number of lines of text).
    ["(%0-%1 of %2 lines, see more?)"] = {
        de = "(%0-%1 von %2 Zeilen, mehr anschauen?)",
        nb = "(%0-%1 av %2 linjer, se mer?)",
        se = "(%0-%1 av %2 linjer, se mer?)",
    };

    -- The stdio UI uses this sentence in the prompt if the user is able
    --  to return to a previous stage of installation (from the options
    --  section to the "choose installation destination" section, etc).
    ["Type '%0' to go back."] = {
        de = "Drücken Sie '%0' um zurückzugehen.",
        nb = "Skriv '%0' for å gå tilbake.",
        se = "Skriv '%0' för att gå tillbaka.",
    };

    -- This is the string used for the '%0' in the above string.
    --  This is only for the stdio UI, so choose something easy and
    --  reasonable for the user to manually type. The UIs use a different
    --  string for their button ("Back" vs "back" specifically).
    ["back"] = {
        de = "zurück",
        nb = "tilbake",
        se = "tillbaka",
    };

    -- This is the prompt in the stdio driver when user input is expected.
    ["> "] = {
        de = "> ",
        nb = "> ",
        se = "> ",
    };

    -- That's meant to be the name of an item (%0) and the percent done (%1).
    ["%0: %1%%"] = {
        de = "%0: %1%%",
        nb = "%0: %1%%",
        se = "%0: %1%%",
    };

    -- The stdio UI uses this to show current status (%0),
    --  and overall progress as percentage of work complete (%1).
    ["%0 (total progress: %1%%)"] = {
        de = "%0 (Gesamtfortschritt: %1%%)",
        nb = "%0 (totalt: %1%%)",
        se = "%0 (totalt: %1%%)",
    };

    -- This prompt is shown to the end-user after an End User License Agreement
    --  has been displayed, asking them if the license text is acceptable to
    --  them. It's a yes/no question.
    ["Accept this license?"] = {
        de = "Nehmen Sie die Lizenzbedingungen an?",
        nb = "Akseptere denne lisensen?",
        se = "Acceptera licensen?",
    };

    -- This is a GTK+ button label for yes/no/always/never questions.
    --  The '_' comes before the hotkey character.
    ["_Always"] = {
        de = "_Immer",
        nb = "_Alltid",
        se = "_Alltid",
    };

    -- This is an error message reported when a .zip file (or whatever) that
    --  we need can't be located.
    ["Archive not found"] = {
        de = "Archiv nicht gefunden",
        nb = "Fant ikke arkiv",
        se = "Hittade inte arkivet",
    };

    -- This prompt is shown to the user when they click the "Cancel" button,
    --  to confirm they really want to stop. It's a yes/no question.
    ["Are you sure you want to cancel installation?"] = {
        de = "Sind Sie sicher, dass Sie die Installation abbrechen wollen?",
        nb = "Er du sikker på at du vil avbryte installasjonen?",
        se = "Är du säker på att du vill avbryta installationen?",
    };

    -- The opposite of "next", used as a UI button label.
    ["Back"] = {
        de = "Zurück",
        nb = "Tilbake",
        se = "Tillbaka",
    };

    -- This is a GTK+ button label. The '_' comes before the hotkey character.
    --  "Back" might be using 'B' in English. This button brings up a file
    --  dialog where the end-user can navigate to and select files.
    ["B_rowse..."] = {
        de = "D_urchsuchen",
        nb = "B_la gjennom...",
        se = "B_läddra",
    };


    --  All the "BUG:" strings are generally meant to be seen by developers,
    --   not end users. They represent programming errors and configuration
    --   file problems.

    -- This is shown if the configuration file has specified two cd-roms (or
    --  whatever) with the same media id, which is a bug the developer must
    --  fix before shipping her installer.
    -- "media id" refers to Setup.Media.id in the config file. It's not meant
    --  to be a proper name, in this case.
    ["BUG: duplicate media id"] = {
        de = "FEHLER: Doppelte Medien-ID",
        nb = "FEIL: duplisert media-id",
        se = "FEL: dupliserat media id",
    };

    -- This is shown if the configuration file has no installable options,
    --  either because none are listed or they've all become disabled, which
    --  is a bug the developer must fix before shipping her installer.
    ["BUG: no options"] = {
        de = "FEHLER: Keine Optionen",
        nb = "FEIL: ingen valg",
        se = "FEL: inget option",
    };

    -- This is a file's permissions. Programmers give these as strings, and
    --  if one isn't valid, the program will report this. So, on Unix, they
    --  might specify "0600" as a valid string, but "sdfksjdfk" wouldn't be
    --  and cause this error.
    ["BUG: '%0' is not a valid permission string"] = {
        de = "FEHLER: '%0' ist keine zulässiger Berechtigungs-String",
        nb = "FEIL: '%0' er ikke en gyldig rettighetsstreng",
        se = "FEL: '%0' är inte en giltig rättighetssträng",
    };

    -- If there's a string in the program that needs be formatted with
    --  %0, %1, etc, and it specifies an invalid sequence like "%'", this
    --  error pops up to inform the programmer/translator.
    -- "format()" is a proper name in this case (program function name)
    ["BUG: Invalid format() string"] = {
        de = "FEHLER: Unzulässiger format() String",
        nb = "FEIL: Ugyldig format()-streng",
        se = "FEL: Ogiltig format() sträng",
    };

    -- The program runs in "stages" and as it transitions from one stage to
    --  another, it has to report some data about what happened during the
    --  stage. A programming bug may cause unexpected type of data to be
    --  reported, causing this error to pop up.
    ["BUG: stage returned wrong type"] = {
        de = "FEHLER: Abschnitt gab falschen Typ zurück",
        nb = "FEIL: nivå returnerte feil type",
        se = "FEL: nivå returnerade fel type",
    };

    -- The program runs in "stages" and as it transitions from one stage to
    --  another, it has to report some data about what happened during the
    --  stage. A programming bug may cause unexpected information to be
    --  reported, causing this error to pop up.
    ["BUG: stage returned wrong value"] = {
        de = "FEHLER: Abschnitt gab falschen Wert zurück",
        nb = "FEIL: nivå returnerte feil verdi",
        se = "FEL: nivå returnerade fel värde",
    };

    -- The program runs in "stages", which can in many cases be revisited
    --  by the user clicking the "Back" button. If the program has a bug
    --  that allows the user to click "Back" on the initial stage, this
    --  error pops up.
    ["BUG: stepped back over start of stages"] = {
        de = "FEHLER: Über den Startabschnitt hinaus zurückgegangen",
        nb = "FEIL: Gikk tilbake forbi startnivå",
        se = "FEL: Gick tillbaka förbi startnivå",
    };

    -- This happens if there's an unusual case when writing out Lua scripts
    --  to disk. This should never be seen by an end-user.
    ["BUG: Unhandled data type"] = {
        de = "FEHLER: Unbehandelter Datentyp",
        nb = "FEIL: Uhåndtert datatype",
        se = "FEL: Ohanterad datatyp",
    };

    -- This is triggered by a logic bug in the i/o subsystem.
    --  This should never be seen by an end-user.
    --  "tar" is a proper name in this case (it's a file format).
    ["BUG: Can't duplicate tar inputs"] = {
        de = "FEHLER: Tar-Eingaben können nicht dupliziert werden",
        nb = "FEIL: Kan ikke duplisere innfiler for tar",
        se = "FEL: Kan inte duplicera infiler för tar",
    };

    -- This is a generic error message when a programming bug produced a
    --  result we weren't expecting (a negative number when we expected
    --  positive, etc...)
    ["BUG: Unexpected value"] = {
        se = "FEL: Oväntat värde",
    };

    -- Buggy config elements:
    -- This is supposed to be a config element (%0) and something that's wrong
    --  with it (%1), such as "BUG: Config Package::description not a string"
    -- The grammar can be imperfect here; this is a developer error, not an
    --  end-user error, so we haven't made this very flexible.
    ["BUG: Config %0 %1"] = {
        de = "FEHLER: Konfiguration %0 %1",
        nb = "FEIL: Konfigurasjon %0 %1",
        se = "FEL: Konfiguration %0 %1",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be explicitly specified"] = {
        de = "muss explizit angegeben werden",
        nb = "må gis eksplisitt",
        se = "måste vara explicit specifierad",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be string or table of strings"] = {
        de = "muss ein String oder eine Tabelle von Strings sein",
        nb = "må være streng eller tabell av strenger",
        se = "måste vara sträng eller tabell av strängar",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be a string or number"] = {
        de = "muss ein String oder eine Zahl sein",
        nb = "må være streng eller nummer",
        se = "måste vara sträng eller nummer",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["can't be empty string"] = {
        de = "darf kein leerer String sein",
        nb = "kan ikke være tom streng",
        se = "kan inte vara tom sträng",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have protocol"] = {
        de = "URL hat kein Protokoll",
        nb = "URL har ikke protokoll",
        se = "URL saknar protokoll",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have host"] = {
        de = "URL hat keinen Host",
        nb = "URL har ikke vert",
        se = "URL saknar värd",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have path"] = {
        de = "URL hat keinen Pfad",
        nb = "URL har ikke sti",
        se = "URL saknar sökväg",
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL protocol is unsupported"] = {
        de = "URL Protokoll wird nicht unterstützt",
        nb = "URL-protokoll er ikke støttet",
        se = "URL protokollet har inget stöd",
    };

    -- This is an error string for a buggy config element. See notes above.
    --  "Permission string" is text representing a file's permissions,
    --  such as "0644" on Unix.
    ["Permission string is invalid"] = {
        de = "Berechtigungs-String ist ungültig",
        nb = "Rettighetsstreng er ugyldig",
        se = "Rättighetssträngen är ogiltig",
    };

    -- This is an error string for a buggy config element. See notes above.
    --  "property" means attribute, not something owned, in this case.
    ["is not a valid property"] = {
        de = "ist keine gültige Eigenschaft",
        nb = "er ikke en gyldig egenskap",
        se = "är inte ett giltigt attribut",
    };

    -- This is an error string for a buggy config element. See notes above.
    --  %0 is a data type name (string, number, table, etc).
    ["must be %0"] = {
        de = "muss vom Typ %0 sein",
        nb = "må være %0",
        se = "måste vara %0",
    };

    -- Data type for "must be %0" above...
    ["string"] = {
        de = "String",
        nb = "streng",
        se = "sträng",
    };

    -- Data type for "must be %0" above...
    ["boolean"] = {
        de = "Bool",
        nb = "boolsk verdi",
        se = "boolskt värde",
    };

    -- Data type for "must be %0" above...
    ["number"] = {
        de = "Zahl",
        nb = "nummer",
        se = "nummer",
    };

    -- Data type for "must be %0" above...
    ["function"] = {
        de = "Funktion",
        nb = "funksjon",
        se = "funktion",
    };

    -- Data type for "must be %0" above...
    ["table"] = {
        de = "Tabelle",
        nb = "tabell",
        se = "tabell",
    };


    -- bzlib is a proper name. The error message (%0) may not be localized,
    --  it's meant to be a developer error and not an end-user message.
    ["bzlib triggered an internal error: %0"] = {
        de = "bzlib hat einen internen Fehler ausgelöst: %0",
        nb = "intern feil i bzlib: %0",
        se = "internt fel i bzlib: %0",
    };

    -- This is a UI button label, usually paired with "OK", but also usually
    --  present as a generic "stop the program" button.
    ["Cancel"] = {
        de = "Abbrechen",
        nb = "Avbryt",
        se = "Avbryt",
    };

    -- This is a message box title when prompting for confirmation when the
    --  the user clicks the Cancel button.
    ["Cancel installation"] = {
        de = "Installation abbrechen",
        nb = "Avbryt installasjonen",
        se = "Avbryt installationen",
    };

    -- This error is reported for i/o failures while listing files contained
    --  in a .zip (or whatever) file.
    ["Couldn't enumerate archive"] = {
        de = "Archiv kann nicht aufgelistet werden",
        nb = "Kunne ikke enumerere arkiv",
        se = "Kunde inte lista filen",
    };

    -- This error is reported for i/o failures while opening a .zip
    --  (or whatever) file.
    ["Couldn't open archive"] = {
        de = "Archiv kann nicht geöffnet werden",
        nb = "Kunne ikke åpne arkiv",
        se = "Kunde inte öppna filen",
    };

    -- This is used by the stdio UI to choose a location to write files.
    --  A numbered list of options is printed, and the user may choose one by
    --  its number (default is number one), or enter their own text instead of
    --  choosing a default. This string is the instructions printed for the
    --  user before the prompt.
    ["Choose install destination by number (hit enter for #1), or enter your own."] = {
        de = "Wählen Sie eine Nummer für das Installationsziel (drücken Sie Enter für #1), oder geben Sie ein eigenes an.",
        nb = "Velg installasjonssti etter nummer (trykk enter for #1), eller skriv din egen.",
        se = "Välg sökväg för installationen efter nummer (tryck enter för #1), eller skriv in din egen.",
    };

    -- This is used by the stdio UI to toggle options. A numbered list is
    --  printed, and the user can enter one of those numbers to toggle that
    --  option on or off. This string is the instructions printed for the
    --  user before the prompt.
    ["Choose number to change."] = {
        de = "Wählen Sie eine Nummer zum Ändern.",
        nb = "Velg nummer som skal endres.",
        se = "Välg nummer som skall ändras.",
    };

    -- As in "two different files want to use the same name." This is a title
    --  on a message box.
    ["Conflict!"] = {
        de = "Konflikt!",
        nb = "Konflikt!",
        se = "Konflikt!",
    };

    -- This is an error message shown to the user. When a file is to be
    --  overwritten, we move it out of the way instead, so we can restore it
    --  ("roll the file back") in case of problems, with the goal of having
    --  an installation that fails halfway through reverse any changes it made.
    -- This error is shown if we can't move a file out of the way.
    ["Couldn't backup file for rollback"] = {
        de = "Konnte Datei nicht zum Zurücknehmen sichern",
        nb = "Kunne ikke sikkerhetskopiere fil for tilbakerulling",
        se = "Kunde inte säkerhetskopiera filen för återställning",
    };

    -- This error is shown if we aren't able to write the list of files
    --  that were installed (the "manifest") to disk.
    ["Couldn't create manifest"] = {
        de = "Konnte Manifest nicht erstellen",
        nb = "Kunne ikke lage manifest",
        se = "Kunde inte skapa manifest",
    };

    -- This is an error message. It speaks for itself.   :)
    ["Couldn't restore some files. Your existing installation is likely damaged."] = {
        de = "Konnte einige Dateien nicht widerherstellen. Ihre Installation ist wahrscheinlich beschädigt.",
        nb = "Noen filer kunne ikke tilbakestilles. Den eksisterende installasjonen er sannsynligvis skadet.",
        se = "Några filer kunde inte återskapas. Den existerande installationen är troligtvis skadad.",
    };

    -- Error message when deleting a file fails.
    ["Deletion failed!"] = {
        de = "Löschen fehlgeschlagen!",
        nb = "Kunne ikke slette!",
        se = "Kunde inte radera!",
    };

    -- This is a label displayed next to the text entry in the GTK+ UI where
    --  the user specifies the installation destination (folder/directory) in
    --  the filesystem.
    ["Folder:"] = {
        se = "Katalog:",
    };

    -- This is a window title when user is selecting a path to install files.
    ["Destination"] = {
        de = "Ziel",
        nb = "Destinasjon",
        se = "Destination",
    };

    -- This is a window title while the program is downloading external files
    --  it needs from the network.
    ["Downloading"] = {
        de = "Herunterladen",
        nb = "Laster ned",
        se = "Laddar ner",
    };

    -- Several UIs use this string as a prompt to the end-user when selecting
    --  a destination for newly-installed files.
    ["Enter path where files will be installed."] = {
        de = "Geben Sie den Pfad an, wo Dateien installiert werden sollen.",
        nb = "Skriv inn destinasjonssti for installasjonen.",
        se = "Skriv in sökvägen för installationen.",
    };

    -- Error message when a file we expect to load can't be read from disk.
    ["failed to load file '%0'"] = {
        de = "Laden von Datei '%0' fehlgeschlagen",
        nb = "kunne ikke laste fil '%0'",
        se = "kunde inte ladda filen '%0'",
    };

    -- This is a window title when something goes very wrong.
    ["Fatal error"] = {
        de = "Schwerer Fehler",
        nb = "Fatal feil",
        se = "Fatalt fel",
    };

    -- This is an error message when failing to write a file to disk.
    ["File creation failed!"] = {
        de = "Dateierstellung fehlgeschlagen!",
        nb = "Kunne ikke lage fil!",
        se = "Kunde inte skapa filen!",
    };

    -- This is an error message when failing to get a file from the network.
    ["File download failed!"] = {
        de = "Dateidownload fehlgeschlagen!",
        nb = "Kunne ikke laste ned fil!",
        se = "Kunde inte ladda ner filen!",
    };

    -- This prompt is shown to users when we may overwrite an existing file.
    --  "%0" is the filename.
    ["File '%0' already exists! Replace?"] = {
        de = "Datei '%0' existiert bereits! Ersetzen?",
        nb = "Filen '%0' eksisterer allerede! Skrive over?",
        se = "Filen '%0' existerer redan! Skriv över?",
    };

    -- This is a button in the UI. It replaces "Next" when there are no more
    --  stages to move forward to.
    ["Finish"] = {
        de = "Fertig",
        nb = "Ferdig",
        se = "Färdig",
    };

    -- This error message is (hopefully) shown to the user if the UI
    --  subsystem can't create the main application window.
    ["GUI failed to start"] = {
        de = "GUI konnte nicht gestartet werden",
        nb = "Kunne ikke starte grafisk grensesnitt",
        se = "Kunde inte starta grafisk gränssnitt",
    };

    -- This message is shown to the user if an installation encounters a fatal
    --  problem (or the user clicked "cancel"), telling them that we'll try
    --  to put everything back how it was before we started.
    ["Incomplete installation. We will revert any changes we made."] = {
        de = "Unvollständige Installation. Vollzogene Veränderungen werden zurückgenommen.",
        nb = "Installasjonen ble ikke ferdig. Vi vil tilbakestille alle endringer som ble gjort.",
        se = "Ofullständig installation. Återställning av alla gjorda ändringar utförs.",
    };

    -- Reported to the user if everything worked out.
    ["Installation was successful."] = {
        de = "Installation war erfolgreich.",
        nb = "Installasjonen var en suksess.",
        se = "Installationen blev lyckad.",
    };

    -- This is a window title, shown while the actual installation to disk
    --  is in process and a progress meter is being shown.
    ["Installing"] = {
        de = "Installieren",
        nb = "Installerer",
        se = "Installerar",
    };

    -- This is a window title, shown while the user is choosing
    --  installation-specific options.
    ["Options"] = {
        de = "Optionen",
        nb = "Valg",
        se = "Välg",
    };

    -- Shown as an option in the ncurses UI as the final element in a list of
    --  default filesystem paths where a user may install files. They can
    --  choose this to enter a filesystem path manually.
    ["(I want to specify a path.)"] = {
        de = "(Ich möchte einen Pfad angeben.)",
        nb = "(Jeg vil skrive min egen sti.)",
        se = "(Jag vill skriva in en egen sökväg.)",
    };

    -- "kilobytes per second" ... download rate.
    ["KB/s"] = {
        de = "KB/s",
        nb = "KB/s",
        se = "KB/s",
    };

    -- "bytes per second" ... download rate.
    ["B/s"] = {
        de = "B/s",
        nb = "B/s",
        se = "B/s",
    };

    -- Download rate when we don't know the goal (can't report time left).
    -- This is a number (%0) followed by the localized "KB/s" or "B/s" (%1).
    ["%0 %1"] = {
        de = "%0 %1",
        nb = "%0 %1",
        se = "%0 %1",
    };

    -- Download rate when we know the goal (can report time left).
    -- This is a number (%0) followed by the localized "KB/s" or "B/s" (%1),
    --  then the hours (%2), minutes (%3), and seconds (%4) remaining
    ["%0 %1, %2:%3:%4 remaining"] = {
        de = "%0 %1, %2:%3:%4 verbleibend",
        nb = "%0 %1, %2:%3:%4 igjen",
        se = "%0 %1, %2:%3:%4 återstår",
    };

    -- download rate when download isn't progressing at all.
    ["stalled"] = {
        de = "Stillstand", -- FIXME Translated as "halt" or "standstill"
        nb = "står fast",
        se = "avstannad",
    };

    -- Download progress string: filename (%0), percent downloaded (%1),
    --  download rate determined in one of the above strings (%2).
    ["%0: %1%% (%2)"] = {
        de = "%0: %1%% (%2)",
        nb = "%0: %1%% (%2)",
        se = "%0: %1%% (%2)",
    };

    -- This is a window title when prompting the user to insert a new disc.
    ["Media change"] = {
        de = "Medienwechsel",
        nb = "Mediaendring",
        se = "Mediabyte",
    };

    -- This error message is shown to the end-user when we can't make a new
    --  folder/directory in the filesystem.
    ["Directory creation failed"] = {
        de = "Erstellung eines Verzeichnisses fehlgeschlagen",
        nb = "Kunne ikke lage katalog",
        se = "Kunde inte skapa katalog",
    };

    -- This is a GTK+ button label. The '_' comes before the hotkey character.
    --  "No" would take the 'N' hotkey in English.
    ["N_ever"] = {
        de = "N_iemals",
        nb = "Al_dri",
        se = "Al_drig",
    };

    -- This is a GUI button label, to move forward to the next stage of
    --  installation. It's opposite is "Back" in this case.
    ["Next"] = {
        de = "Weiter",
        nb = "Neste",
        se = "Nästa",
    };

    -- This is a GUI button label, indicating a negative response.
    ["No"] = {
        de = "Nein",
        nb = "Nei",
        se = "Nej",
    };

    -- This is a GUI button label, indicating a positive response.
    ["Yes"] = {
        de = "Ja",
        nb = "Ja",
        se = "Ja",
    };

    -- HTTP error message in the www UI, as in "404 Not Found" ... requested
    --  file is missing.
    ["Not Found"] = {
        de = "Nicht gefunden",
        nb = "Ikke funnet",
        se = "Inte funnen",
    };

    -- This is reported to the user when there are no files to install, and
    --  thus no installation to go forward.
    ["Nothing to do!"] = {
        de = "Nichts zu tun!",
        nb = "Ingenting å gjøre!",
        se = "Ingenting att göra!",
    };

    -- This is a GUI button label, sometimes paired with "Cancel"
    ["OK"] = {
        de = "OK",
        nb = "OK",
        se = "OK",
    };

    -- This is displayed when the application has a serious problem, such as
    --  crashing again in the crash handler, or being unable to find basic
    --  files it needs to get started. It may be a window title, or written
    --  to stdout, or whatever, but it's basically meant to be a title or
    --  header, with more information to follow later.
    ["PANIC"] = {
        de = "PANIK",
        nb = "PANIKK",
        se = "PANIK",
    };

    -- Prompt shown to user when we need her to insert a new disc.
    ["Please insert '%0'"] = {
        de = "Bitte legen Sie '%0' ein",
        nb = "Sett inn '%0'",
        se = "Sätt in '%0'",
    };

    -- Prompt shown to user in the stdio UI when we need to pause before
    --  continuing, usually to let them read the outputted text that is
    --  scrolling by.
    ["Press enter to continue."] = {
        de = "Drücken Sie Enter um fortzufahren",
        nb = "Trykk enter for å fortsette.",
        se = "Tryck enter för att fortsätta.",
    };

    -- This is a window title when informing the user that something
    --  important has gone wrong (such as being unable to revert changes
    --  during a rollback).
    ["Serious problem"] = {
        de = "Ernstes Problem",
        nb = "Alvorlig problem",
        se = "Allvarligt problem",
    };

    -- The www UI uses this as a page title when the program is terminating.
    ["Shutting down..."] = {
        de = "Schließe...",
        nb = "Avslutter...",
        se = "Avslutar...",
    };

    -- The www UI uses this as page text when the program is terminating.
    ["You can close this browser now."] = {
        de = "Sie können diesen Browser nun schließen.",
        se = "Du kan stänga denna browser nu.",
    };

    -- Error message shown to end-user when we can't write a symbolic link
    --  to the filesystem.
    ["Symlink creation failed!"] = {
        de = "Erzeugung einer Verknüpfung fehlgeschlagen!",
        nb = "Kunne likke lage symbolsk lenke!",
        se = "Kunde inte skapa symbolisk länk!",
    };

    -- Error message shown to the end-user when the OS has requested
    --  termination of the program (SIGINT/ctrl-c on Unix, etc).
    ["The installer has been stopped by the system."] = {
        de = "Das Installationsprogramm wurde vom System gestoppt.",
        nb = "Installasjonsprogrammet ble stoppet av systemet.",
        se = "Installationsprogrammet blev stoppat av systemet.",
    };

    -- Error message shown to the end-user when the program crashes with a
    --  bad memory access (segfault on Unix, GPF on Windows, etc).
    ["The installer has crashed due to a bug."] = {
        de = "Das Installationsprogramm ist aufgrund eines Fehlers abgestürzt.",
        nb = "Installasjonsprogrammet kræsjet pga. en feil.",
        se = "Installationsprogrammet kraschade pga. ett fel.",
    };

    -- This is a button label in the ncurses ui to flip an option on/off.
    ["Toggle"] = {
        de = "Umschalten",
        nb = "Inverter valg",
        se = "Invertera",
    };

    -- This is an error message shown to the end-user when there is an
    --  unexpected entry in a .zip (or whatever) file that we didn't know
    --  how to handle.
    ["Unknown file type in archive"] = {
        de = "Unbekannter Dateityp im Archiv",
        nb = "Ukjent filtype i arkivet",
        se = "Okänd filtyp i arkivet",
    };

    -- This is an error message shown to the end-user if they refuse to
    --  agree to the license of the software they are try to install.
    ["You must accept the license before you may install"] = {
        de = "Sie müssen den Lizenzbedingungen zustimmen, bevor sie installieren können",
        nb = "Lisensen må godkjennes før du kan installere",
        se = "Licensen måste godkännas för att installationen skall fortsätta",
    };

    -- Installations display the currently-installing component, such as
    --  "Base game" or "Bonus pack content" or whatnot. The installer lists
    --  the current component as "Metadata" when writing out its own
    --  information, such as file manifests, uninstall support, etc.
    ["Metadata"] = {
        de = "Metadaten",
        nb = "Metadata",
        se = "Metadata",
    };
};

-- end of localization.lua ...

