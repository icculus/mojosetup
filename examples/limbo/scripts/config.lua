local TOTAL_INSTALL_SIZE = 113202507;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "limbogame.org",
    id = "limbo",
    description = "LIMBO",
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
        description = _("LIMBO README"),
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
        description = "LIMBO",

        Setup.File
        {
            wildcards = "*";
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "LIMBO",
            genericname = "LIMBO",
            tooltip = _("Uncertain of his sister's fate, a boy enters LIMBO"),
            builtin_icon = false,
            icon = "limbo.png",
            commandline = "%0/limbo",
            workingdir = "%0",
            category = "Game"
        }
    }
}

-- end of config.lua ...

