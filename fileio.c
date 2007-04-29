#include <sys/stat.h>  // !!! FIXME: unix dependency for stat().

#include "fileio.h"
#include "platform.h"

#ifdef alloca
#undef alloca
#endif
#define alloca xmalloc

typedef MojoArchive* (*MojoArchiveCreateEntryPoint)(MojoInput *io);

MojoArchive *MojoArchive_createZIP(MojoInput *io);

typedef struct
{
    const char *ext;
    MojoArchiveCreateEntryPoint create;
} MojoArchiveType;

static const MojoArchiveType archives[] =
{
    { "zip", MojoArchive_createZIP },
};

MojoArchive *MojoArchive_newFromInput(MojoInput *io, const char *origfname)
{
    int i;
    MojoArchive *retval = NULL;
    const char *ext = ((origfname != NULL) ? strrchr(origfname, '.') : NULL);
    if (ext != NULL)
    {
        ext++;  // skip '.'

        // Try for an exact match.
        for (i = 0; i < STATICARRAYLEN(archives); i++)
        {
            if (strcasecmp(ext, archives[i].ext) == 0)
                return archives[i].create(io);
        } // for
    } // if

    // Try them all...
    for (i = 0; i < STATICARRAYLEN(archives); i++)
    {
        if ((retval = archives[i].create(io)) != NULL)
            return retval;
    } // for

    io->close(io);
    return NULL;  // nothing can handle this data.
} // MojoArchive_newFromInput


void MojoArchive_resetEntry(MojoArchiveEntry *info, int basetoo)
{
    char *base = info->basepath;
    free(info->filename);
    memset(info, '\0', sizeof (MojoArchiveEntry));

    if (basetoo)
        free(base);
    else
        info->basepath = base;
} // MojoArchive_resetEntry



boolean MojoInput_toPhysicalFile(MojoInput *in, const char *fname)
{
    FILE *out = NULL;
    boolean iofailure = false;
    int32 br = 0;

    STUBBED("mkdir first?");
    STUBBED("file permissions?");

    if (in == NULL)
        return false;

    STUBBED("fopen?");
    MojoPlatform_unlink(fname);
    out = fopen(fname, "wb");
    if (out == NULL)
        return false;

    while (!iofailure)
    {
        br = (int32) in->read(in, scratchbuf_128k, sizeof (scratchbuf_128k));
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
        MojoPlatform_unlink(fname);
        return false;
    } // if

    return true;
} // MojoInput_toPhysicalFile


MojoInput *MojoInput_newFromArchivePath(MojoArchive *ar, const char *fname)
{
    MojoInput *retval = NULL;
    char *fullpath = xstrdup(fname);
    char *file = strrchr(fullpath, '/');
    char *path = fullpath;
    if (file != NULL)
        *(file++) = '\0';
    else
    {
        path = "";
        file = fullpath;
    } // else

    if (ar->enumerate(ar, path))
    {
        const MojoArchiveEntry *entinfo;
        while ((entinfo = ar->enumNext(ar)) != NULL)
        {
            if (strcmp(entinfo->filename, file) == 0)
            {
                if (entinfo->type == MOJOARCHIVE_ENTRY_FILE)
                    retval = ar->openCurrentEntry(ar);
                break;
            } // if
        } // while
    } // if

    free(fullpath);
    return retval;
} // MojoInput_newFromArchivePath



// MojoInputs from files on the OS filesystem.

typedef struct
{
    FILE *handle;
    char *path;
} MojoInputFileInstance;

static int64 MojoInput_file_read(MojoInput *io, void *buf, uint32 bufsize)
{
    MojoInputFileInstance *inst = (MojoInputFileInstance *) io->opaque;
    return (int64) fread(buf, 1, bufsize, inst->handle);
} // MojoInput_file_read

static boolean MojoInput_file_seek(MojoInput *io, uint64 pos)
{
    MojoInputFileInstance *inst = (MojoInputFileInstance *) io->opaque;
#if 1
    rewind(inst->handle);
    while (pos)
    {
        // do in a loop to make sure we seek correctly in > 2 gig files.
        if (fseek(inst->handle, (long) (pos & 0x7FFFFFFF), SEEK_CUR) == -1)
            return false;
        pos -= (pos & 0x7FFFFFFF);
    } // while
    return true;
#else
    return (fseeko(inst->handle, pos, SEEK_SET) == 0);
#endif
} // MojoInput_file_seek

static int64 MojoInput_file_tell(MojoInput *io)
{
    MojoInputFileInstance *inst = (MojoInputFileInstance *) io->opaque;
//    return (int64) ftello(inst->handle);
    STUBBED("ftell is 32 bit!\n");
    return (int64) ftell(inst->handle);
} // MojoInput_file_tell

static int64 MojoInput_file_length(MojoInput *io)
{
    MojoInputFileInstance *inst = (MojoInputFileInstance *) io->opaque;
    int fd = fileno(inst->handle);
    struct stat statbuf;
    if ((fd == -1) || (fstat(fd, &statbuf) == -1))
        return -1;
    return((int64) statbuf.st_size);
} // MojoInput_file_length

