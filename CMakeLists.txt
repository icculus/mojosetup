PROJECT(MojoSetup)
SET(MOJOSETUP_VERSION 0.1)

# I hate that they define "WIN32" ... we're about to move to Win64...I hope!
IF(WIN32 AND NOT WINDOWS)
    SET(WINDOWS TRUE)
ENDIF(WIN32 AND NOT WINDOWS)

# Bleh, let's do it for "APPLE" too.
IF(APPLE AND NOT MACOSX)
    SET(MACOSX TRUE)
ENDIF(APPLE AND NOT MACOSX)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckLibraryExists)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckCCompilerFlag)
INCLUDE(TestBigEndian)

ADD_DEFINITIONS(-D__MOJOSETUP__=1)
ADD_DEFINITIONS(-DAPPID=mojosetup)
ADD_DEFINITIONS(-DAPPREV=${MOJOSETUP_VERSION})
ADD_DEFINITIONS(-D_REENTRANT)
ADD_DEFINITIONS(-D_THREAD_SAFE)

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(lua-5.1.1/src)

IF(WINDOWS)
    ADD_DEFINITIONS(-DPLATFORM_WINDOWS=1)
ENDIF(WINDOWS)

IF(MACOSX)
    ADD_DEFINITIONS(-DPLATFORM_MACOSX=1)
    ADD_DEFINITIONS(-DLUA_USE_DYLD=1)
    IF(CMAKE_OSX_ARCHITECTURES MATCHES ppc)
        ADD_DEFINITIONS(-DMAC_OS_X_VERSION_MIN_REQUIRED=1020)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} "-mmacosx-version-min=10.2")
    ENDIF(CMAKE_OSX_ARCHITECTURES MATCHES ppc)
    SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} "-framework Carbon")
ENDIF(MACOSX)

IF(BEOS)
    ADD_DEFINITIONS(-DPLATFORM_BEOS=1)
    # !!! FIXME: Workaround for lua issue...fix this better.
    ADD_DEFINITIONS(-D_setjmp=setjmp)
    ADD_DEFINITIONS(-D_longjmp=longjmp)
ENDIF(BEOS)

IF(UNIX)
    # !!! FIXME: probably not safe long-term.
    # CMake mailing list had this hack for getting rid of -rdynamic:
    #   http://public.kitware.com/pipermail/cmake/2006-July/010404.html
    IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
    ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    ADD_DEFINITIONS(-DPLATFORM_UNIX=1)
    ADD_DEFINITIONS(-DLUA_USE_POSIX=1)
    IF(NOT MACOSX AND NOT BEOS)
        ADD_DEFINITIONS(-DLUA_USE_DLOPEN=1)
    ENDIF(NOT MACOSX AND NOT BEOS)
ENDIF(UNIX)

