local GAME_INSTALL_SIZE = 245613846;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "runningwithscissors.com",
    id = "postal1",
    description = _("Postal 1"),
    version = "1.0",
    splash = "splash.bmp",
    superuser = false,
    write_manifest = true,
    support_uninstall = true,
    recommended_destinations =
    {
        MojoSetup.info.homedir,
        "/opt/games",
        "/usr/local/games"
    },

    postinstall = function(package)
        MojoSetup.launchbrowser(MojoSetup.destination .. "/postal1_guide.html")
    end,

    Setup.Eula
    {
        description = _("Postal License"),
        source = _("postal1_license.txt")
    },

    Setup.Option
    {
        value = true,
        required = true,
        disabled = false,
        bytes = GAME_INSTALL_SIZE,
        description = _("Postal 1"),

        Setup.File
        {
            -- Just install everything we see...
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = _("Postal 1"),
            genericname = _("Postal 1"),
            tooltip = _("Politically incorrect shoot-em-up from Running With Scissors"),
            builtin_icon = false,
            icon = "rws.png",
            commandline = "%0/postal1",
            category = "Game"
        }
    }
}

-- end of config.lua ...

