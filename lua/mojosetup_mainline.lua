
local _ = MojoSetup.translate;
local saw_an_installer = false

for i in pairs(MojoSetup.installs) do

    saw_an_installer = true
end

if (not saw_an_installer) then
    MojoSetup.fatal(_("Nothing to do!"));
end

-- end of mojosetup_mainline.lua ...

