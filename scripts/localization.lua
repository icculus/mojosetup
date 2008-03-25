-- MojoSetup; a portable, flexible installation application.
--
-- Please see the file LICENSE.txt in the source's root directory.
--
-- DO NOT EDIT BY HAND.
-- This file was generated with po2localization.pl, version svn-560 ...
--  on 2008-03-25 02:55:01-0400
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
-- X-Launchpad-Export-Date: 2008-03-25 03:44+0000
-- X-Generator: Launchpad (build Unknown)

MojoSetup.languages = {
    en_US = "English (United States)",
    cs = "Czech",
    da = "Danish",
    de = "German",
    el = "Greek",
    en_AU = "English (Australia)",
    en_CA = "English (Canada)",
    en_GB = "English (United Kingdom)",
    es = "Spanish",
    et = "Estonian",
    fi = "Finnish",
    fr = "French",
    hu = "Hungarian",
    it = "Italian",
    ja = "Japanese",
    nb = "Norwegian Bokmal",
    nds = "German, Low",
    nl = "Dutch",
    pl = "Polish",
    pt = "Portuguese",
    pt_BR = "Brazilian Portuguese",
    ru = "Russian",
    sk = "Slovak",
    sv = "Swedish",
    tr = "Turkish",
    uk = "Ukrainian",
    zh_TW = "Traditional Chinese"
};

