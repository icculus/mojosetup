#!/bin/bash

# This script is now used for most of the examples, since this was meant to
#  be a simple script, but grew to be fairly complicated, and mostly identical
#  between each project. So you have to have a bunch of arguments to this
#  script, which are set by the "details.txt" in a given example's
#  subdirectory. Run this like: "./build-example.sh braid" or whatever.

# This script is not robust for all platforms or situations. Use as a rough
#  example, but invest effort in what it's trying to do, and what it produces.
#  (make sure you don't build in features you don't need, etc).

if [ -z "$1" ]; then
    echo "USAGE: $0 <appid> [--debug]" 1>&2
    exit 1
fi

APPID="$1"
cd `dirname "$0"`
BASEDIR=`pwd`/"$APPID"
source "$BASEDIR/details.txt"

MISSING_ARG=0
if [ -z "$MINIMAL" ]; then MINIMAL=0 ; fi
if [ -z "$NEEDHTTP" ]; then NEEDHTTP=0 ; fi
if [ -z "$DATAFILE" ]; then MISSING_ARG=1 ; fi
if [ -z "$APPTITLE" ]; then MISSING_ARG=1 ; fi

if [ "$MISSING_ARG" == "1" ]; then
    echo "Missing required arguments." 1>&2
    echo "Please make sure $BASEDIR/details.txt is complete." 1>&2
    exit 1
fi

# The real work happens below here.

# Stop if anything produces an error.
set -e

DEBUG=0
if [ "$2" = "--debug" ]; then
    echo "debug build!"
    DEBUG=1
fi

if [ ! -f "$BASEDIR/$DATAFILE" ]; then
    echo "We don't see '$BASEDIR/$DATAFILE' ..." 1>&2
    echo " Either you're in the wrong directory, or you didn't copy the" 1>&2
    echo " install data into here (it's unreasonably big to store it in" 1>&2
    echo " revision control for no good reason)." 1>&2
    exit 1
fi

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

OSTYPE=`uname -s`
if [ "$OSTYPE" = "Linux" ]; then
    NCPU=`cat /proc/cpuinfo |grep vendor_id |wc -l`
    let NCPU=$NCPU+1
elif [ "$OSTYPE" = "Darwin" ]; then
    NCPU=`sysctl -n hw.ncpu`
elif [ "$OSTYPE" = "SunOS" ]; then
    NCPU=`/usr/sbin/psrinfo |wc -l |sed -e 's/^ *//g;s/ *$//g'`
else
    NCPU=1
fi

if [ -z "$NCPU" ]; then
    NCPU=1
elif [ "$NCPU" = "0" ]; then
    NCPU=1
fi

echo "Will use make -j$NCPU. If this is wrong, check NCPU at top of script."

# Show everything that we do here on stdout.
set -x


if [ "$MINIMAL" = "1" ]; then
    FALSEIFMINIMAL="FALSE"
else
    FALSEIFMINIMAL="TRUE"
fi

if [ "$NEEDHTTP" = "1" ]; then
    TRUEIFNEEDHTTP="TRUE"
else
    TRUEIFNEEDHTTP="FALSE"
fi

if [ "$DEBUG" = "1" ]; then
    LUASTRIPOPT=""
    BUILDTYPE="Debug"
    TRUEIFDEBUG="TRUE"
    FALSEIFDEBUG="FALSE"
else
    LUASTRIPOPT="-s"
    BUILDTYPE="MinSizeRel"
    TRUEIFDEBUG="FALSE"
    FALSEIFDEBUG="TRUE"
fi

cd "$BASEDIR"

# this is a little nasty, but it works!
TOTALINSTALL=`du -sb data |perl -w -p -e 's/\A(\d+)\s+data\Z/$1/;'`
perl -w -pi -e "s/\A\s*(local TOTAL_INSTALL_SIZE)\s*\=\s*\d+\s*;\s*\Z/\$1 = $TOTALINSTALL;\n/;" scripts/config.lua

