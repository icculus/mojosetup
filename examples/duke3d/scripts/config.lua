local _ = MojoSetup.translate

local function arch()
    -- We (currently) only support 32-bit OSes, but some platforms can use
    --  these through compatibility layers.
    if MojoSetup.info.arch == "x86-64" then
        return "x86"
    elseif MojoSetup.info.arch == "powerpc64" then
        return "powerpc"
    end
    return MojoSetup.info.arch  -- good luck with everything else.
end

local function ostype()
    -- We (currently) only support some OSes, but some platforms can use
    --  these through compatibility layers.
    if MojoSetup.info.ostype == "freebsd" then
        return "linux"
    end
    return MojoSetup.info.ostype  -- good luck with everything else.
end

Setup.Package
{
    vendor = "icculus.org",
    id = "duke3d",
    description = "Duke Nukem 3D",
    version = "1.5",
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

    Setup.Eula
    {
        description = _("GNU General Public License"),
        source = _("gpl.txt")
    },

    Setup.Readme
    {
        description = _("MojoSetup README"),
        source = _("mojosetup_readme.txt")
    },

    Setup.Readme
    {
        description = _("Duke Nukem 3D README"),
        source = _("duke3d_readme.txt")
    },

    Setup.Media
    {
        id = "mac-cd",
        description = _("MacSoft Duke3D CD-ROM"),
        uniquefile = "Goodies/Utilities/Duke File Typer"
    },

    Setup.Media
    {
        id = "pc-cd",
        description = _("PC Atomic Edition CD-ROM"),
        uniquefile = "atominst/duke3d.grp"
    },

    -- Install a desktop menu item with all install types.
    Setup.DesktopMenuItem
    {
        disabled = false,
        name = "Duke Nukem 3D",
        genericname = "Duke Nukem 3D",
        tooltip = "Always bet on Duke!",
        builtin_icon = false,
        icon = "duke3d.png",  -- relative to the dest; you must install it!
        commandline = "%0/duke3d",
        category = "Game",
    },

    Setup.OptionGroup
    {
        description = _("Installation type"),

        -- This is a kind of useless tooltip; I'm just using it for testing.
        tooltip = _("Pick your installation type"),

        Setup.Option
        {
            value = true,
            required = false,
            disabled = false,
            bytes = 31102768,
            description = _("Install Shareware version"),
            tooltip = _("This does not need a retail copy of the game. It's free!"),

            Setup.File
            {
                -- catch all the EULAs and READMEs, and the desktop menu icon.
                wildcards = { "*.txt", "duke3d.png" }
            },
            Setup.File
            {
                source = "http://icculus.org/mojosetup/examples/duke3d/data/duke3d_shareware_bins_" .. ostype() .. "_" .. arch() .. ".zip"
            },
            Setup.File
            {
                source = "http://icculus.org/mojosetup/examples/duke3d/data/duke3d_shareware_data.zip"
            },
            Setup.File
            {
                source = "http://icculus.org/mojosetup/examples/duke3d/data/duke3d_unified_content.zip"
            },
        },

        Setup.Option
        {
            value = false,
            required = false,
            disabled = false,
            bytes = 64525364,
            description = _("Install full game from Atomic Edition disc"),
            tooltip = _("Pick this if you have a retail game disc from 3DRealms."),

            Setup.File
            {
                -- catch all the EULAs and READMEs, and the desktop menu icon.
                wildcards = { "*.txt", "duke3d.png" }
            },

            Setup.File
            {
                source = "http://icculus.org/mojosetup/examples/duke3d/data/duke3d_retail_bins_" .. ostype() .. "_" .. arch() .. ".zip"
            },

            Setup.File
            {
                source = "http://icculus.org/mojosetup/examples/duke3d/data/duke3d_unified_content.zip"
            },

            Setup.File
            {
                source = "http://icculus.org/mojosetup/examples/duke3d/data/duke3d_retail_demos.zip"
            },

            Setup.File
            {
                source = "media://pc-cd/",

                -- do a filter to catch filename case differences.
                filter = function(dest)
                if string.lower(dest) == "atominst/duke3d.grp" then
                    return "duke3d.grp"   -- chop "atominst/".
                end
                return nil  -- don't install anything else here.
                end
            },
        },
    },
}

-- end of config.lua ...