IF(CMAKE_COMPILER_IS_GNUCC)
    ADD_DEFINITIONS(-pipe -Wall -Werror -fsigned-char)

    # See if -fvisibility=hidden is available to us.
    CHECK_C_SOURCE_COMPILES("
        #if ((defined(__GNUC__)) && (__GNUC__ >= 4))
        int main(int argc, char **argv) { int is_gcc4 = 1; return 0; }
        #else
        #error This is not gcc4.
        #endif
    " MOJOSETUP_IS_GCC4)

    IF(MOJOSETUP_IS_GCC4)
        ADD_DEFINITIONS(-fvisibility=hidden)
    ENDIF(MOJOSETUP_IS_GCC4)

    # See if -fno-stack-protector is available to us.
    # It doesn't seem to work well, and it adds bulk to the binary.
    CHECK_C_COMPILER_FLAG("-fno-stack-protector" MOJOSETUP_GCC_HAS_STACKPROT)
    IF(MOJOSETUP_GCC_HAS_STACKPROT)
        ADD_DEFINITIONS(-fno-stack-protector)
    ENDIF(MOJOSETUP_GCC_HAS_STACKPROT)
ENDIF(CMAKE_COMPILER_IS_GNUCC)

TEST_BIG_ENDIAN(MOJOSETUP_IS_BIGENDIAN)
IF(MOJOSETUP_IS_BIGENDIAN)
    ADD_DEFINITIONS(-DPLATFORM_BIGENDIAN=1)
ELSE(MOJOSETUP_IS_BIGENDIAN)
    ADD_DEFINITIONS(-DPLATFORM_LITTLEENDIAN=1)
ENDIF(MOJOSETUP_IS_BIGENDIAN)

SET(ZLIB_DIR zlib-1.2.3)
SET(ZLIB_SRCS
    ${ZLIB_DIR}/adler32.c
    ${ZLIB_DIR}/compress.c
    ${ZLIB_DIR}/crc32.c
    ${ZLIB_DIR}/deflate.c
    ${ZLIB_DIR}/gzio.c
    ${ZLIB_DIR}/infback.c
    ${ZLIB_DIR}/inffast.c
    ${ZLIB_DIR}/inflate.c
    ${ZLIB_DIR}/inftrees.c
    ${ZLIB_DIR}/trees.c
    ${ZLIB_DIR}/uncompr.c
    ${ZLIB_DIR}/zutil.c
)

SET(BZLIB_DIR bzip2-1.0.4)
SET(BZLIB_SRCS
    ${BZLIB_DIR}/blocksort.c
    ${BZLIB_DIR}/huffman.c
    ${BZLIB_DIR}/crctable.c
    ${BZLIB_DIR}/randtable.c
    ${BZLIB_DIR}/compress.c
    ${BZLIB_DIR}/decompress.c
    ${BZLIB_DIR}/bzlib.c
)

SET(LIBFETCH_DIR libfetch)
SET(LIBFETCH_SRCS
    ${LIBFETCH_DIR}/fetch.c
    ${LIBFETCH_DIR}/common.c
    ${LIBFETCH_DIR}/ftp.c
    ${LIBFETCH_DIR}/http.c
)

SET(LUA_DIR lua-5.1.1)
SET(LUA_SRCS
    ${LUA_DIR}/src/lapi.c
    ${LUA_DIR}/src/ldebug.c
    ${LUA_DIR}/src/ldo.c
    ${LUA_DIR}/src/ldump.c
    ${LUA_DIR}/src/lfunc.c
    ${LUA_DIR}/src/lgc.c
    ${LUA_DIR}/src/lmem.c
    ${LUA_DIR}/src/lobject.c
    ${LUA_DIR}/src/lopcodes.c
    ${LUA_DIR}/src/lstate.c
    ${LUA_DIR}/src/lstring.c
    ${LUA_DIR}/src/ltable.c
    ${LUA_DIR}/src/ltm.c
    ${LUA_DIR}/src/lundump.c
    ${LUA_DIR}/src/lvm.c
    ${LUA_DIR}/src/lzio.c
    ${LUA_DIR}/src/lauxlib.c
    ${LUA_DIR}/src/lbaselib.c
    ${LUA_DIR}/src/ldblib.c
    ${LUA_DIR}/src/liolib.c
    ${LUA_DIR}/src/lmathlib.c
    ${LUA_DIR}/src/loslib.c
    ${LUA_DIR}/src/ltablib.c
    ${LUA_DIR}/src/lstrlib.c
    ${LUA_DIR}/src/loadlib.c
    ${LUA_DIR}/src/linit.c
)

SET(LUA_PARSER_SRCS
    ${LUA_DIR}/src/lparser.c
    ${LUA_DIR}/src/llex.c
    ${LUA_DIR}/src/lcode.c
)

SET(MOJOSETUP_SRCS
    buildver.c
    mojosetup.c
    gui.c
    fileio.c
    archive_zip.c
    archive_tar.c
    platform_unix.c
    platform_beos.c
    lua_glue.c
    ${LUA_SRCS}
)

SET(MOJOLUAC_SRCS
    ${LUA_SRCS}
    ${LUA_PARSER_SRCS}
    ${LUA_DIR}/src/luac.c
    ${LUA_DIR}/src/print.c
)

# Disabling the parser cuts the Lua binary bits by about 35% (about 30 kbytes
#  uncompressed on amd64 Linux), plus .luac files are almost always smaller
#  than the original scripts. The downside is you (and end users in the field)
#  can't just tweak a script without recompiling it, but even that's not an
#  unclimbable obstacle.
# In reality, you probably want to keep the parser, though, unless you REALLY
#  must save every single byte in the download.
OPTION(MOJOSETUP_LUA_PARSER "Bigger binary but scripts don't need to be compiled." TRUE)
IF(MOJOSETUP_LUA_PARSER)
    SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} ${LUA_PARSER_SRCS})
