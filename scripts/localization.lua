-- MojoSetup; a portable, flexible installation application.
--
-- Please see the file LICENSE.txt in the source's root directory.
--
-- DO NOT EDIT BY HAND.
-- This file was generated with po2localization.pl, version svn-534 ...
--  on 2008-03-01 22:00:16-0500
--
-- Your own installer's localizations go into app_localization.lua instead.
-- If you want to add strings to be translated to this file, contact Ryan
-- (icculus@icculus.org). If you want to add or change a translation for
-- existing strings, please use our nice web interface here for your work:
--
--    https://translations.launchpad.net/mojosetup/
--
-- ...and that work eventually ends up in this file.
--
-- X-Launchpad-Export-Date: 2008-03-02 02:32+0000
-- X-Generator: Launchpad (build Unknown)

MojoSetup.languages = {
    en_US = "English (United States)",
    cs = "Czech",
    de = "German",
    en_GB = "English (United Kingdom)",
    es = "Spanish",
    fi = "Finnish",
    fr = "French",
    it = "Italian",
    nb = "Norwegian Bokmal",
    nds = "German, Low",
    nl = "Dutch",
    pt = "Portuguese",
    ru = "Russian",
    sv = "Swedish",
    uk = "Ukrainian"
};

MojoSetup.localization = {
    -- zlib error message
    ["need dictionary"] = {
        cs = "je třeba slovník",
        de = "Wörterbuch benötigt",
        en_GB = "need dictionary",
        es = "necesita diccionario",
        fi = "tarve sanastolle",
        fr = "dictionnaire nécessaire",
        nb = "trenger ordbok",
        nds = "Wörterbuch benötigt",
        nl = "woordenboek nodig",
        pt = "é necessário um dicionário",
        ru = "необходим словарь",
        sv = "behöver ordbok",
        uk = "потрібен словник"
    };

    -- zlib error message
    ["data error"] = {
        cs = "datová chyba",
        de = "Datenfehler",
        en_GB = "data error",
        es = "error de datos",
        fi = "datavirhe",
        fr = "erreur de données",
        nb = "datafeil",
        nds = "Datenfehler",
        nl = "data fout",
        pt = "erro de dados",
        ru = "ошибка данных",
        sv = "datafel",
        uk = "помилка даних"
    };

    -- zlib error message
    ["memory error"] = {
        cs = "paměťová chyba",
        de = "Speicherfehler",
        en_GB = "memory error",
        es = "error de memoria",
        fi = "muistivirhe",
        fr = "erreur de mémoire",
        nb = "minnefeil",
        nds = "Speicherfehler",
        nl = "geheugen fout",
        pt = "erro na memória",
        ru = "ошибка памяти",
        sv = "minnesfel",
        uk = "помилка пам'яті"
    };

    -- zlib error message
    ["buffer error"] = {
        cs = "chyba bufferu",
        de = "Pufferfehler",
        en_GB = "buffer error",
        es = "error de buffer",
        fi = "puskurivirhe",
        fr = "erreur de tampon",
        nb = "bufferfeil",
        nds = "Pufferfehler",
        nl = "buffer fout",
        pt = "erro no tampão",
        ru = "ошибка буфера",
        sv = "bufferfel",
        uk = "помилка буферу"
    };

    -- zlib error message
    ["version error"] = {
        cs = "chyba verze",
        de = "Versionsfehler",
        en_GB = "version error",
        es = "error de versión",
        fi = "versiovirhe",
        fr = "erreur de version",
        nb = "versjonsfeil",
        nds = "Versionsfehler",
        nl = "versie fout",
        pt = "erro na versão",
        ru = "ошибка версии",
        sv = "versionsfel",
        uk = "помилка версії"
    };

    -- zlib error message
    ["unknown error"] = {
        cs = "neznámá chyba",
        de = "Unbekannter Fehler",
        en_GB = "unknown error",
        es = "error desconocido",
        fi = "tuntematon virhe",
        fr = "erreur inconnue",
        it = "errore sconosciuto",
        nb = "ukjent feil",
        nds = "unbekannter Fehler",
        nl = "onbekende fout",
        pt = "erro desconhecido",
        ru = "неизвестная ошибка",
        sv = "okänt fel",
        uk = "невідома помилка"
    };

    -- stdio UI plugin says this for "OK"-only msgboxes. "%0" is the message
    --  box's text content.
    --  stdio UI plugin says this for yes/no prompts that default to yes.
    --  "%0" is the question the user is being asked to respond to.
    ["%0 [Y/n]: "] = {
        cs = "%0 [A/n]: ",
        de = "%0 [J/n]: ",
        en_GB = "%0 [Y/n] ",
        es = "%0 [S/n]: ",
        fi = "%0 [K/e]: ",
        fr = "%0 [O/n]: ",
        it = "%0 [S/n]: ",
        nb = "%0 [J/n]: ",
        nds = "%0 [J/n] ",
        nl = "%0 [J/n]: ",
        pt = "%0 [S/n] ",
        ru = "%0 [Y/n]: ",
        sv = "%0 [J/n]: ",
        uk = "%0 [ТАК/ні] "
    };

    -- stdio UI plugin says this for yes/no prompts that default to no.
    --  "%0" is the question the user is being asked to respond to.
    ["%0 [y/N]: "] = {
        cs = "%0 [a/N] ",
        de = "%0 [j/N]: ",
        en_GB = "%0 [y/N]: ",
        es = "%0 [s/N]: ",
        fi = "%0 [k/E]: ",
        fr = "%0 [o/N]: ",
        it = "%0 [s/N]: ",
        nb = "%0 [j/N]: ",
        nds = "%0 [j/N] ",
        nl = "%0 [j/N]: ",
        pt = "% [s/N] ",
        ru = "%0 [y/N]: ",
        sv = "%0 [j/N]: ",
        uk = "%0 [так/НІ] "
    };

    -- stdio UI plugin says this for yes/no/always/never prompts.
    --  "%0" is the question the user is being asked to respond to.
    ["%0 [y/n/Always/Never]: "] = {
        cs = "%0 [a/n/Vždy/niKdy] ",
        de = "%0 [j/n/Immer/Niemals]: ",
        en_GB = "%0 [y/n/Always/Never]: ",
        es = "%0 [s/n/Siempre/Nunca]: ",
        fi = "%0 [k/e/Aina/ei Koskaan]: ",
        fr = "%0 [o/n/Toujours/Jamais]: ",
        it = "%0 [s/n/Sempre/Mai]: ",
        nb = "%0 [j/n/Alltid/Aldri]: ",
        nds = "%0 [j/n/Immer/Niemals] ",
        nl = "%0 [j/n/Altijd/Nooit]: ",
        pt = "%0 [s/n/Sempre/Nunca] ",
        ru = "%0 [y/n/Всегда(A)/Никогда(N)]: ",
        sv = "%0 [j/n/Alltid/Aldrig]: ",
        uk = "%0 [так/ні/Завжди/Ніколи]: "
    };

    -- This is used for "yes" in stdio UI's yes/no prompts (case insensitive).
    ["Y"] = {
        cs = "A",
        de = "J",
        en_GB = "Y",
        es = "S",
        fi = "K",
        fr = "O",
        it = "S",
        nb = "J",
        nds = "J",
        nl = "J",
        pt = "S",
        ru = "Y",
        sv = "J"
    };

    -- This is used for "no" in stdio UI's yes/no prompts (case insensitive).
    ["N"] = {
        cs = "N",
        de = "N",
        en_GB = "N",
        es = "N",
        fi = "E",
        fr = "N",
        it = "N",
        nb = "N",
        nds = "N",
        nl = "N",
        pt = "N",
        ru = "N",
        sv = "N"
    };

    -- This is used for "always" in stdio UI's yes/no/always/never prompts
    --  (case insensitive).
    ["Always"] = {
        cs = "Vždy",
        de = "Immer",
        en_GB = "Always",
        es = "Siempre",
        fi = "Aina",
        fr = "Toujours",
        it = "Sempre",
        nb = "Alltid",
        nds = "Immer",
        nl = "Altijd",
        pt = "Sempre",
        ru = "Всегда",
        sv = "Alltid"
    };

    -- This is used for "never" in stdio UI's yes/no/always/never prompts
    --  (case insensitive).
    ["Never"] = {
        cs = "niKdy",
        de = "Niemals",
        en_GB = "Never",
        es = "Nunca",
        fi = "ei Koskaan",
        fr = "Jamais",
        it = "Mai",
        nb = "Aldri",
        nds = "Niemals",
        nl = "Nooit",
        pt = "Nunca",
        ru = "Никогда",
        sv = "Aldrig"
    };

    -- This is shown when using stdio UI's built-in README pager, to
    --  show what range of lines of text are being displayed (%0 is first
    --  line, %1 is last line, %2 is the total number of lines of text).
    ["(%0-%1 of %2 lines, see more?)"] = {
        cs = "(%0-%1 z %2 řádků, zobrazit více?)",
        de = "(%0-%1 von %2 Zeilen, mehr anschauen?)",
        en_GB = "(%0-%1 of %2 lines, see more?)",
        es = "(%0-%1 de %2 líneas, ¿ves más?)",
        fi = "(%0-%1 %2:sta rivistä, lue lisää?)",
        fr = "(lignes %0 à %1 sur %2, en voir plus ?)",
        nb = "(%0-%1 av %2 linjer, se mer?)",
        nds = "%0-%1 vom %2 lines, mehr?",
        nl = "(%0-%1 van %2 regels, meer zien?)",
        pt = "(%0-%1 de %2 linhas, ver mais?)",
        ru = "(%0-%1 из %2 строк, дальше?)",
        sv = "(%0-%1 av %2 rader, visa fler?)"
    };

    -- The stdio UI uses this sentence in the prompt if the user is able
    --  to return to a previous stage of installation (from the options
    --  section to the "choose installation destination" section, etc).
    ["Type '%0' to go back."] = {
        cs = "Napište '%0' pro návrat zpět.",
        de = "Drücken Sie '%0' um zurückzugehen.",
        en_GB = "Type '%0' to go back.",
        es = "Pulsa '%0' para ir atrás.",
        fi = "Kirjoita '%0' palataksesi edelliseen osioon.",
        fr = "Tapez '%0' pour revenir en arrière.",
        it = "Digita %0 per andare indietro.",
        nb = "Skriv '%0' for å gå tilbake.",
        nds = "Drücken Sie '%0' um zurückzukehren",
        nl = "Typ %0 om terug te gaan.",
        pt = "Escreva '%0' para regressar.",
        ru = "Нажмите '%0' чтобы вернуться.",
        sv = "Skriv '%0' för att gå tillbaka."
    };

    -- This is the string used for the '%0' in the above string.
    --  This is only for the stdio UI, so choose something easy and
    --  reasonable for the user to manually type. The UIs use a different
    --  string for their button ("Back" vs "back" specifically).
    ["back"] = {
        cs = "zpět",
        de = "zurück",
        en_GB = "back",
        es = "atrás",
        fi = "takaisin",
        fr = "retour",
        it = "indietro",
        nb = "tilbake",
        nds = "Zurück",
        nl = "terug",
        pt = "retroceder",
        ru = "назад",
        sv = "tillbaka"
    };

    -- This is the prompt in the stdio driver when user input is expected.
    ["> "] = {
        cs = "> ",
        de = "> ",
        en_GB = "> ",
        es = "> ",
        fi = "> ",
        fr = "> ",
        it = "> ",
        nb = "> ",
        nl = "> ",
        pt = "> ",
        ru = "> ",
        sv = "> "
    };

    -- That's meant to be the name of an item (%0) and the percent done (%1).
    ["%0: %1%%"] = {
        cs = "%0: %1%%",
        de = "%0: %1%%",
        en_GB = "%0: %1%%",
        es = "%0: %1%%",
        fi = "%0: %1%%",
        fr = "%0 : %1%%",
        it = "%0: %1%%",
        nb = "%0: %1%%",
        nds = "%0: %1%%",
        nl = "%0: %1%%",
        pt = "%0: %1%%",
        ru = "%0: %1%%",
        sv = "%0: %1%%"
    };

    -- The stdio UI uses this to show current status (%0),
    --  and overall progress as percentage of work complete (%1).
    ["%0 (total progress: %1%%)"] = {
        cs = "%0 (celkový průběh: %1%%)",
        de = "%0 (Gesamtfortschritt: %1%%)",
        en_GB = "%0 (total progress: %1%%)",
        es = "%0 (Progreso total: %1%%)",
        fi = "%0 (kokonaisedistyminen: %1%%)",
        fr = "%0 (progression totale : %1%%)",
        nb = "%0 (totalt: %1%%)",
        nds = "%0 (Fortschritt insgesamt: %1%%)",
        nl = "%0 (totale voortgang: %1%%)",
        pt = "%0 (progresso total: %1%%)",
        ru = "%0 (всего: %1%%)",
        sv = "%0 (totalt: %1%%)"
    };

    -- This prompt is shown to the end-user after an End User License Agreement
    --  has been displayed, asking them if the license text is acceptable to
    --  them. It's a yes/no question.
    ["Accept this license?"] = {
        cs = "Přijímáte podmínky licence?",
        de = "Nehmen Sie die Lizenzbedingungen an?",
        en_GB = "Accept this licence?",
        es = "¿Acepta esta licencia?",
        fi = "Hyväksy lisenssi?",
        fr = "Accepter cette licence ?",
        it = "Accetti questa licenza?",
        nb = "Akseptere denne lisensen?",
        nds = "Akzeptiere diese Lizenz?",
        nl = "Accepteert u deze licentie?",
        pt = "Aceita esta licença?",
        ru = "Вы соглашаетесь?",
        sv = "Acceptera licensen?"
    };

    -- This is a GTK+ button label for yes/no/always/never questions.
    --  The '_' comes before the hotkey character.
    ["_Always"] = {
        cs = "_Vždy",
        de = "_Immer",
        en_GB = "_Always",
        es = "_Siempre",
        fi = "_Aina",
        fr = "_Toujours",
        it = "_Sempre",
        nb = "_Alltid",
        nds = "Immer",
        nl = "_Altijd",
        pt = "_Sempre",
        ru = "_Всегда",
        sv = "_Alltid"
    };

    -- This is an error message reported when a .zip file (or whatever) that
    --  we need can't be located.
    ["Archive not found"] = {
        cs = "Archiv nenalezen",
        de = "Archiv nicht gefunden",
        en_GB = "Archive not found",
        es = "Archivo no encontrado",
        fi = "Pakettia ei löydy",
        fr = "Archive introuvable",
        nb = "Fant ikke arkiv",
        nds = "Archiv nicht gefunden",
        nl = "Archief niet gevonden",
        pt = "O Arquivo não foi encontrado",
        ru = "Архив не найден",
        sv = "Hittade inte arkivet"
    };

    -- This prompt is shown to the user when they click the "Cancel" button,
    --  to confirm they really want to stop. It's a yes/no question.
    ["Are you sure you want to cancel installation?"] = {
        cs = "Opravdu chcete zrušit instalaci?",
        de = "Sind Sie sicher, dass Sie die Installation abbrechen wollen?",
        en_GB = "Are you sure you want to cancel installation?",
        es = "¿Estás seguro de que quieres cancelar la instalación?",
        fi = "Haluatko varmasti keskeyttää asennuksen?",
        fr = "Êtes-vous sûr de vouloir annuler l'installation ?",
        it = "Sei sicuro di voler annullare l'installazione?",
        nb = "Er du sikker på at du vil avbryte installasjonen?",
        nds = "Sind Sie sicher, dass Sie die Installation abbrechen wollen?",
        nl = "Weet u zeker dat u de installatie wilt afbreken?",
        pt = "Tem a certeza que quer cancelar a instalação?",
        ru = "Вы уведены что вы хотите прекратить установку?",
        sv = "Är du säker på att du vill avbryta installationen?"
    };

    -- The opposite of "next", used as a UI button label.
    ["Back"] = {
        cs = "Zpět",
        de = "Zurück",
        en_GB = "Back",
        es = "Atrás",
        fi = "Takaisin",
        fr = "Retour",
        it = "Indietro",
        nb = "Tilbake",
        nds = "Zurück",
        nl = "Vorige",
        pt = "Anterior",
        ru = "Назад",
        sv = "Tillbaka"
    };

    -- This is a GTK+ button label. The '_' comes before the hotkey character.
    --  "Back" might be using 'B' in English. This button brings up a file
    --  dialog where the end-user can navigate to and select files.
    ["B_rowse..."] = {
        cs = "_Procházet...",
        de = "Du_rchsuchen",
        en_GB = "B_rowse...",
        es = "_Navegar...",
        fi = "_Selaa",
        fr = "_Choisir...",
        it = "_Sfoglia...",
        nb = "B_la gjennom...",
        nds = "Du_rchsuchen",
        nl = "Bl_aderen...",
        pt = "_Procurar...",
        ru = "Открыть...",
        sv = "B_läddra"
    };

    -- All the "BUG:" strings are generally meant to be seen by developers,
    --  not end users. They represent programming errors and configuration
    --  file problems.
    --  This is shown if the configuration file has specified two cd-roms (or
    --  whatever) with the same media id, which is a bug the developer must
    --  fix before shipping her installer.
    --  "media id" refers to Setup.Media.id in the config file. It's not meant
    --  to be a proper name, in this case.
    ["BUG: duplicate media id"] = {
        cs = "CHYBA: duplikátní id média",
        de = "FEHLER: Doppelte Medien-ID",
        en_GB = "BUG: duplicate media id",
        es = "FALLO: id de medio duplicada",
        fi = "BUGI: kaksi identtistä media id:tä",
        nb = "FEIL: duplisert media-id",
        nds = "BUG: Doppelte media id",
        nl = "BUG: dubbele media id",
        pt = "BUG: media id duplicada",
        sv = "FEL: dupliserat media id"
    };

    -- This is shown if the configuration file has no installable options,
    --  either because none are listed or they've all become disabled, which
    --  is a bug the developer must fix before shipping her installer.
    ["BUG: no options"] = {
        cs = "CHYBA: nejsou dostupné žádné volby",
        de = "FEHLER: Keine Optionen",
        en_GB = "BUG: no options",
        es = "FALLO: sin opciones",
        fi = "BUGI: ei vaihtoehtoja",
        fr = "BOGUE: Pas d'options",
        nb = "FEIL: ingen valg",
        nds = "BUG: keine Optionen",
        nl = "BUG: geen opties",
        pt = "BUG: sem opções",
        ru = "Баг: нету опций",
        sv = "FEL: inget option"
    };

    -- This is shown if the config file wants us to add an installer to the
    --  files we write to disk, but didn't enable the manifest support the
    --  installer needs. This is a bug the developer must fix before shipping
    --  her installer.
    ["BUG: support_uninstall requires write_manifest"] = {
        cs = "CHYBA: support_uninstall vyžaduje write_manifest",
        de = "FEHLER: support_uninstall benötigt write_manifest",
        en_GB = "BUG: support_uninstall requires write_manifest",
        es = "FALLO: support_uninstall requiere write_manifest",
        fi = "BUGI: support_uninstall vaatii write_manifest:n",
        nb = "FEIL: support_uninstall krever write_manifest",
        nds = "BUG: support_uninstall benötigt write_manifest",
        nl = "BUG: ondersteuning_deinstalleren heeft schrijf_manifest nodig",
        pt = "BUG:  support_uninstall requer write_manifest",
        ru = "Баг: support_uninstall нуждается в write_manifest",
        sv = "FEL: support_uninstall kräver write_manifest"
    };

    -- This is shown if the config file wants us to add a manifest to the
    --  files we write to disk, but didn't enable Lua parser support in the
    --  binary (this is done through CMake when compiling the C code). This is
    --  a bug the developer must fix before shipping her installer.
    ["BUG: write_manifest requires Lua parser support"] = {
        cs = "CHYBA: write_manifest vyžaduje podporu Lua parseru",
        de = "FEHLER: write_manifest benötigt Lua Parser Unterstützung",
        en_GB = "BUG: write_manifest requires Lua parser support",
        es = "FALLO: write_manifest requiere el soporte de un intérprete Lua",
        fi = "BUGI: write_manifest vaatii Lua parser -tuen",
        nb = "FEIL: write_manifest krever støtte for Lua-parser",
        pt = "BUG: write_manifest requer suporte do analisador da linguagem Lua",
        ru = "Баг: write_manifest нуждается в поддержке Lua parser",
        sv = "FEL: write_manifest kräver Lua parser stöd"
    };

    -- This is shown if the config file wants us to add desktop menu items
    --  but uninstaller support isn't enabled. It is considered bad taste
    --  to add system menu items without a way to remove them. This is
    --  a bug the developer must fix before shipping her installer.
    ["BUG: Setup.DesktopMenuItem requires support_uninstall"] = {
        cs = "CHYBA: Setup.DesktopMenuItem vyžaduje support_uninstall",
        de = "FEHLER: Setup.DesktopMenuItem benötigt support_uninstall",
        en_GB = "BUG: Setup.DesktopMenuItem requires support_uninstall",
        es = "FALLO: Setup.DesktopMenuItem requiere support_uninstall",
        fi = "BUGI: Setup.DesktopMenuItem vaatii support_uninstall:n",
        nds = "BUG: Setup.DesktopMenuItem benötigt support_uninstall",
        pt = "BUG: Setup.DesktopMenuItem requer support_uninstall",
        ru = "Баг: Setup.DesktopMenuItem нуждается в support_uninstall",
        sv = "FEL: Setup.DesktopMenuItem kräver support_uninstall"
    };

    -- This is a file's permissions. Programmers give these as strings, and
    --  if one isn't valid, the program will report this. So, on Unix, they
    --  might specify "0600" as a valid string, but "sdfksjdfk" wouldn't be
    --  valid and would cause this error.
    ["BUG: '%0' is not a valid permission string"] = {
        cs = "CHYBA: '%0' není platným řetězcem vyjadřujícím oprávnění",
        de = "FEHLER: '%0' ist kein zulässiger Berechtigungs-String",
        en_GB = "BUG: '%0' is not a valid permission string",
        es = "FALLO: '%0' no es una cadena de permisos válida",
        fi = "BUGI: \"%0\" ei ole kelvollinen oikeuksia määrittävä merkkijono",
        nb = "FEIL: '%0' er ikke en gyldig rettighetsstreng",
        nds = "BUG: '%0' ist kein gültiger \"permission string\"",
        nl = "BUG: '%0' is geen geldige permissie string",
        pt = "BUG: '%0' não é uma expressão válida",
        ru = "Баг: '%0' не правильный текст для прав.",
        sv = "FEL: '%0' är inte en giltig rättighetssträng"
    };

    -- If there's a string in the program that needs be formatted with
    --  %0, %1, etc, and it specifies an invalid sequence like "%z", this
    --  error pops up to inform the programmer/translator.
    --  "format()" is a proper name in this case (program function name)
    ["BUG: Invalid format() string"] = {
        cs = "CHYBA: Neplatný řetězec pro format()",
        de = "FEHLER: Unzulässiger format() String",
        en_GB = "BUG: Invalid format() string",
        es = "FALLO: Cadena format() no válida",
        fi = "BUGI: Epäkelpo format()-merkkijono",
        nb = "FEIL: Ugyldig format()-streng",
        nds = "Fehler: Ungültiger format() string",
        pt = "BUG: format() inválido da expressão",
        ru = "Баг: Недействитeльний текст format()",
        sv = "FEL: Ogiltig format() sträng"
    };

    -- The program runs in "stages" and as it transitions from one stage to
    --  another, it has to report some data about what happened during the
    --  stage. A programming bug may cause unexpected type of data to be
    --  reported, causing this error to pop up.
    ["BUG: stage returned wrong type"] = {
        cs = "CHYBA: instalační krok vrátil chybný datový typ",
        de = "FEHLER: Abschnitt gab falschen Typ zurück",
        en_GB = "BUG: stage returned wrong type",
        es = "FALLO: la etapa ha devuelto tipo erróneo",
        fi = "BUGI: vaihe palautti väärää tyyppiä",
        nb = "FEIL: nivå returnerte feil type",
        nds = "Fehler: Stage Rückgabewert ist vom falschen Typ",
        nl = "BUG: stadium gaf verkeerd type op",
        pt = "BUG: A etapa retornou um tipo errado",
        ru = "Баг: этап выдал неожиданный тип результата",
        sv = "FEL: nivå returnerade fel typ"
    };

    -- The program runs in "stages" and as it transitions from one stage to
    --  another, it has to report some data about what happened during the
    --  stage. A programming bug may cause unexpected information to be
    --  reported, causing this error to pop up.
    ["BUG: stage returned wrong value"] = {
        cs = "CHYBA: instalační krok vrátil chybnou hodnotu",
        de = "FEHLER: Abschnitt gab falschen Wert zurück",
        en_GB = "BUG: stage returned wrong value",
        es = "FALLO: la etapa ha devuelto valor erróneo",
        fi = "BUGI: vaihe palautti väärän arvon",
        nb = "FEIL: nivå returnerte feil verdi",
        nl = "BUG: stadium gaf verkeerde waarde terug",
        pt = "BUG: A etapa retornou um valor errado",
        ru = "Баг: этап выдал неожиданный результат",
        sv = "FEL: nivå returnerade fel värde"
    };

    -- The program runs in "stages", which can in many cases be revisited
    --  by the user clicking the "Back" button. If the program has a bug
    --  that allows the user to click "Back" on the initial stage, this
    --  error pops up.
    ["BUG: stepped back over start of stages"] = {
        cs = "CHYBA: pokus o krok zpět před začátek instalace",
        de = "FEHLER: Über den Startabschnitt hinaus zurückgegangen",
        en_GB = "BUG: stepped back over start of stages",
        es = "FALLO: retroceso más atrás del inicio de las etapas",
        fi = "BUGI: palattiin ensimmäisen vaiheen ohi",
        nb = "FEIL: Gikk tilbake forbi startnivå",
        nl = "BUG: teruggegaan naar voor eerste stadium.",
        pt = "BUG: Retrocedeu na primeira etapa",
        ru = "Баг: этап вышел сзади первого этапа",
        sv = "FEL: Gick tillbaka förbi startnivå"
    };

    -- This happens if there's an unusual case when writing out Lua scripts
    --  to disk. This should never be seen by an end-user.
    ["BUG: Unhandled data type"] = {
        cs = "CHYBA: Datový typ není obsluhován",
        de = "FEHLER: Unbehandelter Datentyp",
        en_GB = "BUG: Unhandled data type",
        es = "FALLO: Tipo de datos sin manipular",
        fi = "BUGI: Käsittelemätön datatyyppi",
        nb = "FEIL: Uhåndtert datatype",
        nl = "BUG: onbekend data type",
        pt = "BUG: tipo sem tratamento",
        sv = "FEL: Ohanterad datatyp"
    };

    -- This is triggered by a logic bug in the i/o subsystem.
    --  This should never be seen by an end-user.
    --  "tar" is a proper name in this case (it's a file format).
    ["BUG: Can't duplicate tar inputs"] = {
        cs = "CHYBA: Nemohu zduplikovat vstupy z taru",
        de = "FEHLER: Tar-Eingaben können nicht dupliziert werden",
        en_GB = "BUG: Can't duplicate tar inputs",
        es = "FALLO: No se pueden duplicar las entradas tar",
        fi = "BUGI: tar-syötteitä ei voida kahdentaa",
        nb = "FEIL: Kan ikke duplisere innfiler for tar",
        nl = "BUG: kan tar input kan niet gedupliceerd woorden.",
        pt = "BUG: Impossível duplicar as entradas do tar",
        ru = "Баг: Не можем дублировать ввод tar",
        sv = "FEL: Kan inte duplicera infiler för tar"
    };

    -- This is a generic error message when a programming bug produced a
    --  result we weren't expecting (a negative number when we expected
    --  positive, etc...)
    ["BUG: Unexpected value"] = {
        cs = "CHYBA: Neočekávaná hodnota",
        de = "FEHLER: Unerwarteter Wert",
        en_GB = "BUG: Unexpected value",
        es = "FALLO: Valor inesperado",
        fi = "BUGI: Odottamaton arvo",
        nds = "Unerwarteter Wert",
        nl = "BUG: Onverwachte waarde",
        pt = "BUG: Valor inexperado",
        ru = "Баг: Неожидданый ответ",
        sv = "FEL: Oväntat värde"
    };

    -- Buggy config elements:
    --  This is supposed to be a config element (%0) and something that's wrong
    --  with it (%1), such as "BUG: Config Package::description not a string"
    --  The grammar can be imperfect here; this is a developer error, not an
    --  end-user error, so we haven't made this very flexible.
    ["BUG: Config %0 %1"] = {
        cs = "CHYBA: Konfigurační hodnota %0 %1",
        de = "FEHLER: Konfiguration %0 %1",
        en_GB = "BUG: Config %0 %1",
        es = "FALLO: Configuración %0 %1",
        fi = "BUGI: Config %0 %1",
        it = "BUG: Config %0 %1",
        nb = "FEIL: Konfigurasjon %0 %1",
        nds = "Fehler: Config %0 %1",
        nl = "BUG: configuratie %0 %1",
        pt = "BUG: Configuração %0 %1",
        ru = "Баг: Опция %0 %1",
        sv = "FEL: Konfiguration %0 %1"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be explicitly specified"] = {
        cs = "musí být explicitně určen",
        de = "muss explizit angegeben werden",
        en_GB = "must be explicitly specified",
        es = "debe ser especificado explícitamente",
        fi = "täytyy määritellä täsmällisesti",
        fr = "doit être spécifié explicitement",
        nb = "må spesifiseres eksplisitt",
        nds = "muss ausdrücklich spezifiziert werden",
        nl = "moet expliciet gespecifieerd worden",
        pt = "Tem que ser especificado explicitamente",
        ru = "должна быть указана прямо",
        sv = "måste vara explicit specifierad"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be string or table of strings"] = {
        cs = "musí být řetězcem nebo tabulkou řetězců",
        de = "muss ein String oder eine Tabelle von Strings sein",
        en_GB = "must be string or table of strings",
        es = "debe ser una cadena o tabla de cadenas",
        fi = "täytyy olla merkkijono tai merkkijonotaulukko",
        fr = "doit être une chaîne de caractères ou un tableau de chaînes de caractères",
        nb = "må være streng eller tabell av strenger",
        nds = "muss String oder Tabelle von Strings sein",
        nl = "moet een string of een tabel met strings zijn",
        pt = "Tem que ser uma expressão ou uma tabela de expressões",
        ru = "должна быть текстом или таблицей с текстами",
        sv = "måste vara en sträng eller en tabell av strängar"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be a string or number"] = {
        cs = "musí být řetězcem nebo číslem",
        de = "muss ein String oder eine Zahl sein",
        en_GB = "must be a string or number",
        es = "debe ser una cadena o un número",
        fi = "täytyy olla merkkijono tai luku",
        fr = "doit être une chaîne de caractères ou un nombre",
        nb = "må være streng eller nummer",
        nds = "muss string oder number sein",
        nl = "moet een string of een nummer zijn",
        pt = "Tem que ser uma expressão ou um número",
        ru = "должна текстом или номером",
        sv = "måste vara en sträng eller ett nummer"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["can't be empty string"] = {
        cs = "nemůže být prázdným řetězcem",
        de = "darf kein leerer String sein",
        en_GB = "can't be empty string",
        es = "no puede ser una cadena vacía",
        fi = "ei saa olla tyhjä merkkijono",
        fr = "ne peut être une chaîne de caractères vide",
        nb = "kan ikke være tom streng",
        nds = "kann String nicht leeren",
        nl = "kan geen lege string zijn",
        pt = "Não pode ser uma expressão vazia",
        ru = "не может быть пустым текстом",
        sv = "kan inte vara en tom sträng"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have protocol"] = {
        cs = "URL nemá určený protokol",
        de = "URL hat kein Protokoll",
        en_GB = "URL doesn't have protocol",
        es = "URL sin especificar protocolo",
        fi = "URL ei sisällä protokollaa",
        fr = "L'URL manque un protocole",
        nb = "URL har ikke protokoll",
        nds = "URL hat kein Protokoll",
        nl = "URL heeft geen protocol",
        pt = "O URL não tem o protocolo",
        ru = "не имеет протокола для URL",
        sv = "URL saknar protokoll"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have host"] = {
        cs = "URL nemá určeného hostitele",
        de = "URL hat keinen Host",
        en_GB = "URL doesn't have host",
        es = "URL sin especificar host",
        fi = "URL ei sisällä palvelinta",
        fr = "L'URL manque un nom d'hôte",
        nb = "URL har ikke ver",
        nds = "URL hat keinen Host",
        nl = "URL heeft geen host",
        pt = "O URL não tem servidor",
        sv = "URL saknar värd"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have path"] = {
        cs = "URL nemá určenou cestu",
        de = "URL hat keinen Pfad",
        en_GB = "URL doesn't have path",
        es = "URL sin especificar ruta",
        fi = "URL ei sisällä polkua",
        fr = "L'URL manque un chemin",
        nb = "URL har ikke sti",
        nds = "URL hat keinen Pfad",
        pt = "O URL não tem recurso",
        sv = "URL saknar sökväg"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL protocol is unsupported"] = {
        cs = "Protokol v URL je nepodporovaný",
        de = "URL Protokoll wird nicht unterstützt",
        en_GB = "URL protocol is unsupported",
        es = "Protocolo de URL no soportado",
        fi = "URL:n protokollaa ei tueta",
        fr = "Le protocole de l'URL n'est pas supporté",
        nb = "URL-protokollen er ikke støttet",
        nds = "URL Protokoll wird nicht unterstützt",
        nl = "protocol van URL wordt niet ondersteund",
        pt = "O protocolo do URL não é suportado",
        ru = "URL прротокол не поддержан",
        sv = "URL-protokollet har inget stöd"
    };

    -- This is an error string for a buggy config element. See notes above.
    --  "Permission string" is text representing a file's permissions,
    --  such as "0644" on Unix.
    ["Permission string is invalid"] = {
        cs = "Řetězec s oprávněními je neplatný",
        de = "Berechtigungsstring ist ungültig",
        en_GB = "Permission string is invalid",
        es = "Cadena de permisos no válida",
        fi = "oikeuksia määrittävä merkkijono ei ole kelvollinen",
        fr = "Chaine de permissions invalide",
        it = "Stringa dei permessi non valida",
        nb = "Rettighetsstrengen er ugyldig",
        nds = "Zugriffsberechtigungs String ist ungültig",
        nl = "Permissie string is niet geldig",
        pt = "A expressão de permissão não é válida",
        ru = "права не правильные",
        sv = "Rättighetssträngen är ogiltig"
    };

    -- This is an error string for a buggy config element. See notes above.
    --  "property" means attribute, not something owned, in this case.
    ["is not a valid property"] = {
        cs = "není platnou vlastností",
        de = "ist keine gültige Eigenschaft",
        en_GB = "is not a valid property",
        es = "no es un atributo válido",
        fi = "ei ole kelvollinen määrite",
        fr = "n'est pas une propriété valide",
        it = "non è una proprietà valida",
        nb = "er ikke en gyldig egenskap",
        nds = "ist keine gültige Eigenschaft",
        nl = "is geen geldige eigenschap",
        pt = "não é uma propriedade válida",
        sv = "är inte ett giltigt attribut"
    };

    -- This is an error string for a buggy config element. See notes above.
    --  %0 is a data type name (string, number, table, etc).
    ["must be %0"] = {
        cs = "musí být %0",
        de = "muss vom Typ %0 sein",
        en_GB = "must be %0",
        es = "debe ser %0",
        fi = "täytyy olla %0",
        fr = "doit être de type %0",
        it = "deve essere %0",
        nb = "må være %0",
        nds = "muss %0 sein",
        nl = "moet %0 zijn",
        pt = "tem que ser %0",
        ru = "должна быть %0",
        sv = "måste vara %0"
    };

    -- Data type for "must be %0" above...
    ["string"] = {
        cs = "řetězec",
        de = "String",
        en_GB = "string",
        es = "cadena",
        fi = "merkkijono",
        fr = "chaîne de caractères",
        it = "stringa",
        nb = "streng",
        nds = "string",
        nl = "string",
        pt = "expressão",
        ru = "строкой",
        sv = "sträng"
    };

    -- Data type for "must be %0" above...
    ["boolean"] = {
        cs = "booleovská hodnota",
        de = "Bool",
        en_GB = "boolean",
        es = "booleano",
        fi = "totuusarvo",
        fr = "booléen",
        it = "booleano",
        nb = "boolsk verdi",
        nds = "boolean",
        nl = "boolean",
        pt = "boleano",
        sv = "boolskt värde"
    };

    -- Data type for "must be %0" above...
    ["number"] = {
        cs = "číslo",
        de = "Zahl",
        en_GB = "number",
        es = "número",
        fi = "luku",
        fr = "nombre",
        it = "numero",
        nb = "nummer",
        nds = "number",
        nl = "nummer",
        pt = "número",
        ru = "числом",
        sv = "nummer"
    };

    -- Data type for "must be %0" above...
    ["function"] = {
        cs = "funkce",
        de = "Funktion",
        en_GB = "function",
        es = "función",
        fi = "funktio",
        fr = "fonction",
        it = "funzione",
        nb = "funksjon",
        nds = "Funktion",
        nl = "functie",
        pt = "função",
        ru = "функцией",
        sv = "funktion"
    };

    -- Data type for "must be %0" above...
    ["table"] = {
        cs = "tabulka",
        de = "Tabelle",
        en_GB = "table",
        es = "tabla",
        fi = "taulukko",
        fr = "tableau",
        it = "tabella",
        nb = "tabell",
        nds = "Tabelle",
        nl = "tabel",
        pt = "tabela",
        ru = "таблицей",
        sv = "tabell"
    };

    -- bzlib is a proper name. The error message (%0) may not be localized,
    --  it's meant to be a developer error and not an end-user message.
    ["bzlib triggered an internal error: %0"] = {
        cs = "bzlib vyvolalo vnitřní chybu: %0",
        de = "bzlib hat einen internen Fehler ausgelöst: %0",
        en_GB = "bzlib triggered an internal error: %0",
        es = "bzlib ha provocado un error interno: %0",
        fi = "bzlib laukaisi sisäisen virheen: %0",
        fr = "bzlib a causé une erreur interne: %0",
        nb = "intern feil i bzlib: %0",
        nds = "bzlib löste eine internen Fehler aus: %0",
        nl = "bzlib heeft een interne fout veroorzaakt: %0",
        pt = "A bzlib despoletou um erro interno: %0",
        sv = "internt fel i bzlib: %0"
    };

    -- This is a UI button label, usually paired with "OK", but also usually
    --  present as a generic "stop the program" button.
    ["Cancel"] = {
        cs = "Zrušit",
        de = "Abbrechen",
        en_GB = "Cancel",
        es = "Cancelar",
        fi = "Peru",
        fr = "Annuler",
        it = "Annulla",
        nb = "Avbryt",
        nds = "Abbrechen",
        nl = "Annuleren",
        pt = "Cancelar",
        ru = "Отменить",
        sv = "Avbryt"
    };

    -- This is a message box title when prompting for confirmation when the
    --  the user clicks the Cancel button.
    ["Cancel installation"] = {
        cs = "Zrušit instalaci",
        de = "Installation abbrechen",
        en_GB = "Cancel installation",
        es = "Cancelar instalación",
        fi = "Peru asennus",
        fr = "Annuler l'installation",
        it = "Annullare l'installazione",
        nb = "Avbryt installasjonen",
        nds = "Installation abbrechen",
        nl = "Installatie afbreken?",
        pt = "Cancelar a instalação",
        ru = "Отменить установку",
        sv = "Avbryt installationen"
    };

    -- This error is reported for i/o failures while listing files contained
    --  in a .zip (or whatever) file.
    ["Couldn't enumerate archive"] = {
        cs = "Nemohu projít archiv",
        de = "Archiv kann nicht aufgelistet werden",
        en_GB = "Couldn't enumerate archive",
        es = "No se puede enumerar el archivo",
        fi = "Paketin tiedostojen listaus epäonnistui",
        fr = "Ne peux pas énumérer les fichiers de l'archive",
        nb = "Kunne ikke liste filer i arkivet",
        pt = "Foi impossível enumerar o arquivo",
        sv = "Kunde inte lista filen"
    };

    -- This error is reported for i/o failures while opening a .zip
    --  (or whatever) file.
    ["Couldn't open archive"] = {
        cs = "Nemohu otevřít archiv",
        de = "Archiv kann nicht geöffnet werden",
        en_GB = "Couldn't open archive",
        es = "No se puede abrir el archivo",
        fi = "Paketin purkaminen epäonnistui",
        fr = "Impossible d'ouvrir l'archive.",
        it = "Non posso aprire l'archivio",
        nb = "Kunne ikke åpne arkivet",
        nds = "Kann Archiv nicht öffnen",
        nl = "Kon archief niet openen",
        pt = "Foi impossível abrir o arquivo",
        ru = "Не могли открыть архив",
        sv = "Kunde inte öppna filen"
    };

    -- This is used by the stdio UI to choose a location to write files.
    --  A numbered list of options is printed, and the user may choose one by
    --  its number (default is number one), or enter their own text instead of
    --  choosing a default. This string is the instructions printed for the
    --  user before the prompt.
    --  This is used by the stdio UI to toggle options. A numbered list is
    --  printed, and the user can enter one of those numbers to toggle that
    --  option on or off. This string is the instructions printed for the
    --  user before the prompt.
    ["Choose number to change."] = {
        cs = "Zadejte číslo pro změnu.",
        de = "Wählen Sie eine Nummer zum Ändern.",
        en_GB = "Choose number to change.",
        es = "Elegir número para cambiar",
        fi = "Valitse muutettavan numero.",
        fr = "Entrez le numéro de l'option à changer.",
        nb = "Velg nummer som skal endres.",
        nds = "Wähle eine Nummer zum ändern",
        nl = "Kies een nummer om te veranderen",
        pt = "Escolha um número para alterar",
        ru = "Выберите номер чтобы поменять",
        sv = "Välj nummer som skall ändras."
    };

    -- As in "two different files want to use the same name." This is a title
    --  on a message box.
    ["Conflict!"] = {
        cs = "Konflikt!",
        de = "Konflikt!",
        en_GB = "Conflict!",
        es = "¡Conflicto!",
        fi = "Ristiriita!",
        fr = "Conflit !",
        nb = "Konflikt!",
        nds = "Konflikt!",
        nl = "Conflict!",
        pt = "Conflito!",
        ru = "Конфликт!",
        sv = "Konflikt!"
    };

    -- This is an error message shown to the user. When a file is to be
    --  overwritten, we move it out of the way instead, so we can restore it
    --  ("roll the file back") in case of problems, with the goal of having
    --  an installation that fails halfway through reverse any changes it made.
    --  This error is shown if we can't move a file out of the way.
    ["Couldn't backup file for rollback"] = {
        cs = "Nemohu zazálohovat soubor pro obnovu",
        de = "Konnte Datei nicht zur Wiederherstellung sichern",
        en_GB = "Couldn't backup file for rollback",
        es = "No se pudo guardar el archivo para inversión",
        fi = "Tiedoston varmuuskopionti ei onnistunut",
        nb = "Kunne ikke sikkerhetskopiere fil for tilbakerulling",
        nl = "Kon bestand niet backupen om de installatie terug te kunnen draaien",
        pt = "Foi impossível salvaguardar o ficheiro para refazer as acções",
        sv = "Kunde inte säkerhetskopiera filen för återställning"
    };

    -- This error is shown if we aren't able to write the list of files
    --  that were installed (the "manifest") to disk.
    ["Couldn't create manifest"] = {
        cs = "Nemohu vytvořit manifest",
        de = "Konnte Manifest nicht erstellen",
        en_GB = "Couldn't create manifest",
        es = "No se podía crear manifest",
        fi = "Asennusluettelon luominen epäonnistui",
        fr = "Echec de création du fichier manifest",
        nb = "Kunne ikke lage manifest",
        nds = "Erstellen des Manifests schlug fehl.",
        nl = "Kon het installatiemanifest niet maken",
        pt = "Foi impossível criar o manifesto",
        sv = "Kunde inte skapa manifest"
    };

    -- This is an error message. It speaks for itself.   :)
    --  Error message when deleting a file fails.
    ["Deletion failed!"] = {
        cs = "Mazání selhalo!",
        de = "Löschen fehlgeschlagen!",
        en_GB = "Deletion failed!",
        es = "¡Borrado fallido!",
        fi = "Tiedoston poisto epäonnistui!",
        fr = "Suppression de fichier échouée!",
        it = "Cancellazione fallita!",
        nb = "Kunne ikke slette!",
        nds = "Löschen fehlgeschlagen!",
        nl = "Verwijderen mislukt!",
        pt = "A remoção falhou!",
        ru = "Не смогли удалить!",
        sv = "Kunde inte radera!"
    };

    -- This is a label displayed next to the text entry in the GTK+ UI where
    --  the user specifies the installation destination (folder/directory) in
    --  the filesystem.
    ["Folder:"] = {
        cs = "Složka:",
        de = "Verzeichnis:",
        en_GB = "Folder:",
        es = "Carpeta:",
        fi = "Kansio:",
        fr = "Dossier :",
        it = "Cartella:",
        nb = "Katalog:",
        nds = "Ordner:",
        nl = "Map:",
        pt = "Directoria:",
        ru = "Папка:",
        sv = "Katalog:"
    };

    -- This is a window title when user is selecting a path to install files.
    ["Destination"] = {
        cs = "Cíl",
        de = "Ziel",
        en_GB = "Destination",
        es = "Destino",
        fi = "Kohde",
        fr = "Destination",
        it = "Destinazione",
        nb = "Destinasjon",
        nds = "Ziel",
        nl = "Bestemming",
        pt = "Destino",
        ru = "Назначение",
        sv = "Mål"
    };

    -- This is a window title while the program is downloading external files
    --  it needs from the network.
    ["Downloading"] = {
        cs = "Stahuji",
        de = "Lade herunter",
        en_GB = "Downloading",
        es = "Descargando",
        fi = "Noudetaan",
        fr = "Téléchargement en cours",
        it = "Download in corso",
        nb = "Laster ned",
        nds = "Lade herunter",
        nl = "Bezig met downloaden",
        pt = "A descarregar",
        ru = "Идёт скачивание",
        sv = "Hämtar"
    };

    -- Several UIs use this string as a prompt to the end-user when selecting
    --  a destination for newly-installed files.
    ["Enter path where files will be installed."] = {
        cs = "Zadejte cestu, kam mají být soubory nainstalovány.",
        de = "Geben Sie den Pfad an, wohin die Dateien installiert werden sollen.",
        en_GB = "Enter path where files will be installed.",
        es = "Introduce la ruta donde los archivos serán instalados",
        fi = "Syötä polku, johon tiedostot asennetaan.",
        fr = "Entrez le chemin d'installation souhaité pour vos fichiers.",
        it = "Inserisci il percorso dove installare i file.",
        nb = "Skriv inn destinasjonssti for installasjonen.",
        nds = "Pfad eingeben, in den installiert werden soll.",
        nl = "Geef de map op waar de bestanden geinstalleerd zullen worden",
        pt = "Introduza o caminho para os ficheiros que serão instalados.",
        sv = "Välj den mapp där filerna ska installeras."
    };

    -- Error message when a file we expect to load can't be read from disk.
    ["failed to load file '%0'"] = {
        cs = "nepodařilo se načíst soubor '%0'",
        de = "Laden von Datei '%0' fehlgeschlagen",
        en_GB = "failed to load file '%0'",
        es = "no se pudo leer el archivo '%0'",
        fi = "tiedoston \"%0\" lukeminen epäonnistui",
        fr = "Echec du chargement du fichier '%0'",
        nb = "kunne ikke laste fil '%0'",
        nds = "Fehler beim Laden von '%0'",
        nl = "laden van bestand \"%0\" mislukt",
        pt = "falhou a carregar o filheiro '%0'",
        ru = "не смогли загрузить файл '%0'",
        sv = "Misslyckades med att läsa filen '%0'"
    };

    -- This is a window title when something goes very wrong.
    ["Fatal error"] = {
        cs = "Kritická chyba",
        de = "Schwerer Fehler",
        en_GB = "Fatal error",
        es = "Error grave",
        fi = "Ylitsepääsemätön virhe",
        fr = "Erreur fatale",
        it = "Errore fatale",
        nb = "Fatal feil",
        nds = "Fataler Fehler",
        nl = "Fatale fout",
        pt = "Erro fatal",
        ru = "Фатальная ошибка",
        sv = "Allvarligt fel"
    };

    -- This is an error message when failing to write a file to disk.
    ["File creation failed!"] = {
        cs = "Nepodařilo se vytvořit soubor!",
        de = "Dateierstellung fehlgeschlagen!",
        en_GB = "File creation failed!",
        es = "¡Escritura de archivo ha fallado!",
        fi = "Tiedoston luominen epäonnistui!",
        fr = "Création de fichier échouée!",
        it = "Creazione file fallita",
        nb = "Kunne ikke lage fil!",
        nl = "aanmaken bestand mislukt!",
        pt = "Falhou a criação do ficheiro!",
        ru = "Не смогли сделать файл!",
        sv = "Misslyckades med att skapa fil!"
    };

    -- This is an error message when failing to get a file from the network.
    ["File download failed!"] = {
        cs = "Nepodařilo se stáhnout soubor!",
        de = "Dateidownload fehlgeschlagen!",
        en_GB = "File download failed!",
        es = "¡Descarga de archivo ha fallado!",
        fi = "Tiedoston noutaminen epäonnistui!",
        fr = "Téléchargement échoué!",
        it = "Download file fallito!",
        nb = "Kunne ikke laste ned fil!",
        nds = "Dateidownload fehlgeschlagen!",
        nl = "downloaden van bestand mislukt!",
        pt = "Falhou a descarregar!",
        ru = "Не смогли скачать файл!",
        sv = "Hämtning av fil misslyckades!"
    };

    -- This prompt is shown to users when we may overwrite an existing file.
    --  "%0" is the filename.
    ["File '%0' already exists! Replace?"] = {
        cs = "Soubor '%0' již existuje! Přepsat?",
        de = "Datei '%0' existiert bereits! Ersetzen?",
        en_GB = "File '%0' already exists! Replace?",
        es = "¡El archivo '%0' ya existe! ¿Sustituirlo?",
        fi = "Tiedosto \"%0\" on jo olemassa! Korvaa?",
        fr = "Le fichier '%0' existe déjà! Le remplacer?",
        it = "Il file %0 esiste già! Rimpiazzare?",
        nb = "Filen '%0' eksisterer allerede! Skrive over?",
        nds = "Datei '%0' existiert bereits! Überschreiben?",
        nl = "Bestand \"%0\" bestaat al! Vervangen?",
        pt = "O ficheiro '%0' já existe! Substituir?",
        ru = "Уже есть файл '%0'. Хотите заменить?",
        sv = "Filen '%0' existerar redan! Vill du ersätta filen?"
    };

    -- This is a button in the UI. It replaces "Next" when there are no more
    --  stages to move forward to.
    ["Finish"] = {
        cs = "Dokončit",
        de = "Fertig stellen",
        en_GB = "Finish",
        es = "Terminar",
        fi = "Valmis",
        fr = "Terminer",
        it = "Fine",
        nb = "Ferdig",
        nds = "Fertig",
        nl = "Voltooien",
        pt = "Finalizar",
        ru = "Закончить",
        sv = "Slutför"
    };

    -- This error message is (hopefully) shown to the user if the UI
    --  subsystem can't create the main application window.
    ["GUI failed to start"] = {
        cs = "Nepodařilo se spustit GUI",
        de = "GUI konnte nicht gestartet werden",
        en_GB = "GUI failed to start",
        es = "El interfaz gráfico de usuario ha fallado al arrancar",
        fi = "Graafinen käyttöliittymä ei käynnistynyt",
        fr = "Echec du démarrage de l'interface",
        nb = "Kunne ikke starte grafisk grensesnitt",
        nds = "Fehler beim starten der GUI",
        nl = "Opstarten GUI mislukt",
        pt = "O GUI falhou a iniciar",
        ru = "Графический елемент не змог запустится",
        sv = "Kunde inte starta grafisk gränssnitt"
    };

    -- This message is shown to the user if an installation encounters a fatal
    --  problem (or the user clicked "cancel"), telling them that we'll try
    --  to put everything back how it was before we started.
    ["Incomplete installation. We will revert any changes we made."] = {
        cs = "Instalace nebyla dokončena. Vše bude uvedeno do původního stavu.",
        de = "Unvollständige Installation. Änderungen werden rückgängig gemacht.",
        en_GB = "Incomplete installation. We will revert any changes we made.",
        es = "Instalación incompleta. Vamos a deshacer cualquier cambio que hayamos hecho.",
        fi = "Asennus jäi kesken. Tehdyt muutokset perutaan.",
        fr = "Installation incomplète. Tous les changements effectués seront annulés.",
        nb = "Installasjonen ble ikke ferdig. Vi vil tilbakestille alle endringer som ble gjort.",
        nl = "Incomplete installatie. De veranderingen zullen ongedaan gemaakt worden.",
        pt = "Instalação incompleta. Todas as alterações serão desfeitas.",
        ru = "Не змогли установить программу. Мы отменим все сделанные изменения.",
        sv = "Ofullständig installation. Återställning av alla gjorda ändringar utförs."
    };

    -- Reported to the user if everything worked out.
    ["Installation was successful."] = {
        cs = "Instalace byla úspěšná.",
        de = "Installation war erfolgreich.",
        en_GB = "Installation was successful.",
        es = "La instalación fue un éxito.",
        fi = "Asentaminen onnistui.",
        fr = "Installation réussie.",
        it = "Installazione riuscita.",
        nb = "Installasjonen var en suksess.",
        nds = "Die Installation war erfolgreich",
        nl = "De installatie is succesvol verlopen.",
        pt = "A instalação foi bem sucedida.",
        ru = "Установка была успешна.",
        sv = "Installationen lyckades."
    };

    -- This is a window title, shown while the actual installation to disk
    --  is in process and a progress meter is being shown.
    ["Installing"] = {
        cs = "Instaluji",
        de = "Installiere",
        en_GB = "Installing",
        es = "Instalando",
        fi = "Asennetaan",
        fr = "En cours d'installation",
        it = "Installazione in corso",
        nb = "Installerer",
        nds = "Installiere ...",
        nl = "Installeren",
        pt = "A instalar",
        ru = "Устанавливаем",
        sv = "Installerar"
    };

    -- This is a window title, shown while the user is choosing
    --  installation-specific options.
    ["Options"] = {
        cs = "Volby",
        de = "Optionen",
        en_GB = "Options",
        es = "Opciones",
        fi = "Valinnat",
        fr = "Options",
        it = "Opzioni",
        nb = "Valg",
        nds = "Optionen",
        nl = "Opties",
        pt = "Opções",
        ru = "Параметры",
        sv = "Alternativ"
    };

    -- Shown as an option in the ncurses UI as the final element in a list of
    --  default filesystem paths where a user may install files. They can
    --  choose this to enter a filesystem path manually.
    ["(I want to specify a path.)"] = {
        cs = "(Chci určit cestu.)",
        de = "(Ich möchte einen Pfad angeben.)",
        en_GB = "(I want to specify a path.)",
        es = "(Quiero especificar una ruta.)",
        fi = "(Haluan syöttää polun.)",
        fr = "(Je veux spécifier une destination.)",
        it = "(Voglio specificare un percorso)",
        nb = "(Jeg vil skrive inn min egen sti.)",
        nl = "(Ik wil een pad specificeren.)",
        pt = "(Eu quero especificar o caminho.)",
        sv = "(Jag vill skriva in en egen sökväg.)"
    };

    -- "kilobytes per second" ... download rate.
    ["KB/s"] = {
        cs = "KB/s",
        de = "KB/s",
        en_GB = "KB/s",
        es = "KB/s",
        fi = "kt/s",
        fr = "Ko/s",
        it = "KB/s",
        nb = "KB/s",
        nds = "kB/s",
        nl = "KB/s",
        pt = "KB/s",
        ru = "кб/с",
        sv = "KB/s"
    };

    -- "bytes per second" ... download rate.
    ["B/s"] = {
        cs = "B/s",
        de = "B/s",
        en_GB = "B/s",
        es = "B/s",
        fi = "t/s",
        fr = "octets/s",
        it = "B/s",
        nb = "B/s",
        nds = "B/s",
        nl = "B/s",
        pt = "B/s",
        ru = "б/с",
        sv = "B/s"
    };

    -- Download rate when we don't know the goal (can't report time left).
    --  This is a number (%0) followed by the localized "KB/s" or "B/s" (%1).
    ["%0 %1"] = {
        cs = "%0 %1",
        de = "%0 %1",
        en_GB = "%0 %1",
        es = "%0 %1",
        fi = "%0 %1",
        fr = "%0 %1",
        it = "%0 %1",
        nb = "%0 %1",
        nds = "%0 %1",
        nl = "%0 %1",
        pt = "%0 %1",
        ru = "0% %1",
        sv = "%0 %1"
    };

    -- Download rate when we know the goal (can report time left).
    --  This is a number (%0) followed by the localized "KB/s" or "B/s" (%1),
    --  then the hours (%2), minutes (%3), and seconds (%4) remaining
    ["%0 %1, %2:%3:%4 remaining"] = {
        cs = "%0 %1, %2:%3:%4 zbývá",
        de = "%0 %1, %2:%3:%4 verbleibend",
        en_GB = "%0 %1, %2:%3:%4 remaining",
        es = "%0 %1, %2:%3:%4 restantes",
        fi = "%0 %1, %2:%3:%4 jäljellä",
        fr = "%0 %1, %2:%3:%4 restantes",
        nb = "%0 %1, %2:%3:%4 igjen",
        nds = "%0 %1, %2:%3:%4 verbleiben",
        nl = "%0 %1, %2:%3:%4 resterend",
        pt = "restam %0 %1, %2:%3:%4",
        ru = "%0 %1, осталось %2:%3:%4",
        sv = "%0 %1, %2:%3:%4 återstår"
    };

    -- download rate when download isn't progressing at all.
    ["stalled"] = {
        cs = "zaseknuté",
        de = "wartend",
        en_GB = "stalled",
        es = "parado",
        fi = "seisahtunut",
        fr = "bloqué",
        it = "in stallo",
        nb = "står fast",
        nds = "Angehalten",
        nl = "geen progressie",
        pt = "Parado",
        ru = "застряло",
        sv = "avstannad"
    };

    -- Download progress string: filename (%0), percent downloaded (%1),
    --  download rate determined in one of the above strings (%2).
    ["%0: %1%% (%2)"] = {
        cs = "%0: %1%% (%2)",
        de = "%0: %1%% (%2)",
        en_GB = "%0: %1%% (%2)",
        es = "%0: %1%% (%2)",
        fi = "%0: %1%% (%2)",
        fr = "%0: %1%% (%2)",
        nb = "%0: %1%% (%2)",
        nds = "%0: %1%% (%2)",
        nl = "%0: %1%% (%2)",
        pt = "%0: %1%% (%2)",
        ru = "%0: %1%% (%2)",
        sv = "%0: %1%% (%2)"
    };

    -- This is a window title when prompting the user to insert a new disc.
    ["Media change"] = {
        cs = "Výměna média",
        de = "Medienwechsel",
        en_GB = "Media change",
        es = "Cambio de disco",
        fi = "Median vaihto",
        fr = "Changement de média",
        nb = "Mediaendring",
        nl = "Verwissel medium",
        pt = "Mudança de media",
        sv = "Mediabyte"
    };

    -- This error message is shown to the end-user when we can't make a new
    --  folder/directory in the filesystem.
    ["Directory creation failed"] = {
        cs = "Selhalo vytváření adresáře",
        de = "Erstellung eines Verzeichnisses fehlgeschlagen",
        en_GB = "Directory creation failed",
        es = "Fallo en la creación de directorio",
        fi = "Hakemiston luominen epäonnistui",
        fr = "Création de répertoire échouée",
        it = "Creazione directory fallita",
        nb = "Kunne ikke lage katalog",
        nl = "Aanmaken folder mislukt",
        pt = "A criação da directoria falhou",
        ru = "Не смогли сделать папку",
        sv = "Kunde inte skapa katalog"
    };

    -- This is a GTK+ button label. The '_' comes before the hotkey character.
    --  "No" would take the 'N' hotkey in English.
    ["N_ever"] = {
        cs = "Ni_kdy",
        de = "Ni_emals",
        en_GB = "N_ever",
        es = "N_unca",
        fi = "Ei _koskaan",
        fr = "_Jamais",
        it = "_Mai",
        nb = "Al_dri",
        nds = "N_ie",
        pt = "N_unca",
        ru = "Никогда",
        sv = "Al_drig"
    };

    -- This is a GUI button label, to move forward to the next stage of
    --  installation. It's opposite is "Back" in this case.
    ["Next"] = {
        cs = "Další",
        de = "Weiter",
        en_GB = "Next",
        es = "Siguiente",
        fi = "Seuraava",
        fr = "Suivant",
        it = "Avanti",
        nb = "Neste",
        nds = "Weiter",
        nl = "Volgende",
        pt = "Seguinte",
        ru = "Далее",
        sv = "Nästa"
    };

    -- This is a GUI button label, indicating a negative response.
    ["No"] = {
        cs = "Ne",
        de = "Nein",
        en_GB = "No",
        es = "No",
        fi = "Ei",
        fr = "Non",
        it = "No",
        nb = "Nei",
        nds = "Nein",
        nl = "Nee",
        pt = "Não",
        ru = "Нет",
        sv = "Nej"
    };

    -- This is a GUI button label, indicating a positive response.
    ["Yes"] = {
        cs = "Ano",
        de = "Ja",
        en_GB = "Yes",
        es = "Sí",
        fi = "Kyllä",
        fr = "Oui",
        it = "Si",
        nb = "Ja",
        nds = "Ja",
        nl = "Ja",
        pt = "Sim",
        ru = "Да",
        sv = "Ja"
    };

    -- HTTP error message in the www UI, as in "404 Not Found" ... requested
    --  file is missing.
    ["Not Found"] = {
        cs = "Nenalezeno",
        de = "Nicht gefunden",
        en_GB = "Not Found",
        es = "No encontrado",
        fi = "Ei löytynyt",
        fr = "Introuvable",
        it = "Non Trovato",
        nb = "Ikke funnet",
        nds = "Nicht gefunden",
        nl = "Niet Gevonden",
        pt = "Inexistente",
        ru = "Не найдено",
        sv = "Hittades inte"
    };

    -- This is reported to the user when there are no files to install, and
    --  thus no installation to go forward.
    ["Nothing to do!"] = {
        cs = "Není co instalovat!",
        de = "Nichts zu tun!",
        en_GB = "Nothing to do!",
        es = "¡Nada que hacer!",
        fi = "Ei tehtävää!",
        fr = "Rien à faire!",
        nb = "Ingenting å gjøre!",
        nds = "Nichts zu tun!",
        nl = "Niets te doen!",
        pt = "Nada por fazer!",
        ru = "Нечего делать!",
        sv = "Ingenting att göra!"
    };

    -- This is a GUI button label, sometimes paired with "Cancel"
    ["OK"] = {
        cs = "OK",
        de = "OK",
        en_GB = "OK",
        es = "Aceptar",
        fi = "Hyväksy",
        fr = "OK",
        it = "OK",
        nb = "OK",
        nds = "OK",
        nl = "OK",
        pt = "Aceitar",
        ru = "Далее",
        sv = "OK"
    };

    -- This is displayed when the application has a serious problem, such as
    --  crashing again in the crash handler, or being unable to find basic
    --  files it needs to get started. It may be a window title, or written
    --  to stdout, or whatever, but it's basically meant to be a title or
    --  header, with more information to follow later.
    ["PANIC"] = {
        cs = "SELHÁNÍ",
        de = "PANIK",
        en_GB = "PANIC",
        es = "PÁNICO",
        fi = "HÄTÄTILA",
        fr = "PANIC",
        nb = "PANIKK",
        nds = "PANIK",
        nl = "PROBLEEM",
        pt = "PÂNICO",
        ru = "ПАНИКА",
        sv = "PANIK"
    };

    -- Prompt shown to user when we need her to insert a new disc.
    ["Please insert '%0'"] = {
        cs = "Prosím vložte '%0'",
        de = "Bitte legen Sie '%0' ein",
        en_GB = "Please insert '%0'",
        es = "Por favor introduce '%0'",
        fi = "Syötä \"%0\"",
        fr = "Veuillez insérer '%0'",
        nb = "Sett inn '%0'",
        nds = "Bitte '%0' in das Laufwerk legen.",
        nl = "Laadt '%0' a.u.b.",
        pt = "Por favor insira '%0'",
        sv = "Sätt in '%0'"
    };

    -- Prompt shown to user in the stdio UI when we need to pause before
    --  continuing, usually to let them read the outputted text that is
    --  scrolling by.
    ["Press enter to continue."] = {
        cs = "Pro pokračování stiskněte enter.",
        de = "Drücken Sie Enter um fortzufahren",
        en_GB = "Press enter to continue.",
        es = "Presione Intro para continuar.",
        fi = "Jatka painamalla enter.",
        fr = "Veuillez appuyer sur Entrée pour continuer.",
        it = "Premere invio per continuare.",
        nb = "Trykk enter for å fortsette.",
        nds = "Drücke Enter, um fortzufahren.",
        nl = "Toets enter om door te gaan",
        pt = "Pressione enter para continuar.",
        sv = "Tryck enter för att fortsätta."
    };

    -- This is a window title when informing the user that something
    --  important has gone wrong (such as being unable to revert changes
    --  during a rollback).
    ["Serious problem"] = {
        cs = "Závažný problém",
        de = "Ernstes Problem",
        en_GB = "Serious problem",
        es = "Problema grave",
        fi = "Vakava ongelma",
        fr = "Problème grave",
        nb = "Alvorlig problem",
        nds = "Ernstzunehmender Fehler",
        nl = "Serieus probleem",
        pt = "Problema sério",
        sv = "Allvarligt problem"
    };

    -- The www UI uses this as a page title when the program is terminating.
    ["Shutting down..."] = {
        cs = "Ukončuji se...",
        de = "Schließe...",
        en_GB = "Shutting down...",
        es = "Cerrando ...",
        fi = "Ajetaan alas...",
        fr = "Fermeture en cours...",
        nb = "Avslutter...",
        nl = "Afsluiten...",
        pt = "A terminar...",
        sv = "Avslutar..."
    };

    -- The www UI uses this as page text when the program is terminating.
    ["You can close this browser now."] = {
        cs = "Nyní můžete ukončit tento prohlížeč.",
        de = "Sie können diesen Browser nun schließen.",
        en_GB = "You can close this browser now.",
        es = "Puedes cerrar este navegador ahora.",
        fi = "Voit sulkea tämän selaimen.",
        fr = "Vous pouvez maintenant fermer ce navigateur",
        it = "Puoi chiudere il browser adesso.",
        nb = "Du kan lukke denne nettleseren nå.",
        nds = "Sie können den Browser nun schließen.",
        nl = "U kunt dit scherm nu sluiten.",
        pt = "Já pode fechar este browser.",
        sv = "Du kan stänga denna webbläsare nu."
    };

    -- Error message shown to end-user when we can't write a symbolic link
    --  to the filesystem.
    ["Symlink creation failed!"] = {
        cs = "Selhalo vytváření symbolického odkazu!",
        de = "Erzeugung einer Verknüpfung fehlgeschlagen!",
        en_GB = "Symlink creation failed!",
        es = "¡Creación de enlace simbólico ha fallado!",
        fi = "Symbolisen linkin luominen epäonnistui!",
        fr = "Création de lien symbolique échouée!",
        it = "Creazione del collegamento fallita!",
        nb = "Kunne ikke lage symbolsk lenke!",
        nl = "Aanmaken Symlink mislukt!",
        pt = "A criação de uma ligação simbólica falhou!",
        sv = "Kunde inte skapa symbolisk länk!"
    };

    -- Error message shown to the end-user when the OS has requested
    --  termination of the program (SIGINT/ctrl-c on Unix, etc).
    ["The installer has been stopped by the system."] = {
        cs = "Instalátor byl zastaven signálem ze systému.",
        de = "Das Installationsprogramm wurde vom System gestoppt.",
        en_GB = "The installer has been stopped by the system.",
        es = "El instalador ha sido interrumpido por el sistema.",
        fi = "Järjestelmä pysäytti asennusohjelman.",
        fr = "L'installeur a été interrompu par le système.",
        nb = "Installasjonsprogrammet ble stoppet av systemet.",
        nl = "Het installatieprogramma is door het systeem gestopt.",
        pt = "O instalador foi parado pelo sistema.",
        sv = "Installationsprogrammet blev stoppat av systemet."
    };

    -- Error message shown to the end-user when the program crashes with a
    --  bad memory access (segfault on Unix, GPF on Windows, etc).
    ["The installer has crashed due to a bug."] = {
        cs = "Instalátor zhavaroval vinou vlastní chyby.",
        de = "Das Installationsprogramm ist aufgrund eines Fehlers abgestürzt.",
        en_GB = "The installer has crashed due to a bug.",
        es = "El instalador se ha bloqueado por un fallo de memoria.",
        fi = "Bugi kaatoi asennusohjelman.",
        fr = "L'installateur a quitté inopinément à cause d'un bogue.",
        nb = "Installasjonsprogrammet kræsjet pga. en feil.",
        nl = "Het installatieprogramma is door een bug gecrasht.",
        pt = "O instalador terminou acidentalmente devido a um erro.",
        sv = "Installationsprogrammet kraschade pga. ett fel."
    };

    -- This is a button label in the ncurses ui to flip an option on/off.
    ["Toggle"] = {
        cs = "Přepnout",
        de = "Umschalten",
        en_GB = "Toggle",
        es = "Conmutar",
        fi = "Vipu",
        fr = "Basculer",
        it = "Attiva/Disattiva",
        nb = "Inverter valg",
        nl = "Aan/Uit",
        pt = "Alternar",
        sv = "Växla"
    };

    -- This is an error message shown to the end-user when there is an
    --  unexpected entry in a .zip (or whatever) file that we didn't know
    --  how to handle.
    ["Unknown file type in archive"] = {
        cs = "Neznámý typ souboru v archivu.",
        de = "Unbekannter Dateityp im Archiv",
        en_GB = "Unknown file type in archive",
        es = "Tipo de fichero desconocido en el archivo",
        fi = "Paketissa on tiedosto, jonka tyyppiä ei tunneta.",
        fr = "Type de fichier inconnu dans cette archive.",
        nb = "Ukjent filtype i arkivet",
        nl = "Onbekend bestandstype in archief",
        pt = "Tipo de ficheiro desconhecido no arquivo",
        sv = "Okänd filtyp påträffad i arkivet"
    };

    -- This is an error message shown to the end-user if they refuse to
    --  agree to the license of the software they are try to install.
    ["You must accept the license before you may install"] = {
        cs = "Před instalací je nutné odsouhlasit licenci",
        de = "Sie müssen den Lizenzbedingungen zustimmen, bevor sie installieren können",
        en_GB = "You must accept the licence before you may install",
        es = "Tienes que aceptar la licencia antes de que puedas instalar",
        fi = "Asennusta ei suoriteta, ellet hyväksy lisenssiä.",
        fr = "Vous devez accepter la licence avant de pouvoir installer",
        nb = "Lisensen må godkjennes før du kan installere",
        nds = "Du must den Lizenzbedingungen zustimmen, um die Installation fortzusetzen",
        nl = "U moet akkoord gaan met de licentie om te kunnen installeren",
        pt = "Tem que aceitar a licença antes de prosseguir com a instalação",
        sv = "Du måste acceptera licensavtalet innan du kan fortsätta med installationen"
    };

    -- Installations display the currently-installing component, such as
    --  "Base game" or "Bonus pack content" or whatnot. The installer lists
    --  the current component as "Metadata" when writing out its own
    --  information, such as file manifests, uninstall support, etc.
    ["Metadata"] = {
        cs = "Metadata",
        de = "Metadaten",
        en_GB = "Metadata",
        es = "Metadatos",
        fi = "Metadata",
        fr = "Métadonnées",
        it = "Metadati",
        nb = "Metadata",
        nl = "Metadata",
        pt = "Meta-Informação",
        sv = "Metadata"
    };

    -- This error is shown if incorrect command line arguments are used.
    ["Invalid command line"] = {
        cs = "Neplatné argumenty",
        de = "Ungültige Kommandozeile",
        en_GB = "Invalid command line",
        es = "Parámetros de comando incorrectos",
        fi = "Virheellinen komentorivi",
        fr = "Ligne de commande invalide",
        it = "Linea di comando non valida",
        nb = "Ugyldig kommandolinje",
        nl = "Ongeldige opdrachtregel",
        pt = "Argumento da linha de comandos inválido",
        sv = "Ogiltigt argument"
    };

    -- This error is shown when updating the manifest, if it can't load the
    --  file for some reason. '%0' is the manifest's package name.
    ["Couldn't load manifest file for '%0'"] = {
        cs = "Nemohu načíst soubor s manifestem pro '%0'",
        de = "Konnte Manifestdatei für '%0' nicht laden",
        en_GB = "Couldn't load manifest file for '%0'",
        es = "No pudo cargar el archivo manifest de '%0'",
        fi = "Asennusluetteloa paketille \"%0\" ei kyetty lukemaan.",
        nb = "Kunne ikke laste manifestfil for '%0'",
        pt = "Foi impossível carregar o ficheiro de manifesto para '%0'",
        sv = "Kunde inte ladda manifestfil för '%0'"
    };

    -- This error is shown when the user prompted the app to read a filename
    --  (%0) that doesn't exist.
    ["File %0 not found"] = {
        cs = "Soubor '%0' nebyl nalezen",
        de = "Datei %0 nicht gefunden.",
        en_GB = "File %0 not found",
        es = "Archivo %0 no encontrado",
        fi = "Tiedostoa \"%0\" ei ole.",
        fr = "Fichier %0 introuvable",
        nb = "Filen '%0' ble ikke funnet",
        nds = "Datei %0 wurde nicht gefunden",
        nl = "Bestand %0 niet gevonden",
        pt = "O ficheiro %0 não foi encontrado",
        sv = "Filen '%0' hittades inte"
    };

    -- This is a window title on the message box when asking if user is sure
    --  they want to uninstall a package.
    ["Uninstall"] = {
        cs = "Odinstalovat",
        de = "Deinstallieren",
        en_GB = "Uninstall",
        es = "Desinstalar",
        fi = "Asennuksen poisto",
        fr = "Désinstaller",
        it = "Rimuovi",
        nb = "Avinstallasjon",
        nds = "Deinstalleren",
        nl = "Deïnstalleren",
        pt = "Desinstalar",
        sv = "Avinstallera"
    };

    -- This is the text when asking the user if they want to uninstall
    --  the package named '%0'.
    ["Are you sure you want to uninstall '%0'?"] = {
        cs = "Opravdu chcete odinstalovat '%0'?",
        de = "Sind Sie sicher, dass Sie '%0' deinstallieren wollen?",
        en_GB = "Are you sure you want to uninstall '%0'?",
        es = "¿Estás seguro de que quieres desinstalar '%0'?",
        fi = "Haluatko varmasti poistaa asennuksen \"%0\"?",
        fr = "Êtes-vous sûr de vouloir désinstaller '%0' ?",
        nb = "Er du sikker på at du vil avinstallere '%0'?",
        nl = "Bent u er zeker van dat u '%0' wilt deïnstalleren?",
        pt = "Tem a certeza que quer desinstalar '%0'?",
        sv = "Är du säker på att du vill avinstallera '%0'?"
    };

    -- This is a window title, shown while the actual uninstall is in process
    --  and a progress meter is being shown.
    ["Uninstalling"] = {
        cs = "Odinstalovávám",
        de = "Deinstalliere",
        en_GB = "Uninstalling",
        es = "Desinstalando",
        fi = "Poistetaan asennusta",
        fr = "Désinstallation en cours",
        nb = "Avinstallerer",
        nds = "Deinstallation",
        nl = "Deïnstallatie",
        pt = "A desinstalar",
        sv = "Avinstallerar"
    };

    -- This is shown to the user in a message box when uninstallation is done.
    ["Uninstall complete"] = {
        cs = "Odinstalace dokončena",
        de = "Deinstallation abgeschlossen",
        en_GB = "Uninstall complete",
        es = "Desinstalación terminada",
        fi = "Asennus poistettu",
        fr = "Désinstallation complète",
        nb = "Avinstallasjonen er ferdig",
        nds = "Deinstallation abgeschlossen",
        nl = "Deïnstallatie voltooid",
        pt = "A desinstalação foi bem sucedida",
        sv = "Avinstallation slutförd"
    };

    -- This is written to the terminal in the ncurses UI when the window is
    --  too thin to be usable; it's asking the user to resize the terminal
    --  window horizontally. Since space may be extremely tight, try to be as
    --  terse as possible (but we can wrap the text if possible, so don't be
    --  cryptic). The '[' and ']' characters are just decoration to imply this
    --  is a system problem outside the scope of the application, but they
    --  aren't required.
    ["[Make the window wider!]"] = {
        cs = "[Zvětšit šířku okna!]",
        de = "[Fenster breiter machen!]",
        en_GB = "[Make the window wider!]",
        es = "[¡Ensancha la ventana!]",
        fi = "[Levennä ikkunaa!]",
        fr = "[Élargissez la fenêtre!]",
        nb = "[Gjør vinduet bredere!]",
        nl = "[Maak het scherm breder!]",
        pt = "[Torne a janela mais larga!]",
        sv = "[Gör fönstret bredare!]"
    };

    -- This is written to the terminal in the ncurses UI when the window is
    --  too short to be usable; it's asking the user to resize the terminal
    --  window vertically. Since space may be extremely tight, try to be as
    --  terse as possible (but we can wrap the text if possible, so don't be
    --  cryptic). The '[' and ']' characters are just decoration to imply this
    --  is a system problem outside the scope of the application, but they
    --  aren't required.
    ["[Make the window taller!]"] = {
        cs = "[Zvětšit výšku okna!]",
        de = "[Fenster höher machen!]",
        en_GB = "[Make the window taller!]",
        es = "[¡Estira la ventana!]",
        fi = "[Tee ikkunasta korkeampi!]",
        fr = "[Agrandissez la fenêtre!]",
        nb = "[Gjør vinduet høyere!]",
        nl = "[Maak het scherm langer!]",
        pt = "[Torne a janela mais alta!]",
        sv = "[Gör fönstret högre!]"
    };

    -- This is written out if we failed to add an item to the desktop
    --  application menu (or "Start" bar on Windows, or maybe the Dock on
    --  Mac OS X, etc).
    ["Failed to install desktop menu item"] = {
        cs = "Nepodařilo se nainstalovat položku do menu",
        de = "Konnte Verknüpfung für Desktop-Menü nicht installieren",
        en_GB = "Failed to install desktop menu item",
        es = "Fallo al añadir un elemento al menú de escritorio",
        fi = "Kohdan lisääminen työpöytävalikkoon epäonnistui.",
        fr = "Echec de l'installation des éléments du bureau",
        nl = "Installatie van het bureaublad menu item is mislukt",
        pt = "A instalação do menu no desktop falhou",
        sv = "Misslycklades med att lägga till genväg i programmenyn"
    };

    -- This is written out if we failed to remove an item from the desktop
    --  application menu (or "Start" bar on Windows, or maybe the Dock on
    --  Mac OS X, etc).
    ["Failed to uninstall desktop menu item"] = {
        cs = "Nepodařilo se odstranit položku z menu",
        de = "Konnte Verknüpfung für Desktop-Menü nicht deinstallieren",
        en_GB = "Failed to uninstall desktop menu item",
        es = "Fallo al quitar un elemento del menú de escritorio",
        fi = "Kohdan poistaminen työpöytävalikkosta epäonnistui.",
        fr = "Echec de la désinstallation des éléments du bureau",
        nl = "Deïnstallatie van het bureaublad menu item is mislukt",
        pt = "A desinstalação do menu no desktop falhou",
        sv = "Misslyckades med att ta bort genväg i programmenyn"
    };
};

-- end of localization.lua ...

