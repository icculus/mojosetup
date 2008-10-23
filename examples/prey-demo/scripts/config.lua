local GAME_INSTALL_SIZE = 480726423;
local PB_INSTALL_SIZE = 3738916;
local ENCFG_INSTALL_SIZE = 1689;
local FRCFG_INSTALL_SIZE = 1739;
local ESCFG_INSTALL_SIZE = 1731;
local DECFG_INSTALL_SIZE = 1730;
local ITCFG_INSTALL_SIZE = 1731;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "icculus.org",
    id = "prey-demo",
    description = _("Prey Demo");
    version = "1.4",
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
        description = _("Prey README"),
        source = _("prey_demo_readme.txt")
    },

    Setup.Eula
    {
        description = _("Prey License"),
        source = _("prey_demo_license.txt")
    },

    Setup.Option
    {
        value = true,
        required = true,
        disabled = false,
        bytes = GAME_INSTALL_SIZE,
        description = "Prey",

        -- !!! FIXME: All this filter nonsense is because
        -- !!! FIXME:   source = "base:///some_dir_in_basepath/"
        -- !!! FIXME: doesn't work, since it wants a file when drilling
        -- !!! FIXME: for the final archive, not a directory. Fixing this
        -- !!! FIXME: properly is a little awkward, though.

        Setup.File
        {
            wildcards = "prey-demo-linux-data/*";
            filter = function(fn)
                fn = string.gsub(fn, "^prey%-demo%-linux%-data/", "", 1);
                return fn, nil;
            end
        },

        Setup.File
        {
            wildcards = "prey-demo-linux-x86/*";
            filter = function(fn)
                fn = string.gsub(fn, "^prey%-demo%-linux%-x86/", "", 1);
                return fn, nil;
            end
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = _("Prey Demo"),
            genericname = _("Prey Demo"),
            tooltip = _("Demo of first-person shooter from Human Head and 3D Realms"),
            builtin_icon = false,
            icon = "prey.png",
            commandline = "%0/prey-demo",
            category = "Game",
        },

        Setup.Option
        {
            value = false,
            required = false,
            disabled = false,
            bytes = PB_INSTALL_SIZE,
            description = _("PunkBuster(TM) support"),

            Setup.Eula
            {
                description = _("PunkBuster(TM) License"),
                source = _("punkbuster_license.txt")
            },

            Setup.File
            {
                wildcards = "punkbuster-linux-x86/*";
                filter = function(fn)
                    fn = string.gsub(fn, "^punkbuster%-linux%-x86/", "", 1);
                    return fn, nil;
                end
            },
        },

        Setup.OptionGroup
        {
            disabled = false,
            description = _("Initial language and keyboard layout"),
            Setup.Option
            {
                value = string.match(MojoSetup.info.locale, "^en") ~= nil,
                required = false,
                disabled = false,
                bytes = ENCFG_INSTALL_SIZE,
                description = _("English"),
                Setup.File
                {
                    wildcards = "configs/english.cfg";
                    filter = function(fn) return "base/default.cfg"; end
                },
            },
            Setup.Option
            {
                value = string.match(MojoSetup.info.locale, "^fr") ~= nil,
                required = false,
                disabled = false,
                bytes = FRCFG_INSTALL_SIZE,
                description = _("French"),
                Setup.File
                {
                    wildcards = "configs/french.cfg";
                    filter = function(fn) return "base/default.cfg"; end
                },
            },
            Setup.Option
            {
                value = string.match(MojoSetup.info.locale, "^it") ~= nil,
                required = false,
                disabled = false,
                bytes = ITCFG_INSTALL_SIZE,
                description = _("Italian"),
                Setup.File
                {
                    wildcards = "configs/italian.cfg";
                    filter = function(fn) return "base/default.cfg"; end
                },
            },
            Setup.Option
            {
                value = string.match(MojoSetup.info.locale, "^de") ~= nil,
                required = false,
                disabled = false,
                bytes = DECFG_INSTALL_SIZE,
                description = _("German"),
                Setup.File
                {
                    wildcards = "configs/german.cfg";
                    filter = function(fn) return "base/default.cfg"; end
                },
            },
            Setup.Option
            {
                value = string.match(MojoSetup.info.locale, "^es") ~= nil,
                required = false,
                disabled = false,
                bytes = ESCFG_INSTALL_SIZE,
                description = _("Spanish"),
                Setup.File
                {
                    wildcards = "configs/spanish.cfg";
                    filter = function(fn) return "base/default.cfg"; end
                },
            },
        },
    },
}

-- end of config.lua ...

