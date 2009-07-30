local GAME_INSTALL_SIZE = 1401484620;

local _ = MojoSetup.translate

-- We override MojoSetup.gui.destination to make sure we have a real p2:stp
--  install at that point. This happens to work at the moment, but hooks an
--  internal, undocumented API. We'll add a formal hook later. Don't emulate
--  this behaviour!
local origdestfn = MojoSetup.gui.destination
MojoSetup.gui.destination = function(recommend, thisstage, maxstage)
    while true do
        local rc, dst = origdestfn(recommend, thisstage, maxstage)
        if rc ~= 1 then
            return rc, dst
        end

        if MojoSetup.platform.exists(dst .. "/System/Postal2Game.u") then
            return rc, dst
        end

        -- Some versions (Postal 10th Anniversary disc) have several titles
        --  installed in one base dir.
        if MojoSetup.platform.exists(dst .. "postal2game/System/Postal2Game.u") then
            dst = dst .. "/postal2game"
            return rc, dst
        end

        MojoSetup.msgbox(_("Wrong path"), _("We don't see a copy of Postal 2: Share the Pain in that directory. You need Share the Pain to install Apocalypse Weekend. Please pick another directory."));
    end
end

Setup.Package
{
    vendor = "runningwithscissors.com",
    id = "postal2",
    description = _("Postal 2: Apocalypse Weekend"),
    version = "1.0",
    splash = "splash.bmp",
    superuser = false,
    write_manifest = true,
    support_uninstall = true,
    recommended_destinations =
    {
        MojoSetup.info.homedir .. "/postal2",
        "/opt/games/postal2",
        "/usr/local/games/postal2"
        MojoSetup.info.homedir .. "/postal_10th_anniversary/postal2game",
        "/opt/games/postal_10th_anniversary/postal2game",
        "/usr/local/games/postal_10th_anniversary/postal2game"
        MojoSetup.info.homedir .. "/postal_fudge_pack/postal2game",
        "/opt/games/postal_fudge_pack/postal2game",
        "/usr/local/games/postal_fudge_pack/postal2game"
    },

    Setup.Eula
    {
        description = _("Postal License"),
        source = _("postal2aw_license.txt")
    },

    Setup.Option
    {
        value = true,
        required = true,
        disabled = false,
        bytes = GAME_INSTALL_SIZE,
        description = _("Postal 2: Apocalypse Weekend"),

        Setup.File
        {
            -- Just install everything we see...
        },

        Setup.DesktopMenuItem
        {
            disabled = false,
            name = _("Postal 2: Apocalypse Weekend"),
            genericname = _("Postal 2"),
            tooltip = _("Politically incorrect expansion pack from Running With Scissors"),
            builtin_icon = false,
            icon = "p2aw_rws.png",
            commandline = "%0/postal2aw",
            category = "Game"
        }
    }
}

-- end of config.lua ...