ELSE(MOJOSETUP_LUA_PARSER)
    SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} ${LUA_DIR}/etc/noparser.c)
    ADD_DEFINITIONS(-DDISABLE_LUA_PARSER=1)
ENDIF(MOJOSETUP_LUA_PARSER)

OPTION(MOJOSETUP_GUI_STDIO "Enable stdio GUI" TRUE)
IF(MOJOSETUP_GUI_STDIO)
    ADD_DEFINITIONS(-DSUPPORT_GUI_STDIO=1)
    OPTION(MOJOSETUP_GUI_STDIO_STATIC "Statically link stdio GUI" TRUE)
    IF(MOJOSETUP_GUI_STDIO_STATIC)
        ADD_DEFINITIONS(-DGUI_STATIC_LINK_STDIO=1)
        SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} gui_stdio.c)
    ELSE(MOJOSETUP_GUI_STDIO_STATIC)
        ADD_LIBRARY(stdio SHARED gui_stdio.c)
    ENDIF(MOJOSETUP_GUI_STDIO_STATIC)
ENDIF(MOJOSETUP_GUI_STDIO)

IF(MACOSX)
    OPTION(MOJOSETUP_GUI_MACOSX "Enable Mac OS X GUI" TRUE)
    IF(MOJOSETUP_GUI_MACOSX)
        ADD_DEFINITIONS(-DSUPPORT_GUI_MACOSX=1)
        OPTION(MOJOSETUP_GUI_MACOSX_STATIC "Statically link Mac OS X GUI" TRUE)
        IF(MOJOSETUP_GUI_MACOSX_STATIC)
            ADD_DEFINITIONS(-DGUI_STATIC_LINK_MACOSX=1)
            SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} gui_macosx.c)
        ELSE(MOJOSETUP_GUI_MACOSX_STATIC)
            ADD_LIBRARY(macosx SHARED gui_macosx.c)
            TARGET_LINK_LIBRARIES(macosx
                "-framework Carbon -mmacosx-version-min=10.2"
            )
        ENDIF(MOJOSETUP_GUI_MACOSX_STATIC)
    ENDIF(MOJOSETUP_GUI_MACOSX)
ENDIF(MACOSX)

IF(WINDOWS)
    OPTION(MOJOSETUP_GUI_WINDOWS "Enable Windows GUI" TRUE)
    IF(MOJOSETUP_GUI_WINDOWS)
        ADD_DEFINITIONS(-DSUPPORT_GUI_WINDOWS=1)
        OPTION(MOJOSETUP_GUI_WINDOWS_STATIC "Statically link Windows GUI" TRUE)
        IF(MOJOSETUP_GUI_WINDOWS_STATIC)
            ADD_DEFINITIONS(-DGUI_STATIC_LINK_WINDOWS=1)
            SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} gui_windows.c)
        ELSE(MOJOSETUP_GUI_WINDOWS_STATIC)
            ADD_LIBRARY(windows SHARED gui_windows.c)
        ENDIF(MOJOSETUP_GUI_WINDOWS_STATIC)
    ENDIF(MOJOSETUP_GUI_WINDOWS)
ENDIF(WINDOWS)

