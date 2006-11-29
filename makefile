support_zip := true
use_internal_zlib := false

# Generally you shouldn't touch anything below here.  :)

APPID := mojosetup
APPREV := $(shell head -n 1 BUILD_REVISION.txt)
ifeq ($(strip $(APPREV)),)
  $(error Cannot determine build revision. Does BUILD_REVISION.txt exist?)
endif

#target := macosx_ppc
#target := macosx_x86
#target := linux_x86
#target := linux_ppc

ifeq ($(strip $(target)),)
  arch := $(shell uname -m)
  os := $(shell uname -s)

  ifeq ($(strip $(os)),Darwin)
    ifeq ($(strip $(arch)),Power Macintosh)
      target := macosx_ppc
    endif
    ifeq ($(strip $(arch)),x86)
      target := macosx_x86
    endif
    ifeq ($(strip $(arch)),i386)
      target := macosx_x86
    endif
  endif

  ifeq ($(strip $(os)),Linux)
    ifeq ($(strip $(arch)),i686)
     target := linux_x86
    endif
    ifeq ($(strip $(arch)),x86_64)
     target := linux_x86
    endif
    ifeq ($(strip $(arch)),amd64)
     target := linux_x86
    endif
    ifeq ($(strip $(arch)),ppc)
     target := linux_ppc
    endif
  endif
endif


#----------------------------------------------------------------------

settarget := false
ifeq ($(strip $(target)),macosx_ppc)
  isunix := true
  islinux := false
  ismacosx := true
  iswindows := false
  is32bit := true
  is64bit := false
  isbigendian := true
  settarget := true
endif
ifeq ($(strip $(target)),macosx_x86)
  isunix := true
  islinux := false
  ismacosx := true
  iswindows := false
  is32bit := true
  is64bit := false
  isbigendian := false
  settarget := true
endif
ifeq ($(strip $(target)),linux_x86)
  isunix := true
  islinux := true
  ismacosx := false
  iswindows := false
  is32bit := true
  is64bit := false
  isbigendian := false
  settarget := true
endif
ifeq ($(strip $(target)),linux_ppc)
  isunix := true
  islinux := true
  ismacosx := false
  iswindows := false
  is32bit := true
  is64bit := false
  isbigendian := true
  settarget := true
endif

ifneq ($(strip $(settarget)),true)
  $(error unknown target.)
endif

CC := ccache gcc
LD := gcc

OPTS := -Os -fno-strict-aliasing -fomit-frame-pointer
#OPTS := -O0

DEFINES := \
    -DAPPID=$(APPID) \
    -DAPPREV=$(APPREV) \

INCLUDES := \
    -I. \

SRCS := \
    mojosetup.c \
    misc.c \
    gui.c \
    fileio.c \
    archive_zip.c \
    gui/gui_stdio.c \

# !!! FIXME: Optionally strip out the parser and add-on libraries...
LUASRCS := \
    lua-5.1.1/src/lapi.c \
    lua-5.1.1/src/lcode.c \
    lua-5.1.1/src/ldebug.c \
    lua-5.1.1/src/ldo.c \
    lua-5.1.1/src/ldump.c \
    lua-5.1.1/src/lfunc.c \
    lua-5.1.1/src/lgc.c \
    lua-5.1.1/src/llex.c \
    lua-5.1.1/src/lmem.c \
    lua-5.1.1/src/lobject.c \
    lua-5.1.1/src/lopcodes.c \
    lua-5.1.1/src/lparser.c \
    lua-5.1.1/src/lstate.c \
    lua-5.1.1/src/lstring.c \
    lua-5.1.1/src/ltable.c \
    lua-5.1.1/src/ltm.c \
    lua-5.1.1/src/lundump.c \
    lua-5.1.1/src/lvm.c \
    lua-5.1.1/src/lzio.c \
    lua-5.1.1/src/lauxlib.c \
    lua-5.1.1/src/lbaselib.c \
    lua-5.1.1/src/ldblib.c \
    lua-5.1.1/src/liolib.c \
    lua-5.1.1/src/lmathlib.c \
    lua-5.1.1/src/loslib.c \
    lua-5.1.1/src/ltablib.c \
    lua-5.1.1/src/lstrlib.c \
    lua-5.1.1/src/loadlib.c \
    lua-5.1.1/src/linit.c \

