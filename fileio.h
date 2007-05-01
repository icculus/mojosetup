#ifndef _INCL_FILEIO_H_
#define _INCL_FILEIO_H_

#include "universal.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * File i/o may go through multiple layers: the archive attached to the binary,
 *  then an archive in there that's being read entirely out of memory that's
 *  being uncompressed to on the fly, or it might be a straight read from a
 *  regular uncompressed file on physical media. It might be a single file
 *  compressed with bzip2. As such, we have to have an abstraction over the
 *  usual channels...basically what we need here is archives-within-archives,
 *  done transparently and with arbitrary depth, although usually not more
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
typedef struct MojoArchiveEntry
{
    char *filename;
    char *basepath;
    char *linkdest;
    MojoArchiveEntryType type;
    int64 filesize;
} MojoArchiveEntry;

void MojoArchive_resetEntry(MojoArchiveEntry *info, int basetoo);


typedef struct MojoArchive MojoArchive;
struct MojoArchive 
{
    // public
    boolean (*enumerate)(MojoArchive *ar, const char *path);
    const MojoArchiveEntry* (*enumNext)(MojoArchive *ar);
    MojoInput* (*openCurrentEntry)(MojoArchive *ar);
    void (*close)(MojoArchive *ar);

    // private
    MojoInput *io;
    MojoArchiveEntry prevEnum;
    void *opaque;
};

MojoArchive *MojoArchive_newFromDirectory(const char *dirname);
MojoArchive *MojoArchive_newFromInput(MojoInput *io, const char *origfname);

// This will reset enumeration in the archive, don't use it while iterating!
// Also, this can be very slow depending on the archive in question, so
//  try to limit your random access filename lookups to known-fast quantities
//  (like directories on the physical filesystem or a zipfile...tarballs and
//  zipfiles-in-zipfiles will bog down here, for example).
MojoInput *MojoInput_newFromArchivePath(MojoArchive *ar, const char *fname);

extern MojoArchive *GBaseArchive;
MojoArchive *MojoArchive_initBaseArchive(void);
void MojoArchive_deinitBaseArchive(void);

typedef boolean (*MojoInput_FileCopyCallback)(int percent, void *data);
boolean MojoInput_toPhysicalFile(MojoInput *in, const char *fname,
                                 MojoInput_FileCopyCallback cb, void *data);

#ifdef __cplusplus
}
#endif

#endif

// end of fileio.h ...

