local TOTAL_INSTALL_SIZE = 308206076;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "lazy8studios.com",
    id = "cogs",
    description = "Cogs",
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

    Setup.Readme
    {
        description = _("Cogs README"),
        source = _("README-linux.txt")
    },

    Setup.Option
    {
        -- !!! FIXME: All this filter nonsense is because
        -- !!! FIXME:   source = "base:///some_dir_in_basepath/"
        -- !!! FIXME: doesn't work, since it wants a file when drilling
        -- !!! FIXME: for the final archive, not a directory. Fixing this
        -- !!! FIXME: properly is a little awkward, though.

        value = true,
        required = true,
        disabled = false,
        bytes = TOTAL_INSTALL_SIZE,
        description = "Cogs",

        Setup.File
        {
            wildcards = "*";
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Cogs",
            genericname = "Cogs",
            tooltip = _("A steampunk puzzle game"),
            builtin_icon = false,
            icon = "cogs.png",
            commandline = "%0/Cogs",
            workingdir = "%0",
            category = "Game"
        }
    }
}

-- end of config.lua ...

