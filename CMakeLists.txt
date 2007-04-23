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
    SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} platform/beos.c)
ENDIF(BEOS)

IF(UNIX)
    ADD_DEFINITIONS(-DPLATFORM_UNIX=1)
    ADD_DEFINITIONS(-DLUA_USE_POSIX=1)
    IF(NOT MACOSX AND NOT BEOS)
        ADD_DEFINITIONS(-DLUA_USE_DLOPEN=1)
    ENDIF(NOT MACOSX AND NOT BEOS)
    SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} platform/unix.c)
ENDIF(UNIX)

# See if -fvisibility=hidden is available to us.
IF(CMAKE_COMPILER_IS_GNUCC)
    ADD_DEFINITIONS(-pipe -Wall -Werror -fsigned-char)

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
ENDIF(CMAKE_COMPILER_IS_GNUCC)

SET(ZLIB_SRCS
    zlib123/adler32.c
    zlib123/compress.c
    zlib123/crc32.c
    zlib123/deflate.c
    zlib123/gzio.c
    zlib123/infback.c
    zlib123/inffast.c
    zlib123/inflate.c
    zlib123/inftrees.c
    zlib123/trees.c
    zlib123/uncompr.c
    zlib123/zutil.c
)

SET(LUA_SRCS
    lua-5.1.1/src/lapi.c
    lua-5.1.1/src/ldebug.c
    lua-5.1.1/src/ldo.c
    lua-5.1.1/src/ldump.c
    lua-5.1.1/src/lfunc.c
    lua-5.1.1/src/lgc.c
    lua-5.1.1/src/lmem.c
    lua-5.1.1/src/lobject.c
    lua-5.1.1/src/lopcodes.c
    lua-5.1.1/src/lstate.c
    lua-5.1.1/src/lstring.c
    lua-5.1.1/src/ltable.c
    lua-5.1.1/src/ltm.c
    lua-5.1.1/src/lundump.c
    lua-5.1.1/src/lvm.c
    lua-5.1.1/src/lzio.c
    lua-5.1.1/src/lauxlib.c
    lua-5.1.1/src/lbaselib.c
    lua-5.1.1/src/ldblib.c
    lua-5.1.1/src/liolib.c
    lua-5.1.1/src/lmathlib.c
    lua-5.1.1/src/loslib.c
    lua-5.1.1/src/ltablib.c
    lua-5.1.1/src/lstrlib.c
    lua-5.1.1/src/loadlib.c
    lua-5.1.1/src/linit.c
)

SET(LUA_PARSER_SRCS
    lua-5.1.1/src/lparser.c
    lua-5.1.1/src/llex.c
    lua-5.1.1/src/lcode.c
)

SET(MOJOSETUP_SRCS
    buildver.c
    mojosetup.c
    gui.c
    fileio.c
    archive_zip.c
    lua_glue.c
    ${LUA_SRCS}
)

SET(MOJOLUAC_SRCS
    ${LUA_SRCS}
    ${LUA_PARSER_SRCS}
    lua-5.1.1/src/luac.c
    lua-5.1.1/src/print.c
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
    SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} lua-5.1.1/etc/noparser.c)
    ADD_DEFINITIONS(-DDISABLE_LUA_PARSER=1)
ENDIF(MOJOSETUP_LUA_PARSER)

OPTION(MOJOSETUP_GUI_STDIO "Enable stdio GUI" TRUE)
IF(MOJOSETUP_GUI_STDIO)
    ADD_DEFINITIONS(-DSUPPORT_GUI_STDIO=1)
    OPTION(MOJOSETUP_GUI_STDIO_STATIC "Statically link stdio GUI" TRUE)
    IF(MOJOSETUP_GUI_STDIO_STATIC)
        ADD_DEFINITIONS(-DGUI_STATIC_LINK_STDIO=1)
        SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} gui/gui_stdio.c)
    ELSE(MOJOSETUP_GUI_STDIO_STATIC)
        ADD_LIBRARY(stdio SHARED gui/gui_stdio.c)
    ENDIF(MOJOSETUP_GUI_STDIO_STATIC)
ENDIF(MOJOSETUP_GUI_STDIO)

IF(MACOSX)
    OPTION(MOJOSETUP_GUI_MACOSX "Enable Mac OS X GUI" TRUE)
    IF(MOJOSETUP_GUI_MACOSX)
        ADD_DEFINITIONS(-DSUPPORT_GUI_MACOSX=1)
        OPTION(MOJOSETUP_GUI_MACOSX_STATIC "Statically link Mac OS X GUI" TRUE)
        IF(MOJOSETUP_GUI_MACOSX_STATIC)
            ADD_DEFINITIONS(-DGUI_STATIC_LINK_MACOSX=1)
            SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} gui/gui_macosx.c)
        ELSE(MOJOSETUP_GUI_MACOSX_STATIC)
            ADD_LIBRARY(macosx SHARED gui/gui_macosx.c)
            TARGET_LINK_LIBRARIES(macosx
                -framework Carbon -mmacosx-version-min=10.2
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
            SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} gui/gui_windows.c)
        ELSE(MOJOSETUP_GUI_WINDOWS_STATIC)
            ADD_LIBRARY(windows SHARED gui/gui_windows.c)
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
#            SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} gui/gui_gtkplus.c)
#        ELSE(MOJOSETUP_GUI_GTKPLUS_STATIC)
#            ADD_LIBRARY(gtkplus SHARED gui/gui_gtkplus.c)
#        ENDIF(MOJOSETUP_GUI_GTKPLUS_STATIC)
#    ENDIF(MOJOSETUP_GUI_GTKPLUS)
#ENDIF(UNIX)

OPTION(MOJOSETUP_ARCHIVE_ZIP "Enable ZIP support" TRUE)
IF(MOJOSETUP_ARCHIVE_ZIP)
    ADD_DEFINITIONS(-DSUPPORT_ZIP=1)
    SET(MOJOSETUP_NEED_ZLIB 1)
ENDIF(MOJOSETUP_ARCHIVE_ZIP)

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
        INCLUDE_DIRECTORIES(zlib123)
        ADD_DEFINITIONS(-DZ_PREFIX=1)
        SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} ${ZLIB_SRCS})
    ELSE(MOJOSETUP_INTERNAL_ZLIB)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} z)
    ENDIF(MOJOSETUP_INTERNAL_ZLIB)
ENDIF(MOJOSETUP_NEED_ZLIB)

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
    ADD_CUSTOM_TARGET(lua "${MOJOLUAC_LOCATION}" -p ${CMAKE_CURRENT_SOURCE_DIR}/lua/*.lua)
ENDIF(MOJOSETUP_BUILD_LUAC)

ADD_EXECUTABLE(mojosetup ${MOJOSETUP_SRCS} ${OPTIONAL_SRCS})
TARGET_LINK_LIBRARIES(mojosetup ${OPTIONAL_LIBS})

# end of CMakeLists.txt ...

