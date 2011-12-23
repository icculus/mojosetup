local TOTAL_INSTALL_SIZE = 258158137;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "supermeatboy.com",
    id = "supermeatboy",
    description = "Super Meat Boy",
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
        description = _("Super Meat Boy README"),
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
        description = "Super Meat Boy",

        Setup.File
        {
            wildcards = "*";
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Super Meat Boy",
            genericname = "Super Meat Boy",
            tooltip = _("An insanely hard and delightfully meaty platformer"),
            builtin_icon = false,
            icon = "supermeatboy.png",
            commandline = "%0/SuperMeatBoy",
            workingdir = "%0",
            category = "Game"
        }
    }
}

-- end of config.lua ...

