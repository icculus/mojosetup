
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
CXX := ccache g++
LD := g++

ifeq ($(iswindows),true)
  EXEEXT := .exe
endif
#ifeq ($(isunix),true)
#  EXEEXT := -bin
#endif

EXE := $(APPID)$(EXEEXT)
EXES := $(EXE)

#OPTS := -O3 -fno-strict-aliasing -falign-loops=16 -ffast-math -fno-math-errno
OPTS := -O0

DEFINES := \
    -DAPPID=$(APPID) \
    -DAPPREV=$(APPREV) \

INCLUDES := \
    -I. \

ifeq ($(isunix),true)
  DEFINES += -DPLATFORM_UNIX=1
endif

ifeq ($(islinux),true)
  DEFINES += -DPLATFORM_LINUX=1
endif

ifeq ($(ismacosx),true)
  DEFINES += -DPLATFORM_MACOSX=1
  LIBS += -framework Carbon
  #OPTS += -mdynamic-no-pic
endif

ifeq ($(iswindows),true)
  DEFINES += -DPLATFORM_WINDOWS=1
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

ZLIBSRCS := \
    $(PHYSFSDIR)/zlib123/adler32.c \
    $(PHYSFSDIR)/zlib123/compress.c \
    $(PHYSFSDIR)/zlib123/crc32.c \
    $(PHYSFSDIR)/zlib123/deflate.c \
    $(PHYSFSDIR)/zlib123/gzio.c \
    $(PHYSFSDIR)/zlib123/infback.c \
    $(PHYSFSDIR)/zlib123/inffast.c \
    $(PHYSFSDIR)/zlib123/inflate.c \
    $(PHYSFSDIR)/zlib123/inftrees.c \
    $(PHYSFSDIR)/zlib123/trees.c \
    $(PHYSFSDIR)/zlib123/uncompr.c \
    $(PHYSFSDIR)/zlib123/zutil.c \

# We force this to build every time, so it can't be an explicit dependency...
LIBS += bin/buildver.o

CFLAGS := -g -pipe -Wall -Werror -fexceptions -fsigned-char $(OPTS) $(INCLUDES) $(DEFINES)
CXXFLAGS := $(CFLAGS)

.PHONY: clean all docs $(APPID)

all: $(APPID)

$(APPID): $(EXE)

docs:
	( cat $(ENGINEDIR)/Doxyfile ; echo 'PROJECT_NUMBER="Build Revision $(APPREV)"' ) | doxygen -

clean:
	rm -rf $(OBJS) $(EXES) bin/$(SDLDIR)/lib/$(target)/libSDLmain.a docs bin/test/*.o bin/tools/*/*.o

bin/%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

bin/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

#$(EXE) : $(OBJS) $(STATICLIBS)
#	$(CXX) $(CXXFLAGS) -c -o bin/$(ENGINEDIR)/buildver.o $(ENGINEDIR)/buildver.cpp
#	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

$(APPID)$(EXEEXT) : $(OBJS) $(STATICLIBS)
	$(CXX) $(CXXFLAGS) -c -o bin/$(ENGINEDIR)/buildver.o $(ENGINEDIR)/buildver.cpp
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

# end of makefile ...