ZLIBSRCS := \
    zlib123/adler32.c \
    zlib123/compress.c \
    zlib123/crc32.c \
    zlib123/deflate.c \
    zlib123/inffast.c \
    zlib123/inflate.c \
    zlib123/inftrees.c \
    zlib123/trees.c \
    zlib123/uncompr.c \
    zlib123/zutil.c \

GUIPLUGINS := \
    gui_stdio \

SRCS += $(LUASRCS)

needzlib := false
ifeq ($(support_zip),true)
    needzlib := true
endif

ifeq ($(needzlib),true)
  	ifeq ($(use_internal_zlib),true)
    	DEFINES += -DZ_PREFIX=1
    	SRCS += $(ZLIBSRCS)
    else
    	LDFLAGS += -lz
    endif
endif

ifeq ($(isunix),true)
  DEFINES += -DPLATFORM_UNIX=1
  DEFINES += -DLUA_USE_POSIX=1
  SRCS += platform/unix.c
endif

ifeq ($(islinux),true)
  DEFINES += -DPLATFORM_LINUX=1
  DEFINES += -DLUA_USE_DLOPEN=1
  LIBS += -ldl -lm
  EXEEXT := -bin
  DLLEXT := .so
  SHARED_LDFLAGS += -shared -fPIC
endif

ifeq ($(ismacosx),true)
  DEFINES += -DPLATFORM_MACOSX=1
  DEFINES += -DLUA_DL_DYLD=1
  LIBS += -framework Carbon
  EXEEXT := -bin
  DLLEXT := .dylib
  SHARED_LDFLAGS += -dynamiclib -fPIC
endif

ifeq ($(iswindows),true)
  DEFINES += -DPLATFORM_WINDOWS=1
  EXEEXT := .exe
  DLLEXT := .dll
endif

ifeq ($(is32bit),true)
  DEFINES += -DPLATFORM_32BITS=1
endif

ifeq ($(is64bit),true)
  ifeq ($(is32bit),true)
    $(error is64bit and is32bit both defined)
  endif
  DEFINES += -DPLATFORM_64BITS=1
endif

ifeq ($(isbigendian),true)
  DEFINES += -DPLATFORM_BIGENDIAN=1
else
  DEFINES += -DPLATFORM_LITTLEENDIAN=1
endif

EXE := bin/$(APPID)$(EXEEXT)
EXES := $(EXE)

# We force this to build every time, so it can't be an explicit dependency...
LIBS += bin/buildver.o

#CFLAGS += -fvisibility=hidden
CFLAGS += -g -pipe -Wall -Werror -fsigned-char $(OPTS) $(INCLUDES) $(DEFINES)
CXXFLAGS += $(CFLAGS)
SHARED_CFLAGS := $(CFLAGS) -fPIC -DPIC
SHARED_CXXFLAGS := $(SHARED_CFLAGS)

ifeq ($(ismacosx),true)
  CFLAGS += -mdynamic-no-pic
  CXXFLAGS += -mdynamic-no-pic
endif

OBJS := $(SRCS)
OBJS := $(OBJS:.cpp=.o)
OBJS := $(OBJS:.c=.o)
OBJS := $(OBJS:.m=.o)
OBJS := $(foreach f,$(OBJS),bin/$(f))

.PHONY: clean all docs

all: $(EXE) $(GUIPLUGINS)

docs:
	( cat $(ENGINEDIR)/Doxyfile ; echo 'PROJECT_NUMBER="Build Revision $(APPREV)"' ) | doxygen -

clean:
	rm -rf bin docs

.PHONY: gui_stdio
gui_stdio: bin/gui/stdio$(DLLEXT)

bin/gui/stdio$(DLLEXT): bin/gui/gui_stdio.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o bin/buildver.o buildver.c
	$(LD) $(SHARED_LDFLAGS) -o $@ $^ $(LIBS)

bin/gui/%.o : gui/%.c
	@mkdir -p $(dir $@)
	$(CC) $(SHARED_CFLAGS) -c -o $@ $<

bin/%.o : %.cpp
	@echo "Don't add C++ sources to this project! C only!"
	@exit 1

bin/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXE) : $(OBJS) $(STATICLIBS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o bin/buildver.o buildver.c
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

# end of makefile ...

