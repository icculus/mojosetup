local GAME_INSTALL_SIZE = 227436679;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "bit-blot.com",
    id = "aquaria",
    description = "Aquaria",
    version = "1.1.3",
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
        description = _("Aquaria README"),
        source = _("README-linux.txt")
    },

    Setup.Option
    {
        value = true,
        required = true,
        disabled = false,
        bytes = GAME_INSTALL_SIZE,
        description = "Aquaria",

        Setup.File
        {
            -- Just install everything we see...
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Aquaria",
            genericname = "Aquaria",
            tooltip = _("Underwater exploration adventure video game"),
            builtin_icon = false,
            icon = "aquaria.png",
            commandline = "%0/aquaria",
            category = "Game"
        }
    }
}

-- end of config.lua ...

