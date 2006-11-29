function GetLanguage()  -- !!! FIXME: should be in C.
    return "en";
end

_installs = {}

function _assert(test, fnname, elem, error)
    assert(test, fnname .. "::" .. elem .. " -- " .. error .. ".")
end

function _mustExist(fnname, elem, val)
    _assert(val ~= nil, fnname, elem, "Must be explicitly specified")
end

function _mustBeSomething(fnname, elem, val, elemtype)
    -- Can be nil...please use _mustExist if this is a problem!
    if val ~= nil then
        _assert(type(val) == elemtype, fnname, elem, "Must be a " .. elemtype)
    end
end

function _mustBeString(fnname, elem, val)
    _mustBeSomething(fnname, elem, val, "string");
end

function _mustBeBool(fnname, elem, val)
    _mustBeSomething(fnname, elem, val, "boolean");
end

function _mustBeNumber(fnname, elem, val)
    _mustBeSomething(fnname, elem, val, "number");
end

function _mustBeFunction(fnname, elem, val)
    _mustBeSomething(fnname, elem, val, "function")
end

function _mustBeTable(fnname, elem, val)
    _mustBeSomething(fnname, elem, val, "table")
end

function _cantBeEmpty(fnname, elem, val)
    -- Can be nil...please use _mustExist if this is a problem!
    if val ~= nil then
        _assert(val ~= "", fnname, elem, "Can't be empty string")
    end
end

function _mustBeValidSplashPos(fnname, elem, val)
    _assert(val=="top" or val=="left", fnname, elem, "Must be 'top' or 'left'")
end

function _mustBeValidInteraction(fnname, elem, val)
    if (val ~= "expert") and (val ~= "normal") and (val ~= "none") then
        _assert(false, fnname, elem, "Must be 'normal' or 'expert' or 'none'")
    end
end

function _mustBeUrl(fnname, elem, val)
    -- !!! FIXME: check for valid URL here.
end

function _sanitize(fnname, tab, elems)
    _mustBeTable(fnname, "", tab)
    tab._type_ = string.lower(fnname) .. "s";   -- "Eula" becomes "eulas".
    for i,elem in ipairs(elems) do
        local child = elem[1]
        local defval = elem[2]

        -- print(child .. " isa " .. type(tab[child]) .. " equals " .. tostring(tab[child]));
        if tab[child] == nil then tab[child] = defval end
        local j = 3
        while elem[j] do
            elem[j](fnname, child, tab[child]);  -- will assert on problem.
            j = j + 1
        end
    end

    return tab
end

function _reform_schema_table(tab)
    for i in pairs(tab) do
        local typestr = type(i)
        if (typestr == "number") and (tab[i]._type_ ~= nil) then
            -- add element to proper named array.
            typestr = tab[i]._type_
            tab[i]._type_ = nil
            table.insert(tab[typestr], tab[i])
            tab[i] = nil
        elseif typestr == "table" then
            tab[i] = _reform_schema_table(tab[i])
        end
    end

    return tab
end

function _dumptable(tab, depth)
    local depthstr = ""
    for i=1,(depth*4) do
        depthstr = depthstr .. " "
    end

    for k in pairs(tab) do
        if type(tab[k]) == "table" then
            print(depthstr .. k .. " = {")
            _dumptable(tab[k], depth + 1)
            print(depthstr .. "}")
        else
            print(depthstr .. k .. " = " .. tostring(tab[k]))
        end
    end
end

-- Actual schema elements are below...

function Install(tab)
    tab = _sanitize("Install", tab,
    {
        { "product", nil, _mustExist, _mustBeString, _cantBeEmpty },
        { "desc", nil, _mustExist, _mustBeString, _cantBeEmpty },
        { "version", nil, _mustExist, _mustBeString, _cantBeEmpty },
        { "path", "/usr/local/games", _mustBeString, _cantBeEmpty },
        { "preinstall", nil, _mustBeFunction },
        { "postinstall", nil, _mustBeFunction },
        { "splash", nil, _mustBeString, _cantBeEmpty },
        { "url", nil, _mustBeString, _cantBeEmpty },
        { "once", true, _mustBeBool },
        { "category", "Games", _mustBeString, _cantBeEmpty },
        { "promptoverwrite", true, _mustBeBool },
        { "binarypath", nil, _mustBeString, _cantBeEmpty },
        { "splashpos", "top", _mustBeString, _mustBeValidSplashPos },
        { "update_url", nil, _mustBeString, _mustBeUrl },
        { "superuser", false, _mustBeBool },
        { "interaction", "normal", _mustBeString, _mustBeValidInteraction },
        { "eulas", {}, _mustBeTable },  -- use the "Eula" function.
        { "readmes", {}, _mustBeTable }, -- use the "Readme" function.
        { "medias", {}, _mustBeTable }, -- use the "Media" function.
    })

    tab._type_ = nil
    tab = _reform_schema_table(tab)
    table.insert(_installs, tab)
    return tab

