local GAME_INSTALL_SIZE = 1738442626;
local PB_INSTALL_SIZE = 3738916;
local ENCFG_INSTALL_SIZE = 1689;
local FRCFG_INSTALL_SIZE = 1739;
local ESCFG_INSTALL_SIZE = 1731;
local DECFG_INSTALL_SIZE = 1730;
local ITCFG_INSTALL_SIZE = 1731;

local _ = MojoSetup.translate

-- change where we look for the "disc" if we are installing from a copy
--  downloaded through Steam or otherwise installed on Windows:
-- ./prey-installer.bin --from-install --media "/mnt/ntfsdisk/Program Files/Steam/common/prey/base"
local media_path = "Setup/Data/Base"
if MojoSetup.cmdline("from-install") then
    media_path = ""
end

-- do a filter to catch filename case differences.
-- Please note this returns a filter function, and isn't the filter itself!
local function make_pakfile_filter(pakregexp)
    -- Upvalue!
    local mpath = string.lower(media_path)
    if mpath ~= "" then
        mpath = mpath .. "/"
    end
    local regexp = "^" .. mpath .. "(" .. pakregexp .. ")$"
    return function(dest)
        local str, matches
        str, matches = string.gsub(string.lower(dest), regexp, "base/%1", 1)
        if matches == 1 then
            return str, nil
        end
        return nil, nil  -- don't install anything else here.
    end
end

Setup.Package
{
    vendor = "icculus.org",
    id = "prey",
    description = "Prey";
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

    -- disc 1, 2, and 3 might all be the same disc for the DVD version!
    Setup.Media
    {
        id = "disc1",
        description = _("Prey Disc 1"),
        uniquefile = media_path .. "/pak000.pk4"
    },

    Setup.Media
    {
        id = "disc2",
        description = _("Prey Disc 2"),
        uniquefile = media_path .. "/pak002.pk4"
    },

    Setup.Media
    {
        id = "disc3",
        description = _("Prey Disc 3"),
        uniquefile = media_path .. "/pak003.pk4"
    },

    Setup.Readme
    {
        description = _("Prey README"),
        source = _("prey_readme.txt")
    },

    Setup.Eula
    {
        description = _("Prey License"),
        source = _("prey_license.txt")
    },

    Setup.Option
    {
        value = true,
        required = true,
        disabled = false,
        bytes = GAME_INSTALL_SIZE,
        description = "Prey",

        Setup.File
        {
            source = "media://disc1/",
            filter = make_pakfile_filter("pak00[01].pk4"),
        },

        Setup.File
        {
            source = "media://disc2/",
            filter = make_pakfile_filter("pak002.pk4"),
        },

        Setup.File
        {
            source = "media://disc3/",
            filter = make_pakfile_filter("pak00[34].pk4"),
        },


        -- !!! FIXME: All this filter nonsense is because
        -- !!! FIXME:   source = "base:///some_dir_in_basepath/"
        -- !!! FIXME: doesn't work, since it wants a file when drilling
        -- !!! FIXME: for the final archive, not a directory. Fixing this
        -- !!! FIXME: properly is a little awkward, though.

        Setup.File
        {
            wildcards = "prey-linux-data/*";
            filter = function(fn)
                fn = string.gsub(fn, "^prey%-linux%-data/", "", 1);
                return fn, nil;
            end
        },

        Setup.File
        {
            wildcards = "prey-linux-x86/*";
            filter = function(fn)
                fn = string.gsub(fn, "^prey%-linux%-x86/", "", 1);
                return fn, nil;
            end
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Prey",
            genericname = "Prey",
            tooltip = _("First-person shooter from Human Head and 3D Realms"),
            builtin_icon = false,
            icon = "prey.png",
            commandline = "%0/prey",
            category = "Game",
        },

        Setup.Option
        {
            -- let command line toggle this to enabled by default for automation.
            value = MojoSetup.cmdline("punkbuster"),
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

