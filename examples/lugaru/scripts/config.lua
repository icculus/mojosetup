local TOTAL_INSTALL_SIZE = 37997185;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "wolfire.com",
    id = "lugaru",
    description = _("Lugaru: The Rabbit's Foot"),
    version = "1.0d",
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
        description = _("Lugaru README"),
        source = _("README-linux.txt")
    },

    Setup.Option
    {
        value = true,
        required = true,
        disabled = false,
        bytes = TOTAL_INSTALL_SIZE,
        description = _("Lugaru: The Rabbit's Foot"),

        Setup.File
        {
            -- Just install everything we see...
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = _("Lugaru"),
            genericname = _("Lugaru"),
            tooltip = _("Kung-Fu Rabbit Adventure Video Game"),
            builtin_icon = false,
            icon = "lugaru.png",
            commandline = "%0/lugaru",
            category = "Game"
        }
    }
}

-- end of config.lua ...