--[[
 preuninstall
            This is a shell script which is executed at the very beginning
            of the uninstall process. It will be run before any RPM uninstall
            scripts. This file is not installed, but is added to the
            beginning of uninstall script.

 postuninstall 
            This is a shell script which is executed at the very end of the 
            uninstall process. It will be run after any RPM uninstall
            scripts. This file is not installed, but is added to the
            end of the uninstall script.

            IMPORTANT: An actual file name for a shell scripts needs to be specified,
            not a command, for both pre/postuninstall entries.
            "sh script.sh" is incorrect, but "script.sh" is correct.

            Both the preuninstall and postuninstall scripts will have access
            to the default environment variables. See the 'SCRIPT' section
            for details. 

            Also, these scripts will be run at the very beginning and very 
            end of the install cleanup if the install is aborted.

 nouninstall This is an optional flag which, if specified, tells setup
             not to generate an uninstall script after it runs. It also
             doesn't generate any data for product queries and auto-updating.

 promptbinaries When set to "yes", setup will create a checkbox
                to allow the user whether or not to create 
                a symbolic link to the binaries.

                This setting has no effect if nobinaries is "yes".

 meta       When this attribute is set to "yes", then setup will act as
            a meta-installer, i.e. it will allow the user to select a
            product and set-up will respawn itself for the selected
            product installation.
            See the section "About Meta-Installation" below.

 component  When this attribute is present, its value is the name of the component
            that will created by this installer. This means that the files will be added
            to an existing installation of the product, and that basic configuration parameters
            such as the installation path will be used from the previous installation.
            Currently setup is not able to update an existing component, thus if the component
            is detected as already installed the installation will fail.
            Important: This attribute can't be mixed with <component> elements.

 appbundle  (CARBON ONLY) If this is "yes", the destination folder does not include the product
            name as part of the path.  An application bundle is typically installed much like
            a single file would be...and so is treated as such.
            
            The app bundle path usually specified in the "path" attribute of the "files" element,
            or it is specified in the archives directly.

 manpages   If set to "yes", then the user will be prompted for the install pages installation path.
            Should be used when using the MANPAGE element described below.

 appbundleid  (CARBON ONLY) This string means that you are installing new files into an existing
              Application Bundle. If the bundle isn't found, the installation aborts, otherwise, all
              files are added relative to the base of the app bundle. The string specified here is
              the app bundle's CFBundleIdentifier entry in its Info.plist file. LaunchServices are
              used to find the bundle and the user can be prompted to manually locate the product
              if LaunchServices fails to find it. This is all MacOS-specific, and unrelated to the
              "component" attribute.

 appbundledesc (CARBON ONLY) This string is used with the "appbundleid" attribute...it's a human
               readable text describing the app bundle in question.
]]--

end

function Eula(tab)
    return _sanitize("Eula", tab,
    {
        { "keepdirs", false, _mustBeBool },
        { "filename", nil, _mustExist, _mustBeString, _cantBeEmpty },
    })
end

function Readme(tab)
    return _sanitize("Readme", tab,
    {
        { "keepdirs", false, _mustBeBool },
        { "filename", nil, _mustExist, _mustBeString, _cantBeEmpty },
    })
end

function Media(tab)
    return _sanitize("Media", tab,
    {
        { "id", nil, _mustExist, _mustBeString, _cantBeEmpty },
        { "name", nil, _mustExist, _mustBeString, _cantBeEmpty },
        { "uniquefile", nil, _mustExist, _mustBeString, _cantBeEmpty },
    })
end

function File(tab)
    return _sanitize("File", tab,
    {
    })

