/**
 * MojoSetup; a portable, flexible installation application.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Steffen Pankratz.
 */

#include "fileio.h"
#include "platform.h"

#if !SUPPORT_PCK
MojoArchive *MojoArchive_createPCK(MojoInput *io) { return NULL; }
#else

#define PCK_MAGIC 0x534c4850

typedef struct
{
    uint32 Magic;               // 4 bytes, has to be 0x534c4850
    uint32 StartOfBinaryData;   // 4 bytes, offset to the data
}PCK_HEADER;

typedef struct
{
    int8  filename[60];         // 60 bytes, null terminated
    uint32 filesize;            //  4 bytes
}PCK_FILE_ENTRY;

typedef struct PCKinfo
{
    uint64 fileCount;
    uint64 dataStart;
    uint64 nextFileStart;
    int64 nextEnumPos;
    MojoArchiveEntry *archiveEntires;
} PCKinfo;

static boolean MojoInput_pck_ready(MojoInput *io)
{
    return true;  // !!! FIXME?
} // MojoInput_pck_ready

static int64 MojoInput_pck_read(MojoInput *io, void *buf, uint32 bufsize)
{
    MojoArchive *ar = (MojoArchive *) io->opaque;
    PCKinfo *info = (PCKinfo *) ar->opaque;
    int64 pos = io->tell(io);
    if ((pos + bufsize) > info->archiveEntires[info->nextEnumPos].filesize)
        bufsize = (uint32) (info->archiveEntires[info->nextEnumPos].filesize - pos);
    return ar->io->read(ar->io, buf, bufsize);
} // MojoInput_pck_read

static boolean MojoInput_pck_seek(MojoInput *io, uint64 pos)
{
    MojoArchive *ar = (MojoArchive *) io->opaque;
    PCKinfo *info = (PCKinfo *) ar->opaque;
    boolean retval = false;
    if (pos < ((uint64) info->archiveEntires[info->nextEnumPos].filesize))
        retval = ar->io->seek(ar->io, (info->nextFileStart - info->archiveEntires[info->nextEnumPos].filesize) + pos);
    return retval;
} // MojoInput_pck_seek

static int64 MojoInput_pck_tell(MojoInput *io)
{
    MojoArchive *ar = (MojoArchive *) io->opaque;
    PCKinfo *info = (PCKinfo *) ar->opaque;
    return ar->io->tell(ar->io) - (info->nextFileStart - info->archiveEntires[info->nextEnumPos].filesize);
} // MojoInput_pck_tell

static int64 MojoInput_pck_length(MojoInput *io)
{
    MojoArchive *ar = (MojoArchive *) io->opaque;
    PCKinfo *info = (PCKinfo *) ar->opaque;
    return info->archiveEntires[info->nextEnumPos].filesize;
} // MojoInput_pck_length

static MojoInput *MojoInput_pck_duplicate(MojoInput *io)
{
    MojoInput *retval = NULL;
    fatal(_("BUG: Can't duplicate pck inputs"));  // !!! FIXME: why not?
    return retval;
} // MojoInput_pck_duplicate

static void MojoInput_pck_close(MojoInput *io)
{
    free(io);
} // MojoInput_pck_close

// MojoArchive implementation...