# Clean up previous run, build fresh dirs for Base Archive.
rm -rf image "$APPID-installer" pdata.zip
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
    -DMOJOSETUP_GUI_GTKPLUS2=$FALSEIFMINIMAL \
    -DMOJOSETUP_GUI_GTKPLUS2_STATIC=FALSE \
    -DMOJOSETUP_GUI_NCURSES=$FALSEIFMINIMAL \
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
    -DMOJOSETUP_LUA_PARSER=$FALSEIFMINIMAL \
    -DMOJOSETUP_IMAGE_BMP=$FALSEIFMINIMAL \
    -DMOJOSETUP_IMAGE_JPG=FALSE \
    -DMOJOSETUP_IMAGE_PNG=FALSE \
    -DMOJOSETUP_INTERNAL_BZLIB=FALSE \
    -DMOJOSETUP_INTERNAL_ZLIB=TRUE \
    -DMOJOSETUP_URL_HTTP=$TRUEIFNEEDHTTP \
    -DMOJOSETUP_URL_FTP=FALSE \
    ..

make -j$NCPU

# Strip the binaries and GUI plugins, put them somewhere useful.
if [ "$DEBUG" != "1" ]; then
    strip ./mojosetup
fi

mv ./mojosetup "$BASEDIR/$APPID-installer"
for feh in *.so *.dll *.dylib ; do
    if [ -f $feh ]; then
        if [ "$DEBUG" != "1" ]; then
            strip $feh
        fi
        mv $feh "$BASEDIR/image/guis/"
    fi
done

# Compile the Lua scripts, put them in the base archive.
for feh in ../scripts/*.lua ; do
    ./mojoluac $LUASTRIPOPT -o "$BASEDIR/image/scripts/${feh}c" $feh
done

# Don't want the example config...use our's instead.
rm -f "$BASEDIR/image/scripts/config.luac"
./mojoluac $LUASTRIPOPT -o "$BASEDIR/image/scripts/config.luac" "$BASEDIR/scripts/config.lua"

# Don't want the example app_localization...use ours instead if it exists.
if [ -f "$BASEDIR/scripts/app_localization.lua" ]; then
    rm -f "$BASEDIR/image/scripts/app_localization.luac"
    ./mojoluac $LUASTRIPOPT -o "$BASEDIR/image/scripts/app_localization.luac" "$BASEDIR/scripts/app_localization.lua"
fi

# Fill in the rest of the Base Archive...
cd "$BASEDIR"
cp -R data/* image/data/
cp meta/* image/meta/

# Need these scripts to do things like install menu items, etc, on Unix.
if [ "$OSTYPE" = "Linux" ]; then
    USE_XDG_UTILS=1
fi
if [ "$OSTYPE" = "SunOS" ]; then
    USE_XDG_UTILS=1
fi

if [ "x$USE_XDG_UTILS" = "x1" ]; then
    mkdir image/meta/xdg-utils
    cp ../../meta/xdg-utils/* image/meta/xdg-utils/
    chmod a+rx image/meta/xdg-utils/*
fi

# Tweak up the permissions in the final image...
chmod -R a+r image
chmod -R go-w image
find image -type d -exec chmod a+x {} \;

if [ "$OSTYPE" = "Darwin" ]; then
    # Build up the application bundle for Mac OS X...
    APPBUNDLE="$APPTITLE.app"
    rm -rf "$APPBUNDLE"
    cp -Rv ../../misc/MacAppBundleSkeleton "$APPBUNDLE"
	perl -w -pi -e 's/YOUR_APPLICATION_NAME_HERE/'"$APPTITLE"'/g;' "${APPBUNDLE}/Contents/Info.plist"
    mv "$APPID-installer" "${APPBUNDLE}/Contents/MacOS/mojosetup"
    mv image/* "${APPBUNDLE}/Contents/MacOS/"
    rmdir image
    ibtool --compile "${APPBUNDLE}/Contents/Resources/MojoSetup.nib" ../../misc/MojoSetup.xib
else
    # Make a .zip archive of the Base Archive dirs and nuke the originals...
    cd image
    zip -9r ../pdata.zip *
    cd ..
    rm -rf image
    # Append the .zip archive to the mojosetup binary, so it's "self-extracting."
    ../../cmake-build/make_self_extracting "$APPID-installer" pdata.zip
    rm -f pdata.zip
fi

# ...and that's that.
set +e
set +x
echo "Successfully built!"
echo "The installer is in '$BASEDIR/$APPID-installer' ..."

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

