#include "fileio.h"
#include "platform.h"

// !!! FIXME: don't have this here. (need unlink for now).
#include <unistd.h>

boolean mojoInputToPhysicalFile(MojoInput *in, const char *fname)
{
    FILE *out = NULL;
    boolean iofailure = false;
    uint32 br;

    STUBBED("mkdir first?");
    STUBBED("file permissions?");

    if (in == NULL)
        return false;

    STUBBED("fopen?");
    unlink(fname);
    out = fopen(fname, "wb");
    if (out == NULL)
        return false;

    while (!iofailure)
    {
        br = in->read(in, scratchbuf_128k, sizeof (scratchbuf_128k));
        STUBBED("how to detect read failures?");
        if (br == 0)  // we're done!
            break;
        else if (br < 0)
            iofailure = true;
        else
        {
            if (fwrite(scratchbuf_128k, br, 1, out) != 1)
                iofailure = true;
        } // else
    } // while

    fclose(out);
    if (iofailure)
    {
        unlink(fname);
        return false;
    } // if

    return true;
} // mojoInputToPhysicalFile


// MojoInputs from files on the OS filesystem.

static uint32 MojoInput_file_read(MojoInput *io, void *buf, uint32 bufsize)
{
    return (uint32) fread(buf, 1, bufsize, (FILE *) io->opaque);
} // MojoInput_file_read

static boolean MojoInput_file_seek(MojoInput *io, uint64 pos)
{
#if 0
    FILE *f = (FILE *) io->opaque;
    rewind(f);
    while (pos)
    {
        // do in a loop to make sure we seek correctly in > 2 gig files.
        if (fseek(f, (long) (pos & 0x7FFFFFFF), SEEK_CUR) == -1)
            return false;
        pos -= (pos & 0x7FFFFFFF);
    } // while
#else
    return (fseeko((FILE *) io->opaque, pos, SEEK_SET) == 0);
#endif
} // MojoInput_file_seek

static uint64 MojoInput_file_tell(MojoInput *io)
{
    return ftello((FILE *) io->opaque);
} // MojoInput_file_tell

static void MojoInput_file_close(MojoInput *io)
{
    fclose((FILE *) io->opaque);
    free(io);
} // MojoInput_file_close

MojoInput *MojoInput_newFromFile(const char *fname)
{
    FILE *f = fopen(fname, "rb");
    if (f == NULL)
        return NULL;

    MojoInput *io = (MojoInput *) xmalloc(sizeof (MojoInput));
    io->read = MojoInput_file_read;
    io->seek = MojoInput_file_seek;
    io->tell = MojoInput_file_tell;
    io->close = MojoInput_file_close;
    io->opaque = f;
    return io;
} // MojoInput_newFromFile



// MojoInputs from blocks of memory.

typedef struct
{
    uint8 *mem;
    uint32 bytes;
    uint32 pos;
} MojoInputMemInstance;

static uint32 MojoInput_memory_read(MojoInput *io, void *buf, uint32 bufsize)
{
    MojoInputMemInstance *inst = (MojoInputMemInstance *) io->opaque;
    uint32 left = inst->bytes - inst->pos;
    if (bufsize > left)
        bufsize = left;

    if (bufsize)
    {
        memcpy(buf, inst->mem + inst->pos, bufsize);
        inst->pos += bufsize;
    } // if
    return bufsize;
} // MojoInput_memory_read

static boolean MojoInput_memory_seek(MojoInput *io, uint64 pos)
{
    MojoInputMemInstance *inst = (MojoInputMemInstance *) io->opaque;
    if (pos > (inst->bytes))
        return false;
    inst->pos = pos;
    return true;
} // MojoInput_memory_seek

static uint64 MojoInput_memory_tell(MojoInput *io)
{
    MojoInputMemInstance *inst = (MojoInputMemInstance *) io->opaque;
    return inst->pos;
} // MojoInput_memory_tell

static void MojoInput_memory_close(MojoInput *io)
{
    free(io->opaque);
    free(io);
} // MojoInput_memory_close

MojoInput *MojoInput_newFromMemory(void *mem, uint32 bytes)
{
    MojoInputMemInstance *inst;
    inst = (MojoInputMemInstance *) xmalloc(sizeof (MojoInputMemInstance));
    inst->mem = mem;
    inst->bytes = bytes;

    MojoInput *io = (MojoInput *) xmalloc(sizeof (MojoInput));
    io->read = MojoInput_memory_read;
    io->seek = MojoInput_memory_seek;
    io->tell = MojoInput_memory_tell;
    io->close = MojoInput_memory_close;
    io->opaque = inst;
    return io;
} // MojoInput_newFromMemory



// MojoArchives from directories on the OS filesystem.

// !!! FIXME: abstract the unixy bits into the platform/ dir.
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <dirent.h>

typedef struct
{
    DIR *dir;
    char *base;
    char *path;
} MojoArchiveDirInstance;