static MojoInput *MojoInput_file_duplicate(MojoInput *io)
{
    MojoInputFileInstance *inst = (MojoInputFileInstance *) io->opaque;
    return MojoInput_newFromFile(inst->path);
} // MojoInput_file_duplicate

static void MojoInput_file_close(MojoInput *io)
{
    MojoInputFileInstance *inst = (MojoInputFileInstance *) io->opaque;
    fclose(inst->handle);
    free(inst->path);
    free(inst);
    free(io);
} // MojoInput_file_close

MojoInput *MojoInput_newFromFile(const char *fname)
{
    FILE *f = NULL;
    MojoInput *io = NULL;
    MojoInputFileInstance *inst;
    char *path = MojoPlatform_realpath(fname);
    
    if (path == NULL)
        return NULL;

    f = fopen(path, "rb");
    if (f == NULL)
    {
        free(path);
        return NULL;
    } // if

    inst = (MojoInputFileInstance *) xmalloc(sizeof (MojoInputFileInstance));
    inst->path = path;
    inst->handle = f;

    io = (MojoInput *) xmalloc(sizeof (MojoInput));
    io->read = MojoInput_file_read;
    io->seek = MojoInput_file_seek;
    io->tell = MojoInput_file_tell;
    io->length = MojoInput_file_length;
    io->duplicate = MojoInput_file_duplicate;
    io->close = MojoInput_file_close;
    io->opaque = inst;
    return io;
} // MojoInput_newFromFile



// MojoInputs from blocks of memory.

typedef struct
{
    uint8 *mem;
    uint32 bytes;
    uint32 pos;
} MojoInputMemInstance;

static int64 MojoInput_memory_read(MojoInput *io, void *buf, uint32 bufsize)
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

static int64 MojoInput_memory_tell(MojoInput *io)
{
    MojoInputMemInstance *inst = (MojoInputMemInstance *) io->opaque;
    return inst->pos;
} // MojoInput_memory_tell

static int64 MojoInput_memory_length(MojoInput *io)
{
    MojoInputMemInstance *inst = (MojoInputMemInstance *) io->opaque;
    return(inst->bytes);
} // MojoInput_memory_length

static MojoInput *MojoInput_memory_duplicate(MojoInput *io)
{
    MojoInputMemInstance *inst = (MojoInputMemInstance *) io->opaque;
    return MojoInput_newFromMemory(inst->mem, inst->bytes);
} // MojoInput_memory_duplicate

static void MojoInput_memory_close(MojoInput *io)
{
    free(io->opaque);
    free(io);
} // MojoInput_memory_close