#IF(UNIX)
#    OPTION(MOJOSETUP_GUI_GTKPLUS "Enable GTK+ GUI" TRUE)
#    IF(MOJOSETUP_GUI_GTKPLUS)
#        ADD_DEFINITIONS(-DSUPPORT_GUI_GTKPLUS=1)
#        OPTION(MOJOSETUP_GUI_GTKPLUS_STATIC "Statically link GTK+ GUI" FALSE)
#        IF(MOJOSETUP_GUI_GTKPLUS_STATIC)
#            ADD_DEFINITIONS(-DGUI_STATIC_LINK_GTKPLUS=1)
#            SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} gui_gtkplus.c)
#        ELSE(MOJOSETUP_GUI_GTKPLUS_STATIC)
#            ADD_LIBRARY(gtkplus SHARED gui_gtkplus.c)
#        ENDIF(MOJOSETUP_GUI_GTKPLUS_STATIC)
#    ENDIF(MOJOSETUP_GUI_GTKPLUS)
#ENDIF(UNIX)

OPTION(MOJOSETUP_ARCHIVE_ZIP "Enable ZIP support" TRUE)
IF(MOJOSETUP_ARCHIVE_ZIP)
    ADD_DEFINITIONS(-DSUPPORT_ZIP=1)
    SET(MOJOSETUP_NEED_ZLIB TRUE)
ENDIF(MOJOSETUP_ARCHIVE_ZIP)

OPTION(MOJOSETUP_ARCHIVE_TAR "Enable TAR support" TRUE)
IF(MOJOSETUP_ARCHIVE_TAR)
    ADD_DEFINITIONS(-DSUPPORT_TAR=1)
    OPTION(MOJOSETUP_ARCHIVE_TAR_GZ "Enable TAR.GZ support" TRUE)
    IF(MOJOSETUP_ARCHIVE_TAR_GZ)
        ADD_DEFINITIONS(-DSUPPORT_TAR_GZ=1)
        SET(MOJOSETUP_NEED_ZLIB TRUE)
    ENDIF(MOJOSETUP_ARCHIVE_TAR_GZ)

    OPTION(MOJOSETUP_ARCHIVE_TAR_BZ2 "Enable TAR.BZ2 support" TRUE)
    IF(MOJOSETUP_ARCHIVE_TAR_BZ2)
        ADD_DEFINITIONS(-DSUPPORT_TAR_BZ2=1)
        SET(MOJOSETUP_NEED_BZLIB TRUE)
    ENDIF(MOJOSETUP_ARCHIVE_TAR_BZ2)
ENDIF(MOJOSETUP_ARCHIVE_TAR)

OPTION(MOJOSETUP_URL_HTTP "Enable http:// support" TRUE)
IF(MOJOSETUP_URL_HTTP)
    ADD_DEFINITIONS(-DSUPPORT_URL_HTTP=1)
    SET(MOJOSETUP_NEED_LIBFETCH TRUE)
ENDIF(MOJOSETUP_URL_HTTP)

OPTION(MOJOSETUP_URL_FTP "Enable ftp://  support" TRUE)
IF(MOJOSETUP_URL_FTP)
    ADD_DEFINITIONS(-DSUPPORT_URL_FTP=1)
    SET(MOJOSETUP_NEED_LIBFETCH TRUE)
ENDIF(MOJOSETUP_URL_FTP)

IF(MOJOSETUP_NEED_LIBFETCH)
    SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} ${LIBFETCH_SRCS})
    # Had to spin up some threads in libfetch...lame.
    INCLUDE(FindThreads)
    SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
ENDIF(MOJOSETUP_NEED_LIBFETCH)

IF(MOJOSETUP_NEED_ZLIB)
    SET(HAVE_SYSTEM_ZLIB FALSE)
    CHECK_INCLUDE_FILE(zlib.h HAVE_ZLIB_H)
    IF(HAVE_ZLIB_H)
        CHECK_LIBRARY_EXISTS("z" "inflate" "" HAVE_LIBZ)
        IF(HAVE_LIBZ)
            SET(HAVE_SYSTEM_ZLIB TRUE)
        ENDIF(HAVE_LIBZ)
    ENDIF(HAVE_ZLIB_H)

    IF(HAVE_SYSTEM_ZLIB)
        OPTION(MOJOSETUP_INTERNAL_ZLIB "Link own zlib instead of system library" FALSE)
    ELSE(HAVE_SYSTEM_ZLIB)
        SET(MOJOSETUP_INTERNAL_ZLIB TRUE)
    ENDIF(HAVE_SYSTEM_ZLIB)

    IF(MOJOSETUP_INTERNAL_ZLIB)
        INCLUDE_DIRECTORIES(${ZLIB_DIR})
        ADD_DEFINITIONS(-DZ_PREFIX=1)
        SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} ${ZLIB_SRCS})
    ELSE(MOJOSETUP_INTERNAL_ZLIB)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} z)
    ENDIF(MOJOSETUP_INTERNAL_ZLIB)