static boolean MojoArchive_dir_enumerate(MojoArchive *ar, const char *path)
{
    char *fullpath = NULL;
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;
    if (inst->dir != NULL)
        closedir(inst->dir);

    free(ar->lastEnumInfo.filename);
    ar->lastEnumInfo.filename = NULL;
    ar->lastEnumInfo.type = MOJOARCHIVE_ENTRY_UNKNOWN;
    ar->lastEnumInfo.filesize = 0;

    free(inst->path);
    fullpath = (char *) alloca(strlen(inst->base) + strlen(path) + 2);
    sprintf(fullpath, "%s/%s", inst->base, path);
    inst->path = xstrdup(fullpath);
    inst->dir = opendir(fullpath);
    return (inst->dir != NULL);
} // MojoArchive_dir_enumerate


static const MojoArchiveEntryInfo *MojoArchive_dir_enumNext(MojoArchive *ar)
{
    struct stat statbuf;
    char *fullpath = NULL;
    struct dirent *dent;
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;
    if (inst->dir == NULL)
        return NULL;

    free(ar->lastEnumInfo.filename);
    ar->lastEnumInfo.filename = NULL;
    ar->lastEnumInfo.type = MOJOARCHIVE_ENTRY_UNKNOWN;
    ar->lastEnumInfo.filesize = 0;

    dent = readdir(inst->dir);
    if (dent == NULL)  // end of dir?
    {
        closedir(inst->dir);
        inst->dir = NULL;
        return NULL;
    } // if

    if ((strcmp(dent->d_name, ".") == 0) || (strcmp(dent->d_name, "..") == 0))
        return MojoArchive_dir_enumNext(ar);

    fullpath = (char *) alloca(strlen(inst->path) + strlen(dent->d_name) + 2);
    sprintf(fullpath, "%s/%s", inst->path, dent->d_name);
    if (stat(fullpath, &statbuf) != -1)
    {
        ar->lastEnumInfo.filesize = statbuf.st_size;
        if (S_ISDIR(statbuf.st_mode))
            ar->lastEnumInfo.type = MOJOARCHIVE_ENTRY_DIR;
        else if (S_ISREG(statbuf.st_mode))
            ar->lastEnumInfo.type = MOJOARCHIVE_ENTRY_FILE;
        else if (S_ISLNK(statbuf.st_mode))
            ar->lastEnumInfo.type = MOJOARCHIVE_ENTRY_SYMLINK;
    } // if

    ar->lastEnumInfo.filename = xstrdup(dent->d_name);
    return &ar->lastEnumInfo;
} // MojoArchive_dir_enumNext


static MojoInput *MojoArchive_dir_openCurrentEntry(MojoArchive *ar)
{
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;
    char *fullpath = (char *) alloca(strlen(inst->path) +
                                     strlen(ar->lastEnumInfo.filename) + 2);
    sprintf(fullpath, "%s/%s", inst->path, ar->lastEnumInfo.filename);
    return MojoInput_newFromFile(fullpath);
} // MojoArchive_dir_openCurrentEntry


static void MojoArchive_dir_close(MojoArchive *ar)
{
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;
    if (inst->dir != NULL)
        closedir(inst->dir);
    free(inst->base);
    free(inst->path);
    free(ar->lastEnumInfo.filename);
    free(ar->opaque);
    free(ar);
} // MojoArchive_dir_close


MojoArchive *MojoArchive_newFromDirectory(const char *dirname)
{
    char resolved[PATH_MAX];
    MojoArchiveDirInstance *inst;
    if (realpath(dirname, resolved) == NULL)
        return NULL;

    inst = (MojoArchiveDirInstance *) xmalloc(sizeof (MojoArchiveDirInstance));
    inst->base = xstrdup(resolved);
    MojoArchive *ar = (MojoArchive *) xmalloc(sizeof (MojoArchive));
    ar->enumerate = MojoArchive_dir_enumerate;
    ar->enumNext = MojoArchive_dir_enumNext;
    ar->openCurrentEntry = MojoArchive_dir_openCurrentEntry;
    ar->close = MojoArchive_dir_close;
    ar->opaque = inst;
    return ar;
} // MojoArchive_newFromDirectory



MojoArchive *GBaseArchive = NULL;

MojoArchive *MojoArchive_initBaseArchive(void)
{
    if (GBaseArchive != NULL)
        return GBaseArchive;
    else
    {
        const char *basepath = MojoPlatform_appBinaryPath();
        MojoInput *io = MojoInput_newFromFile(basepath);

        STUBBED("chdir to path of binary");

        if (io != NULL)
            GBaseArchive = MojoArchive_newFromInput(io, basepath);

        if (GBaseArchive == NULL)
            GBaseArchive = MojoArchive_newFromDirectory(".");
    } // else

    return GBaseArchive;
} // MojoArchive_initBaseArchive


void MojoArchive_deinitBaseArchive(void)
{
    if (GBaseArchive != NULL)
    {
        GBaseArchive->close(GBaseArchive);
        GBaseArchive = NULL;
    } // if
} // MojoArchive_deinitBaseArchive

// end of fileio.c ...

