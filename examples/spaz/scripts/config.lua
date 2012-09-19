local TOTAL_INSTALL_SIZE = 168345717;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "spacepiratesandzombies.com",
    id = "spaz",
    description = "Space Pirates and Zombies",
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
        description = _("Space Pirates and Zombies README"),
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
        description = "Space Pirates and Zombies",

        Setup.File
        {
            wildcards = "*";
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Space Pirates and Zombies",
            genericname = "Space Pirates and Zombies",
            tooltip = _("A space odyssey. With pirates. And zombies."),
            builtin_icon = false,
            icon = "SPAZ.png",
            commandline = "%0/SPAZ",
            workingdir = "%0",
            category = "Game"
        }
    }
}

-- end of config.lua ...

