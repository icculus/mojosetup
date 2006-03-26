#ifndef _INCL_FILEIO_H_
#define _INCL_FILEIO_H_

#include "universal.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SUPPORT_ZIP
#define SUPPORT_ZIP 1
#endif

/*
 * File i/o may go through multiple layers: the archive attached to the binary,
 *  then an archive in there that's being read entirely out of memory that's
 *  being uncompressed to on the fly, or it might be a straight read from a
 *  regular uncompressed file on physical media. It might be a single file
 *  compressed with bzip2. As such, we have to have an abstraction over the
 *  usual channels...basically what we need here is archives-within-archives,
 *  done transparently and with arbitrary depth (although usually not more
 *  than one deep. This also works as a general transport layer, so the
 *  abstraction could be extended to network connections and such, too.
 */

// Abstract input interface. Files, memory, archive entries, etc.
typedef struct MojoInput MojoInput;
struct MojoInput
{
    // public
    int64 (*read)(MojoInput *io, void *buf, uint32 bufsize);
    boolean (*seek)(MojoInput *io, uint64 pos);
    int64 (*tell)(MojoInput *io);
    int64 (*length)(MojoInput *io);
    MojoInput* (*duplicate)(MojoInput *io);
    void (*close)(MojoInput *io);

    // private
    void *opaque;
};

MojoInput *MojoInput_newFromFile(const char *fname);
MojoInput *MojoInput_newFromMemory(void *mem, uint32 bytes);

typedef enum
{
    MOJOARCHIVE_ENTRY_UNKNOWN = 0,
    MOJOARCHIVE_ENTRY_FILE,
    MOJOARCHIVE_ENTRY_DIR,
    MOJOARCHIVE_ENTRY_SYMLINK,
} MojoArchiveEntryType;

// Abstract archive interface. Archives, directories, etc.
typedef struct MojoArchiveEntryInfo MojoArchiveEntryInfo;
struct MojoArchiveEntryInfo
{
    char *filename;
    char *basepath;
    MojoArchiveEntryType type;
    int64 filesize;
};

void MojoArchive_resetEntryInfo(MojoArchiveEntryInfo *info, int basetoo);


typedef struct MojoArchive MojoArchive;
struct MojoArchive 
{
    // public
    boolean (*enumerate)(MojoArchive *ar, const char *path);
    const MojoArchiveEntryInfo* (*enumNext)(MojoArchive *ar);
    MojoInput* (*openCurrentEntry)(MojoArchive *ar);
    void (*close)(MojoArchive *ar);

    // private
    MojoInput *io;
    MojoArchiveEntryInfo prevEnum;
    void *opaque;
};

MojoArchive *MojoArchive_newFromDirectory(const char *dirname);
MojoArchive *MojoArchive_newFromInput(MojoInput *io, const char *origfname);

extern MojoArchive *GBaseArchive;
MojoArchive *MojoArchive_initBaseArchive(void);
void MojoArchive_deinitBaseArchive(void);

boolean mojoInputToPhysicalFile(MojoInput *in, const char *fname);

#ifdef __cplusplus
}
#endif

#endif

// end of fileio.h ...

