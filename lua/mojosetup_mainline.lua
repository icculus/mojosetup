
local _ = MojoSetup.translate
local saw_an_installer = false

for k,install in pairs(MojoSetup.installs) do
    saw_an_installer = true
    if (not MojoSetup.startgui(install.desc, install.splash)) then
        MojoSetup.fatal(_("GUI failed to start"))
    end


    MojoSetup.endgui()
end

if (not saw_an_installer) then
    MojoSetup.fatal(_("Nothing to do!"))
end

-- end of mojosetup_mainline.lua ...

