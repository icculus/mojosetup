#!/bin/sh

# This script is not robust for all platforms or situations. Use as a rough
#  example, but invest effort in what it's trying to do, and what it produces.
#  (make sure you don't build in features you don't need, etc).

# Show everything that we do here on stdout.
set -x

# Stop if anything produces an error.
set -e

# Clean up previous run, build fresh dirs for Base Archive.
rm -rf image duke3d-installer pdata.zip
mkdir image
mkdir image/guis
mkdir image/scripts
mkdir image/data

# Build MojoSetup binaries from scratch.
cd ../..
rm -rf *.so mojosetup `svn propget svn:ignore .`
cmake \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DMOJOSETUP_ARCHIVE_TAR=FALSE \
    -DMOJOSETUP_ARCHIVE_TAR_BZ2=FALSE \
    -DMOJOSETUP_ARCHIVE_TAR_GZ=FALSE \
    -DMOJOSETUP_LUALIB_DB=FALSE \
    -DMOJOSETUP_LUALIB_IO=FALSE \
    -DMOJOSETUP_LUALIB_MATH=FALSE \
    -DMOJOSETUP_LUALIB_OS=FALSE \
    -DMOJOSETUP_LUALIB_PACKAGE=FALSE \
    -DMOJOSETUP_LUA_PARSER=FALSE \
    .
make -j5

# Strip the binaries and GUI plugins, put them somewhere useful.
strip ./mojosetup
mv ./mojosetup ./examples/duke3d/duke3d-installer
for feh in *.so ; do
    strip $feh
    mv $feh examples/duke3d/image/guis
done

# Compile the Lua scripts, put them in the base archive.
for feh in scripts/*.lua ; do
    ./mojoluac -s -o examples/duke3d/image/${feh}c $feh
done

# Don't want the example config...use our's instead.
rm -f examples/duke3d/image/scripts/config.luac
./mojoluac -s -o examples/duke3d/image/scripts/config.luac examples/duke3d/scripts/config.lua

# Fill in the rest of the Base Archive...
cd examples/duke3d
cp data/* image/data/

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
exit 0

