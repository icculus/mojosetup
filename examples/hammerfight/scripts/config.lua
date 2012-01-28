local TOTAL_INSTALL_SIZE = 126978833;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "kranx.com",
    id = "hammerfight",
    description = "Hammerfight",
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
        description = _("Hammerfight README"),
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
        description = "Hammerfight",

        Setup.File
        {
            wildcards = "*";
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Hammerfight",
            genericname = "Hammerfight",
            tooltip = _("A game of flying, battling machines"),
            builtin_icon = false,
            icon = "hammerfight.png",
            commandline = "%0/Hammerfight",
            workingdir = "%0",
            category = "Game"
        }
    }
}

-- end of config.lua ...

