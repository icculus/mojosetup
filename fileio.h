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
 *  done transparently and with arbitrary depth (although usually not more
 *  than one deep. This also works as a general transport layer, so the
 *  abstraction could be extended to network connections and such, too.
 */

/* Abstract input interface. Files, memory, archive entries, etc. */
typedef struct MojoInput MojoInput;
struct MojoInput
{
    /*public*/
    size_t (*read)(MojoInput *io, void *buf, size_t bufsize);
    boolean (*seek)(MojoInput *io, uint64 pos);
    uint64 (*tell)(MojoInput *io);
    void (*close)(MojoInput *io);

    /*private*/
    void *opaque;
};

MojoInput *MojoInput_newFromFile(const char *fname);
MojoInput *MojoInput_newFromMemory(void *mem, size_t bytes);

typedef enum
{
    MOJOARCHIVE_ENTRY_FILE = 0,
    MOJOARCHIVE_ENTRY_DIR,
    MOJOARCHIVE_ENTRY_SYMLINK,
} MojoArchiveEntryType;

/* Abstract archive interface. Archives, directories, etc. */
typedef struct MojoArchiveEntryInfo MojoArchiveEntryInfo;
struct MojoArchiveEntryInfo
{
    const char *filename;
    MojoArchiveEntryType type;
    int64 filesize;
};

typedef struct MojoArchive MojoArchive;
struct MojoArchive 
{
    /*public*/
    void (*restartEnumeration)(MojoArchive *ar);
    const MojoArchiveEntryInfo* (*enumEntry)(MojoArchive *ar);
    MojoInput* (*openCurrentEntry)(MojoArchive *ar);
    void (*close)(MojoArchive *ar);

    /*private*/
    MojoInput *io;
    MojoArchiveEntryInfo lastEnumInfo;
    void *opaque;
};

MojoArchive *MojoArchive_newFromDirectory(const char *dirname);
MojoArchive *MojoArchive_newFromInput(MojoInput *io, const char *origfname);

#ifdef __cplusplus
}
#endif

#endif

/* end of fileio.h ... */

