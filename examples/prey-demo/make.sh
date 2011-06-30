#!/bin/bash

# This script is not robust for all platforms or situations. Use as a rough
#  example, but invest effort in what it's trying to do, and what it produces.
#  (make sure you don't build in features you don't need, etc).

if [ ! -d data/prey-demo-linux-data ]; then
    echo "We don't see data/prey-demo-linux-data ..."
    echo " Either you're in the wrong directory, or you didn't copy the"
    echo " install data into here (it's unreasonably big to store it in"
    echo " revision control for no good reason)."
    exit 1
fi

# Stop if anything produces an error.
set -e

# I use a "cross compiler" to build binaries that are isolated from the
#  particulars of my Linux workstation's current distribution. This both
#  keeps me at a consistent ABI for generated binaries and prevent subtle
#  dependencies from leaking in.
# You may not care about this at all. In which case, just use the
#  CC=gcc and CXX=g++ lines instead.
CC=/usr/bin/gcc
CXX=/usr/bin/g++
#CC=/opt/crosstool/gcc-3.3.6-glibc-2.3.5/i686-unknown-linux-gnu/i686-unknown-linux-gnu/bin/gcc
#CXX=/opt/crosstool/gcc-3.3.6-glibc-2.3.5/i686-unknown-linux-gnu/i686-unknown-linux-gnu/bin/g++

pkg="prey-demo"
pkgbin="$pkg-installer-`date +%m%d%Y`.bin"
readmefname="prey_demo_readme.txt"

DEBUG=0
if [ "$1" = "--debug" ]; then
    echo "debug build!"
    DEBUG=1
fi

OSTYPE=`uname -s`
if [ "$OSTYPE" = "Linux" ]; then
    NCPU=`cat /proc/cpuinfo |grep vendor_id |wc -l`
    let NCPU=$NCPU+1
elif [ "$OSTYPE" = "Darwin" ]; then
    NCPU=`sysctl -n hw.ncpu`
else
    NCPU=1
fi

echo "Will use make -j$NCPU. If this is wrong, check NCPU at top of script."

# Show everything that we do here on stdout.
set -x

if [ "$DEBUG" = "1" ]; then
    LUASTRIPOPT=
    BUILDTYPE=Debug
    TRUEIFDEBUG=TRUE
    FALSEIFDEBUG=FALSE
else
    LUASTRIPOPT=-s
    BUILDTYPE=MinSizeRel
    TRUEIFDEBUG=FALSE
    FALSEIFDEBUG=TRUE
fi

# this is a little nasty, but it works!
GAMEINSTALL=`du -csb data/prey* |tail -n 1 |perl -w -pi -e 's/\A(\d+)\s+total\Z/$1/;'`
perl -w -pi -e "s/\A\s*(local GAME_INSTALL_SIZE)\s*\=\s*\d+\s*;\s*\Z/\$1 = $GAMEINSTALL;\n/;" scripts/config.lua
PBINSTALL=`du -csb data/punkbuster* |tail -n 1 |perl -w -pi -e 's/\A(\d+)\s+total\Z/$1/;'`
perl -w -pi -e "s/\A\s*(local PB_INSTALL_SIZE)\s*\=\s*\d+\s*;\s*\Z/\$1 = $PBINSTALL;\n/;" scripts/config.lua
ENCFGINSTALL=`du -csb data/configs/english.cfg |tail -n 1 |perl -w -pi -e 's/\A(\d+)\s+total\Z/$1/;'`
perl -w -pi -e "s/\A\s*(local ENCFG_INSTALL_SIZE)\s*\=\s*\d+\s*;\s*\Z/\$1 = $ENCFGINSTALL;\n/;" scripts/config.lua
FRCFGINSTALL=`du -csb data/configs/french.cfg |tail -n 1 |perl -w -pi -e 's/\A(\d+)\s+total\Z/$1/;'`
perl -w -pi -e "s/\A\s*(local FRCFG_INSTALL_SIZE)\s*\=\s*\d+\s*;\s*\Z/\$1 = $FRCFGINSTALL;\n/;" scripts/config.lua
ITCFGINSTALL=`du -csb data/configs/italian.cfg |tail -n 1 |perl -w -pi -e 's/\A(\d+)\s+total\Z/$1/;'`
perl -w -pi -e "s/\A\s*(local ITCFG_INSTALL_SIZE)\s*\=\s*\d+\s*;\s*\Z/\$1 = $ITCFGINSTALL;\n/;" scripts/config.lua
DECFGINSTALL=`du -csb data/configs/german.cfg |tail -n 1 |perl -w -pi -e 's/\A(\d+)\s+total\Z/$1/;'`
perl -w -pi -e "s/\A\s*(local DECFG_INSTALL_SIZE)\s*\=\s*\d+\s*;\s*\Z/\$1 = $DECFGINSTALL;\n/;" scripts/config.lua
ESCFGINSTALL=`du -csb data/configs/spanish.cfg |tail -n 1 |perl -w -pi -e 's/\A(\d+)\s+total\Z/$1/;'`
perl -w -pi -e "s/\A\s*(local ESCFG_INSTALL_SIZE)\s*\=\s*\d+\s*;\s*\Z/\$1 = $ESCFGINSTALL;\n/;" scripts/config.lua

