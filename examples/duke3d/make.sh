#!/bin/sh

# This script is not robust for all platforms or situations. Use as a rough
#  example, but invest effort in what it's trying to do, and what it produces.
#  (make sure you don't build in features you don't need, etc).

# Stop if anything produces an error.
set -e

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
elif [ "$OSTYPE" = "SunOS" ]; then
    NCPU=`/usr/sbin/psrinfo |wc -l |sed -e 's/^ *//g;s/ *$//g'`
else
    NCPU=1
fi

if [ "x$NCPU" = "x" ]; then
    NCPU=1
fi
if [ "x$NCPU" = "x0" ]; then
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

# Clean up previous run, build fresh dirs for Base Archive.
rm -rf image duke3d-installer pdata.zip
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
    -DMOJOSETUP_ARCHIVE_TAR=FALSE \
    -DMOJOSETUP_ARCHIVE_TAR_BZ2=FALSE \
    -DMOJOSETUP_ARCHIVE_TAR_GZ=FALSE \
    -DMOJOSETUP_LUALIB_DB=FALSE \
    -DMOJOSETUP_LUALIB_IO=FALSE \
    -DMOJOSETUP_LUALIB_MATH=FALSE \
    -DMOJOSETUP_LUALIB_OS=FALSE \
    -DMOJOSETUP_LUALIB_PACKAGE=FALSE \
    -DMOJOSETUP_LUA_PARSER=TRUE \
    -DMOJOSETUP_IMAGE_BMP=TRUE \
    -DMOJOSETUP_IMAGE_JPG=FALSE \
    -DMOJOSETUP_IMAGE_PNG=FALSE \
    -DMOJOSETUP_URL_FTP=FALSE \
    ..
make -j$NCPU

# Strip the binaries and GUI plugins, put them somewhere useful.
if [ "$DEBUG" != "1" ]; then
    strip ./mojosetup
fi

mv ./mojosetup ../examples/duke3d/duke3d-installer
for feh in *.so *.dll *.dylib ; do
    if [ -f $feh ]; then
        if [ "$DEBUG" != "1" ]; then
            strip $feh
        fi
        mv $feh ../examples/duke3d/image/guis
    fi
done

# Compile the Lua scripts, put them in the base archive.
for feh in ../scripts/*.lua ; do
    ./mojoluac $LUASTRIPOPT -o ../examples/duke3d/image/scripts/${feh}c $feh
done

# Don't want the example config...use our's instead.
rm -f ../examples/duke3d/image/scripts/config.luac
./mojoluac $LUASTRIPOPT -o ../examples/duke3d/image/scripts/config.luac ../examples/duke3d/scripts/config.lua

# Don't want the example app_localization...use our's instead.
rm -f ../examples/duke3d/image/scripts/app_localization.luac
./mojoluac $LUASTRIPOPT -o ../examples/duke3d/image/scripts/app_localization.luac ../examples/duke3d/scripts/app_localization.lua

# Fill in the rest of the Base Archive...
cd ../examples/duke3d
cp data/* image/data/
cp meta/* image/meta/

# Need these scripts to do things like install menu items, etc, on Unix.
if [ "$OSTYPE" = "Linux" ]; then
    mkdir image/meta/xdg-utils
    cp -av ../../meta/xdg-utils/* image/meta/xdg-utils/
fi

# Make a .zip archive of the Base Archive dirs and nuke the originals...
cd image
zip -9r ../pdata.zip *
cd ..
rm -rf image

# Append the .zip archive to the mojosetup binary, so it's "self-extracting."
cat pdata.zip >> ./duke3d-installer
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