MojoInput *MojoInput_newFromMemory(void *mem, uint32 bytes)
{
    MojoInput *io = NULL;
    MojoInputMemInstance *inst;
    inst = (MojoInputMemInstance *) xmalloc(sizeof (MojoInputMemInstance));
    inst->mem = mem;
    inst->bytes = bytes;

    io = (MojoInput *) xmalloc(sizeof (MojoInput));
    io->read = MojoInput_memory_read;
    io->seek = MojoInput_memory_seek;
    io->tell = MojoInput_memory_tell;
    io->length = MojoInput_memory_length;
    io->duplicate = MojoInput_memory_duplicate;
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

typedef struct DirStack
{
    DIR *dir;
    char *basepath;
    struct DirStack *next;
} DirStack;

static void pushDirStack(DirStack **_stack, const char *basepath, DIR *dir)
{
    DirStack *stack = (DirStack *) xmalloc(sizeof (DirStack));
    stack->dir = dir;
    stack->basepath = xstrdup(basepath);
    stack->next = *_stack;
    *_stack = stack;
} // pushDirStack

static void popDirStack(DirStack **_stack)
{
    DirStack *stack = *_stack;
    if (stack != NULL)
    {
        DirStack *next = stack->next;
        if (stack->dir)
            closedir(stack->dir);
        free(stack->basepath);
        free(stack);
        *_stack = next;
    } // if
} // popDirStack

static void freeDirStack(DirStack **_stack)
{
    while (*_stack)
        popDirStack(_stack);
} // freeDirStack


typedef struct
{
    DirStack *dirs;
    char *base;
} MojoArchiveDirInstance;

static boolean MojoArchive_dir_enumerate(MojoArchive *ar, const char *path)
{
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;
    char *fullpath = NULL;
    DIR *dir = NULL;

    freeDirStack(&inst->dirs);
    MojoArchive_resetEntry(&ar->prevEnum, 1);

    if (path == NULL)
    {
        fullpath = (char *) alloca(strlen(inst->base) + 1);
        strcpy(fullpath, inst->base);
    } // if
    else
    {
        size_t dlen = strlen(inst->base) + strlen(path);
        fullpath = (char *) alloca(dlen + 1);
        sprintf(fullpath, "%s/%s", inst->base, path);
        while ((dlen > 0) && (fullpath[dlen - 1] == '/'))
            fullpath[--dlen] = '\0'; // ignore trailing '/'
    } // else

    dir = opendir(fullpath);
    if (dir != NULL)
    {
        if (path != NULL)
        {
            size_t dlen = strlen(path);
            ar->prevEnum.basepath = xstrdup(path);
            while ((dlen > 0) && (ar->prevEnum.basepath[dlen - 1] == '/'))
                ar->prevEnum.basepath[--dlen] = '\0'; // ignore trailing '/'
        } // if
        pushDirStack(&inst->dirs, fullpath, dir);
    } // if

    return (dir != NULL);
} // MojoArchive_dir_enumerate


static const MojoArchiveEntry *MojoArchive_dir_enumNext(MojoArchive *ar)
{
    const boolean enumall = (ar->prevEnum.basepath == NULL);
    struct stat statbuf;
    char *fullpath = NULL;
    struct dirent *dent = NULL;
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;
    if (inst->dirs == NULL)
        return NULL;

    MojoArchive_resetEntry(&ar->prevEnum, 0);

    dent = readdir(inst->dirs->dir);
    if (dent == NULL)  // end of dir?
    {
        popDirStack(&inst->dirs);
        return MojoArchive_dir_enumNext(ar);  // try higher level in tree.
    } // if

    if ((strcmp(dent->d_name, ".") == 0) || (strcmp(dent->d_name, "..") == 0))
        return MojoArchive_dir_enumNext(ar);  // skip these.

    fullpath = (char *) alloca(strlen(inst->dirs->basepath) +
                               strlen(dent->d_name) + 2);

    sprintf(fullpath, "%s/%s", inst->dirs->basepath, dent->d_name);

    if (!enumall)
        ar->prevEnum.filename = xstrdup(dent->d_name);
    else
    {
        ar->prevEnum.filename = xmalloc(strlen(fullpath) + 1);
        strcpy(ar->prevEnum.filename, fullpath + strlen(inst->base) + 1);
    } // else

    if (lstat(fullpath, &statbuf) == -1)
        ar->prevEnum.type = MOJOARCHIVE_ENTRY_UNKNOWN;
    else
    {
        ar->prevEnum.filesize = statbuf.st_size;
        if (S_ISREG(statbuf.st_mode))
            ar->prevEnum.type = MOJOARCHIVE_ENTRY_FILE;
        else if (S_ISLNK(statbuf.st_mode))
            ar->prevEnum.type = MOJOARCHIVE_ENTRY_SYMLINK;
        else if (S_ISDIR(statbuf.st_mode))
        {
            ar->prevEnum.type = MOJOARCHIVE_ENTRY_DIR;
            if (enumall)
            {
                // push this dir on the stack. Next enum will start there.
                DIR *dir = opendir(fullpath);
                if (dir != NULL)  // !!! FIXME: what to do if NULL?
                    pushDirStack(&inst->dirs, fullpath, dir);
            } // if
        } // else if
    } // else

    return &ar->prevEnum;
} // MojoArchive_dir_enumNext


static MojoInput *MojoArchive_dir_openCurrentEntry(MojoArchive *ar)
{
    MojoInput *retval = NULL;
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;

    if ((inst->dirs != NULL) && (ar->prevEnum.type != MOJOARCHIVE_ENTRY_DIR))
    {
        char *fullpath = (char *) alloca(strlen(inst->dirs->basepath) +
                                         strlen(ar->prevEnum.filename) + 2);
        sprintf(fullpath, "%s/%s", inst->dirs->basepath, ar->prevEnum.filename);
        retval = MojoInput_newFromFile(fullpath);
    } // if

    return retval;
} // MojoArchive_dir_openCurrentEntry


static void MojoArchive_dir_close(MojoArchive *ar)
{
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;
    freeDirStack(&inst->dirs);
    free(inst->base);
    free(inst);
    MojoArchive_resetEntry(&ar->prevEnum, 1);
    free(ar);
} // MojoArchive_dir_close


MojoArchive *MojoArchive_newFromDirectory(const char *dirname)
{
    MojoArchive *ar = NULL;
    MojoArchiveDirInstance *inst;
    char *real = MojoPlatform_realpath(dirname);
    struct stat st;

    if ( (real == NULL) || (stat(real, &st) == -1) || (!S_ISDIR(st.st_mode)) )
        return NULL;

    inst = (MojoArchiveDirInstance *) xmalloc(sizeof (MojoArchiveDirInstance));
    inst->base = real;
    ar = (MojoArchive *) xmalloc(sizeof (MojoArchive));
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
        char *basepath = MojoPlatform_appBinaryPath();
        MojoInput *io = MojoInput_newFromFile(basepath);

        if (io != NULL)
            GBaseArchive = MojoArchive_newFromInput(io, basepath);

        if (GBaseArchive == NULL)
        {
            // Just use the same directory as the binary instead.
            const char *parentdir = basepath;
            char *ptr = strrchr(basepath, '/');
            if (ptr != NULL)
                *ptr = '\0';
            else
                parentdir = ".";  // oh well.
            GBaseArchive = MojoArchive_newFromDirectory(parentdir);
        } // if

        free(basepath);   // caller free()s this string.
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

