local GAME_INSTALL_SIZE = 1401484620;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "runningwithscissors.com",
    id = "postal2",
    description = _("Postal 2: Apocalypse Weekend"),
    version = "1.0",
    splash = "splash.bmp",
    superuser = false,
    write_manifest = true,
    support_uninstall = true,
    recommended_destinations =
    {
        MojoSetup.info.homedir .. "/postal2",
        "/opt/games/postal2",
        "/usr/local/games/postal2"
    },

    Setup.Eula
    {
        description = _("Postal License"),
        source = _("postal2aw_license.txt")
    },

    Setup.Option
    {
        value = true,
        required = true,
        disabled = false,
        bytes = GAME_INSTALL_SIZE,
        description = _("Postal 2: Apocalypse Weekend"),

        Setup.File
        {
            -- Just install everything we see...
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = _("Postal 2: Apocalypse Weekend"),
            genericname = _("Postal 2"),
            tooltip = _("Politically incorrect expansion pack from Running With Scissors"),
            builtin_icon = false,
            icon = "p2aw_rws.png",
            commandline = "%0/postal2aw",
            category = "Game"
        }
    }
}

-- end of config.lua ...

