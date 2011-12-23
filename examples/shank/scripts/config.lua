local TOTAL_INSTALL_SIZE = 2375134192;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "kleientertainment.com",
    id = "shank",
    description = "Shank",
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
        description = _("Shank README"),
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
        description = "Shank",

        Setup.File
        {
            wildcards = "*";
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Shank",
            genericname = "Shank",
            tooltip = _("A video game tale of revenge"),
            builtin_icon = false,
            icon = "shank.png",
            commandline = "%0/bin/Shank",
            workingdir = "%0/bin",
            category = "Game"
        }
    }
}

-- end of config.lua ...

