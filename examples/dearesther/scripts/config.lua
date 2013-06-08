local TOTAL_INSTALL_SIZE = 1415624618;

local _ = MojoSetup.translate

Setup.Package
{
    vendor = "dear-esther.com",
    id = "dearesther",
    description = "Dear Esther",
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
        description = _("Dear Esther README"),
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
        description = "Dear Esther",

        Setup.File
        {
            wildcards = "*";
            filter = function(dest)
                if dest == "dearesther_linux" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "Dear_Esther" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/datacache.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/engine.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/filesystem_stdio.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/inputsystem.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/launcher.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/libMiles.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/libSDL2-2.0.so.0" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/libsteam_api.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/libtier0.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/libtogl.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/libvstdlib.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/localize.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/materialsystem.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/scenefilecache.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/shaderapidx9.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/soundemittersystem.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/stdshader_dx9.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/studiorender.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/valve_avi.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/vaudio_miles.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/vgui2.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/vguimatsurface.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/vphysics.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/vscript.so" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/linux32/binkawin.asi" then
                    return dest, "0755"   -- make sure it's executable.
                end
                if dest == "bin/linux32/mssmp3.asi" then
                    return dest, "0755"   -- make sure it's executable.
                end
                return dest   -- everything else just goes through as-is.
            end
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = "Dear Esther",
            genericname = "Dear Esther",
            tooltip = _("An exploration of loss and grief"),
            builtin_icon = false,
            icon = "dearesther.png",
            commandline = "%0/Dear_Esther",
            workingdir = "%0",
            category = "Game"
        }
    }
}

-- end of config.lua ...

