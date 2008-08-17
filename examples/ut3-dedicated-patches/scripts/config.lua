local TOTAL_INSTALL_SIZE = 327467549;
local _ = MojoSetup.translate

Setup.Package
{
    vendor = "epicgames.com",
    id = "ut3-dedicated",
    description = "Unreal Tournament 3 Dedicated Server",
    version = "1.3",
    write_manifest = false,     -- don't want to update...
    support_uninstall = false,  -- ...or uninstall. This is just a fancy unzip.

    recommended_destinations =
    {
        MojoSetup.info.homedir,
        "/opt/games",
        "/opt",
        "/usr/local/games",
        "/usr/local"
    },

    Setup.Readme
    {
        description = _("README"),
        source = _("README-ut3-patch3.txt")
    },

    Setup.Eula
    {
        description = _("End User License Agreement"),
        source = _("UT3-linux-server-EULA.txt")
    },

    Setup.Option
    {
        value = true,
        required = true,
        disabled = false,
        bytes = TOTAL_INSTALL_SIZE,
        description = _("Dedicated Server"),
        Setup.File
        {
            allowoverwrite = true,   -- just replace files that we update.
            -- ...and just copy the whole archive.
        },  
    },
}

-- end of config.lua ...

