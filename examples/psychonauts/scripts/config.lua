local TOTAL_INSTALL_SIZE = 5485393918;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "doublefine.com",
    id = "psychonauts",
    description = "Psychonauts",
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
        description = _("Psychonauts README"),
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
        description = "Psychonauts",

        Setup.File
        {
            wildcards = "*";
            filter = function(dest)
                if dest == "Psychonauts" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "libSDL-1.2.so.0" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "libopenal.so.1" then
                    return dest, "0755"   -- make sure it's executable.
                end
                return dest   -- everything else just goes through as-is.
            end
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Psychonauts",
            genericname = "Psychonauts",
            tooltip = _("A mind-bending platforming adventure"),
            builtin_icon = false,
            icon = "psychonauts.png",
            commandline = "%0/Psychonauts",
            workingdir = "%0",
            category = "Game"
        }
    }
}

-- end of config.lua ...