--[[
 path       This is a system path that these files should be installed into.
            If the path is relative (i.e. does not begin with '/'), then it
            is interpreted as being relative to the installation root for
            the product. Thus an empty value ("") can be used to designate
            the installation directory.

 arch       "any" is synonymous with the current architecture. You can also
            use this attribute to force a precise architecture, for example
            "ppc" or "sparc64".

 libc       "any" is synonymous with the current libc version. This can
            also be used to force a libc version for the binary, i.e
            "glibc-2.0" or "glibc-2.1".

 distro     Files are only installed with the specified OS distribution.
            Look at the end of this file for a list of possible values.

 srcpath    This is a directory relative to the top of the CD where the files
            for this element should be copied from.

 cdrom      If this attribute has a "yes" value, then setup will look for
            the files on one of the mounted CDROMs on the system.
            *OBSOLETE: Do not use. Use 'cdromid' instead.*

 cdromid    Specifies a CDROM from which the file will be pulled.

 mutable    If set to "yes", then the file will be considered mutable, i.e. it
            will not be considered corrupted if its checksum changes.
 
 lang       The files are only installed if the current locale matches the
            string of this attribute. See 'About localization' below for
            more details.

 relocate   If relocate="true" and an RPM package file is listed, the install
            will force the RPM to be installed into the user-selected install 
            directory. This is equivalent to installing the RPM like this: 
              rpm -U --relocate /=INSTALLDIR --badreloc [rpmfile]
            This will apply to all RPM files within this <files> tag.

 nodeps     If nodeps="true" and an RPM package file is listed,then the
            --nodeps option will be added to the RPM command when the files 
            are installed. This will force RPM to install the package, even if
            its dependencies are not met. This will apply to all RPM files
            within this <files> tag.
 
 autoremove If autoremove="true" and an RPM package file is listed in the 
            files section, then that RPM package will be automatically removed
            ("rpm -e package") when the uninstall script is run. If the 
            autoremove option is not set, then the uninstall script will list 
            the package name at the end of the uninstall, but it will not 
            remove it. This will apply to all RPM files within this <files> 
            tag.

 md5sum     You can optionally specify a MD5 checksum string (32 characters from
            the output of the 'md5sum' command), and the copied file will be verified
            against this checksum. Installation will abort in the case of an invalid
            checksum, indicating corruption.

 mode       Use an octal value to specify the permissions of the file once installed (644
            is the default for regular files).
            When specifying archives handled by a plugin, the plugin may choose to use this
            mode for all files uncompressed from the archive.
            NOTE: This doesn't apply to directories, which are currently always created with
            mode 755.

 suffix     An optional file extension that is forced on all files. Can be used
	    to make loki-setup recognize e.g. SFX ZIP archives as such even if
	    they end in .exe.
]]--
end

function Option(tab)
--[[
 install    If this attribute is set to "true", then the option will
            be installed by default.  It may be deselected by the user.
            Another possible value is "command", in which case the script specified
            in the "command" attribute will determine the final value of this
            property.

 command    This attribute must be set to a shell script if "install" is "command".
            If the command returns with a value of 0 (normal), then the option will
            be selected (and unselected otherwise).
            This can be used for auto-detection purposes.

 required   If this attribute is set to "true", the option will always
            be installed. The user won't be able to disable it.

 show       If present, this attribute specifies a command to be run whose return
            value will determine if this option will be presented to the user at all.
            A 0 return value means that the option will be displayed.
            A "false" string for this attribute will always hide the option from the user.
            All suboptions are also affected by this setting. Note that default values
            and actual selection of the option (with the above tags) is not affected.

 help       This attribute is a help string which is displayed to the
            user if the user requests more information about the option.
            This string can be translated to other languages using the 'help'
            sub-element explained below.

 arch       This option is only available on the specified architectures.
            The architecture can be any of:
               x86, ppc, alpha, sparc64, etc.

 libc       This option is only available with the specified version of libC.
            The version can be one of:
               libc5, glibc-2.0, glibc-2.1, etc.

 distro     This option is only available with the specified OS distribution.
            Look at the end of this file for a list of possible values.

 size       This is an optional size of the install option. The size can be
            expressed in megabytes (with a M suffix), kilobytes (K), gigabytes
            (G), or even bytes (B, or no suffix). Please note that versions of
            setup earlier than 1.3.0 used to specify the size in MBs only.
            If this element isn't specified, setup  will try to autodetect
            the size of the install option itself.

 product    If this XML file describes a meta-installation (if the 'meta' attribute
            to the root install element has been specified), then the value of this
            attribute is used to specify the XML file that will be passed to setup
            to install the product described by this option element. This attribute
            is ignored if this is not a meta-installation, and required if it is.
            See the section 'About Meta-Installations' below for more details.

 productdir Valid only together with the 'product' attribute. Specifies the
            directory to change to before executing the installer for the new
            product.

 tag        If specified, this string will be included in the SETUP_OPTIONTAGS
            environment variable that is set before calling any scripts. This allows
            scripts to know what user selections are active.

 reinstall  If this is a reinstallation, you can individually disable options that can
            be reinstalled with this attribute set to "false".
]]--
end

-- end of schema.lua ...

