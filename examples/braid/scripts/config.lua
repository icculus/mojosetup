local GAME_INSTALL_SIZE = 158514678;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "number-none.com",
    id = "braid",
    description = "Braid",
    version = "1.5.2",
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

    Setup.Readme
    {
        description = _("Braid README"),
        source = _("README-linux.txt")
    },

    Setup.Option
    {
        value = true,
        required = true,
        disabled = false,
        bytes = GAME_INSTALL_SIZE,
        description = "Braid",

        Setup.File
        {
            -- Just install everything we see...
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Braid",
            genericname = "Braid",
            tooltip = _("Time-bending puzzle video game"),
            builtin_icon = false,
            icon = "braid.png",
            commandline = "%0/braid",
            workingdir = "%0",
            category = "Game"
        }
    }
}

-- end of config.lua ...

