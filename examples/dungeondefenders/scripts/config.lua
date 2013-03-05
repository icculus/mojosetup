local TOTAL_INSTALL_SIZE = 6027721827;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "trendyent.com",
    id = "dungeondefenders",
    description = "Dungeon Defenders",
    version = "7.48",
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
        description = _("Dungeon Defenders README"),
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
        description = "Dungeon Defenders",

        Setup.File
        {
            wildcards = "*";
            filter = function(dest)
                if dest == "DungeonDefenders" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "UDKGame/Binaries/DungeonDefenders-x86" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "UDKGame/Binaries/libSDL2-2.0.so.0" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "UDKGame/Binaries/libopenal.so.1" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "UDKGame/Binaries/xdg-open" then
                    return dest, "0755"   -- make sure it's executable.
                end
                return dest   -- everything else just goes through as-is.
            end
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Dungeon Defenders",
            genericname = "Dungeon Defenders",
            tooltip = _("Dungeon Defenders"),
            builtin_icon = false,
            icon = "DunDefIcon.png",
            commandline = "%0/DungeonDefenders",
            workingdir = "%0",
            category = "Game"
        }
    }
}

-- end of config.lua ...

