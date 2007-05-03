#include <unistd.h>  // !!! FIXME: unix dependency for readlink().
#include <sys/stat.h>  // !!! FIXME: unix dependency for stat().

#include "fileio.h"
#include "platform.h"

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

// !!! FIXME: origfname should just be the extension, I think...
MojoArchive *MojoArchive_newFromInput(MojoInput *io, const char *ext)
{
    int i;
    MojoArchive *retval = NULL;
    if (ext != NULL)
    {
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
    free(info->linkdest);
    memset(info, '\0', sizeof (MojoArchiveEntry));

    if (basetoo)
        free(base);
    else
        info->basepath = base;
} // MojoArchive_resetEntry


// !!! FIXME: I'd rather not use a callback here, but I can't see a cleaner
// !!! FIXME:  way right now...
boolean MojoInput_toPhysicalFile(MojoInput *in, const char *fname, uint16 perms,
                                 MojoInput_FileCopyCallback cb, void *data)
{
    FILE *out = NULL;
    boolean iofailure = false;
    int32 br = 0;
    int64 flen = 0;
    int64 bw = 0;

    if (in == NULL)
        return false;

    flen = in->length(in);

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
            else
            {
                bw += br;
                if (cb != NULL)
                {
                    int pct = -1;
                    if (flen > 0)
                        pct = (int) ((((double)bw) / ((double)flen)) * 100.0);
                    if (!cb(pct, data))
                        iofailure = true;
                } // if
            } // else
        } // else
    } // while

    if (fclose(out) != 0)
        iofailure = true;

    if (iofailure)
    {
        MojoPlatform_unlink(fname);
        return false;
    } // if

    MojoPlatform_chmod(fname, perms);
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
        size_t dlen = strlen(inst->base) + strlen(path) + 1;
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
        ar->prevEnum.perms = statbuf.st_mode;
        ar->prevEnum.filesize = statbuf.st_size;
        if (S_ISREG(statbuf.st_mode))
            ar->prevEnum.type = MOJOARCHIVE_ENTRY_FILE;

        else if (S_ISLNK(statbuf.st_mode))
        {
            ar->prevEnum.type = MOJOARCHIVE_ENTRY_SYMLINK;
            ar->prevEnum.linkdest = (char *) xmalloc(statbuf.st_size + 1);
            if (readlink(fullpath, ar->prevEnum.linkdest, statbuf.st_size) < 0)
                return MojoArchive_dir_enumNext(ar);
            ar->prevEnum.linkdest[statbuf.st_size] = '\0';
        } // else if

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


// !!! FIXME: this code is going to be cut-and-paste for every archiver!
static MojoInput *MojoArchive_dir_openCurrentEntry(MojoArchive *ar)
{
    MojoInput *retval = NULL;
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;

    if ((inst->dirs != NULL) && (ar->prevEnum.type == MOJOARCHIVE_ENTRY_FILE))
    {
        char *fullpath = NULL;
        if (ar->prevEnum.basepath != NULL)
        {
            fullpath = (char *) alloca(strlen(inst->dirs->basepath) +
                                       strlen(ar->prevEnum.filename) + 2);
            sprintf(fullpath, "%s/%s", inst->dirs->basepath,
                    ar->prevEnum.filename);
        } // if
        else
        {
            fullpath = (char *) alloca(strlen(inst->base) +
                                       strlen(ar->prevEnum.filename) + 2);
            sprintf(fullpath, "%s/%s", inst->base, ar->prevEnum.filename);
        } // else

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
            GBaseArchive = MojoArchive_newFromInput(io, NULL);

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

