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


# !!! FIXME: TODO:
# GUI plugin building

INCLUDE(CheckIncludeFile)
INCLUDE(CheckLibraryExists)
INCLUDE(CheckCSourceCompiles)

OPTION(MOJOSETUP_DEBUG "Enable debugging options" FALSE)

# There are more cflags later in the compiler-specific bits.
IF(DEBUG_BUILD)
    ADD_DEFINITIONS(-DDEBUG=1 -D_DEBUG=1)
ELSE(DEBUG_BUILD)
    ADD_DEFINITIONS(-DNDEBUG=1 -D_NDEBUG=1)
ENDIF(DEBUG_BUILD)

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
    SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} "-framework Carbon")
ENDIF(MACOSX)

IF(UNIX)
    ADD_DEFINITIONS(-DPLATFORM_UNIX=1)
    ADD_DEFINITIONS(-DLUA_USE_POSIX=1)
    IF(NOT MACOSX)
        ADD_DEFINITIONS(-DLUA_USE_DLOPEN=1)
    ENDIF(NOT MACOSX)
    SET(OPTIONAL_SRCS ${OPTIONAL_SRCS} platform/unix.c)
ENDIF(UNIX)

# See if -fvisibility=hidden is available to us.
IF(CMAKE_COMPILER_IS_GNUCC)
    ADD_DEFINITIONS(-pipe -Wall -Werror -fsigned-char)
    IF(DEBUG_BUILD)
        ADD_DEFINITIONS(-O0 -g)
    ELSE(DEBUG_BUILD)
        ADD_DEFINITIONS(-Os -fno-strict-aliasing -fomit-frame-pointer)
    ENDIF(DEBUG_BUILD)

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

SET(MOJOSETUP_SRCS
    buildver.c
    mojosetup.c
    misc.c
    gui.c
    fileio.c
    archive_zip.c
    lua_glue.c
)

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

# !!! FIXME: Optionally strip out the parser and add-on libraries...
SET(LUA_SRCS
    lua-5.1.1/src/lapi.c
    lua-5.1.1/src/lcode.c
    lua-5.1.1/src/ldebug.c
    lua-5.1.1/src/ldo.c
    lua-5.1.1/src/ldump.c
    lua-5.1.1/src/lfunc.c
    lua-5.1.1/src/lgc.c
    lua-5.1.1/src/llex.c
    lua-5.1.1/src/lmem.c
    lua-5.1.1/src/lobject.c
    lua-5.1.1/src/lopcodes.c
    lua-5.1.1/src/lparser.c
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
            TARGET_LINK_LIBRARIES(macosx "-framework Carbon")
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

IF(UNIX AND NOT MACOSX)
    CHECK_LIBRARY_EXISTS("dl" "dlopen" "" HAVE_LIBDL)
    IF(HAVE_LIBDL)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} dl)
    ENDIF(HAVE_LIBDL)
    CHECK_LIBRARY_EXISTS("m" "sin" "" HAVE_LIBM)
    IF(HAVE_LIBM)
        SET(OPTIONAL_LIBS ${OPTIONAL_LIBS} m)
    ENDIF(HAVE_LIBM)
ENDIF(UNIX AND NOT MACOSX)


ADD_EXECUTABLE(mojosetup ${MOJOSETUP_SRCS} ${LUA_SRCS} ${OPTIONAL_SRCS})
TARGET_LINK_LIBRARIES(mojosetup ${OPTIONAL_LIBS})

# end of CMakeLists.txt ...

