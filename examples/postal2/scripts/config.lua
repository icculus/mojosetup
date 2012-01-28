local TOTAL_INSTALL_SIZE = 1401484620;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "runningwithscissors.com",
    id = "postal2",
    description = _("Postal 2: Share the Pain"),
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

    Setup.Eula
    {
        description = _("Postal License"),
        source = _("postal2_license.txt")
    },

    Setup.Option
    {
        value = true,
        required = true,
        disabled = false,
        bytes = TOTAL_INSTALL_SIZE,
        description = _("Postal 2: Share the Pain"),

        Setup.File
        {
            -- Just install everything we see...
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = _("Postal 2: Share the Pain"),
            genericname = _("Postal 2"),
            tooltip = _("Politically incorrect first person shooter from Running With Scissors"),
            builtin_icon = false,
            icon = "rws.png",
            commandline = "%0/postal2",
            category = "Game"
        }
    }
}

-- end of config.lua ...