static boolean MojoArchive_pck_enumerate(MojoArchive *ar)
{
    MojoArchive_resetEntry(&ar->prevEnum);

    PCKinfo *info = (PCKinfo *) ar->opaque;

    int dataStart = info->dataStart;
    int fileCount = dataStart / sizeof(PCK_FILE_ENTRY);

    MojoArchiveEntry *archiveEntries = xmalloc(fileCount *  sizeof(MojoArchiveEntry));
    PCK_FILE_ENTRY fileEntry;
    uint64 i, realFileCount = 0;
    char directory[256] = {'\0'};

    for(i = 0; i < fileCount; i++)
    {
        ar->io->read(ar->io, &fileEntry, sizeof(PCK_FILE_ENTRY));

        if(strcmp(fileEntry.filename, "..") != 0 && fileEntry.filesize == 0x80000000)
        {
            strcat(directory, fileEntry.filename);
            strcat(directory, "/");

            archiveEntries[realFileCount].filename = xstrdup(directory);
            archiveEntries[realFileCount].type = MOJOARCHIVE_ENTRY_DIR;
            archiveEntries[realFileCount].perms = MojoPlatform_defaultDirPerms();
            archiveEntries[realFileCount].filesize = 0L;
            realFileCount++;
        }
        else if(strcmp(fileEntry.filename, "..") == 0 && fileEntry.filesize == 0x80000000)
        {
            //remove trailing path separator
            size_t strLength = strlen(directory);
            directory[strLength - 1] = '\0';

            char *pathSep = strrchr(directory, '/');

            if(pathSep != NULL)
            {
                pathSep++;
                *pathSep = '\0';
            }
        }
        else
        {
            if(directory[0] == '\0')
            {
                archiveEntries[realFileCount].filename = xstrdup(fileEntry.filename);
            }
            else
            {
                archiveEntries[realFileCount].filename = (char *)xmalloc(sizeof(char) * (strlen(directory) + strlen(fileEntry.filename) + 1));
                strcat(archiveEntries[realFileCount].filename, directory);
                strcat(archiveEntries[realFileCount].filename, fileEntry.filename);
            }

            archiveEntries[realFileCount].perms = MojoPlatform_defaultFilePerms();
            archiveEntries[realFileCount].type = MOJOARCHIVE_ENTRY_FILE;
            archiveEntries[realFileCount].filesize = fileEntry.filesize;
            realFileCount++;
        }
    }

    info->fileCount = realFileCount;
    info->archiveEntires = archiveEntries;
    info->nextEnumPos = -1;
    info->nextFileStart = dataStart;

    return true;
} // MojoArchive_pck_enumerate


static const MojoArchiveEntry *MojoArchive_pck_enumNext(MojoArchive *ar)
{
    PCKinfo *info = (PCKinfo *) ar->opaque;

    if (info->nextEnumPos + 1 >= info->fileCount)
        return NULL;

    if (info->nextEnumPos > 0 && !ar->io->seek(ar->io, info->nextFileStart))
       return NULL;

    info->nextEnumPos++;
    info->nextFileStart += info->archiveEntires[info->nextEnumPos].filesize;

    ar->prevEnum = info->archiveEntires[info->nextEnumPos];

    return &ar->prevEnum;
} // MojoArchive_pck_enumNext


static MojoInput *MojoArchive_pck_openCurrentEntry(MojoArchive *ar)
{
    MojoInput *io = NULL;
    io = (MojoInput *) xmalloc(sizeof (MojoInput));
    io->ready = MojoInput_pck_ready;
    io->read = MojoInput_pck_read;
    io->seek = MojoInput_pck_seek;
    io->tell = MojoInput_pck_tell;
    io->length = MojoInput_pck_length;
    io->duplicate = MojoInput_pck_duplicate;
    io->close = MojoInput_pck_close;
    io->opaque = ar;
    return io;
} // MojoArchive_pck_openCurrentEntry


static void MojoArchive_pck_close(MojoArchive *ar)
{
    int i;
    PCKinfo *info = (PCKinfo *) ar->opaque;
    ar->io->close(ar->io);

    for(i = 0; i < info->fileCount; i++)
    {
        MojoArchiveEntry entry = info->archiveEntires[i];
        free(entry.filename);
    }

    free(info->archiveEntires);
    free(info);
    free(ar);
} // MojoArchive_pck_close

MojoArchive *MojoArchive_createPCK(MojoInput *io, const char *origfname, const char *origfname23)
{
    MojoArchive *ar = NULL;

    PCK_HEADER pckHeader;

    const int64 br = io->read(io, &pckHeader, sizeof (PCK_HEADER));

    // Check if this is a *.pck file.
    if ( (br != sizeof (PCK_HEADER)) || (pckHeader.Magic != PCK_MAGIC) )
        return NULL;

    PCKinfo *pckInfo = (PCKinfo *) xmalloc(sizeof (PCKinfo));
    pckInfo->dataStart = pckHeader.StartOfBinaryData + sizeof(PCK_HEADER);

    ar = (MojoArchive *) xmalloc(sizeof (MojoArchive));
    ar->opaque = pckInfo;
    ar->enumerate = MojoArchive_pck_enumerate;
    ar->enumNext = MojoArchive_pck_enumNext;
    ar->openCurrentEntry = MojoArchive_pck_openCurrentEntry;
    ar->close = MojoArchive_pck_close;
    ar->io = io;

    return ar;
} // MojoArchive_createPCK

#endif // SUPPORT_PCK

// end of archive_pck.c ...