ENDIF(MOJOSETUP_NEED_ZLIB)

IF(MOJOSETUP_NEED_BZLIB)
    SET(HAVE_SYSTEM_BZLIB FALSE)
    CHECK_INCLUDE_FILE(bzlib.h HAVE_BZLIB_H)
    IF(HAVE_BZLIB_H)
        CHECK_LIBRARY_EXISTS("bz2" "BZ2_bzDecompress" "" HAVE_LIBBZ2)
        IF(HAVE_LIBBZ2)
            SET(HAVE_SYSTEM_BZLIB TRUE)
        ENDIF(HAVE_LIBBZ2)
    ENDIF(HAVE_BZLIB_H)

    IF(HAVE_SYSTEM_BZLIB)
        OPTION(MOJOSETUP_INTERNAL_BZLIB "Link own bzlib instead of system library" FALSE)
    ELSE(HAVE_SYSTEM_BZLIB)
        SET(MOJOSETUP_INTERNAL_BZLIB TRUE)
    ENDIF(HAVE_SYSTEM_BZLIB)

    IF(MOJOSETUP_INTERNAL_BZLIB)
        INCLUDE_DIRECTORIES(${BZLIB_DIR})
        SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} ${BZLIB_SRCS})
    ELSE(MOJOSETUP_INTERNAL_BZLIB)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} bz2)
    ENDIF(MOJOSETUP_INTERNAL_BZLIB)
ENDIF(MOJOSETUP_NEED_BZLIB)

IF(UNIX)
    CHECK_INCLUDE_FILE(sys/ucred.h HAVE_UCRED_H)
    IF(HAVE_UCRED_H)
        ADD_DEFINITIONS(-DMOJOSETUP_HAVE_SYS_UCRED_H=1)
    ENDIF(HAVE_UCRED_H)

    CHECK_INCLUDE_FILE(mntent.h HAVE_MNTENT_H)
    IF(HAVE_MNTENT_H)
        ADD_DEFINITIONS(-DMOJOSETUP_HAVE_MNTENT_H=1)
    ENDIF(HAVE_MNTENT_H)

    IF(NOT MACOSX)
        CHECK_LIBRARY_EXISTS("dl" "dlopen" "" HAVE_LIBDL)
        IF(HAVE_LIBDL)
            SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} dl)
        ENDIF(HAVE_LIBDL)
        CHECK_LIBRARY_EXISTS("m" "sin" "" HAVE_LIBM)
        IF(HAVE_LIBM)
            SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} m)
        ENDIF(HAVE_LIBM)
    ENDIF(NOT MACOSX)
ENDIF(UNIX)

OPTION(MOJOSETUP_BUILD_LUAC "Build separate Lua compiler" TRUE)
IF(MOJOSETUP_BUILD_LUAC)
    ADD_EXECUTABLE(mojoluac ${MOJOLUAC_SRCS})
    TARGET_LINK_LIBRARIES(mojoluac ${OPTIONAL_LIBS})
    GET_TARGET_PROPERTY(MOJOLUAC_LOCATION mojoluac LOCATION)
    # !!! FIXME: actually compile this.
    ADD_CUSTOM_TARGET(lua "${MOJOLUAC_LOCATION}" -p ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.lua)
ENDIF(MOJOSETUP_BUILD_LUAC)

ADD_EXECUTABLE(mojosetup ${MOJOSETUP_SRCS} ${OPTIONAL_SRCS})
TARGET_LINK_LIBRARIES(mojosetup ${OPTIONAL_LIBS})

# end of CMakeLists.txt ...

