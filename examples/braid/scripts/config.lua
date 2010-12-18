local GAME_INSTALL_SIZE = 144119222
local X86_INSTALL_SIZE = 14440962
local AMD64_INSTALL_SIZE = 20224358

local _ = MojoSetup.translate

-- If we decide you have a 32-bit machine, we don't give you the install
--  choice. If we think you're on 64-bit, we'll ask which you want, defaulting
--  to 64.
local is32bit =
        MojoSetup.cmdline("32bit") or
        MojoSetup.info.machine == "x86" or
        MojoSetup.info.machine == "i386" or
        MojoSetup.info.machine == "i586" or
        MojoSetup.info.machine == "i686"

Setup.Package
{
    vendor = "number-none.com",
    id = "braid",
    description = "Braid",
    version = "1.5.2",
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
        description = _("Braid README"),
        source = _("gamedata/README-linux.txt")
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
        bytes = GAME_INSTALL_SIZE,
        description = "Braid",

        Setup.OptionGroup
        {
            description = _("CPU Architecture"),
            Setup.Option
            {
                value = is32bit,
                required = is32bit,
                disabled = false,
                bytes = X86_INSTALL_SIZE,
                description = "x86",
                Setup.File
                {
                    wildcards = "x86/*";
                    filter = function(fn)
                        return string.gsub(fn, "^x86/", "", 1), nil
                    end
                },
            },
            Setup.Option
            {
                value = not is32bit,
                required = false,
                disabled = is32bit,
                bytes = AMD64_INSTALL_SIZE,
                description = "amd64",
                Setup.File
                {
                    wildcards = "amd64/*";
                    filter = function(fn)
                        return string.gsub(fn, "^amd64/", "", 1), nil
                    end
                },
            },
        },

        Setup.File
        {
            wildcards = "gamedata/*";
            filter = function(fn)
                return string.gsub(fn, "^gamedata/", "", 1), nil
            end
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Braid",
            genericname = "Braid",
            tooltip = _("Time-bending puzzle video game"),
            builtin_icon = false,
            icon = "braid.png",
            commandline = "%0/braid",
            workingdir = "%0",
            category = "Game"
        }
    }
}

-- end of config.lua ...