MojoSetup.localization = {
    -- This may be shown to the end-user if the application gets extremely
    --  confused. It may be shown in desperate sitations when we know
    --  something has gone wrong, but not what exactly.
    ["unknown error"] = {
        cs = "neznámá chyba",
        da = "ukendt fejl",
        de = "Unbekannter Fehler",
        el = "άγνωστο σφάλμα",
        en_AU = "unknown error",
        en_CA = "unknown error",
        en_GB = "unknown error",
        es = "error desconocido",
        et = "tundmatu viga",
        fi = "tuntematon virhe",
        fr = "Erreur inconnue",
        hu = "ismeretlen hiba",
        it = "errore sconosciuto",
        ja = "不明なエラー",
        nb = "ukjent feil",
        nds = "unbekannter Fehler",
        nl = "onbekende fout",
        pl = "nieznany błąd",
        pt = "erro desconhecido",
        pt_BR = "erro desconhecido",
        ru = "неизвестная ошибка",
        sk = "neznáma chyba",
        sv = "okänt fel",
        tr = "bilinmeyen hata",
        uk = "невідома помилка",
        zh_TW = "未知的錯誤"
    };

    -- stdio UI plugin says this for "OK"-only msgboxes. "%0" is the message
    --  box's text content. "[hit enter]" should be translated, too! It's
    --  trying to tell the application's user that they should press the
    --  key that is normally used to complete a line of text at a terminal's
    --  prompt.
    ["NOTICE: %0\n[hit enter]"] = {
        cs = "UPOZORNĚNÍ: %0\n[stiskněte enter]",
        da = "NB: %0\n[tryk enter]",
        de = "HINWEIS: %0\n[Drücken Sie Enter]",
        el = "ΠΡΟΣΟΧΗ: %0\n[πατήστε ENTER]",
        en_AU = "NOTICE %0",
        en_CA = "NOTICE: %0\n[hit enter]",
        en_GB = "NOTICE: %0\n[hit enter]",
        es = "AVISO: %0\n[presiona Intro]",
        fi = "ILMOITUS: %0\n[paina enter]",
        fr = "INFORMATION : %0\n[pressez entrée]",
        hu = "FIGYELEM: %0\n[üss entert]",
        it = "ATTENZIONE: %0",
        ja = "通知：0％\n「エンターを打つ",
        nb = "NB: %0\n[trykk enter]",
        nds = "HINWEIS: %0",
        nl = "OPMERKING: %0\n[Toets enter]",
        pl = "UWAGA: %0\n[hit enter]",
        pt = "AVISO: %0\n[pressione enter]",
        pt_BR = "AVISO: %0",
        ru = "УВЕДОМЛЕНИЕ: %0",
        sk = "Správa: %0",
        sv = "NOTERA: %0\n[tryck enter]",
        tr = "UYARI: %0\n[giriş tuşuna basınız]",
        uk = "ДО УВАГИ: %0",
        zh_TW = "提醒：%0"
    };

    -- stdio UI plugin says this for yes/no prompts that default to yes.
    --  "%0" is the question the user is being asked to respond to.
    --  "[Y/n]" are two options that the user will have to type in response.
    --  Please pick reasonable abbreviations for YES and NO that can be
    --  typed easily by the user. Case of the "Y" and "n" are not important,
    --  although we are using the capital letter to show the default option
    --  if the user just presses enter. Be sure to translate the strings "Y"
    --  and "N", elsewhere, to match what you enter here! At runtime, the
    --  user's response is compared to those strings without case sensitivity.
    ["%0 [Y/n]: "] = {
        cs = "%0 [A/n]: ",
        da = "%0 [J/n]: ",
        de = "%0 [J/n]: ",
        el = "%0 [Ν/ο]: ",
        en_AU = "%0 [Y/n]: ",
        en_CA = "%0 [Y/n]: ",
        en_GB = "%0 [Y/n] ",
        es = "%0 [S/n]: ",
        fi = "%0 [K/e]: ",
        fr = "%0 [O/n]: ",
        hu = "%0 [I/n]: ",
        it = "%0 [S/n]: ",
        ja = "%0「y/N」 ",
        nb = "%0 [J/n]: ",
        nds = "%0 [J/n] ",
        nl = "%0 [J/n]: ",
        pl = "%0 [T/n]: ",
        pt = "%0 [S/n] ",
        pt_BR = "%0 [S/n]: ",
        ru = "%0 [Y/n]: ",
        sk = "%0 [A/n]: ",
        sv = "%0 [J/n]: ",
        tr = "%0 [E/h]]: ",
        uk = "%0 [ТАК/ні] ",
        zh_TW = "%0 [Y/n]: "
    };

    -- stdio UI plugin says this for yes/no prompts that default to no.
    --  "%0" is the question the user is being asked to respond to.
    --  "[y/N]" are two options that the user will have to type in response.
    --  Please pick reasonable abbreviations for YES and NO that can be
    --  typed easily by the user. Case of the "Y" and "n" are not important,
    --  although we are using the capital letter to show the default option
    --  if the user just presses enter. Be sure to translate the strings "Y"
    --  and "N", elsewhere, to match what you enter here! At runtime, the
    --  user's response is compared to those strings without case sensitivity.
    ["%0 [y/N]: "] = {
        cs = "%0 [a/N] ",
        da = "%0 [j/N]: ",
        de = "%0 [j/N]: ",
        el = "%0 [ν/Ο]: ",
        en_AU = "%0 [y/N]: ",
        en_CA = "%0 [y/N]: ",
        en_GB = "%0 [y/N]: ",
        es = "%0 [s/N]: ",
        fi = "%0 [k/E]: ",
        fr = "%0 [o/N]: ",
        hu = "%0 [i/N]: ",
        it = "%0 [s/N]: ",
        ja = "%0「Y/n」 ",
        nb = "%0 [j/N]: ",
        nds = "%0 [j/N] ",
        nl = "%0 [j/N]: ",
        pl = "%0[t/N]: ",
        pt = "%0 [s/N] ",
        pt_BR = "%0 [s/N]: ",
        ru = "%0 [y/N]: ",
        sk = "%0 [a/N]: ",
        sv = "%0 [j/N]: ",
        tr = "%0 [e/H]: ",
        uk = "%0 [так/НІ] ",
        zh_TW = "%0 [y/N]: "
    };

    -- stdio UI plugin says this for yes/no/always/never prompts.
    --  "%0" is the question the user is being asked to respond to.
    --  "[Y/n/Always/Never]" are options that the user will have to type in
    --  response. Please pick reasonable abbreviations for YES and NO (and
    --  full words for ALWAYS and NEVER) that can be typed easily by the user.
    --  Case of the "Y" and "n" are not important; there is no default option
    --  chosen here, so the user will have to fully type out one of these four
    --  options in response. Be sure to translate the strings "Y", "N",
    --  "Always", and "Never", elsewhere, to match what you enter here! At
    --  runtime, the user's response is compared to those strings without
    --  case sensitivity.
    ["%0 [y/n/Always/Never]: "] = {
        cs = "%0 [a/n/Vždy/niKdy] ",
        da = "%0 [j/n/Alltid/Aldrig]: ",
        de = "%0 [j/n/Immer/Niemals]: ",
        el = "%0 [ν/ο/Πάντα/Ποτέ]: ",
        en_AU = "%0 [y/n/Always/Never]: ",
        en_CA = "%0 [y/n/Always/Never]: ",
        en_GB = "%0 [y/n/Always/Never]: ",
        es = "%0 [s/n/Siempre/Nunca]: ",
        fi = "%0 [k/e/Aina/ei Koskaan]: ",
        fr = "%0 [o/n/Toujours/Jamais]: ",
        hu = "%0 [i/n/Mindig/Soha]: ",
        it = "%0 [s/n/Sempre/Mai]: ",
        ja = "%0「Y/n/いつも/決して」 ",
        nb = "%0 [j/n/Alltid/Aldri]: ",
        nds = "%0 [j/n/Immer/Niemals] ",
        nl = "%0 [j/n/Altijd/Nooit]: ",
        pl = "%0 [t/n/Zawsze/Nigdy]: ",
        pt = "%0 [s/n/Sempre/Nunca] ",
        pt_BR = "%0 [s/n/Sempre/Nunca]: ",
        ru = "%0 [y/n/Всегда(A)/Никогда(N)]: ",
        sk = "%0 [a/n/Vzdy/Nikdy]: ",
        sv = "%0 [j/n/Alltid/Aldrig]: ",
        tr = "%0 [e/h/Daima/Asla]: ",
        uk = "%0 [так/ні/Завжди/Ніколи]: ",
        zh_TW = "%0[y/n/永遠(A)/永不(N)]: "
    };

    -- This is used for "yes" in stdio UI's yes/no prompts (case insensitive).
    --  Make sure this matches the string you chose for the "[y/n]" and
    --  the "y/n/always/never" prompt, above! This should be reasonable for
    --  the end user to enter at a terminal prompt.
    ["Y"] = {
        cs = "A",
        da = "J",
        de = "J",
        el = "Ν",
        en_AU = "Y",
        en_CA = "Y",
        en_GB = "Y",
        es = "S",
        et = "J",
        fi = "K",
        fr = "O",
        hu = "I",
        it = "S",
        ja = "Y",
        nb = "J",
        nds = "J",
        nl = "J",
        pl = "T",
        pt = "S",
        pt_BR = "S",
        ru = "Y",
        sk = "A",
        sv = "J",
        tr = "E",
        zh_TW = "是"
    };

    -- This is used for "no" in stdio UI's yes/no prompts (case insensitive).
    --  Make sure this matches the string you chose for the "[y/n]" and
    --  the "y/n/always/never" prompt, above! This should be reasonable for
    --  the end user to enter at a terminal prompt.
    ["N"] = {
        cs = "N",
        da = "N",
        de = "N",
        el = "Ο",
        en_AU = "N",
        en_CA = "N",
        en_GB = "N",
        es = "N",
        et = "E",
        fi = "E",
        fr = "N",
        hu = "N",
        it = "N",
        ja = "N",
        nb = "N",
        nds = "N",
        nl = "N",
        pl = "N",
        pt = "N",
        pt_BR = "N",
        ru = "N",
        sk = "N",
        sv = "N",
        tr = "H",
        zh_TW = "否"
    };

    -- This is used for "always" in stdio UI's yes/no/always/never prompts
    --  (case insensitive). Make sure this matches the string you chose for
    --  the "y/n/always/never" prompt, above! This should be reasonable for
    --  the end user to enter at a terminal prompt.
    ["Always"] = {
        cs = "Vždy",
        da = "Altid",
        de = "Immer",
        el = "Πάντα",
        en_AU = "Always",
        en_CA = "Always",
        en_GB = "Always",
        es = "Siempre",
        et = "Alati",
        fi = "Aina",
        fr = "Toujours",
        hu = "Mindig",
        it = "Sempre",
        ja = "いつも",
        nb = "Alltid",
        nds = "Immer",
        nl = "Altijd",
        pl = "Zawsze",
        pt = "Sempre",
        pt_BR = "Sempre",
        ru = "Всегда",
        sk = "Vždy",
        sv = "Alltid",
        tr = "Daima",
        zh_TW = "總是"
    };

    -- This is used for "never" in stdio UI's yes/no/always/never prompts
    --  (case insensitive). Make sure this matches the string you chose for
    --  the "y/n/always/never" prompt, above! This should be reasonable for
    --  the end user to enter at a terminal prompt.
    ["Never"] = {
        cs = "niKdy",
        da = "Aldrig",
        de = "Niemals",
        el = "Ποτέ",
        en_AU = "Never",
        en_CA = "Never",
        en_GB = "Never",
        es = "Nunca",
        et = "Mitte kunagi",
        fi = "ei Koskaan",
        fr = "Jamais",
        hu = "Soha",
        it = "Mai",
        ja = "決して",
        nb = "Aldri",
        nds = "Niemals",
        nl = "Nooit",
        pl = "Nigdy",
        pt = "Nunca",
        pt_BR = "Nunca",
        ru = "Никогда",
        sk = "Nikdy",
        sv = "Aldrig",
        tr = "Asla",
        zh_TW = "永不"
    };

    -- This is shown when using stdio UI's built-in README pager, to
    --  show what range of lines of text are being displayed (%0 is first
    --  line, %1 is last line, %2 is the total number of lines of text).
    ["(%0-%1 of %2 lines, see more?)"] = {
        cs = "(%0-%1 z %2 řádků, zobrazit více?)",
        da = "(%0-%1 af %2 linie, vis flere?)",
        de = "(%0-%1 von %2 Zeilen, mehr anschauen?)",
        el = "(%0-%1 από %2 γραμμές, θέλετε να δείτε περισσότερες?)",
        en_AU = "(%0-%1 of %2 lines, see more?)",
        en_CA = "(%0-%1 of %2 lines, see more?)",
        en_GB = "(%0-%1 of %2 lines, see more?)",
        es = "(%0-%1 de %2 líneas, ¿ves más?)",
        fi = "(%0-%1 %2:sta rivistä, lue lisää?)",
        fr = "(lignes %0 à %1 sur %2, en voir plus ?)",
        hu = "%0-%1 a %2 sorból, tovább?",
        it = "%0-%1 di %2 linee, desideri visualizzare le successive?",
        nb = "(%0-%1 av %2 linjer, se mer?)",
        nds = "%0-%1 vom %2 lines, mehr?",
        nl = "(%0-%1 van %2 regels, meer zien?)",
        pl = "(%0-%1 z %2 linii, więcej?)",
        pt = "(%0-%1 de %2 linhas, ver mais?)",
        pt_BR = "(%0-%1 de %2 linhas, ver mais?)",
        ru = "(%0-%1 из %2 строк, дальше?)",
        sk = "(%0-%1 of %2 riadkov, zobraziť ďalšie?)",
        sv = "(%0-%1 av %2 rader, visa fler?)",
        tr = "(%2 satırdan %0-%1 satır, daha fazlasına bak?)",
        zh_TW = "（%2 總行數中的第 %0-%1 行，瀏覽更多？）"
    };

    -- The stdio UI uses this sentence in the prompt if the user is able
    --  to return to a previous stage of installation (from the options
    --  section to the "choose installation destination" section, etc).
    ["Type '%0' to go back."] = {
        cs = "Napište '%0' pro návrat zpět.",
        da = "Skriv '%0' for at gå tillbage.",
        de = "Drücken Sie '%0' um zurückzugehen.",
        el = "Πατήστε '%0' για να πάτε πίσω.",
        en_AU = "Type '%0' to go back.",
        en_CA = "Type '%0' to go back.",
        en_GB = "Type '%0' to go back.",
        es = "Pulsa '%0' para ir atrás.",
        fi = "Kirjoita '%0' palataksesi edelliseen osioon.",
        fr = "Tapez '%0' pour revenir en arrière.",
        hu = "Üss '%0'-t a visszalépéshez",
        it = "Digita %0 per andare indietro.",
        nb = "Skriv '%0' for å gå tilbake.",
        nds = "Drücken Sie '%0' um zurückzukehren",
        nl = "Typ '%0' om terug te gaan.",
        pl = "Wpisz '%0' aby powrócić.",
        pt = "Escreva '%0' para regressar.",
        pt_BR = "Digite '%0' para voltar.",
        ru = "Нажмите '%0' чтобы вернуться.",
        sk = "Zadajte '%0' pre návrat spať.",
        sv = "Skriv '%0' för att gå tillbaka.",
        tr = "Geri dönmek için '%0' yaz.",
        zh_TW = "輸入 '%0' 以返回"
    };

    -- This is the string used for the '%0' in the above string.
    --  This is only for the stdio UI, so choose something easy and
    --  reasonable for the user to manually type. The graphical UIs use a
    --  different string for their button ("Back" vs "back" specifically).
    ["back"] = {
        cs = "zpět",
        da = "tilbage",
        de = "zurück",
        el = "πίσω",
        en_AU = "back",
        en_CA = "back",
        en_GB = "back",
        es = "atrás",
        et = "tagasi",
        fi = "takaisin",
        fr = "retour",
        hu = "vissza",
        it = "indietro",
        ja = "戻る",
        nb = "tilbake",
        nds = "Zurück",
        nl = "terug",
        pl = "tył",
        pt = "retroceder",
        pt_BR = "voltar",
        ru = "назад",
        sk = "späť",
        sv = "tillbaka",
        tr = "geri",
        zh_TW = "返回"
    };

    -- This is the prompt in the stdio driver when user input is expected.
    ["> "] = {
        cs = "> ",
        da = "> ",
        de = "> ",
        el = "> ",
        en_AU = "> ",
        en_CA = "> ",
        en_GB = "> ",
        es = "> ",
        fi = "> ",
        fr = "> ",
        hu = "> ",
        it = "> ",
        ja = "> ",
        nb = "> ",
        nds = "> ",
        nl = "> ",
        pl = "> ",
        pt = "> ",
        pt_BR = "> ",
        ru = "> ",
        sk = "> ",
        sv = "> ",
        tr = "> ",
        zh_TW = "> "
    };

    -- That's meant to be the name of an item (%0) and the percent done (%1).
    ["%0: %1%%"] = {
        cs = "%0: %1%%",
        da = "%0: %1%%",
        de = "%0: %1%%",
        el = "%0: %1%%",
        en_AU = "%0: %1%%",
        en_CA = "%0: %1%%",
        en_GB = "%0: %1%%",
        es = "%0: %1%%",
        fi = "%0: %1%%",
        fr = "%0 : %1%%",
        hu = "%0: %1%%",
        it = "%0: %1%%",
        ja = "%0: %1%%",
        nb = "%0: %1%%",
        nds = "%0: %1%%",
        nl = "%0: %1%%",
        pl = "%0: %1%%",
        pt = "%0: %1%%",
        pt_BR = "%0: %1%%",
        ru = "%0: %1%%",
        sk = "%0: %1%%",
        sv = "%0: %1%%",
        tr = "%0: %1%%",
        zh_TW = "%0: %1%%"
    };

    -- The stdio UI uses this to show current status (%0),
    --  and overall progress as percentage of work complete (%1).
    ["%0 (total progress: %1%%)"] = {
        cs = "%0 (celkový průběh: %1%%)",
        da = "%0 (totalt: %1%%)",
        de = "%0 (Gesamtfortschritt: %1%%)",
        el = "%0 (συνολική πρόοδος: %1%%)",
        en_AU = "%0 (total progress: %1%%)",
        en_CA = "%0 (total progress: %1%%)",
        en_GB = "%0 (total progress: %1%%)",
        es = "%0 (Progreso total: %1%%)",
        fi = "%0 (kokonaisedistyminen: %1%%)",
        fr = "%0 (progression totale : %1%%)",
        hu = "%0 (%1%% kész)",
        it = "%0 (progresso: %1%%))",
        ja = "%0 (全体の全身: %1%%)",
        nb = "%0 (totalt: %1%%)",
        nds = "%0 (Fortschritt insgesamt: %1%%)",
        nl = "%0 (totale voortgang: %1%%)",
        pl = "%0 (całkowity postęp: %1%%)",
        pt = "%0 (progresso total: %1%%)",
        pt_BR = "%0 (progresso total: %1%%)",
        ru = "%0 (всего: %1%%)",
        sk = "%0 (celkovo priebeh: %1%%)",
        sv = "%0 (totalt: %1%%)",
        tr = "%0 (toplam ilerleme: %1%%)",
        zh_TW = "%0（所有進度：%1%%）"
    };

    -- This prompt is shown to the end-user after an End User License Agreement
    --  has been displayed, asking them if the license text is acceptable to
    --  them. It's a yes/no question.
    ["Accept this license?"] = {
        cs = "Přijímáte podmínky licence?",
        da = "Accepter denne licens?",
        de = "Nehmen Sie die Lizenzbedingungen an?",
        el = "Δέχεστε αυτή την άδεια χρήσης?",
        en_AU = "Accept this license?",
        en_CA = "Accept this licence?",
        en_GB = "Accept this licence?",
        es = "¿Acepta esta licencia?",
        fi = "Hyväksy lisenssi?",
        fr = "Accepter cette licence ?",
        hu = "Elfogadja a liszenszt?",
        it = "Accetti questa licenza?",
        ja = "このトウェアライセンスを認めるか。",
        nb = "Akseptere denne lisensen?",
        nds = "Diese Lizenz akzeptieren?",
        nl = "Accepteert u deze licentie?",
        pl = "Akceptujesz licencję?",
        pt = "Aceita esta licença?",
        pt_BR = "Aceitar essa licensa?",
        ru = "Вы соглашаетесь?",
        sk = "Súhlasíte s licensiou?",
        sv = "Godkänn licensavtalet?",
        tr = "Bu lisansı kabul ediyor musunuz?",
        zh_TW = "同意此許可證？"
    };

    -- This is a GTK+ button label for yes/no/always/never questions.
    --  The '_' comes before the hotkey character.
    ["_Always"] = {
        cs = "_Vždy",
        da = "_Altid",
        de = "_Immer",
        el = "_Πάντα",
        en_AU = "_Always",
        en_CA = "_Always",
        en_GB = "_Always",
        es = "_Siempre",
        fi = "_Aina",
        fr = "_Toujours",
        hu = "_Mindig",
        it = "_Sempre",
        nb = "_Alltid",
        nds = "_Immer",
        nl = "_Altijd",
        pl = "_Zawsze",
        pt = "_Sempre",
        pt_BR = "_Sempre",
        ru = "_Всегда",
        sk = "_Vždy",
        sv = "_Alltid",
        tr = "_Her zaman",
        zh_TW = "總是(_A)"
    };

    -- This is an error message reported when a .zip file (or whatever) that
    --  we need can't be located.
    ["Archive not found"] = {
        cs = "Archiv nenalezen",
        da = "Arkiv ikke fundet",
        de = "Archiv nicht gefunden",
        el = "Το αρχείο δεν βρέθηκε.",
        en_AU = "Archive not found",
        en_CA = "Archive not found",
        en_GB = "Archive not found",
        es = "Archivo no encontrado",
        fi = "Pakettia ei löydy",
        fr = "Archive introuvable",
        hu = "Arhív nem található",
        it = "Archivio non presente",
        nb = "Fant ikke arkiv",
        nds = "Archiv nicht gefunden",
        nl = "Archief niet gevonden",
        pl = "Archiwum nie znalezione",
        pt = "O Arquivo não foi encontrado",
        pt_BR = "Arquivo não encontrado",
        ru = "Архив не найден",
        sk = "Archív nenájdený",
        sv = "Hittade inte arkivet",
        tr = "Arşiv bulunamadı",
        zh_TW = "找不到檔案包"
    };

    -- This prompt is shown to the user when they click the "Cancel" button,
    --  to confirm they really want to stop. It's a yes/no question.
    ["Are you sure you want to cancel installation?"] = {
        cs = "Opravdu chcete zrušit instalaci?",
        da = "Vil du afbryde installationen?",
        de = "Sind Sie sicher, dass Sie die Installation abbrechen wollen?",
        el = "Είστε σίγουροι ότι θέλετε να ακυρώσετε την εγκατάσταση?",
        en_AU = "Are you sure you want to cancel installation?",
        en_CA = "Are you sure you want to cancel installation?",
        en_GB = "Are you sure you want to cancel installation?",
        es = "¿Estás seguro de que quieres cancelar la instalación?",
        fi = "Haluatko varmasti keskeyttää asennuksen?",
        fr = "Êtes-vous sûr de vouloir annuler l'installation ?",
        hu = "Tényleg megszakítja a telepítést?",
        it = "Sei sicuro di voler annullare l'installazione?",
        nb = "Er du sikker på at du vil avbryte installasjonen?",
        nds = "Sind Sie sicher, dass Sie die Installation abbrechen wollen?",
        nl = "Weet u zeker dat u de installatie wilt afbreken?",
        pl = "Czy na pewno chcesz anulować instalację?",
        pt = "Tem a certeza que quer cancelar a instalação?",
        pt_BR = "Você tem certeza que deseja cancelar a instalação?",
        ru = "Вы уведены что вы хотите прекратить установку?",
        sk = "Ste si istý že chcete prerušiť inštaláciu?",
        sv = "Är du säker på att du vill avbryta installationen?",
        tr = "Kurulumu iptal etmek istediğinizden emin misiniz?",
        zh_TW = "確定取消安裝程序？"
    };

    -- The opposite of "next", used as a UI button label.
    ["Back"] = {
        cs = "Zpět",
        da = "Tilbage",
        de = "Zurück",
        el = "Προηγούμενο",
        en_AU = "Back",
        en_CA = "Back",
        en_GB = "Back",
        es = "Atrás",
        fi = "Takaisin",
        fr = "Retour",
        hu = "Vissza",
        it = "Indietro",
        nb = "Tilbake",
        nds = "Zurück",
        nl = "Vorige",
        pl = "Wstecz",
        pt = "Anterior",
        pt_BR = "Voltar",
        ru = "Назад",
        sk = "Späť",
        sv = "Tillbaka",
        tr = "Geri",
        zh_TW = "返回"
    };

    -- This is a GTK+ button label. The '_' comes before the hotkey character.
    --  "Back" might be using 'B' in English. This button brings up a file
    --  dialog where the end-user can navigate to and select files.
    ["B_rowse..."] = {
        cs = "_Procházet...",
        da = "_Gennemse...",
        de = "Du_rchsuchen",
        el = "_Αναζήτηση...",
        en_AU = "B_rowse...",
        en_CA = "B_rowse...",
        en_GB = "B_rowse...",
        es = "_Navegar...",
        fi = "_Selaa",
        fr = "_Choisir...",
        hu = "_Tallózás...",
        it = "_Sfoglia...",
        nb = "B_la gjennom...",
        nds = "Du_rchsuchen",
        nl = "Bl_aderen...",
        pl = "P_rzeglądaj...",
        pt = "_Procurar...",
        pt_BR = "_Procurar...",
        ru = "Открыть...",
        sk = "P_rehliadať...",
        sv = "B_läddra",
        tr = "T_ara...",
        zh_TW = "瀏覽(_B)"
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
        el = "ΣΦΑΛΜΑ: διπλή αναφορά στο ίδιο media id",
        en_AU = "BUG: duplicate media id",
        en_CA = "BUG: duplicate media id",
        en_GB = "BUG: duplicate media id",
        es = "FALLO: id de medio duplicada",
        fi = "BUGI: kaksi identtistä media id:tä",
        fr = "BUG: Identifiant du media déjà utilisé",
        hu = "BUG: duplikált média azonosító (media id)",
        it = "BUG: media id duplicato",
        nb = "FEIL: duplisert media-id",
        nds = "BUG: Doppelte media id",
        nl = "BUG: dubbele media id",
        pl = "BŁĄ",
        pt = "BUG: media id duplicada",
        pt_BR = "BUG: id da mídia  duplicada",
        ru = "Ошибка: дублирование media_id",
        sk = "CHYBA: duplicitné media id",
        sv = "FEL: duplicerat media id",
        tr = "HATA: çifte medya id'si",
        zh_TW = "臭蟲：重複的媒體序號"
    };

    -- This is shown if the configuration file has no installable options,
    --  either because none are listed or they've all become disabled, which
    --  is a bug the developer must fix before shipping her installer.
    ["BUG: no options"] = {
        cs = "CHYBA: nejsou dostupné žádné volby",
        da = "FEJL: ingen muligheder",
        de = "FEHLER: Keine Optionen",
        el = "ΣΦΑΛΜΑ: δεν υπάρχουν ρυθμίσεις εγκατάστασης",
        en_AU = "BUG: no options",
        en_CA = "BUG: no options",
        en_GB = "BUG: no options",
        es = "FALLO: sin opciones",
        fi = "BUGI: ei vaihtoehtoja",
        fr = "BUG: Pas d'options",
        hu = "BUG: nincsenek opciók",
        it = "BUG: nessuna opzione disponibile",
        nb = "FEIL: ingen valg",
        nds = "BUG: keine Optionen",
        nl = "BUG: geen opties",
        pt = "BUG: sem opções",
        pt_BR = "BUG: Sem opções",
        ru = "Ошибка: отсутствуют опции",
        sk = "CHYBA: žiadne inštalačné možnosti",
        sv = "FEL: inget alternativ",
        tr = "HATA: seçenek yok",
        zh_TW = "臭蟲：沒有選項"
    };

    -- This is shown if the config file wants us to include the uninstaller
    --  program with the rest of the files we write to disk, but didn't enable
    --  the manifest support the installer needs. This is a bug the developer
    --  must fix before shipping her installer.
    ["BUG: support_uninstall requires write_manifest"] = {
        cs = "CHYBA: support_uninstall vyžaduje write_manifest",
        de = "FEHLER: support_uninstall benötigt write_manifest",
        el = "ΣΦΑΛΜΑ: το \"support_uninstall\" απαιτεί το \"write_manifest\"",
        en_AU = "BUG: support_uninstall requires write_manifest",
        en_CA = "BUG: support_uninstall requires write_manifest",
        en_GB = "BUG: support_uninstall requires write_manifest",
        es = "FALLO: support_uninstall requiere write_manifest",
        fi = "BUGI: support_uninstall vaatii write_manifest:n",
        fr = "BOGUE: 'support_uninstall' exige 'write_manifest'",
        hu = "BUG: support_uninstall igényli a write_manifest -et",
        it = "BUG: support_uninstall richiede write_manifest",
        nb = "FEIL: support_uninstall krever write_manifest",
        nds = "BUG: support_uninstall benötigt write_manifest",
        nl = "BUG: support_uninstall heeft write_manifest nodig",
        pt = "BUG:  support_uninstall requer write_manifest",
        pt_BR = "BUG: support_uninstall necessita de write_manifest",
        ru = "Ошибка: support_uninstall нуждается в write_manifest",
        sk = "CHYBA: support_uninstall potrebuje write_manifest",
        sv = "FEL: support_uninstall kräver write_manifest",
        tr = "HATA: support_uninstall write_manifest gerektirir",
        zh_TW = "臭蟲：support_uninstall 需要 write_manifest"
    };

    -- This is shown if the config file wants us to install a manifest (a list
    --  of everything we wrote to disk) but didn't enable Lua parser support
    --  in the binary, which they need to handle Lua source code without
    --  compiling it first (as it needs to be "parsed")...the manifest is
    --  ultimately just an uncompiled Lua program that the installer generates.
    --  This is a bug the developer must fix before shipping her installer.
    ["BUG: write_manifest requires Lua parser support"] = {
        cs = "CHYBA: write_manifest vyžaduje podporu Lua parseru",
        de = "FEHLER: write_manifest benötigt Lua Parser Unterstützung",
        el = "ΣΦΑΛΜΑ: το \"write_manifest\" απαιτεί να υπάρχει υποστήριξη από \"Lua parser\"",
        en_AU = "BUG: write_manifest requires Lua parser support",
        en_CA = "BUG: write_manifest requires Lua parser support",
        en_GB = "BUG: write_manifest requires Lua parser support",
        es = "FALLO: write_manifest requiere el soporte de un intérprete Lua",
        fi = "BUGI: write_manifest vaatii Lua parser -tuen",
        fr = "BOGUE: 'write_manifest' exige le support du parser Lua",
        hu = "BUG: write_manifest igényli a Lua értelmező támogatást",
        it = "BUG: write_manifest richiede Lua parser support",
        nb = "FEIL: write_manifest krever støtte for Lua-parser",
        nds = "Fehler: write_manifest benötift Lua Parser Unterstützung",
        nl = "BUG: write_manifest heeft Lua parser ondersteuning nodig",
        pt = "BUG: write_manifest requer suporte do analisador da linguagem Lua",
        pt_BR = "BUG: write_manifest necessita do suporte ao parser de Lua",
        ru = "Ошибка: write_manifest требует поддержки Lua parser",
        sk = "CHYBA: write_manifest potrebuje zapnutú podporu Lua parsera (Lua parser support)",
        sv = "FEL: write_manifest kräver Lua parser stöd",
        tr = "HATA: write_manifest  Lua yorumlayıcı desteği gerektirir",
        zh_TW = "臭蟲：write_manifest 需要 Lua 語法分析支援"
    };

    -- This is shown if the config file wants us to add desktop menu items
    --  but uninstaller support isn't enabled. It is considered bad taste
    --  to add system menu items without a way to remove them. This is
    --  a bug the developer must fix before shipping her installer.
    ["BUG: Setup.DesktopMenuItem requires support_uninstall"] = {
        cs = "CHYBA: Setup.DesktopMenuItem vyžaduje support_uninstall",
        de = "FEHLER: Setup.DesktopMenuItem benötigt support_uninstall",
        el = "ΣΦΑΛΜΑ: το Setup.DesktopMenuItem απαιτεί το \"support_uninstall\"",
        en_AU = "BUG: Setup.DesktopMenuItem requires support_uninstall",
        en_CA = "BUG: Setup.DesktopMenuItem requires support_uninstall",
        en_GB = "BUG: Setup.DesktopMenuItem requires support_uninstall",
        es = "FALLO: Setup.DesktopMenuItem requiere support_uninstall",
        fi = "BUGI: Setup.DesktopMenuItem vaatii support_uninstall:n",
        fr = "BOGUE: 'Setup.DesktopMenuItem' exige 'support_uninstall'",
        hu = "BUG: Setup.DesktopMenuItem igényli a support_uninstall-t",
        it = "BUG: Setup.DesktopMenuItem richiede support_uninstall",
        nds = "BUG: Setup.DesktopMenuItem benötigt support_uninstall",
        nl = "BUG: Setup.DesktopMenuItem heeft support_uninstall nodig",
        pt = "BUG: Setup.DesktopMenuItem requer support_uninstall",
        pt_BR = "BUG: Setup.DesktopMenuItem precisa do support_uninstall",
        ru = "Ошибка: Setup.DesktopMenuItem нуждается в support_uninstall",
        sk = "CHYBA: Setup.DesktopMenuItem potrebuje support_uninstall",
        sv = "FEL: Setup.DesktopMenuItem kräver support_uninstall",
        tr = "HATA: Setup.DesktopMenuItem support_uninstall gerektirir",
        zh_TW = "臭蟲：Setup.DesktopMenuItem 需要 support_uninstall"
    };

    -- This is a file's permissions. Programmers give these as strings, and
    --  if one isn't valid, the program will report this. So, on Unix, they
    --  might specify "0600" as a valid string, but "sdfksjdfk" wouldn't be
    --  valid and would cause this error.
    ["BUG: '%0' is not a valid permission string"] = {
        cs = "CHYBA: '%0' není platným řetězcem vyjadřujícím oprávnění",
        de = "FEHLER: '%0' ist keine zulässige Berechtigungszeichenkette",
        el = "ΣΦΑΛΜΑ: '%0' δεν είναι έγγυρο αλφαριθμητικό περιγραφής δικαιωμάτων",
        en_AU = "BUG: '%0' is not a valid permission string",
        en_CA = "BUG: '%0' is not a valid permission string",
        en_GB = "BUG: '%0' is not a valid permission string",
        es = "FALLO: '%0' no es una cadena de permisos válida",
        fi = "BUGI: \"%0\" ei ole kelvollinen oikeuksia määrittävä merkkijono",
        fr = "BUG \"%0\" n'est pas une chaîne de permission valide",
        hu = "BUG: '%0' nem érvényes jogosultság sztring",
        it = "BUG: '%0' non è una stringa valida di permessi",
        nb = "FEIL: '%0' er ikke en gyldig rettighetsstreng",
        nds = "BUG: '%0' ist kein gültiger \"permission string\"",
        nl = "BUG: '%0' is geen geldige permissie string",
        pt = "BUG: '%0' não é uma expressão válida",
        pt_BR = "BUG: '%0' não é um texto de permissão válido",
        ru = "Ошибка: '%0' неправильное значение для прав.",
        sk = "CHYBA: '%0' nieje správny spôsob zápisu práv",
        sv = "FEL: '%0' är inte en giltig rättighetssträng",
        tr = "HATA: '%0' geçerli bir izin karakter dizisi değil",
        zh_TW = "臭蟲：'%0' 並不是有效的檔案權限設定"
    };

    -- If there's a string in the program that needs be formatted with
    --  %0, %1, etc, and it specifies an invalid sequence like "%z", this
    --  error pops up to inform the programmer/translator.
    --  "format()" is a proper name in this case (program function name)
    ["BUG: Invalid format() string"] = {
        cs = "CHYBA: Neplatný řetězec pro format()",
        de = "FEHLER: Unzulässige format() Zeichenkette",
        el = "ΣΦΑΛΜΑ: Λάθος format() αλφαριθμητικό",
        en_AU = "BUG: Invalid format() string",
        en_CA = "BUG: Invalid format() string",
        en_GB = "BUG: Invalid format() string",
        es = "FALLO: Cadena format() no válida",
        fi = "BUGI: Epäkelpo format()-merkkijono",
        fr = "BUG: Chaîne format() invalide",
        hu = "BUG: Helytelen format() sztring",
        it = "BUG: stringa format() non valida",
        nb = "FEIL: Ugyldig format()-streng",
        nds = "BUG: Ungültiger format() string",
        nl = "BUG: Ongeldige format() string",
        pt = "BUG: format() inválido da expressão",
        pt_BR = "BUG: Texto format() inválido",
        ru = "Ошибка: Некорректная строка format()",
        sk = "CHYBA: nesprávny format() reťazec",
        sv = "FEL: Ogiltig format() sträng",
        tr = "HATA: Geçersiz format() karakter dizisi",
        zh_TW = "臭蟲：無效的 format() 字串"
    };

    -- The program runs in "stages" and as it transitions from one stage to
    --  another, it has to report some data about what happened during the
    --  stage. A programming bug may cause unexpected type of data to be
    --  reported, causing this error to pop up.
    ["BUG: stage returned wrong type"] = {
        cs = "CHYBA: instalační krok vrátil chybný datový typ",
        de = "FEHLER: Abschnitt gab falschen Typ zurück",
        el = "ΣΦΑΛΜΑ: η φάση εγκατάστασης επέστρεψε λάθος τύπο δεδομένων",
        en_AU = "BUG: stage returned wrong type",
        en_CA = "BUG: stage returned wrong type",
        en_GB = "BUG: stage returned wrong type",
        es = "FALLO: la etapa ha devuelto tipo erróneo",
        fi = "BUGI: vaihe palautti väärää tyyppiä",
        fr = "BUG: L'étape renvoie un mauvais type de données",
        hu = "BUG: az állomás rossz típussal tért vissza",
        it = "BUG: la fase attuale (stage) ha restituito un tipo non valido",
        nb = "FEIL: nivå returnerte feil type",
        nds = "BUG: Stage Rückgabewert ist vom falschen Typ",
        nl = "BUG: programma-fase leverde een verkeerd datatype",
        pt = "BUG: A etapa retornou um tipo errado",
        pt_BR = "BUG: a etapa retornou o tipo errado",
        ru = "Ошибка: этап выдал неожиданный тип",
        sk = "CHYBA: fáza vrátila nesprávny typ",
        sv = "FEL: nivå returnerade fel typ",
        tr = "HATA: aşama yanlış tip getirdi",
        zh_TW = "臭蟲：此階段傳回錯誤類別"
    };

    -- The program runs in "stages" and as it transitions from one stage to
    --  another, it has to report some data about what happened during the
    --  stage. A programming bug may cause unexpected information to be
    --  reported, causing this error to pop up.
    ["BUG: stage returned wrong value"] = {
        cs = "CHYBA: instalační krok vrátil chybnou hodnotu",
        de = "FEHLER: Abschnitt gab falschen Wert zurück",
        el = "ΣΦΑΛΜΑ: η φάση εγκατάστασης επέστρεψε λάθος τιμή",
        en_AU = "BUG: stage returned wrong value",
        en_CA = "BUG: stage returned wrong value",
        en_GB = "BUG: stage returned wrong value",
        es = "FALLO: la etapa ha devuelto valor erróneo",
        fi = "BUGI: vaihe palautti väärän arvon",
        fr = "BUG: L'étape renvoie une mauvaise valeur",
        hu = "BUG: az állomás rossz értékkel tért vissza",
        it = "BUG: la fase attuale (stage) ha restituito un valore non valido",
        nb = "FEIL: nivå returnerte feil verdi",
        nds = "Fehler: stage gab falschen Wert zurück",
        nl = "BUG: programma-fase leverde een verkeerde waarde",
        pt = "BUG: A etapa retornou um valor errado",
        pt_BR = "BUG: a etapa retornou o valor errado",
        ru = "Ошибка: этап выдал неожиданный результат",
        sk = "CHYBA: fáza vrátila nesprávnu hodnotu",
        sv = "FEL: nivå returnerade fel värde",
        tr = "HATA: aşama yanlış değer getirdi",
        zh_TW = "臭蟲：此階段傳回錯誤值"
    };

    -- The program runs in "stages", which can in many cases be revisited
    --  by the user clicking the "Back" button. If the program has a bug
    --  that allows the user to click "Back" on the initial stage, this
    --  error pops up.
    ["BUG: stepped back over start of stages"] = {
        cs = "CHYBA: pokus o krok zpět před začátek instalace",
        de = "FEHLER: Über den Startabschnitt hinaus zurückgegangen",
        el = "ΣΦΑΛΜΑ: ζητήθηκε η επιστροφή σε προηγούμενη φάση που δεν υπάρχει",
        en_AU = "BUG: stepped back over start of stages",
        en_CA = "BUG: stepped back over start of stages",
        en_GB = "BUG: stepped back over start of stages",
        es = "FALLO: retroceso más atrás del inicio de las etapas",
        fi = "BUGI: palattiin ensimmäisen vaiheen ohi",
        fr = "BUG: Revenu au début des étapes",
        hu = "BUG: visszalépés a kezdő állomás után",
        it = "BUG: ritornato all'inizio della fase (stage)",
        nb = "FEIL: Gikk tilbake forbi startnivå",
        nds = "BUG: Zurück vor dem ersten Vorgang",
        nl = "BUG: teruggegaan naar voor eerste stadium.",
        pt = "BUG: Retrocedeu na primeira etapa",
        pt_BR = "BUG: voltar na primeira etapa",
        ru = "Ошибка: первый этап содержит кнопку \"Назад\"",
        sk = "CHYBA: vrátili ste sa pred začiatok počiatočnej fázy",
        sv = "FEL: Gick tillbaka förbi startnivå",
        tr = "HATA: aşamaların başlangıcına geri adım atıldı",
        zh_TW = "臭蟲：在起始階段進行返回"
    };

    -- This happens if there's an unusual case when writing out Lua scripts
    --  to disk. This should never be seen by an end-user.
    ["BUG: Unhandled data type"] = {
        cs = "CHYBA: Datový typ není obsluhován",
        de = "FEHLER: Unbehandelter Datentyp",
        en_AU = "BUG: Unhandled data type",
        en_CA = "BUG: Unhandled data type",
        en_GB = "BUG: Unhandled data type",
        es = "FALLO: Tipo de datos sin manipular",
        fi = "BUGI: Käsittelemätön datatyyppi",
        fr = "BUG: Type de fichier inconnu",
        hu = "BUG: Lekezeletlen adattípus",
        it = "BUG: tipo di dato non gestito",
        nb = "FEIL: Uhåndtert datatype",
        nds = "BUG: Unbehandelter Datentyp",
        nl = "BUG: onbekend data type",
        pt = "BUG: tipo sem tratamento",
        pt_BR = "BUG: tipo de dados não suportado",
        ru = "Ошибка: неподдерживаемый тип данных",
        sk = "CHYBA: Nespracovateľný typ dát",
        sv = "FEL: Ohanterad datatyp",
        tr = "HATA: Ele alınmamış veri tipi",
        zh_TW = "臭蟲：未處理的資料類別"
    };

    -- This is triggered by a logic bug in the i/o subsystem.
    --  This should never be seen by an end-user.
    --  "tar" is a proper name in this case (it's a file format).
    ["BUG: Can't duplicate tar inputs"] = {
        cs = "CHYBA: Nemohu zduplikovat vstupy z taru",
        de = "FEHLER: Tar-Eingaben können nicht dupliziert werden",
        el = "ΣΦΑΛΜΑ: Δεν είναι δυναταή αντιγραφή των δεδομένων εισόδου του \"tar\"",
        en_AU = "BUG: Can't duplicate tar inputs",
        en_CA = "BUG: Can't duplicate tar inputs",
        en_GB = "BUG: Can't duplicate tar inputs",
        es = "FALLO: No se pueden duplicar las entradas tar",
        fi = "BUGI: tar-syötteitä ei voida kahdentaa",
        fr = "BUG: Impossible de dupliquer les entrées tar",
        hu = "BUG: tar bemenet nem duplikálható",
        it = "BUG: impossibile duplicare il .tar di input",
        nb = "FEIL: Kan ikke duplisere innfiler for tar",
        nds = "Fehler: Kann tar-Eingabe nicht duplizieren",
        nl = "BUG: kan tar input kan niet gedupliceerd woorden.",
        pt = "BUG: Impossível duplicar as entradas do tar",
        pt_BR = "BUG: Não é possível duplicar as entradas do tar",
        ru = "Ошибка: нельзя дублировать исходные данные для tar",
        sk = "CHYBA: Nemôžete duplikovať tar vstupy",
        sv = "FEL: Kan inte duplicera infiler för tar",
        tr = "HATA: Tar girişleri çoklanamıyor",
        zh_TW = "臭蟲：無法重複 tar 的輸入"
    };

    -- This is a generic error message when a programming bug produced a
    --  result we weren't expecting (a negative number when we expected
    --  positive, etc...)
    ["BUG: Unexpected value"] = {
        cs = "CHYBA: Neočekávaná hodnota",
        de = "FEHLER: Unerwarteter Wert",
        el = "ΣΦΑΛΜΑ: Μη αναμενόμενη τιμή",
        en_AU = "BUG: Unexpected value",
        en_CA = "BUG: Unexpected value",
        en_GB = "BUG: Unexpected value",
        es = "FALLO: Valor inesperado",
        fi = "BUGI: Odottamaton arvo",
        fr = "BUG: Valeur inattendue",
        hu = "BUG: váratlan érték",
        it = "BUG: Valore inatteso",
        nds = "BUG: Unerwarteter Wert",
        nl = "BUG: Onverwachte waarde",
        pt = "BUG: Valor inexperado",
        pt_BR = "BUG: valor inesperado",
        ru = "Ошибка: Неожиданное значение",
        sk = "CHYBA: Neočakávaná hodnota",
        sv = "FEL: Oväntat värde",
        tr = "HATA: Beklenmeyen değer",
        zh_TW = "臭蟲：意外的值"
    };

    -- Buggy config elements:
    --  This is supposed to be a config element (%0) and something that's wrong
    --  with it (%1), such as "BUG: Config Package::description not a string"
    --  The grammar can be imperfect here; this is a developer error, not an
    --  end-user error, so we haven't made this very flexible.
    ["BUG: Config %0 %1"] = {
        cs = "CHYBA: Konfigurační hodnota %0 %1",
        de = "FEHLER: Konfiguration %0 %1",
        el = "ΣΦΑΛΜΑ: Παράμετρος %0 %1",
        en_AU = "BUG: Config %0 %1",
        en_CA = "BUG: Config %0 %1",
        en_GB = "BUG: Config %0 %1",
        es = "FALLO: Configuración %0 %1",
        fi = "BUGI: Config %0 %1",
        fr = "BUG: Config %0 %1",
        hu = "BUG: Beállítás %0 %1",
        it = "BUG: Config %0 %1",
        nb = "FEIL: Konfigurasjon %0 %1",
        nds = "BUG: Config %0 %1",
        nl = "BUG: configuratie %0 %1",
        pt = "BUG: Configuração %0 %1",
        pt_BR = "BUG: Configuração %0 %1",
        ru = "Ошибка: Опция %0 %1",
        sk = "CHYBA: Config %0 %1",
        sv = "FEL: Konfiguration %0 %1",
        tr = "HATA: Ayar %0 %1",
        zh_TW = "臭蟲：設定 %0 %1"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be explicitly specified"] = {
        cs = "musí být explicitně určen",
        de = "muss explizit angegeben werden",
        el = "πρέπει να οριστεί ρητά",
        en_AU = "must be explicitly specified",
        en_CA = "must be explicitly specified",
        en_GB = "must be explicitly specified",
        es = "debe ser especificado explícitamente",
        fi = "täytyy määritellä täsmällisesti",
        fr = "doit être spécifié explicitement",
        hu = "ezt explicit módon kell meghatározni",
        it = "deve essere esplicitato",
        nb = "må spesifiseres eksplisitt",
        nds = "muss ausdrücklich spezifiziert werden",
        nl = "moet expliciet gespecifieerd worden",
        pl = "musi być jasno podany",
        pt = "Tem que ser especificado explicitamente",
        pt_BR = "precisa ser especificado explicitamente",
        ru = "должна быть указана явно",
        sk = "musí byť explicitne špecifikovaný",
        sv = "måste vara explicit specifierad",
        tr = "açıkça belirtilmeli",
        zh_TW = "必須明確指定"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be string or table of strings"] = {
        cs = "musí být řetězcem nebo tabulkou řetězců",
        de = "muss eine Zeichenkette oder eine Tabelle mit Zeichenketten sein",
        el = "πρέπει να είναι ένα αλφαριθμητικό ή πίνακας απο αλφαριθμητικά",
        en_AU = "must be string or table of strings",
        en_CA = "must be string or table of strings",
        en_GB = "must be string or table of strings",
        es = "debe ser una cadena o tabla de cadenas",
        fi = "täytyy olla merkkijono tai merkkijonotaulukko",
        fr = "doit être une chaîne de caractères ou un tableau de chaînes de caractères",
        hu = "sztring vagy sztringtáblának kell lennie",
        it = "deve essere una stringa o una tabella di stringhe",
        nb = "må være streng eller tabell av strenger",
        nds = "muss String oder Tabelle von Strings sein",
        nl = "moet een string of een tabel met strings zijn",
        pl = "musi być ciągiem lub tabelą ciągów",
        pt = "Tem que ser uma expressão ou uma tabela de expressões",
        pt_BR = "precisa ser um texto ou uma tabela de textos",
        ru = "должна быть строкой или таблицей строк",
        sk = "musí byť reťazec alebo tabuľka reťazcov",
        sv = "måste vara en sträng eller en tabell av strängar",
        tr = "bir karakter dizisi ya da karakter dizi tablosu olması gerekir",
        zh_TW = "必須是字串或字串表格"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["must be a string or number"] = {
        cs = "musí být řetězcem nebo číslem",
        de = "muss eine Zeichenkette oder Zahl sein",
        el = "πρέπει να είναι αλφαριθμητικό ή αριθμός",
        en_AU = "must be a string or number",
        en_CA = "must be a string or number",
        en_GB = "must be a string or number",
        es = "debe ser una cadena o un número",
        fi = "täytyy olla merkkijono tai luku",
        fr = "doit être une chaîne de caractères ou un nombre",
        hu = "sztringnek vagy számnak kell lennie",
        it = "deve essere una stringa oppure un numero",
        nb = "må være streng eller nummer",
        nds = "muss String oder Nummer sein",
        nl = "moet een string of een nummer zijn",
        pl = "musi być ciągiem lub liczbą",
        pt = "Tem que ser uma expressão ou um número",
        pt_BR = "precisa ser um texto ou um número",
        ru = "должна быть строкой или числом",
        sk = "musí byť reťazec alebo číslo",
        sv = "måste vara en sträng eller ett nummer",
        tr = "bir karakter dizisi ya da sayı olmalı",
        zh_TW = "必須是字串或數字"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["can't be empty string"] = {
        cs = "nemůže být prázdným řetězcem",
        de = "darf keine leere Zeichenkette sein",
        el = "δεν μπορεί να είναι άδειο αλφαριθμητικό",
        en_AU = "can't be empty string",
        en_CA = "can't be empty string",
        en_GB = "can't be empty string",
        es = "no puede ser una cadena vacía",
        fi = "ei saa olla tyhjä merkkijono",
        fr = "ne peut être une chaîne de caractères vide",
        hu = "nem lehet üres sztring",
        it = "non può essere una stringa vuota",
        nb = "kan ikke være tom streng",
        nds = "kann String nicht leeren",
        nl = "kan geen lege string zijn",
        pl = "nie może być pustym ciągiem",
        pt = "Não pode ser uma expressão vazia",
        pt_BR = "não pode ser um texto em branco",
        ru = "не может быть пустой строкой",
        sk = "nemôže byť reťazec",
        sv = "kan inte vara en tom sträng",
        tr = "boş bir karakter dizisi olamaz",
        zh_TW = "不能是空字串"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have protocol"] = {
        cs = "URL nemá určený protokol",
        da = "URL har ikke en protekol",
        de = "URL hat kein Protokoll",
        el = "το URL δεν περιέχει περιγραφή προτοκόλου",
        en_AU = "URL doesn't have protocol",
        en_CA = "URL doesn't have protocol",
        en_GB = "URL doesn't have protocol",
        es = "URL sin especificar protocolo",
        fi = "URL ei sisällä protokollaa",
        fr = "L'URL manque un protocole",
        hu = "URL-nek nincs protokollja",
        it = "URL malformato (senza protocollo)",
        nb = "URL har ikke protokoll",
        nds = "URL hat kein Protokoll",
        nl = "URL heeft geen protocol",
        pl = "URL nie ma protokołu",
        pt = "O URL não tem o protocolo",
        pt_BR = "Não existe protocolo na URL",
        ru = "URL не содержит протокол",
        sk = "URL neurčuje protokol",
        sv = "URL saknar protokoll",
        tr = "URL'nin protokolü yok",
        zh_TW = "URL 沒有通訊協定"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have host"] = {
        cs = "URL neobsahuje hostitele",
        da = "URL mangler vært",
        de = "URL hat keinen Host",
        el = "το URL δεν περιέχει περιγραφή διακομιστή",
        en_AU = "URL doesn't have host",
        en_CA = "URL doesn't have host",
        en_GB = "URL doesn't have host",
        es = "URL sin especificar host",
        fi = "URL ei sisällä palvelinta",
        fr = "L'URL manque un nom d'hôte",
        hu = "URL-nek nincs hosztja",
        it = "URL malformato (senza host)",
        nb = "URL har ikke ver",
        nds = "URL hat keinen Host",
        nl = "URL heeft geen host",
        pl = "URL nie ma hosta",
        pt = "O URL não tem servidor",
        pt_BR = "Não existe o host na URL",
        ru = "URL не содержит хост",
        sk = "URL neobsahuje hostiteľa",
        sv = "URL saknar värd",
        tr = "URL'nin host u yok",
        zh_TW = "URL 沒有主機"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL doesn't have path"] = {
        cs = "URL neobsahuje cestu",
        da = "URL mangler sti",
        de = "URL hat keinen Pfad",
        el = "το URL δεν περιέχει το μονοπάτι",
        en_AU = "URL doesn't have path",
        en_CA = "URL doesn't have path",
        en_GB = "URL doesn't have path",
        es = "URL sin especificar ruta",
        fi = "URL ei sisällä polkua",
        fr = "L'URL manque un chemin",
        hu = "URL-nek nincs útvonala",
        it = "URL malformato (senza percorso)",
        nb = "URL har ikke sti",
        nds = "URL hat keinen Pfad",
        nl = "URL heeft geen pad",
        pl = "URL nie ma ścieżki",
        pt = "O URL não tem recurso",
        pt_BR = "Não existe o caminho na URL",
        ru = "URL не содержит путь",
        sk = "URL neobsahuje cestu",
        sv = "URL saknar sökväg",
        tr = "URL'nin yol u yok",
        zh_TW = "URL 沒有路徑"
    };

    -- This is an error string for a buggy config element. See notes above.
    ["URL protocol is unsupported"] = {
        cs = "Protokol v URL není podporován",
        da = "URL protekol er ikke understøttet",
        de = "URL Protokoll wird nicht unterstützt",
        el = "το προτόκολο που περιέχει το URL δεν υποστηρίζεται",
        en_AU = "URL protocol is unsupported",
        en_CA = "URL protocol is unsupported",
        en_GB = "URL protocol is unsupported",
        es = "Protocolo de URL no soportado",
        fi = "URL:n protokollaa ei tueta",
        fr = "Le protocole de l'URL n'est pas supporté",
        hu = "URL protokoll nem támogatott",
        it = "URL malformato (protocollo non supportato)",
        nb = "URL-protokollen er ikke støttet",
        nds = "URL Protokoll wird nicht unterstützt",
        nl = "protocol van URL wordt niet ondersteund",
        pl = "protokół URL nie obsługiwany",
        pt = "O protocolo do URL não é suportado",
        pt_BR = "O protocolo da URL não é suportado",
        ru = "Протокол URL не поддерживается",
        sk = "URL protokol nieje podporovaný",
        sv = "URL-protokollet har inget stöd",
        tr = "URL protokolü desteklenmiyor",
        zh_TW = "URL 通訊協定不支援"
    };

    -- This is an error string for a buggy config element. See notes above.
    --  "Permission string" is text representing a file's permissions,
    --  such as "0644" on Unix.
    ["Permission string is invalid"] = {
        cs = "Řetězec s oprávněními je neplatný",
        de = "Berechtigungszeichenkette ist ungültig",
        el = "Το αλφαριθμητικό που περιγράφει τα δικαιώματα είναι λάθος.",
        en_AU = "Permission string is invalid",
        en_CA = "Permission string is invalid",
        en_GB = "Permission string is invalid",
        es = "Cadena de permisos no válida",
        fi = "oikeuksia määrittävä merkkijono ei ole kelvollinen",
        fr = "Chaine de permissions invalide",
        hu = "Helytelen engedélysztring",
        it = "Stringa dei permessi non valida",
        nb = "Rettighetsstrengen er ugyldig",
        nds = "Zugriffsberechtigungs String ist ungültig",
        nl = "Permissie string is niet geldig",
        pl = "Ciąg uprawnień jest niepoprawny",
        pt = "A expressão de permissão não é válida",
        pt_BR = "O texto de permissão é inválido",
        ru = "Строка прав доступа некорректна",
        sk = "Reťazec práv je neplatný",
        sv = "Rättighetssträngen är ogiltig",
        tr = "İzin hakları dizesi geçersiz",
        zh_TW = "無效的檔案權限設定"
    };

    -- This is an error string for a buggy config element. See notes above.
    --  "property" means attribute, not something owned, in this case.
    ["is not a valid property"] = {
        cs = "není platnou vlastností",
        de = "ist keine gültige Eigenschaft",
        el = "δεν είναι έγγυρη ιδιότητα",
        en_AU = "is not a valid property",
        en_CA = "is not a valid property",
        en_GB = "is not a valid property",
        es = "no es un atributo válido",
        fi = "ei ole kelvollinen määrite",
        fr = "n'est pas une propriété valide",
        hu = "érvénytelen tulajdonság",
        it = "non è una proprietà valida",
        nb = "er ikke en gyldig egenskap",
        nds = "ist keine gültige Eigenschaft",
        nl = "is geen geldige eigenschap",
        pl = "nie jest poprawym atrybutem",
        pt = "não é uma propriedade válida",
        pt_BR = "não é uma propriedade válida",
        ru = "недопустимое значение",
        sk = "nieje správnym atribútom",
        sv = "är inte ett giltigt attribut",
        tr = "geçerli bir özellik değil",
        zh_TW = "並不是有效設定"
    };

    -- This is an error string for a buggy config element. See notes above.
    --  %0 is a data type name (string, number, table, etc).
    ["must be %0"] = {
        cs = "musí být %0",
        da = "skal være %0",
        de = "muss vom Typ %0 sein",
        el = "πρέπει να είναι %0",
        en_AU = "must be %0",
        en_CA = "must be %0",
        en_GB = "must be %0",
        es = "debe ser %0",
        fi = "täytyy olla %0",
        fr = "doit être de type %0",
        hu = "%0 -nak kell lennie",
        it = "deve essere %0",
        nb = "må være %0",
        nds = "muss %0 sein",
        nl = "moet %0 zijn",
        pl = "musi być %0",
        pt = "tem que ser %0",
        pt_BR = "precisa ser %0",
        ru = "должна быть %0",
        sk = "musí byť %0",
        sv = "måste vara %0",
        tr = "%0 olmalı",
        zh_TW = "必須是 %0"
    };

    -- Data type for "must be %0" above...
    ["string"] = {
        cs = "řetězec",
        da = "streng",
        de = "Zeichenkette",
        el = "αλφαριθμητικό",
        en_AU = "string",
        en_CA = "string",
        en_GB = "string",
        es = "cadena",
        fi = "merkkijono",
        fr = "chaîne de caractères",
        hu = "szöveg",
        it = "stringa",
        nb = "streng",
        nds = "string",
        nl = "string",
        pl = "ciągiem",
        pt = "expressão",
        pt_BR = "texto",
        ru = "строкой",
        sk = "reťazec",
        sv = "sträng",
        tr = "karakter dizesi",
        zh_TW = "字串"
    };

    -- Data type for "must be %0" above...
    ["boolean"] = {
        cs = "booleovská hodnota",
        da = "boolsk",
        de = "Bool",
        el = "boolean",
        en_AU = "boolean",
        en_CA = "boolean",
        en_GB = "boolean",
        es = "booleano",
        fi = "totuusarvo",
        fr = "booléen",
        hu = "logikai",
        it = "booleano",
        nb = "boolsk verdi",
        nds = "boolean",
        nl = "boolean",
        pl = "wartością logiczną",
        pt = "boleano",
        pt_BR = "booleano",
        ru = "булевым",
        sk = "booleovská hodnota",
        sv = "boolskt värde",
        tr = "mantıksal değer",
        zh_TW = "布林值"
    };

    -- Data type for "must be %0" above...
    ["number"] = {
        cs = "číslo",
        da = "nummer",
        de = "Zahl",
        el = "αριθμός",
        en_AU = "number",
        en_CA = "number",
        en_GB = "number",
        es = "número",
        fi = "luku",
        fr = "nombre",
        hu = "szám",
        it = "numero",
        nb = "nummer",
        nds = "Nummer",
        nl = "nummer",
        pl = "liczbą",
        pt = "número",
        pt_BR = "número",
        ru = "числом",
        sk = "číslo",
        sv = "nummer",
        tr = "sayı",
        zh_TW = "數字"
    };

    -- Data type for "must be %0" above...
    ["function"] = {
        cs = "funkce",
        da = "funktion",
        de = "Funktion",
        el = "function",
        en_AU = "function",
        en_CA = "function",
        en_GB = "function",
        es = "función",
        fi = "funktio",
        fr = "fonction",
        hu = "függvény",
        it = "funzione",
        nb = "funksjon",
        nds = "Funktion",
        nl = "functie",
        pl = "funkcją",
        pt = "função",
        pt_BR = "função",
        ru = "функцией",
        sk = "funkcia",
        sv = "funktion",
        tr = "fonksiyon",
        zh_TW = "函式"
    };

    -- Data type for "must be %0" above...
    ["table"] = {
        cs = "tabulka",
        da = "tabel",
        de = "Tabelle",
        el = "πίνακας",
        en_AU = "table",
        en_CA = "table",
        en_GB = "table",
        es = "tabla",
        fi = "taulukko",
        fr = "tableau",
        hu = "táblázat",
        it = "tabella",
        nb = "tabell",
        nds = "Tabelle",
        nl = "tabel",
        pl = "tabelą",
        pt = "tabela",
        pt_BR = "tabela",
        ru = "таблицей",
        sk = "tabuľka",
        sv = "tabell",
        tr = "tablo",
        zh_TW = "表格"
    };

    -- bzlib is a proper name. The error message (%0) may not be localized,
    --  it's meant to be a developer error and not an end-user message.
    ["bzlib triggered an internal error: %0"] = {
        cs = "bzlib vyvolala vnitřní chybu: %0",
        da = "bzlib forårsagede en intern fejl:",
        de = "bzlib hat einen internen Fehler ausgelöst: %0",
        el = "το bzlib παρουσίασε εσωτερικό σφάλμα: %0",
        en_AU = "bzlib triggered an internal error: %0",
        en_CA = "bzlib triggered an internal error: %0",
        en_GB = "bzlib triggered an internal error: %0",
        es = "bzlib ha provocado un error interno: %0",
        fi = "bzlib laukaisi sisäisen virheen: %0",
        fr = "bzlib a causé une erreur interne: %0",
        hu = "bzlib belső hibát észlelt: %0",
        it = "bzlib ha causato un errore interno: %0",
        nb = "intern feil i bzlib: %0",
        nds = "bzlib löste eine internen Fehler aus: %0",
        nl = "bzlib heeft een interne fout veroorzaakt: %0",
        pl = "bzlib wywołał błąd wewnętrzny: %0",
        pt = "A bzlib despoletou um erro interno: %0",
        pt_BR = "bzlib disparou um erro interno: %0",
        ru = "произошла ошибка в bzlib: %0",
        sk = "bzlib spôsobilo vnútornú chybu: %0",
        sv = "internt fel i bzlib: %0",
        tr = "bzlib içsel bir hata tetikledi: %0",
        zh_TW = "bzlib 遭遇一個內部錯誤： %0"
    };

    -- This is a UI button label, usually paired with "OK", but also usually
    --  present as a generic "stop the program" button.
    ["Cancel"] = {
        cs = "Zrušit",
        da = "Annuller",
        de = "Abbrechen",
        el = "Άκυρο",
        en_AU = "Cancel",
        en_CA = "Cancel",
        en_GB = "Cancel",
        es = "Cancelar",
        fi = "Peru",
        fr = "Annuler",
        hu = "Mégsem",
        it = "Annulla",
        nb = "Avbryt",
        nds = "Abbrechen",
        nl = "Annuleren",
        pl = "Anuluj",
        pt = "Cancelar",
        pt_BR = "Cancelar",
        ru = "Отменить",
        sk = "Zruš",
        sv = "Avbryt",
        tr = "İptal",
        zh_TW = "取消"
    };

    -- This is a message box title when prompting for confirmation when the
    --  the user clicks the Cancel button.
    ["Cancel installation"] = {
        cs = "Zrušit instalaci",
        da = "Afbryd installation",
        de = "Installation abbrechen",
        el = "Ακύρωση εγκατάστασης",
        en_AU = "Cancel Installation",
        en_CA = "Cancel installation",
        en_GB = "Cancel installation",
        es = "Cancelar instalación",
        fi = "Peru asennus",
        fr = "Annuler l'installation",
        hu = "Telepítés megszakítása",
        it = "Annulla l'installazione",
        nb = "Avbryt installasjonen",
        nds = "Installation abbrechen",
        nl = "Installatie afbreken?",
        pl = "Anuluj Instalację",
        pt = "Cancelar a instalação",
        pt_BR = "Cancelar instalação",
        ru = "Отменить установку",
        sk = "Zruš inštaláciu",
        sv = "Avbryt installationen",
        tr = "Kurulumu iptal et",
        zh_TW = "取消安裝"
    };

    -- This error is reported for i/o failures while listing files contained
    --  in a .zip (or whatever) file.
    ["Couldn't enumerate archive"] = {
        cs = "Nemohu projít archiv",
        de = "Archiv kann nicht aufgelistet werden",
        el = "Δεν είναι δυνατή η απαρίθμηση του πακέτου αρχείων",
        en_AU = "Couldn't enumerate archive",
        en_CA = "Couldn't enumerate archive",
        en_GB = "Couldn't enumerate archive",
        es = "No se puede enumerar el archivo",
        fi = "Paketin tiedostojen listaus epäonnistui",
        fr = "Ne peux pas énumérer les fichiers de l'archive",
        hu = "Az archívum nem csomagolható ki",
        it = "Impossibile enumerare l'archivio",
        nb = "Kunne ikke liste filer i arkivet",
        nds = "Konnte Archive nicht nummerieren",
        nl = "Kan het archief niet weergeven",
        pl = "nie można przetworzyć archiwum",
        pt = "Foi impossível enumerar o arquivo",
        pt_BR = "Não foi possível enumerar o arquivo",
        ru = "Не могу прочитать архив",
        sk = "Nemôžem prelistovať archív",
        sv = "Kunde inte lista filen",
        tr = "Arşiv numaralandırılamadı",
        zh_TW = "無法列出檔案包的內容"
    };

    -- This error is reported for i/o failures while opening a .zip
    --  (or whatever) file.
    ["Couldn't open archive"] = {
        cs = "Nemohu otevřít archiv",
        da = "Kunne ikke åbne arkiv",
        de = "Archiv kann nicht geöffnet werden",
        el = "Δεν μπόρεσε να ανοίξει το αρχείο",
        en_AU = "Couldn't open archive",
        en_CA = "Couldn't open archive",
        en_GB = "Couldn't open archive",
        es = "No se puede abrir el archivo",
        fi = "Paketin purkaminen epäonnistui",
        fr = "Impossible d'ouvrir l'archive.",
        hu = "Archív megnyitása sikertelen",
        it = "Impossibile aprire l'archivio",
        nb = "Kunne ikke åpne arkivet",
        nds = "Kann Archiv nicht öffnen",
        nl = "Kon archief niet openen",
        pl = "Nie można otworzyć archiwum",
        pt = "Foi impossível abrir o arquivo",
        pt_BR = "Não foi possível abrir o arquivo",
        ru = "Не могу открыть архив",
        sk = "Nemôžem otvoriť archív",
        sv = "Kunde inte öppna filen",
        tr = "Arşiv açılamadı",
        zh_TW = "無法開啟檔案包"
    };

    -- This is used by the stdio UI to choose a location to write files.
    --  A numbered list of options is printed, and the user may choose one by
    --  its number (default is number one), or enter their own text instead of
    --  choosing a default. This string is the instructions printed for the
    --  user before the prompt.
    ["Choose install destination by number (hit enter for #1), or enter your own."] = {
        cs = "Zvolte cíl instalace číslem (stiskněte enter pro #1) nebo zadejte vlastní cíl.",
        de = "Wählen Sie eine Nummer für das Installationsziel (drücken Sie Enter für #1), oder geben Sie ein eigenes an.",
        el = "Διαλέξτε προορισμό εγκατάστασης βάση αριθμού (πατήστε ENTER για #1), ή γραψτε τον δικό σας.",
        en_AU = "Choose install destination by number (hit enter for #1), or enter your own.",
        en_CA = "Choose install destination by number (hit enter for #1), or enter your own.",
        en_GB = "Choose install destination by number (hit enter for #1), or enter your own.",
        es = "Elige lugar de instalación con números (pulsa Intro para #1), o especifícalo",
        fi = "Valitse asennuskohteen numero (enter valitsee kohteen 1) tai syötä oma kohde.",
        fr = "Choisissez la destination d'installation par un nombre (Appuyez sur Entrée pour le n°1), ou entrez votre propre choix.",
        hu = "Válassz telepítési célt szám szerint (üss entert az elsőhöz), vagy adj meg egyet.",
        it = "Seleziona la destinazione dell'installazione tramite il numero corrispondente (invio per la #1), oppure digita il percorso desiderato.",
        nb = "Velg installasjonssti etter nummer (trykk enter for #1), eller skriv inn din egen.",
        nds = "Wählen Sie das Installationsverzeichnis durch Drücken einer Nummer (enter für #1), oder geben Sie selber einen ein.",
        nl = "Kies installatiemap met het bijbehorende nummer, of voer u eigen keuze in.",
        pl = "Wybierz miejsce instalacji wybierając numer (naciśnij enter dla #1), lub wpisz własną.",
        pt = "Escolha o destino da instalação pelo seu número (pressione enter para o primeiro destino), ou introduza um outro destino.",
        pt_BR = "Escolha um destino de instalação pelo número (pressione enter para 1), ou informe o seu própio.",
        ru = "Выберите путь установки (нажмите Enter для номера 1), или введите свой.",
        sk = "Vyberte cieľ inštalácie pomocou čísla (stlačte enter pre prvú možnosť).",
        sv = "Välj sökväg för installationen efter nummer (tryck enter för #1), eller skriv in din egen.",
        tr = "Kurulum yerini sayı yardımıyla seçiniz (#1 için girişe basınız), ya da kendiniz belirtiniz",
        zh_TW = "請依數字選擇欲安裝的目標位置（按 [Enter] 鍵可直接選取預設選項），或者自行輸入位置。"
    };

    -- This is used by the stdio UI to toggle options. A numbered list is
    --  printed, and the user can enter one of those numbers to toggle that
    --  option on or off. This string is the instructions printed for the
    --  user before the prompt.
    ["Choose number to change."] = {
        cs = "Zadejte číslo pro změnu.",
        da = "Vælg nummer som skal ændres.",
        de = "Wählen Sie eine Nummer zum Ändern.",
        el = "Διάλεξτε τον αριθμό που θέλετε να αλλάξετε.",
        en_AU = "Choose number to change.",
        en_CA = "Choose number to change.",
        en_GB = "Choose number to change.",
        es = "Elegir número para cambiar",
        fi = "Valitse muutettavan numero.",
        fr = "Entrez le numéro de l'option à modifier.",
        hu = "Adj meg egy számot a változtatáshoz.",
        it = "Seleziona il numero corrispondente per cambiare.",
        nb = "Velg nummer som skal endres.",
        nds = "Wähle eine Nummer zum ändern",
        nl = "Kies een nummer om te veranderen",
        pl = "Wybierz numer, aby zmienić.",
        pt = "Escolha um número para alterar",
        pt_BR = "Escolha o número para alterar",
        ru = "Выберите число для изменения",
        sk = "Vyberte pomocou čísla ak chcete zmeniť nastavenie",
        sv = "Välj nummer som skall ändras.",
        tr = "Değiştirmek için bir sayı seçiniz",
        zh_TW = "選擇數字以更改"
    };

    -- As in "two different files want to use the same name." This is a title
    --  on a message box.
    ["Conflict!"] = {
        cs = "Konflikt!",
        da = "Konflikt!",
        de = "Konflikt!",
        el = "Διένεξη!",
        en_AU = "Conflict!",
        en_CA = "Conflict!",
        en_GB = "Conflict!",
        es = "¡Conflicto!",
        fi = "Ristiriita!",
        fr = "Conflit !",
        hu = "Ütközés!",
        it = "Conflitto!",
        nb = "Konflikt!",
        nds = "Konflikt!",
        nl = "Conflict!",
        pl = "Konflikt!",
        pt = "Conflito!",
        pt_BR = "Conflito!",
        ru = "Конфликт!",
        sk = "Konflikt!",
        sv = "Konflikt!",
        tr = "Çakışma!",
        zh_TW = "衝突！"
    };

    -- This is an error message shown to the user. When a file is to be
    --  overwritten, we move it out of the way instead, so we can restore it
    --  ("roll the file back") in case of problems, with the goal of having
    --  an installation that fails halfway through reverse any changes it made.
    --  This error is shown if we can't move a file out of the way.
    ["Couldn't backup file for rollback"] = {
        cs = "Nemohu zazálohovat soubor pro obnovu",
        de = "Konnte Datei nicht zur Wiederherstellung sichern",
        el = "Δεν είναι δυνατή η αντιγραφή ασφαλείας για χρήση σε περίπτωση επαναφοράς",
        en_AU = "Couldn't backup file for rollback",
        en_CA = "Couldn't backup file for rollback",
        en_GB = "Couldn't backup file for rollback",
        es = "No se pudo guardar el archivo para inversión",
        fi = "Tiedoston varmuuskopionti ei onnistunut",
        fr = "Impossible de faire une copie de secours du fichier",
        hu = "Fájllista biztonsági mentése nem lehetséges",
        it = "Impossibile eseguire il backup del file per un successivo ripristino",
        nb = "Kunne ikke sikkerhetskopiere fil for tilbakerulling",
        nds = "Datei konnte für die Wiederherstellung nicht gesichert werden",
        nl = "Kon bestand niet backupen om de installatie terug te kunnen draaien",
        pl = "Nie można zaarchiwizować pliku do odtworzenia",
        pt = "Foi impossível salvaguardar o ficheiro para refazer as acções",
        pt_BR = "Não foi possível criar um backup ou restaurar o arquivo",
        ru = "Не могу сохранить копию файла для восстановления",
        sk = "Nemôžem zálohovať súbor pre obnovu",
        sv = "Kunde inte säkerhetskopiera filen för återställning",
        tr = "Dosya geri yükleme için yedeklenemedi",
        zh_TW = "無法備份檔案以復原"
    };

    -- This error is shown if we aren't able to write the list of files
    --  that were installed (the "manifest") to disk. Apparently some languages
    --  don't have a convenient translation of "manifest" ... it is not
    --  important that this word maps directly for end-users, as long as the
    --  general concept is explained.
    ["Couldn't create manifest"] = {
        cs = "Nemohu vytvořit manifest",
        de = "Konnte Manifest nicht erstellen",
        el = "Δεν μπόρεσε να δημιουργήσει το \"manifest\"",
        en_AU = "Couldn't create manifest",
        en_CA = "Couldn't create manifest",
        en_GB = "Couldn't create manifest",
        es = "No se podía crear manifest",
        fi = "Asennusluettelon luominen epäonnistui",
        fr = "Echec de création du fichier manifest",
        hu = "Telepítendő fájlok listájának létrehozása sikertelen",
        it = "Impossibile creare la lista dei file installati su disco.",
        nb = "Kunne ikke lage manifest",
        nds = "Erstellen des Manifests schlug fehl.",
        nl = "Kon het installatiemanifest niet maken",
        pl = "Nie można utworzyć manifestu",
        pt = "Foi impossível criar o manifesto",
        pt_BR = "Não é possível criar o manifest",
        ru = "Не могу создать манифест",
        sk = "Nemôžem vytvoriť manifest",
        sv = "Kunde inte skapa manifest",
        tr = "Manifesto oluşturulamadı",
        zh_TW = "無法建立檔案清單"
    };

    -- This is an error message. It speaks for itself.   :)
    ["Couldn't restore some files. Your existing installation is likely damaged."] = {
        cs = "Nepodařilo se obnovit některé soubory. Vaše existující instalace je pravděpodobně poškozená.",
        de = "Konnte einige Dateien nicht wiederherstellen. Ihre Installation ist wahrscheinlich beschädigt.",
        el = "Δεν μπόρεσε να ανακτήση κάποια αρχεία. Είναι πιθανόν η εγκατάσταση να είναι κατεστραμένη.",
        en_AU = "Couldn't restore some files. Your existing installation is likely damaged.",
        en_CA = "Couldn't restore some files. Your existing installation is likely damaged.",
        en_GB = "Couldn't restore some files. Your existing installation is likely to be damaged.",
        es = "No se pudieron restaurar algunos archivos. Tu instalación posiblemente esté dañada",
        fi = "Joidenkin tiedostojen palauttaminen epäonnistui. Asennuksesi on todennäköisesti rikki.",
        fr = "Impossible de restaurer certains fichiers. Votre installation existante est certainement endommagée.",
        hu = "Néhány fájlt nem lehet visszaállítani. A meglévő telepítés valószínűleg megsérült.",
        it = "Non è stato possibile ripristinare alcuni files. L'installazione corrente probabilmente è danneggiata.",
        nb = "Noen filer kunne ikke tilbakestilles. Den eksisterende installasjonen er sannsynligvis skadet.",
        nds = "Konnte einige Dateien nicht wiederherstellen. Ihre Installation ist möglicherweise beschädigt.",
        nl = "Kon sommige bestanden niet terugzetten. Uw bestaande installatie is waarschijnlijk beschadigd.",
        pl = "Nie można odtworzyć niektórych plików. Twoja aktualna instalacja jest prawdopodobnie uszkodzona.",
        pt = "Foi impossível recuperar alguns ficheiros. Provavelmente, a sua instalação está danificada.",
        pt_BR = "Não foi possível restaurar alguns arquivos. Sua instalação existente está, provavelmente, danificada.",
        ru = "Не могу восстановить некоторые файлы. Ваша установка наверно повреждена.",
        sk = "Nemôžem obnoviť niektoré súbory. Vaša aktuálna inštalácia je zrejme poškodená.",
        sv = "Några filer kunde inte återskapas. Den existerande installationen är troligtvis skadad.",
        tr = "Bazı dosyalar onarılamadı. Varolan kurulumunuz büyük olasılıkla zarar görmüş",
        zh_TW = "一些檔案無法復原，現有的安裝內容似乎已損毀。"
    };

    -- Error message when deleting a file fails.
    ["Deletion failed!"] = {
        cs = "Mazání selhalo!",
        da = "Kunne ikke slette!",
        de = "Löschen fehlgeschlagen!",
        el = "Απέτυχε η διαγραφή!",
        en_AU = "Deletion failed!",
        en_CA = "Deletion failed!",
        en_GB = "Deletion failed!",
        es = "¡Borrado fallido!",
        fi = "Tiedoston poisto epäonnistui!",
        fr = "Suppression de fichier échouée!",
        hu = "Törlés sikertelen!",
        it = "Eliminazione del file fallita!",
        nb = "Kunne ikke slette!",
        nds = "Löschen fehlgeschlagen!",
        nl = "Verwijderen mislukt!",
        pl = "Kasowanie nieudane!",
        pt = "A remoção falhou!",
        pt_BR = "Falha ao apagar o arquivo!",
        ru = "Не могу удалить!",
        sk = "Odstraňovanie súboru zlyhalo!",
        sv = "Radering misslyckades!",
        tr = "Silme işlemi başarılamadı!",
        zh_TW = "刪除失敗"
    };

    -- This is a label displayed next to the text entry in the GTK+ UI where
    --  the user specifies the installation destination (folder/directory) in
    --  the filesystem.
    ["Folder:"] = {
        cs = "Složka:",
        da = "Mappe:",
        de = "Verzeichnis:",
        el = "Φάκελος:",
        en_AU = "Folder:",
        en_CA = "Folder:",
        en_GB = "Folder:",
        es = "Carpeta:",
        fi = "Kansio:",
        fr = "Dossier :",
        hu = "Mappa:",
        it = "Cartella:",
        nb = "Katalog:",
        nds = "Ordner:",
        nl = "Map:",
        pl = "Folder:",
        pt = "Directoria:",
        pt_BR = "Pasta:",
        ru = "Папка:",
        sk = "Adresár:",
        sv = "Katalog:",
        tr = "Dizin:",
        zh_TW = "資料夾："
    };

    -- This is a window title when user is selecting a path to install files.
    ["Destination"] = {
        cs = "Cíl",
        da = "Destination",
        de = "Ziel",
        el = "Προορισμός",
        en_AU = "Destination",
        en_CA = "Destination",
        en_GB = "Destination",
        es = "Destino",
        fi = "Kohde",
        fr = "Destination",
        hu = "Cél",
        it = "Destinazione",
        nb = "Destinasjon",
        nds = "Ziel",
        nl = "Bestemming",
        pl = "Miejsce docelowe",
        pt = "Destino",
        pt_BR = "Destino",
        ru = "Назначение",
        sk = "Cieľ",
        sv = "Mål",
        tr = "Hedef",
        zh_TW = "目的地"
    };

    -- This is a window title while the program is downloading external files
    --  it needs from the network.
    ["Downloading"] = {
        cs = "Stahuji",
        da = "Henter",
        de = "Lade herunter",
        el = "Γίνεται λήψη",
        en_AU = "Downloading",
        en_CA = "Downloading",
        en_GB = "Downloading",
        es = "Descargando",
        fi = "Noudetaan",
        fr = "Téléchargement en cours",
        hu = "Letöltés",
        it = "Download in corso",
        nb = "Laster ned",
        nds = "Lade herunter",
        nl = "Bezig met downloaden",
        pl = "Pobieranie",
        pt = "A descarregar",
        pt_BR = "Baixando",
        ru = "Скачивание данных",
        sk = "Sťahujem",
        sv = "Hämtar",
        tr = "İndiriyor",
        zh_TW = "下載中"
    };

    -- Several UIs use this string as a prompt to the end-user when selecting
    --  a destination for newly-installed files.
    ["Enter path where files will be installed."] = {
        cs = "Zadejte cestu, kam mají být soubory nainstalovány.",
        de = "Geben Sie den Pfad an, wohin die Dateien installiert werden sollen.",
        el = "Δώστε το μονοπάτι που θα εγκατασταθούν τα αρχεία.",
        en_AU = "Enter path where files will be installed.",
        en_CA = "Enter path where files will be installed.",
        en_GB = "Enter path where files will be installed.",
        es = "Introduce la ruta donde los archivos serán instalados",
        fi = "Syötä polku, johon tiedostot asennetaan.",
        fr = "Entrez le chemin d'installation souhaité pour vos fichiers.",
        hu = "Add meg az útvonalat, ahova a fájlokat telepíthetem.",
        it = "Inserisci il percorso di installazione.",
        nb = "Skriv inn destinasjonssti for installasjonen.",
        nds = "Pfad eingeben, in den installiert werden soll.",
        nl = "Geef de map op waar de bestanden geinstalleerd zullen worden",
        pl = "Podaj ścieżkę gdzie pliki będą zainstalowane.",
        pt = "Introduza o caminho para os ficheiros que serão instalados.",
        pt_BR = "Entre o caminho aonde os arquivos serão instalados.",
        ru = "Введите путь куда будет произведена установка.",
        sk = "Zadajte cestu kam chcete inštalovať",
        sv = "Välj sökväg för installationen",
        tr = "Dosyaların kurulacağı yolu giriniz.",
        zh_TW = "輸入欲安裝檔案的路徑"
    };

    -- Error message when a file we expect to load can't be read from disk.
    ["failed to load file '%0'"] = {
        cs = "nepodařilo se načíst soubor '%0'",
        da = "Kunne ikke indlæse fil '%0'",
        de = "Laden von Datei '%0' fehlgeschlagen",
        el = "απέτυχε το φόρτωμα του αρχείου '%0'",
        en_AU = "failed to load file '%0'",
        en_CA = "failed to load file '%0'",
        en_GB = "failed to load file '%0'",
        es = "no se pudo leer el archivo '%0'",
        fi = "tiedoston \"%0\" lukeminen epäonnistui",
        fr = "Echec du chargement du fichier '%0'",
        hu = "fájl betöltése sikertelen '%0'",
        it = "Caricamento del file '%0' fallito",
        nb = "kunne ikke laste fil '%0'",
        nds = "Fehler beim Laden von '%0'",
        nl = "laden van bestand \"%0\" mislukt",
        pl = "Nie można załadować pliku '%0'",
        pt = "falhou a carregar o filheiro '%0'",
        pt_BR = "falha ao carregar o arquivo '%0'",
        ru = "не могу загрузить файл '%0'",
        sk = "nemôžem nahrať súbor '%0'",
        sv = "Misslyckades med att läsa filen '%0'",
        tr = "'%0' dosyası yüklenemedi",
        zh_TW = "無法讀取檔案 '%0'"
    };

    -- This is a window title when something goes very wrong.
    ["Fatal error"] = {
        cs = "Kritická chyba",
        da = "Alvorlig fejl",
        de = "Schwerer Fehler",
        el = "Μοιραίο σφάλμα",
        en_AU = "Fatal error",
        en_CA = "Fatal error",
        en_GB = "Fatal error",
        es = "Error grave",
        fi = "Ylitsepääsemätön virhe",
        fr = "Erreur fatale",
        hu = "Végzetes hiba",
        it = "Errore fatale",
        nb = "Fatal feil",
        nds = "Fataler Fehler",
        nl = "Fatale fout",
        pl = "Poważny błąd",
        pt = "Erro fatal",
        pt_BR = "Erro fatal",
        ru = "Фатальная ошибка",
        sk = "ZÁVAŽNÁ CHYBA",
        sv = "Allvarligt fel",
        tr = "Ölümcül hata",
        zh_TW = "嚴重錯誤"
    };

    -- This is an error message when failing to write a file to disk.
    ["File creation failed!"] = {
        cs = "Nepodařilo se vytvořit soubor!",
        de = "Dateierstellung fehlgeschlagen!",
        el = "Η δημιουργεία αρχείου απέτυχε!",
        en_AU = "File creation failed!",
        en_CA = "File creation failed!",
        en_GB = "File creation failed!",
        es = "¡Escritura de archivo ha fallado!",
        fi = "Tiedoston luominen epäonnistui!",
        fr = "Création de fichier échouée!",
        hu = "Fájl létrehozása sikertelen!",
        it = "Creazione del file fallita!",
        nb = "Kunne ikke lage fil!",
        nds = "Datei Erstellung ist fehlgeschlagen!",
        nl = "aanmaken bestand mislukt!",
        pl = "Nie można utworzyć pliku!",
        pt = "Falhou a criação do ficheiro!",
        pt_BR = "Erro na criação do arquivo!",
        ru = "Не могу создать файл!",
        sk = "Nemôžem vytvoriť súbor!",
        sv = "Misslyckades med att skapa fil!",
        tr = "Dosya oluşturumu başarılamadı!",
        zh_TW = "無法建立檔案！"
    };

    -- This is an error message when failing to get a file from the network.
    ["File download failed!"] = {
        cs = "Nepodařilo se stáhnout soubor!",
        da = "Download af fil mislykkedes!",
        de = "Dateidownload fehlgeschlagen!",
        el = "Η λήψη αρχείου απέτυχε!",
        en_AU = "File download failed!",
        en_CA = "File download failed!",
        en_GB = "File download failed!",
        es = "¡Descarga de archivo ha fallado!",
        fi = "Tiedoston noutaminen epäonnistui!",
        fr = "Téléchargement échoué!",
        hu = "Fájl letöltése sikertelen!",
        it = "Download del file fallito!",
        nb = "Kunne ikke laste ned fil!",
        nds = "Dateidownload fehlgeschlagen!",
        nl = "downloaden van bestand mislukt!",
        pl = "Nie można pobrać pliku!",
        pt = "Falhou a descarregar!",
        pt_BR = "Erro ao baixar o arquivo!",
        ru = "Ошибка скачивания файла!",
        sk = "Sťahovanie zlyhalo!",
        sv = "Hämtning av fil misslyckades!",
        tr = "Dosya indirimi başarılamadı!",
        zh_TW = "檔案下載失敗"
    };

    -- This prompt is shown to users when we may overwrite an existing file.
    --  "%0" is the filename.
    ["File '%0' already exists! Replace?"] = {
        cs = "Soubor '%0' již existuje! Přepsat?",
        de = "Datei '%0' existiert bereits! Ersetzen?",
        el = "Το αρχείο '%0' υπάρχει ήδη! Αντικατάσταση?",
        en_AU = "File '%0' already exists! Replace?",
        en_CA = "File '%0' already exists! Replace?",
        en_GB = "File '%0' already exists! Replace?",
        es = "¡El archivo '%0' ya existe! ¿Sustituirlo?",
        fi = "Tiedosto \"%0\" on jo olemassa! Korvaa?",
        fr = "Le fichier '%0' existe déjà! Le remplacer?",
        hu = "'%0' már létezik! Felülírjam?",
        it = "Il file %0 esiste già! Sostituire?",
        nb = "Filen '%0' eksisterer allerede! Skrive over?",
        nds = "Datei '%0' existiert bereits! Überschreiben?",
        nl = "Bestand \"%0\" bestaat al! Vervangen?",
        pl = "Plik '%0' już istnieje! Nadpisać?",
        pt = "O ficheiro '%0' já existe! Substituir?",
        pt_BR = "Arquivo '%0' já existe! Substituir?",
        ru = "Файл '%0' уже существует! Заменить?",
        sk = "Súbor '%0' už existuje! Prepísať?",
        sv = "Filen '%0' existerar redan! Vill du ersätta den?",
        tr = "'%0' dosyası zaten var! Değiştirilsin mi?",
        zh_TW = "檔案 '%0' 已經存在，是否取代？"
    };

    -- This is a button in the UI. It replaces "Next" when there are no more
    --  stages to move forward to.
    ["Finish"] = {
        cs = "Dokončit",
        da = "Udfør",
        de = "Fertig stellen",
        el = "Τέλος",
        en_AU = "Finish",
        en_CA = "Finish",
        en_GB = "Finish",
        es = "Terminar",
        fi = "Valmis",
        fr = "Terminer",
        hu = "Kész",
        it = "Fine",
        nb = "Ferdig",
        nds = "Fertig",
        nl = "Voltooien",
        pl = "Zakończ",
        pt = "Finalizar",
        pt_BR = "Concluir",
        ru = "Завершение",
        sk = "Dokončiť",
        sv = "Slutför",
        tr = "Bitir",
        zh_TW = "完成"
    };

    -- This error message is (hopefully) shown to the user if the UI
    --  subsystem can't create the main application window.
    ["GUI failed to start"] = {
        cs = "Nepodařilo se spustit GUI",
        de = "GUI konnte nicht gestartet werden",
        el = "Το γραφικό περιβάλλον απέτυχε να ξεκινήσει",
        en_AU = "GUI failed to start",
        en_CA = "GUI failed to start",
        en_GB = "GUI failed to start",
        es = "El interfaz gráfico de usuario ha fallado al arrancar",
        fi = "Graafinen käyttöliittymä ei käynnistynyt",
        fr = "Echec du démarrage de l'interface",
        hu = "Grafikus felület indítása sikertelen",
        it = "Inizializzazione della GUI fallita",
        nb = "Kunne ikke starte grafisk grensesnitt",
        nds = "Fehler beim starten der GUI",
        nl = "Opstarten GUI mislukt",
        pl = "GUI nie mógł się uruchomić",
        pt = "O GUI falhou a iniciar",
        pt_BR = "Falha ao iniciar a GUI",
        ru = "Ошибка запуска графического интерфейса",
        sk = "Nieje možné spustiť GUI",
        sv = "Kunde inte starta grafisk gränssnitt",
        tr = "Grafik arabirimi başlatılamadı",
        zh_TW = "無法啟動圖形使用者介面"
    };

    -- This message is shown to the user if an installation encounters a fatal
    --  problem (or the user clicked "cancel"), telling them that we'll try
    --  to put everything back how it was before we started.
    ["Incomplete installation. We will revert any changes we made."] = {
        cs = "Instalace nebyla dokončena. Vše bude uvedeno do původního stavu.",
        de = "Unvollständige Installation. Änderungen werden rückgängig gemacht.",
        el = "Μισοτελειωμένη εγκατάσταση. Θα διαγράψουμε όσες αλλαγές κάναμε.",
        en_AU = "Incomplete installation. We will revert any changes we made.",
        en_CA = "Incomplete installation. We will revert any changes we made.",
        en_GB = "Incomplete installation. We will revert any changes we made.",
        es = "Instalación incompleta. Vamos a deshacer cualquier cambio que hayamos hecho.",
        fi = "Asennus jäi kesken. Tehdyt muutokset perutaan.",
        fr = "Installation incomplète. Tous les changements effectués seront annulés.",
        hu = "Telepítés befejezetlen. Eredeti helyzet visszaállítása.",
        it = "Installazione incompleta. Ripristino delle modifiche effettuate in corso.",
        nb = "Installasjonen ble ikke ferdig. Vi vil tilbakestille alle endringer som ble gjort.",
        nds = "Unvollständige Installation. Alle Veränderungen werden rückgängig gemacht.",
        nl = "Incomplete installatie. De veranderingen zullen ongedaan gemaakt worden.",
        pl = "Niekompletna instalacja. Cofniemy dokonane zmiany.",
        pt = "Instalação incompleta. Todas as alterações serão desfeitas.",
        pt_BR = "Instalação incompleta. As alterações feitas serão revertidas.",
        ru = "Ошибка установки. Все изменения будут отменены.",
        sk = "Neúplná inštalácia. Vraciam spať vykonané zmeny.",
        sv = "Ofullständig installation. Återställning av alla gjorda ändringar utförs.",
        tr = "Eksik kurulum. Yaptığımız tüm değişiklikleri geri alacağız.",
        zh_TW = "安裝程序未完成，任何更動將會被復原"
    };

    -- Reported to the user if everything worked out.
    ["Installation was successful."] = {
        cs = "Instalace byla úspěšná.",
        da = "Installationen var succesfuld.",
        de = "Installation war erfolgreich.",
        el = "Η εγκατάσταση πέτυχε.",
        en_AU = "Installation was successful.",
        en_CA = "Installation was successful.",
        en_GB = "Installation was successful.",
        es = "La instalación fue un éxito.",
        fi = "Asentaminen onnistui.",
        fr = "Installation réussie.",
        hu = "Sikeres telepítés.",
        it = "Installazione riuscita.",
        nb = "Installasjonen var en suksess.",
        nds = "Die Installation war erfolgreich",
        nl = "De installatie is succesvol verlopen.",
        pl = "Instalacja zakończona poprawnie.",
        pt = "A instalação foi bem sucedida.",
        pt_BR = "Instalação concluída com sucesso.",
        ru = "Установка была успешна.",
        sk = "Inštalácia je hotová.",
        sv = "Installationen lyckades.",
        tr = "Kurulum başarılı oldu.",
        zh_TW = "安裝程序完成。"
    };

    -- This is a window title, shown while the actual installation to disk
    --  is in process and a progress meter is being shown.
    ["Installing"] = {
        cs = "Instaluji",
        da = "Installerer",
        de = "Installiere",
        el = "Γίνεται εγκατάσταση",
        en_AU = "Installing",
        en_CA = "Installing",
        en_GB = "Installing",
        es = "Instalando",
        fi = "Asennetaan",
        fr = "Installation en cours",
        hu = "Telepítés",
        it = "Installazione in corso",
        nb = "Installerer",
        nds = "Installiere ...",
        nl = "Installeren",
        pl = "Instalacja",
        pt = "A instalar",
        pt_BR = "Instalando",
        ru = "Установка",
        sk = "Inštalujem",
        sv = "Installerar",
        tr = "Kuruluyor",
        zh_TW = "正在安裝"
    };

    -- This is a window title, shown while the user is choosing
    --  installation-specific options.
    ["Options"] = {
        cs = "Volby",
        da = "Indstillinger",
        de = "Optionen",
        el = "Επιλογές",
        en_AU = "Options",
        en_CA = "Options",
        en_GB = "Options",
        es = "Opciones",
        fi = "Valinnat",
        fr = "Options",
        hu = "Beállítások",
        it = "Opzioni",
        nb = "Valg",
        nds = "Optionen",
        nl = "Opties",
        pl = "Opcje",
        pt = "Opções",
        pt_BR = "Opções",
        ru = "Настройки",
        sk = "Možnosti",
        sv = "Alternativ",
        tr = "Seçenekler",
        zh_TW = "選項"
    };

    -- Shown as an option in the ncurses UI as the final element in a list of
    --  default filesystem paths where a user may install files. They can
    --  choose this to enter a filesystem path manually.
    ["(I want to specify a path.)"] = {
        cs = "(Chci určit cestu.)",
        de = "(Ich möchte einen Pfad angeben.)",
        el = "(Θέλω να ορίσω μονοπάτι.)",
        en_AU = "(I want to specify a path.)",
        en_CA = "(I want to specify a path.)",
        en_GB = "(I want to specify a path.)",
        es = "(Quiero especificar una ruta.)",
        fi = "(Haluan syöttää polun.)",
        fr = "(Je veux spécifier une destination.)",
        hu = "(egy útvonalat szeretnék)",
        it = "(Voglio specificare un percorso)",
        nb = "(Jeg vil skrive inn min egen sti.)",
        nds = "(Ich möchte einen Pfad angeben.)",
        nl = "(Ik wil een pad specificeren.)",
        pl = "(Chcę podać ścieżkę.)",
        pt = "(Eu quero especificar o caminho.)",
        pt_BR = "(Eu quero especificar um caminho.)",
        ru = "(указать путь.)",
        sk = "(Chcem zadať cestu ručne)",
        sv = "(Jag vill skriva in en egen sökväg.)",
        tr = "(Bir yol belirtmek istiyorum.)",
        zh_TW = "（我想要指定路徑）"
    };

    -- "kilobytes per second" ... download rate.
    ["KB/s"] = {
        cs = "KB/s",
        da = "KB/s",
        de = "KB/s",
        el = "KB/δευτ.",
        en_AU = "KB/s",
        en_CA = "KB/s",
        en_GB = "KB/s",
        es = "KB/s",
        fi = "kt/s",
        fr = "Ko/s",
        hu = "KB/mp",
        it = "KB/s",
        nb = "KB/s",
        nds = "kB/s",
        nl = "KB/s",
        pl = "KB/s",
        pt = "KB/s",
        pt_BR = "KB/s",
        ru = "КБ/с",
        sk = "KB/s",
        sv = "KB/s",
        tr = "KB/s",
        zh_TW = "KB/秒"
    };

    -- "bytes per second" ... download rate.
    ["B/s"] = {
        cs = "B/s",
        de = "B/s",
        el = "B/δευτ",
        en_AU = "B/s",
        en_CA = "B/s",
        en_GB = "B/s",
        es = "B/s",
        fi = "t/s",
        fr = "octets/s",
        hu = "B/mp",
        it = "B/s",
        nb = "B/s",
        nds = "B/s",
        nl = "B/s",
        pl = "B/s",
        pt = "B/s",
        pt_BR = "B/s",
        ru = "Б/с",
        sk = "B/s",
        sv = "B/s",
        tr = "Byte/sn",
        zh_TW = "位元組/秒"
    };

    -- Download rate when we don't know the goal (can't report time left).
    --  This is a number (%0) followed by the localized "KB/s" or "B/s" (%1).
    ["%0 %1"] = {
        cs = "%0 %1",
        da = "%0 %1",
        de = "%0 %1",
        el = "%0 %1",
        en_AU = "%0 %1",
        en_CA = "%0 %1",
        en_GB = "%0 %1",
        es = "%0 %1",
        fi = "%0 %1",
        fr = "%0 %1",
        hu = "%0 %1",
        it = "%0 %1",
        nb = "%0 %1",
        nds = "%0 %1",
        nl = "%0 %1",
        pl = "%0 %1",
        pt = "%0 %1",
        pt_BR = "%0 %1",
        ru = "%0 %1",
        sk = "%0 %1",
        sv = "%0 %1",
        tr = "%0 %1",
        zh_TW = "%0 %1"
    };

    -- Download rate when we know the goal (can report time left).
    --  This is a number (%0) followed by the localized "KB/s" or "B/s" (%1),
    --  then the hours (%2), minutes (%3), and seconds (%4) remaining
    ["%0 %1, %2:%3:%4 remaining"] = {
        cs = "%0 %1, %2:%3:%4 zbývá",
        de = "%0 %1, %2:%3:%4 verbleibend",
        el = "%0 %1, απομένουν %2:%3:%4",
        en_AU = "%0 %1, %2:%3:%4 remaining",
        en_CA = "%0 %1, %2:%3:%4 remaining",
        en_GB = "%0 %1, %2:%3:%4 remaining",
        es = "%0 %1, %2:%3:%4 restantes",
        fi = "%0 %1, %2:%3:%4 jäljellä",
        fr = "%0 %1, %2:%3:%4 restantes",
        hu = "%0 %1, %2:%3:%4 van még hátra",
        it = "%0 %1, %2:%3:%4 rimanenti",
        nb = "%0 %1, %2:%3:%4 igjen",
        nds = "%0 %1, %2:%3:%4 verbleiben",
        nl = "%0 %1, %2:%3:%4 resterend",
        pl = "%0 %1, %2:%3:%4 do końca",
        pt = "restam %0 %1, %2:%3:%4",
        pt_BR = "%0 %1, faltam %2:%3:%4",
        ru = "%0 %1, осталось %2:%3:%4",
        sk = "%0 %1, %2:%3:%4 zostáva",
        sv = "%0 %1, %2:%3:%4 återstår",
        tr = "%0 %1, %2:%3:%4 kalıyor",
        zh_TW = "剩餘 %0 %1, %2:%3:%4"
    };

    -- download rate when download isn't progressing at all.
    ["stalled"] = {
        cs = "zaseknuté",
        de = "wartend",
        el = "καμία πρόοδος",
        en_AU = "stalled",
        en_CA = "stalled",
        en_GB = "stalled",
        es = "parado",
        fi = "seisahtunut",
        fr = "bloqué",
        hu = "beragadt",
        it = "in stallo",
        nb = "står fast",
        nds = "Angehalten",
        nl = "geen progressie",
        pl = "wstrzymano",
        pt = "Parado",
        pt_BR = "parado",
        ru = "застряло",
        sk = "zastavený",
        sv = "avstannad",
        tr = "duraksadı",
        zh_TW = "已延遲"
    };

    -- Download progress string: filename (%0), percent downloaded (%1),
    --  download rate determined in one of the above strings (%2).
    ["%0: %1%% (%2)"] = {
        cs = "%0: %1%% (%2)",
        da = "%0: %1%% (%2)",
        de = "%0: %1%% (%2)",
        el = "%0: %1%% (%2)",
        en_AU = "%0: %1%% (%2)",
        en_CA = "%0: %1%% (%2)",
        en_GB = "%0: %1%% (%2)",
        es = "%0: %1%% (%2)",
        fi = "%0: %1%% (%2)",
        fr = "%0: %1%% (%2)",
        hu = "%0: %1%% (%2)",
        it = "%0: %1%% (%2)",
        nb = "%0: %1%% (%2)",
        nds = "%0: %1%% (%2)",
        nl = "%0: %1%% (%2)",
        pl = "%0: %1%% (%2)",
        pt = "%0: %1%% (%2)",
        pt_BR = "%0: %1%% (%2)",
        ru = "%0: %1%% (%2)",
        sk = "%0: %1%% (%2)",
        sv = "%0: %1%% (%2)",
        tr = "%0: %1%% (%2)",
        zh_TW = "%0: %1%% (%2)"
    };

    -- This is a window title when prompting the user to insert a new disc.
    ["Media change"] = {
        cs = "Výměna média",
        da = "Skift medie",
        de = "Medienwechsel",
        el = "Αλάξτε μέσο αποθήκευσης",
        en_AU = "Media change",
        en_CA = "Media change",
        en_GB = "Media change",
        es = "Cambio de disco",
        fi = "Median vaihto",
        fr = "Changement de média",
        hu = "Következő média",
        it = "Inserire il prossimo disco",
        nb = "Mediaendring",
        nds = "Medien Wechsel",
        nl = "Verwissel CD/medium",
        pl = "Zmiana nośnika",
        pt = "Mudança de media",
        pt_BR = "Mídia alterada",
        ru = "Смена диска",
        sk = "Výmena média",
        sv = "Mediabyte",
        tr = "Medya değişimi",
        zh_TW = "媒體更改"
    };

    -- This error message is shown to the end-user when we can't make a new
    --  folder/directory in the filesystem.
    ["Directory creation failed"] = {
        cs = "Selhalo vytváření adresáře",
        da = "Kunne ikke oprette katalog",
        de = "Erstellung eines Verzeichnisses fehlgeschlagen",
        el = "Η δημιουργεία φακέλου απέτυχε",
        en_AU = "Directory creation failed",
        en_CA = "Directory creation failed",
        en_GB = "Directory creation failed",
        es = "Fallo en la creación de directorio",
        fi = "Hakemiston luominen epäonnistui",
        fr = "Création de répertoire échouée",
        hu = "Mappa létrehozása sikertelen",
        it = "Creazione directory fallita",
        nb = "Kunne ikke lage katalog",
        nds = "Ordner erstellen schlug fehl!",
        nl = "Aanmaken folder mislukt",
        pl = "Tworzenie katalogu nie powiodło się",
        pt = "A criação da directoria falhou",
        pt_BR = "Falha na criação do diretório",
        ru = "Ошибка создания каталога",
        sk = "Nemôžem vytvoriť adresár",
        sv = "Kunde inte skapa katalog",
        tr = "Dizin oluşturumu başarılamadı",
        zh_TW = "無法建立目錄"
    };

    -- This is a GTK+ button label. The '_' comes before the hotkey character.
    --  "No" would take the 'N' hotkey in English.
    ["N_ever"] = {
        cs = "Ni_kdy",
        da = "Al_drig",
        de = "Ni_emals",
        el = "_Ποτέ",
        en_AU = "N_ever",
        en_CA = "N_ever",
        en_GB = "N_ever",
        es = "N_unca",
        fi = "Ei _koskaan",
        fr = "_Jamais",
        hu = "_Soha",
        it = "_Mai",
        nb = "Al_dri",
        nds = "N_ie",
        nl = "No_oit",
        pl = "N_igdy",
        pt = "N_unca",
        pt_BR = "N_unca",
        ru = "Н_икогда",
        sk = "N_ikdy",
        sv = "Al_drig",
        tr = "Asla",
        zh_TW = "永不(_N)"
    };

    -- This is a GUI button label, to move forward to the next stage of
    --  installation. It's opposite is "Back" in this case.
    ["Next"] = {
        cs = "Další",
        da = "Næste",
        de = "Weiter",
        el = "Επόμενο",
        en_AU = "Next",
        en_CA = "Next",
        en_GB = "Next",
        es = "Siguiente",
        fi = "Seuraava",
        fr = "Suivant",
        hu = "Következő",
        it = "Avanti",
        nb = "Neste",
        nds = "Weiter",
        nl = "Volgende",
        pl = "Dalej",
        pt = "Seguinte",
        pt_BR = "Avançar",
        ru = "Далее",
        sk = "Ďalej",
        sv = "Nästa",
        tr = "Sonraki",
        zh_TW = "下一個"
    };

    -- This is a GUI button label, indicating a negative response.
    ["No"] = {
        cs = "Ne",
        da = "Nej",
        de = "Nein",
        el = "Όχι",
        en_AU = "No",
        en_CA = "No",
        en_GB = "No",
        es = "No",
        fi = "Ei",
        fr = "Non",
        hu = "Nem",
        it = "No",
        nb = "Nei",
        nds = "Nein",
        nl = "Nee",
        pl = "Nie",
        pt = "Não",
        pt_BR = "Não",
        ru = "Нет",
        sk = "Nie",
        sv = "Nej",
        tr = "Hayır",
        zh_TW = "否"
    };

    -- This is a GUI button label, indicating a positive response.
    ["Yes"] = {
        cs = "Ano",
        da = "Ja",
        de = "Ja",
        el = "Ναί",
        en_AU = "Yes",
        en_CA = "Yes",
        en_GB = "Yes",
        es = "Sí",
        fi = "Kyllä",
        fr = "Oui",
        hu = "Igen",
        it = "Si",
        nb = "Ja",
        nds = "Ja",
        nl = "Ja",
        pl = "Tak",
        pt = "Sim",
        pt_BR = "Sim",
        ru = "Да",
        sk = "Áno",
        sv = "Ja",
        tr = "Evet",
        zh_TW = "是"
    };

    -- HTTP error message in the www UI, as in "404 Not Found" ... requested
    --  file is missing.
    ["Not Found"] = {
        cs = "Nenalezeno",
        da = "Ikke fundet",
        de = "Nicht gefunden",
        el = "Δεν βρέθηκε",
        en_AU = "Not Found",
        en_CA = "Not Found",
        en_GB = "Not Found",
        es = "No encontrado",
        fi = "Ei löytynyt",
        fr = "Introuvable",
        hu = "Nem található",
        it = "Non Trovato",
        nb = "Ikke funnet",
        nds = "Nicht gefunden",
        nl = "Niet Gevonden",
        pl = "Nie znaleziono",
        pt = "Inexistente",
        pt_BR = "Não Encontrado",
        ru = "Не найдено",
        sk = "Nemôžem nájsť",
        sv = "Hittades inte",
        tr = "Bulunamadı",
        zh_TW = "找不到"
    };

    -- This is reported to the user when there are no files to install, and
    --  thus no installation to go forward.
    ["Nothing to do!"] = {
        cs = "Není co instalovat!",
        de = "Nichts zu tun!",
        el = "Δεν υπάρχει τίποτα για να γίνει!",
        en_AU = "Nothing to do!",
        en_CA = "Nothing to do!",
        en_GB = "Nothing to do!",
        es = "¡Nada que hacer!",
        fi = "Ei tehtävää!",
        fr = "Rien à faire!",
        hu = "Nincs mit telepítenem!",
        it = "Impossible procedere!",
        nb = "Ingenting å gjøre!",
        nds = "Nichts zu tun!",
        nl = "Niets te doen!",
        pl = "Nic do zrobienia!",
        pt = "Nada por fazer!",
        pt_BR = "Nada para fazer!",
        ru = "Нечего делать!",
        sk = "Nemám čo robiť",
        sv = "Ingenting att göra!",
        tr = "Yapılacak bir iş yok!",
        zh_TW = "沒有需要執行的程序"
    };

    -- This is a GUI button label, sometimes paired with "Cancel"
    ["OK"] = {
        cs = "OK",
        da = "OK",
        de = "OK",
        el = "Εντάξει",
        en_AU = "OK",
        en_CA = "OK",
        en_GB = "OK",
        es = "Aceptar",
        fi = "Hyväksy",
        fr = "OK",
        hu = "OK",
        it = "OK",
        nb = "OK",
        nds = "OK",
        nl = "OK",
        pl = "OK",
        pt = "Aceitar",
        pt_BR = "OK",
        ru = "Да",
        sk = "OK",
        sv = "OK",
        tr = "Tamam",
        zh_TW = "確定"
    };

    -- This is displayed when the application has a serious problem, such as
    --  crashing again in the crash handler, or being unable to find basic
    --  files it needs to get started. It may be a window title, or written
    --  to stdout, or whatever, but it's basically meant to be a title or
    --  header, with more information to follow later.
    ["PANIC"] = {
        cs = "SELHÁNÍ",
        da = "PANIK",
        de = "PANIK",
        el = "ΠΑΝΙΚΟΣ",
        en_AU = "PANIC",
        en_CA = "PANIC",
        en_GB = "PANIC",
        es = "PÁNICO",
        fi = "HÄTÄTILA",
        fr = "PANIC",
        hu = "PÁNIK",
        it = "PANICO",
        nb = "PANIKK",
        nds = "PANIK",
        nl = "PROBLEEM",
        pl = "PANIKA",
        pt = "PÂNICO",
        pt_BR = "PÂNICO",
        ru = "СЕРЬЕЗНАЯ ОШИБКА",
        sk = "PANIKA",
        sv = "PANIK",
        tr = "PANİK",
        zh_TW = "嚴重錯誤"
    };

    -- Prompt shown to user when we need her to insert a new disc.
    ["Please insert '%0'"] = {
        cs = "Prosím vložte '%0'",
        de = "Bitte legen Sie '%0' ein",
        el = "Παρακαλώ εισάγεται '%0'",
        en_AU = "Please insert '%0'",
        en_CA = "Please insert '%0'",
        en_GB = "Please insert '%0'",
        es = "Por favor introduce '%0'",
        fi = "Syötä \"%0\"",
        fr = "Veuillez insérer '%0'",
        hu = "Helyezze be '%0'-t",
        it = "Per favore inserire '%0'",
        nb = "Sett inn '%0'",
        nds = "Bitte '%0' in das Laufwerk einlegen.",
        nl = "Laadt '%0' a.u.b.",
        pl = "Proszę włożyć '%0'",
        pt = "Por favor insira '%0'",
        pt_BR = "Por favor, insira '%0'",
        ru = "Вставьте, пожалуйста, '%0'",
        sk = "Prosím vložte '%0'",
        sv = "Sätt in '%0'",
        tr = "Lütfen takınız '%0'",
        zh_TW = "請插入 '%0'"
    };

    -- Prompt shown to user in the stdio UI when we need to pause before
    --  continuing, usually to let them read the outputted text that is
    --  scrolling by.
    ["Press enter to continue."] = {
        cs = "Pro pokračování stiskněte enter.",
        da = "Tryk retur for at fortsætte.",
        de = "Drücken Sie Enter um fortzufahren",
        el = "Πατήστε ENTER για συνέχεια.",
        en_AU = "Press enter to continue.",
        en_CA = "Press enter to continue.",
        en_GB = "Press enter to continue.",
        es = "Presione Intro para continuar.",
        fi = "Jatka painamalla enter.",
        fr = "Veuillez appuyer sur Entrée pour continuer.",
        hu = "Üss egy entert a folytatáshoz.",
        it = "Premere invio per continuare.",
        nb = "Trykk enter for å fortsette.",
        nds = "Enter drücken, um fortzufahren.",
        nl = "Toets enter om door te gaan.",
        pl = "Proszę nacisnąć enter, aby kontynuować.",
        pt = "Pressione enter para continuar.",
        pt_BR = "Pressione enter para continuar.",
        ru = "Нажмите Enter для продолжения",
        sk = "Stlačte Enter pre pokračovanie.",
        sv = "Tryck enter för att fortsätta.",
        tr = "Devam etmek için giriş tuşuna basınız.",
        zh_TW = "請按 [Enter] 鍵繼續。"
    };

    -- This is a window title when informing the user that something
    --  important has gone wrong (such as being unable to revert changes
    --  during a rollback).
    ["Serious problem"] = {
        cs = "Závažný problém",
        da = "Seriøst problem",
        de = "Ernstes Problem",
        el = "Σοβαρό πρόβλημα",
        en_AU = "Serious problem",
        en_CA = "Serious problem",
        en_GB = "Serious problem",
        es = "Problema grave",
        fi = "Vakava ongelma",
        fr = "Problème grave",
        hu = "Komoly probléma",
        it = "Problema critico",
        nb = "Alvorlig problem",
        nds = "Ernstzunehmender Fehler",
        nl = "Serieus probleem",
        pl = "Poważny błąd",
        pt = "Problema sério",
        pt_BR = "Problema sério",
        ru = "Серьезная ошибка",
        sk = "Vážny problém",
        sv = "Allvarligt problem",
        tr = "Ciddi problem",
        zh_TW = "嚴重問題"
    };

    -- The www UI uses this as a page title when the program is terminating.
    ["Shutting down..."] = {
        cs = "Ukončuji se...",
        da = "Lukker ned...",
        de = "Schließe...",
        el = "Γίνεται τερματισμός...",
        en_AU = "Shutting down...",
        en_CA = "Shutting down...",
        en_GB = "Shutting down...",
        es = "Cerrando ...",
        fi = "Ajetaan alas...",
        fr = "Fermeture en cours...",
        hu = "Leállítás...",
        it = "Uscita in corso...",
        nb = "Avslutter...",
        nds = "Fahre runter ...",
        nl = "Afsluiten...",
        pl = "Zamykanie...",
        pt = "A terminar...",
        pt_BR = "Desligando...",
        ru = "Завершение",
        sk = "Vypínam sa...",
        sv = "Avslutar...",
        tr = "Kapatılıyor...",
        zh_TW = "關閉……"
    };

    -- The www UI uses this as page text when the program is terminating.
    ["You can close this browser now."] = {
        cs = "Nyní můžete ukončit tento prohlížeč.",
        da = "Du kan lukke browseren nu.",
        de = "Sie können diesen Browser nun schließen.",
        el = "Μπορείτε να κλείσετε αυτόν τον περιηγητή τώρα.",
        en_AU = "You can close this browser now.",
        en_CA = "You can close this browser now.",
        en_GB = "You can close this browser now.",
        es = "Puedes cerrar este navegador ahora.",
        fi = "Voit sulkea tämän selaimen.",
        fr = "Vous pouvez maintenant fermer ce navigateur",
        hu = "Most már bezárhatod ezt az ablakot.",
        it = "Puoi chiudere il browser adesso.",
        nb = "Du kan lukke denne nettleseren nå.",
        nds = "Sie können den Browser nun schließen.",
        nl = "U kunt dit scherm nu sluiten.",
        pl = "Możesz zamknąć już przeglądarkę.",
        pt = "Já pode fechar este browser.",
        pt_BR = "Você não pode fechar esse navegador agora.",
        ru = "Можете закрыть браузер.",
        sk = "Môžete zavreť tento prehliadač.",
        sv = "Du kan stänga denna webbläsare nu.",
        tr = "Bu tarayıcı penceresini şimdi kapatabilirsiniz.",
        zh_TW = "你現在已可以關閉瀏覽器"
    };

    -- Error message shown to end-user when we can't write a symbolic link
    --  to the filesystem.
    ["Symlink creation failed!"] = {
        cs = "Selhalo vytváření symbolického odkazu!",
        de = "Erzeugung einer Verknüpfung fehlgeschlagen!",
        el = "Απέτυχε η δημιουργεία ενός symlink!",
        en_AU = "Symlink creation failed!",
        en_CA = "Symlink creation failed!",
        en_GB = "Symlink creation failed!",
        es = "¡Creación de enlace simbólico ha fallado!",
        fi = "Symbolisen linkin luominen epäonnistui!",
        fr = "Création de lien symbolique échouée!",
        hu = "Symlink készítés sikertelen!",
        it = "Creazione del collegamento (symlink) fallita!",
        nb = "Kunne ikke lage symbolsk lenke!",
        nds = "Symbolischer Link erstellen schlug fehl!",
        nl = "Aanmaken Symlink mislukt!",
        pl = "Nie udało się utworzyć odnośnika symbolicznego!",
        pt = "A criação de uma ligação simbólica falhou!",
        pt_BR = "Falha na criação do link simbólico!",
        ru = "Ошибка создания ссылки!",
        sk = "Nemôžem vytvoriť symlink",
        sv = "Kunde inte skapa symbolisk länk!",
        tr = "Sembolik bağ oluşturulamadı!",
        zh_TW = "無法建立連結"
    };

    -- Error message shown to the end-user when the OS has requested
    --  termination of the program (SIGINT/ctrl-c on Unix, etc).
    ["The installer has been stopped by the system."] = {
        cs = "Instalátor byl zastaven signálem ze systému.",
        de = "Das Installationsprogramm wurde vom System gestoppt.",
        el = "Το πρόγραμμα εγκατάστασης διακόπηκε από το σύστημα.",
        en_AU = "The installer has been stopped by the system.",
        en_CA = "The installer has been stopped by the system.",
        en_GB = "The installer has been stopped by the system.",
        es = "El instalador ha sido interrumpido por el sistema.",
        fi = "Järjestelmä pysäytti asennusohjelman.",
        fr = "L'installeur a été interrompu par le système.",
        hu = "A telepítő megszakítva a rendszer által.",
        it = "L'installazione è stata terminata dal sistema operativo.",
        nb = "Installasjonsprogrammet ble stoppet av systemet.",
        nds = "Der Installer wurde vom System gestoppt.",
        nl = "Het installatieprogramma is door het systeem gestopt.",
        pl = "Instalator został zatrzymany przez system.",
        pt = "O instalador foi parado pelo sistema.",
        pt_BR = "O instalador foi parado pelo sistema.",
        ru = "Программа установки была остановлена системой.",
        sk = "Inštalátor bol zastavený systémom.",
        sv = "Installationsprogrammet blev stoppat av systemet.",
        tr = "Kurulum programı sistem tarafından durduruldu.",
        zh_TW = "安裝程式已被系統終止"
    };

    -- Error message shown to the end-user when the program crashes with a
    --  bad memory access (segfault on Unix, GPF on Windows, etc).
    ["The installer has crashed due to a bug."] = {
        cs = "Instalátor zhavaroval vinou vlastní chyby.",
        de = "Das Installationsprogramm ist aufgrund eines Fehlers abgestürzt.",
        el = "Το πρόγραμμα εγκατάστασης έσκασε λόγω κάποιου σφάλματος.",
        en_AU = "The installer has crashed due to a bug.",
        en_CA = "The installer has crashed due to a bug.",
        en_GB = "The installer has crashed due to a bug.",
        es = "El instalador se ha bloqueado por un fallo de memoria.",
        fi = "Bugi kaatoi asennusohjelman.",
        fr = "L'installateur a quitté inopinément à cause d'un bogue.",
        hu = "A telepítő egy bug miatt összeomlott.",
        it = "L'installer è crashato a causa di un bug.",
        nb = "Installasjonsprogrammet kræsjet pga. en feil.",
        nds = "Der Installer ist Aufgrund eines Fehlers abgestürzt.",
        nl = "Het installatieprogramma is door een bug gecrasht.",
        pl = "Instalator przerwał działanie z powodu błędu.",
        pt = "O instalador terminou acidentalmente devido a um erro.",
        pt_BR = "O instalador quebrou devido a um bug.",
        ru = "Программа установки завершилась из-за ошибки.",
        sk = "Inštalátor havaroval kôli chybe.",
        sv = "Installationsprogrammet kraschade pga. ett fel.",
        tr = "Kurulum bir hata yüzünden çöktü.",
        zh_TW = "安裝程式因臭蟲而終止"
    };

    -- This is a button label in the ncurses ui to flip an option on/off.
    ["Toggle"] = {
        cs = "Přepnout",
        da = "Skift",
        de = "Umschalten",
        el = "Εναλλαγή",
        en_AU = "Toggle",
        en_CA = "Toggle",
        en_GB = "Toggle",
        es = "Conmutar",
        fi = "Vipu",
        fr = "Basculer",
        hu = "Kapcsoló",
        it = "Attiva/Disattiva",
        nb = "Inverter valg",
        nds = "Umschalten",
        nl = "Aan/Uit",
        pl = "Przełącz",
        pt = "Alternar",
        pt_BR = "Alternar",
        ru = "Переключить",
        sk = "Výber",
        sv = "Växla",
        tr = "Değiştir",
        zh_TW = "切換"
    };

    -- This is an error message shown to the end-user when there is an
    --  unexpected entry in a .zip (or whatever) file that we didn't know
    --  how to handle.
    ["Unknown file type in archive"] = {
        cs = "Neznámý typ souboru v archivu.",
        da = "Ukendt filtype i arkiv",
        de = "Unbekannter Dateityp im Archiv",
        el = "Άγνωστος τύπος αρχείου μέσα στο πακέτο αρχείων.",
        en_AU = "Unknown file type in archive",
        en_CA = "Unknown file type in archive",
        en_GB = "Unknown file type in archive",
        es = "Tipo de fichero desconocido en el archivo",
        fi = "Paketissa on tiedosto, jonka tyyppiä ei tunneta.",
        fr = "Type de fichier inconnu dans cette archive.",
        hu = "Ismeretlen fájl típus az archívumban",
        it = "Non è stato possibile riconoscere un file presente nell'archivio",
        nb = "Ukjent filtype i arkivet",
        nds = "Ubekannter Datey-Typ im Archiv",
        nl = "Onbekend bestandstype in archief",
        pl = "Nie znany typ pliku w archiwum.",
        pt = "Tipo de ficheiro desconhecido no arquivo",
        pt_BR = "Tipo de arquivo desconhecido no arquivo",
        ru = "Неизвестный тип файла в архиве",
        sk = "Neznámy typ súboru v archíve",
        sv = "Okänd filtyp påträffad i arkivet",
        tr = "Arşivde bilinmeyen dosya tipi",
        zh_TW = "檔案包中有未知的檔案類別"
    };

    -- This is an error message shown to the end-user if they refuse to
    --  agree to the license of the software they are try to install.
    ["You must accept the license before you may install"] = {
        cs = "Před instalací je nutné odsouhlasit licenci",
        de = "Sie müssen den Lizenzbedingungen zustimmen, bevor sie installieren können",
        el = "Πρέπει να συμφωνήσετε με την άδεια χρήσης πριν συνεχίσετε με την εγκατάσταση",
        en_AU = "You must accept the license before you may install",
        en_CA = "You must accept the licence before you may install",
        en_GB = "You must accept the licence before you may install",
        es = "Tienes que aceptar la licencia antes de que puedas instalar",
        fi = "Asennusta ei suoriteta, ellet hyväksy lisenssiä.",
        fr = "Vous devez accepter la licence avant de pouvoir installer",
        hu = "El kell fogadnod a liszenszt a telepítés előtt",
        it = "E' necessario accettare la licenza prima di procedere con l'installazione.",
        nb = "Lisensen må godkjennes før du kan installere",
        nds = "Du must den Lizenzbedingungen zustimmen, um die Installation fortzusetzen",
        nl = "U moet akkoord gaan met de licentie om te kunnen installeren",
        pl = "Musisz zaakceptować licencję przed instalacją",
        pt = "Tem que aceitar a licença antes de prosseguir com a instalação",
        pt_BR = "Você precisa aceitar essa licensa antes de poder instalar",
        ru = "Вы должны согласиться с лицензионным соглашением чтобы продолжить установку",
        sk = "Musíte súhlasiť s licenciou pokiaľ chcete pokračovať v inštalácii",
        sv = "Du måste acceptera licensavtalet innan du kan fortsätta med installationen",
        tr = "Kuruluma başlamadan önce lisansı kabul etmelisiniz",
        zh_TW = "你必須同意該許可證以繼續安裝程序"
    };

    -- Installations display the currently-installing component, such as
    --  "Base game" or "Bonus pack content" or whatnot. The installer lists
    --  the current component as "Metadata" when writing out its own
    --  information, such as the final list of installed files, the uninstall
    --  support application, etc. It's a catch-all category: data about the
    --  actual data, basically.
    ["Metadata"] = {
        cs = "Metadata",
        da = "Metadata",
        de = "Metadaten",
        el = "Μεταδεδομένα",
        en_AU = "Metadata",
        en_CA = "Metadata",
        en_GB = "Metadata",
        es = "Metadatos",
        fi = "Metadata",
        fr = "Métadonnées",
        hu = "Metaadat",
        it = "Metadati",
        nb = "Metadata",
        nds = "Metadaten",
        nl = "Metadata",
        pl = "Metadane",
        pt = "Meta-Informação",
        pt_BR = "Meta-dados",
        ru = "Метаданные",
        sk = "Metadáta",
        sv = "Metadata",
        tr = "Temel bilgi",
        zh_TW = "中繼資料"
    };

    -- This error is shown if incorrect command line arguments are used.
    ["Invalid command line"] = {
        cs = "Neplatné argumenty",
        de = "Ungültige Kommandozeile",
        el = "Λάθος γραμμή εντολών",
        en_AU = "Invalid command line",
        en_CA = "Invalid command line",
        en_GB = "Invalid command line",
        es = "Parámetros de comando incorrectos",
        fi = "Virheellinen komentorivi",
        fr = "Ligne de commande invalide",
        hu = "Helytelen parancssor",
        it = "Parametri nella riga di comando non validi",
        nb = "Ugyldig kommandolinje",
        nds = "Ungültige Befehlszeile",
        nl = "Ongeldige opdrachtregel",
        pl = "Niepoprawny parametr",
        pt = "Argumento da linha de comandos inválido",
        pt_BR = "Linha de comando inválida",
        ru = "Неверная командная строка",
        sk = "Chybné parametre",
        sv = "Ogiltigt kommando",
        tr = "Geçersiz komut satırı",
        zh_TW = "無效的命令"
    };

    -- This error is shown when updating the manifest (the list of files that
    --  we installed), if it can't load the file for some reason. '%0' is the
    --  manifest's package name.
    ["Couldn't load manifest file for '%0'"] = {
        cs = "Nemohu načíst soubor s manifestem pro '%0'",
        de = "Konnte Manifestdatei für '%0' nicht laden",
        el = "Δεν μπόρεσε να φορτώσει το αρχείο \"manifest\" για '%0'",
        en_AU = "Couldn't load manifest file for '%0'",
        en_CA = "Couldn't load manifest file for '%0'",
        en_GB = "Couldn't load manifest file for '%0'",
        es = "No pudo cargar el archivo manifest de '%0'",
        fi = "Asennusluetteloa paketille \"%0\" ei kyetty lukemaan.",
        fr = "Impossible de charger le fichier manifeste de '%0'",
        hu = "'%0' fájllista betöltése sikertelen",
        it = "Impossibile caricare il manifest file per '%0'",
        nb = "Kunne ikke laste manifestfil for '%0'",
        nds = "Konnte Manifest für '%0' nicht laden.",
        nl = "Kan manifest bestand for '%0' niet laden",
        pl = "Nie można załadować manifestu dla pliku '%0'",
        pt = "Foi impossível carregar o ficheiro de manifesto para '%0'",
        pt_BR = "Não é possível carregar o arquivo manifest de '%0'",
        ru = "Не могу загрузить файл манифеста '%0'",
        sk = "Nemôžem nahrať manifest súbor pre '%0'",
        sv = "Kunde inte ladda manifestfil för '%0'",
        tr = "'%0' için manifesto dosyası yüklenemedi",
        zh_TW = "無法讀取來自 '%0' 的檔案清單"
    };

    -- This error is shown when the user prompted the app to read a filename
    --  (%0) that doesn't exist.
    ["File %0 not found"] = {
        cs = "Soubor '%0' nebyl nalezen",
        da = "Filen '%0' ikke fundet",
        de = "Datei %0 nicht gefunden.",
        el = "Το αρχείο %0 δεν βρέθηκε",
        en_AU = "File %0 not found",
        en_CA = "File %0 not found",
        en_GB = "File %0 not found",
        es = "Archivo %0 no encontrado",
        fi = "Tiedostoa \"%0\" ei ole.",
        fr = "Fichier %0 introuvable",
        hu = "%0 nem található",
        it = "Impossibile trovare il File %0",
        nb = "Filen '%0' ble ikke funnet",
        nds = "Datei %0 wurde nicht gefunden",
        nl = "Bestand %0 niet gevonden",
        pl = "Plik %0 nie znaleziony",
        pt = "O ficheiro %0 não foi encontrado",
        pt_BR = "Arquivo %0 não encontrado",
        ru = "Файл %0 не найден",
        sk = "Nenašiel som súbor  %0",
        sv = "Filen '%0' hittades inte",
        tr = "%0 dosyası bulunamadı",
        zh_TW = "找不到檔案 %0"
    };

    -- This is a window title on the message box when asking if user is sure
    --  they want to uninstall a package.
    ["Uninstall"] = {
        cs = "Odinstalovat",
        da = "Afinstallér",
        de = "Deinstallieren",
        el = "Απεγκατάσταση",
        en_AU = "Uninstall",
        en_CA = "Uninstall",
        en_GB = "Uninstall",
        es = "Desinstalar",
        et = "Eemalda",
        fi = "Asennuksen poisto",
        fr = "Désinstaller",
        hu = "Eltávolítás",
        it = "Disinstalla",
        nb = "Avinstallasjon",
        nds = "Deinstalleren",
        nl = "Deïnstalleren",
        pl = "Odinstaluj",
        pt = "Desinstalar",
        pt_BR = "Desinstalar",
        ru = "Удалить",
        sk = "Odinštalovať",
        sv = "Avinstallera",
        tr = "Kaldır",
        zh_TW = "移除"
    };

    -- This is the text when asking the user if they want to uninstall
    --  the package named '%0'.
    ["Are you sure you want to uninstall '%0'?"] = {
        cs = "Opravdu chcete odinstalovat '%0'?",
        de = "Sind Sie sicher, dass Sie '%0' deinstallieren wollen?",
        el = "Είστε σίγουροι θέλετε να απεγκαταστήσετε το '%0'?",
        en_AU = "Are you sure you want to uninstall '%0'?",
        en_CA = "Are you sure you want to uninstall '%0'?",
        en_GB = "Are you sure you want to uninstall '%0'?",
        es = "¿Estás seguro de que quieres desinstalar '%0'?",
        fi = "Haluatko varmasti poistaa asennuksen \"%0\"?",
        fr = "Êtes-vous sûr de vouloir désinstaller '%0' ?",
        hu = "Biztosan eltávolítod: '%0' ?",
        it = "Se veramente sicuro di voler disinstallare '%0'?",
        nb = "Er du sikker på at du vil avinstallere '%0'?",
        nds = "Sind sie sicher, dass sie '%0' deinstallieren wollen?",
        nl = "Bent u er zeker van dat u '%0' wilt deïnstalleren?",
        pl = "Czy na pewno chcesz odinsalować '%0'?",
        pt = "Tem a certeza que quer desinstalar '%0'?",
        pt_BR = "Você tem certeza que deseja desinstalar '%0'?",
        ru = "Вы уверены, что хотите удалить '%0'?",
        sk = "Ste si istý že chcete odinštalovať '%0'?",
        sv = "Är du säker på att du vill avinstallera '%0'?",
        tr = "'%0' paketini kaldırmak istediğinizden emin misiniz?",
        zh_TW = "你確定要移除 '%0'"
    };

    -- This is a window title, shown while the actual uninstall is in process
    --  and a progress meter is being shown.
    ["Uninstalling"] = {
        cs = "Odinstalovávám",
        da = "Afinstallerer",
        de = "Deinstalliere",
        el = "Γίνεται απεγκατάσταση",
        en_AU = "Uninstalling",
        en_CA = "Uninstalling",
        en_GB = "Uninstalling",
        es = "Desinstalando",
        fi = "Poistetaan asennusta",
        fr = "Désinstallation en cours",
        hu = "Eltávolítás",
        it = "Disinstallazione in corso",
        nb = "Avinstallerer",
        nds = "Deinstallation",
        nl = "Deïnstallatie",
        pl = "Odinstalowywanie",
        pt = "A desinstalar",
        pt_BR = "Desinstalando",
        ru = "Удаление",
        sk = "Odinštalovávam",
        sv = "Avinstallerar",
        tr = "Kaldırılıyor",
        zh_TW = "正在移除"
    };

    -- This is shown to the user in a message box when uninstallation is done.
    ["Uninstall complete"] = {
        cs = "Odinstalace dokončena",
        de = "Deinstallation abgeschlossen",
        el = "Η απεγκατάσταση ολοκληρώθηκε",
        en_AU = "Uninstall complete",
        en_CA = "Uninstall complete",
        en_GB = "Uninstall complete",
        es = "Desinstalación terminada",
        fi = "Asennus poistettu",
        fr = "Désinstallation complète",
        hu = "Eltávolítás kész",
        it = "Disinstallazione completata",
        nb = "Avinstallasjonen er ferdig",
        nds = "Deinstallation abgeschlossen",
        nl = "Deïnstallatie voltooid",
        pl = "Odinstalowanie zakończone",
        pt = "A desinstalação foi bem sucedida",
        pt_BR = "Desinstalação completa",
        ru = "Удаление завершено",
        sk = "Odinštalácia dokončená",
        sv = "Avinstallation slutförd",
        tr = "Tamamen kaldır",
        zh_TW = "移除完成"
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
        el = "[Μεγαλώστε το παράθυρο κατά πλάτος!]",
        en_AU = "[Make the window wider!]",
        en_CA = "[Make the window wider!]",
        en_GB = "[Make the window wider!]",
        es = "[¡Ensancha la ventana!]",
        fi = "[Levennä ikkunaa!]",
        fr = "[Élargissez la fenêtre!]",
        hu = "[Húzd az ablakot szélesebbre!]",
        it = "[Allarga la finestra!]",
        nb = "[Gjør vinduet bredere!]",
        nds = "[Mach das Fenster breiter!]",
        nl = "[Maak het scherm breder!]",
        pl = "[Zrób szersze okno!]",
        pt = "[Torne a janela mais larga!]",
        pt_BR = "[Aumente a largura da janela!]",
        ru = "Сделайте окно шире!",
        sk = "[Roztiahnite okno do šírky!]",
        sv = "[Gör fönstret bredare!]",
        tr = "[Pencereyi daha geniş yap!]",
        zh_TW = "[使視窗更寬]"
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
        el = "[Μεγαλώστε το παράθυρο κατα ύψος]",
        en_AU = "[Make the window taller!]",
        en_CA = "[Make the window taller!]",
        en_GB = "[Make the window taller!]",
        es = "[¡Estira la ventana!]",
        fi = "[Tee ikkunasta korkeampi!]",
        fr = "[Agrandissez la fenêtre!]",
        hu = "[Húzd az ablakot magasabbra!]",
        it = "[Allunga la finestra!]",
        nb = "[Gjør vinduet høyere!]",
        nds = "[Mach das Fenster höher!]",
        nl = "[Maak het scherm langer!]",
        pl = "[Zrób wyższe okno!]",
        pt = "[Torne a janela mais alta!]",
        pt_BR = "[Aumente a altura da janela!]",
        ru = "Сделайте окно выше!",
        sk = "[Roztiahnite okno do výšky!]",
        sv = "[Gör fönstret högre!]",
        tr = "[Pencereyi daha uzun yap!]",
        zh_TW = "[使視窗更長]"
    };

    -- This is written out if we failed to add an item to the desktop
    --  application menu (or "Start" bar on Windows, or maybe the Dock on
    --  Mac OS X, etc).
    ["Failed to install desktop menu item"] = {
        cs = "Nepodařilo se nainstalovat položku do menu",
        de = "Konnte Verknüpfung für Desktop-Menü nicht installieren",
        el = "Σφάλμα κατα την εγκατάσταση της συντόμευσης εκκίνησης του προγράμματος",
        en_AU = "Failed to install desktop menu item",
        en_CA = "Failed to install desktop menu item",
        en_GB = "Failed to install desktop menu item",
        es = "Fallo al añadir un elemento al menú de escritorio",
        fi = "Kohdan lisääminen työpöytävalikkoon epäonnistui.",
        fr = "Echec de l'installation des éléments du bureau",
        hu = "Asztali ikon létrehozása sikertelen",
        it = "L'installazione di un oggetto nel menu delle applicazioni è fallita",
        nds = "Desktop Menu-Eintrag konnte nicht erstellt werden.",
        nl = "Installatie van het bureaublad menu item is mislukt",
        pl = "Nie udało się utworzyć elementu menu",
        pt = "A instalação do menu no desktop falhou",
        pt_BR = "Falha ao instalar o item do menu",
        ru = "Не удалось установить элемент меню",
        sk = "Nepodarilo sa mi nainštalovať odkaz na plochu",
        sv = "Misslycklades med att lägga till genväg i programmenyn",
        tr = "Masaüstü menü elemanı kurma eylemi başarısız oldu",
        zh_TW = "安裝桌面選單項目失敗"
    };

    -- This is written out if we failed to remove an item from the desktop
    --  application menu (or "Start" bar on Windows, or maybe the Dock on
    --  Mac OS X, etc).
    ["Failed to uninstall desktop menu item"] = {
        cs = "Nepodařilo se odstranit položku z menu",
        de = "Konnte Verknüpfung für Desktop-Menü nicht deinstallieren",
        el = "Σφάλμα κατά την απεγκατάσταση της συντόμευσης εκκίνησης του προγράμματος",
        en_AU = "Failed to uninstall desktop menu item",
        en_CA = "Failed to uninstall desktop menu item",
        en_GB = "Failed to uninstall desktop menu item",
        es = "Fallo al quitar un elemento del menú de escritorio",
        fi = "Kohdan poistaminen työpöytävalikkosta epäonnistui.",
        fr = "Echec de la désinstallation des éléments du bureau",
        hu = "Asztali ikon eltávolítása sikertelen",
        it = "La disinstallazione di un oggetto del menu è fallita",
        nds = "Desktop Menu-Eintrag konnte nicht entfernt werden.",
        nl = "Deïnstallatie van het bureaublad menu item is mislukt",
        pl = "Nie udało się odinstalować elementu menu",
        pt = "A desinstalação do menu no desktop falhou",
        pt_BR = "Falha ao remover o item do menu",
        ru = "Не удалось удалить элемент меню",
        sk = "Nepodarilo sa mi odinštalovať odkaz z plochy",
        sv = "Misslyckades med att ta bort genväg i programmenyn",
        tr = "Masaüstü menü elemanını kaldırma eylemi başarısız oldu",
        zh_TW = "移除桌面選單項目失敗"
    };
};

-- end of localization.lua ...