# Clean up previous run, build fresh dirs for Base Archive.
rm -rf image $pkgbin pdata.zip
mkdir image
mkdir image/guis
mkdir image/scripts
mkdir image/data
mkdir image/meta

# Build MojoSetup binaries from scratch.
# YOU ALWAYS NEED THE LUA PARSER IF YOU WANT UNINSTALL SUPPORT!
cd ../..
rm -rf cmake-build
mkdir cmake-build
cd cmake-build
cmake \
    -DCMAKE_BUILD_TYPE=$BUILDTYPE \
    -DCMAKE_C_COMPILER=$CC \
    -DCMAKE_CXX_COMPILER=$CXX \
    -DMOJOSETUP_MULTIARCH=FALSE \
    -DMOJOSETUP_ARCHIVE_ZIP=TRUE \
    -DMOJOSETUP_ARCHIVE_TAR=FALSE \
    -DMOJOSETUP_ARCHIVE_TAR_BZ2=FALSE \
    -DMOJOSETUP_ARCHIVE_TAR_GZ=FALSE \
    -DMOJOSETUP_GUI_GTKPLUS2=TRUE \
    -DMOJOSETUP_GUI_GTKPLUS2_STATIC=FALSE \
    -DMOJOSETUP_GUI_NCURSES=TRUE \
    -DMOJOSETUP_GUI_NCURSES_STATIC=FALSE \
    -DMOJOSETUP_GUI_STDIO=TRUE \
    -DMOJOSETUP_GUI_STDIO_STATIC=TRUE \
    -DMOJOSETUP_GUI_WWW=FALSE \
    -DMOJOSETUP_GUI_WWW_STATIC=FALSE \
    -DMOJOSETUP_LUALIB_DB=FALSE \
    -DMOJOSETUP_LUALIB_IO=FALSE \
    -DMOJOSETUP_LUALIB_MATH=FALSE \
    -DMOJOSETUP_LUALIB_OS=FALSE \
    -DMOJOSETUP_LUALIB_PACKAGE=FALSE \
    -DMOJOSETUP_LUA_PARSER=TRUE \
    -DMOJOSETUP_IMAGE_BMP=TRUE \
    -DMOJOSETUP_IMAGE_JPG=FALSE \
    -DMOJOSETUP_IMAGE_PNG=FALSE \
    -DMOJOSETUP_INTERNAL_BZLIB=FALSE \
    -DMOJOSETUP_INTERNAL_ZLIB=TRUE \
    -DMOJOSETUP_URL_HTTP=FALSE \
    -DMOJOSETUP_URL_FTP=FALSE \
    ..
make -j$NCPU

# Strip the binaries and GUI plugins, put them somewhere useful.
if [ "$DEBUG" != "1" ]; then
    strip ./mojosetup
fi

mv ./mojosetup ../examples/$pkg/$pkgbin
for feh in *.so *.dll *.dylib ; do
    if [ -f $feh ]; then
        if [ "$DEBUG" != "1" ]; then
            strip $feh
        fi
        mv $feh ../examples/$pkg/image/guis
    fi
done

# Compile the Lua scripts, put them in the base archive.
for feh in ../scripts/*.lua ; do
    ./mojoluac $LUASTRIPOPT -o ../examples/$pkg/image/scripts/${feh}c $feh
done

# Don't want the example config...use our's instead.
rm -f ../examples/$pkg/image/scripts/config.luac
./mojoluac $LUASTRIPOPT -o ../examples/$pkg/image/scripts/config.luac ../examples/$pkg/scripts/config.lua

# Don't want the example app_localization...use our's instead.
rm -f ../examples/$pkg/image/scripts/app_localization.luac
./mojoluac $LUASTRIPOPT -o ../examples/$pkg/image/scripts/app_localization.luac ../examples/$pkg/scripts/app_localization.lua

# Fill in the rest of the Base Archive...
cd ../examples/$pkg
cp -R data/* image/data/
cp -R meta/* image/meta/

echo >> image/data/$readmefname
echo >> image/data/$readmefname
echo "Installer: " >> image/data/$readmefname
./$pkgbin --buildver >> image/data/$readmefname
echo >> image/data/$readmefname

# Need these scripts to do things like install menu items, etc, on Unix.
if [ "$OSTYPE" = "Linux" ]; then
    mkdir image/meta/xdg-utils
    cp -av ../../meta/xdg-utils/xdg-desktop-menu image/meta/xdg-utils/
fi

# Make a .zip archive of the Base Archive dirs and nuke the originals...
chmod -R a+r image
find image -type d -exec chmod a+x {} \;
cd image
zip -9r ../pdata.zip *
cd ..
rm -rf image

# Append the .zip archive to the mojosetup binary, so it's "self-extracting."
../../cmake-build/make_self_extracting ./$pkgbin pdata.zip
rm -f pdata.zip

# ...and that's that.
set +e
set +x
echo "Successfully built!"

if [ "$DEBUG" = "1" ]; then
    echo
    echo
    echo
    echo 'ATTENTION: THIS IS A DEBUG BUILD!'
    echo " DON'T DISTRIBUTE TO THE PUBLIC."
    echo ' THIS IS PROBABLY BIGGER AND SLOWER THAN IT SHOULD BE.'
    echo ' YOU HAVE BEEN WARNED!'
    echo
    echo
    echo
fi

exit 0

