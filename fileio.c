/**
 * MojoSetup; a portable, flexible installation application.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

#include <unistd.h>  // !!! FIXME: unix dependency for readlink().
#include <sys/stat.h>  // !!! FIXME: unix dependency for stat().

#include "fileio.h"
#include "platform.h"

typedef MojoArchive* (*MojoArchiveCreateEntryPoint)(MojoInput *io);

MojoArchive *MojoArchive_createZIP(MojoInput *io);
MojoArchive *MojoArchive_createTAR(MojoInput *io);

typedef struct
{
    const char *ext;
    MojoArchiveCreateEntryPoint create;
} MojoArchiveType;

static const MojoArchiveType archives[] =
{
    { "zip", MojoArchive_createZIP },
    { "tar", MojoArchive_createTAR },
    { "tar.gz", MojoArchive_createTAR },
    { "tar.bz2", MojoArchive_createTAR },
    { "tgz", MojoArchive_createTAR },
    { "tbz2", MojoArchive_createTAR },
    { "tb2", MojoArchive_createTAR },
    { "tbz", MojoArchive_createTAR },
};

MojoArchive *MojoArchive_newFromInput(MojoInput *io, const char *origfname)
{
    int i;
    MojoArchive *retval = NULL;
    const char *ext = NULL;

    if (origfname != NULL)
    {
        ext = strchr(origfname, '/');
        if (ext == NULL)
            ext = strchr(origfname, '.');
        else
            ext = strchr(ext+1, '.');
    } // if

    if (ext != NULL)
    {
        // Try for an exact match.
        ext++;  // skip that '.'
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


void MojoArchive_resetEntry(MojoArchiveEntry *info)
{
    free(info->filename);
    free(info->linkdest);
    memset(info, '\0', sizeof (MojoArchiveEntry));
} // MojoArchive_resetEntry


// !!! FIXME: I'd rather not use a callback here, but I can't see a cleaner
// !!! FIXME:  way right now...
boolean MojoInput_toPhysicalFile(MojoInput *in, const char *fname, uint16 perms,
                                 MojoInput_FileCopyCallback cb, void *data)
{
    boolean retval = false;
    uint32 start = MojoPlatform_ticks();
    FILE *out = NULL;
    boolean iofailure = false;
    int64 flen = 0;
    int64 bw = 0;

    if (in == NULL)
        return false;

    // Wait for a ready(), so length() can be meaningful on network streams.
    while ((!in->ready(in)) && (!iofailure))
    {
        MojoPlatform_sleep(100);
        if (cb != NULL)
        {
            if (!cb(MojoPlatform_ticks() - start, 0, 0, -1, data))
                iofailure = true;
        } // if
    } // while

    flen = in->length(in);

    STUBBED("fopen?");

    MojoPlatform_unlink(fname);
    if (!iofailure)
        out = fopen(fname, "wb");

    if (out != NULL)
    {
        while (!iofailure)
        {
            int64 br = 0;

            // If there's a callback, then poll. Otherwise, just block on
            //  the reads from the MojoInput.
            if ((cb != NULL) && (!in->ready(in)))
                MojoPlatform_sleep(100);
            else
            {
                br = in->read(in, scratchbuf_128k, sizeof (scratchbuf_128k));
                if (br == 0)  // we're done!
                    break;
                else if (br < 0)
                    iofailure = true;
                else
                {
                    if (fwrite(scratchbuf_128k, br, 1, out) != 1)
                        iofailure = true;
                    else
                        bw += br;
                } // else
            } // else

            if (cb != NULL)
            {
                if (!cb(MojoPlatform_ticks() - start, br, bw, flen, data))
                    iofailure = true;
            } // if
        } // while

        if (fclose(out) != 0)
            iofailure = true;

        if (iofailure)
            MojoPlatform_unlink(fname);
        else
        {
            MojoPlatform_chmod(fname, perms);
            retval = true;
        } // else
    } // if

    in->close(in);
    return retval;
} // MojoInput_toPhysicalFile


MojoInput *MojoInput_newFromArchivePath(MojoArchive *ar, const char *fname)
{
    MojoInput *retval = NULL;
    if (ar->enumerate(ar))
    {
        const MojoArchiveEntry *entinfo;
        while ((entinfo = ar->enumNext(ar)) != NULL)
        {
            if (strcmp(entinfo->filename, fname) == 0)
            {
                if (entinfo->type == MOJOARCHIVE_ENTRY_FILE)
                    retval = ar->openCurrentEntry(ar);
                break;
            } // if
        } // while
    } // if

    return retval;
} // MojoInput_newFromArchivePath



// MojoInputs from files on the OS filesystem.

typedef struct
{
    FILE *handle;
    char *path;
} MojoInputFileInstance;

static boolean MojoInput_file_ready(MojoInput *io)
{
    // !!! FIXME: select()? Does that help with network filesystems?
    return true;
} // MojoInput_file_ready

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

MojoInput *MojoInput_newFromFile(const char *path)
{
    MojoInput *io = NULL;
    FILE *f = NULL;

    f = fopen(path, "rb");
    if (f != NULL)
    {
        MojoInputFileInstance *inst;
        inst = (MojoInputFileInstance *) xmalloc(sizeof (MojoInputFileInstance));
        inst->path = xstrdup(path);
        inst->handle = f;

        io = (MojoInput *) xmalloc(sizeof (MojoInput));
        io->ready = MojoInput_file_ready;
        io->read = MojoInput_file_read;
        io->seek = MojoInput_file_seek;
        io->tell = MojoInput_file_tell;
        io->length = MojoInput_file_length;
        io->duplicate = MojoInput_file_duplicate;
        io->close = MojoInput_file_close;
        io->opaque = inst;
    } // if

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

static boolean MojoArchive_dir_enumerate(MojoArchive *ar)
{
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;
    DIR *dir = NULL;

    freeDirStack(&inst->dirs);
    MojoArchive_resetEntry(&ar->prevEnum);

    dir = opendir(inst->base);
    if (dir != NULL)
        pushDirStack(&inst->dirs, inst->base, dir);

    return (dir != NULL);
} // MojoArchive_dir_enumerate


static const MojoArchiveEntry *MojoArchive_dir_enumNext(MojoArchive *ar)
{
    struct stat statbuf;
    char *fullpath = NULL;
    struct dirent *dent = NULL;
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;

    MojoArchive_resetEntry(&ar->prevEnum);

    if (inst->dirs == NULL)
        return NULL;

    dent = readdir(inst->dirs->dir);
    if (dent == NULL)  // end of dir?
    {
        popDirStack(&inst->dirs);
        return MojoArchive_dir_enumNext(ar);  // try higher level in tree.
    } // if

    if ((strcmp(dent->d_name, ".") == 0) || (strcmp(dent->d_name, "..") == 0))
        return MojoArchive_dir_enumNext(ar);  // skip these.

    fullpath = (char *) xmalloc(strlen(inst->dirs->basepath) +
                                strlen(dent->d_name) + 2);

    sprintf(fullpath, "%s/%s", inst->dirs->basepath, dent->d_name);
    ar->prevEnum.filename = xstrdup(fullpath + strlen(inst->base) + 1);

    if (lstat(fullpath, &statbuf) == -1)
        ar->prevEnum.type = MOJOARCHIVE_ENTRY_UNKNOWN;
    else
    {
        ar->prevEnum.filesize = statbuf.st_size;

        // !!! FIXME: not sure this is the best thing.
        // We currently force the perms from physical files, since CDs on
        //  Linux tend to mark every files as executable and read-only. If you
        //  want to install something with specific permissions, wrap it in a
        //  tarball or chmod it from a postinstall hook in your config file.
        //ar->prevEnum.perms = statbuf.st_mode;
        ar->prevEnum.perms = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

        if (S_ISREG(statbuf.st_mode))
            ar->prevEnum.type = MOJOARCHIVE_ENTRY_FILE;

        else if (S_ISLNK(statbuf.st_mode))
        {
            ar->prevEnum.type = MOJOARCHIVE_ENTRY_SYMLINK;
            ar->prevEnum.linkdest = (char *) xmalloc(statbuf.st_size + 1);
            if (readlink(fullpath, ar->prevEnum.linkdest, statbuf.st_size) < 0)
            {
                free(fullpath);
                return MojoArchive_dir_enumNext(ar);
            } // if
            ar->prevEnum.linkdest[statbuf.st_size] = '\0';
        } // else if

        else if (S_ISDIR(statbuf.st_mode))
        {
            DIR *dir = opendir(fullpath);
            ar->prevEnum.type = MOJOARCHIVE_ENTRY_DIR;
            if (dir == NULL)
            {
                free(fullpath);
                return MojoArchive_dir_enumNext(ar);
            } // if
            // push this dir on the stack. Next enum will start there.
            pushDirStack(&inst->dirs, fullpath, dir);
        } // else if
    } // else

    free(fullpath);
    return &ar->prevEnum;
} // MojoArchive_dir_enumNext


static MojoInput *MojoArchive_dir_openCurrentEntry(MojoArchive *ar)
{
    MojoInput *retval = NULL;
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;

    if ((inst->dirs != NULL) && (ar->prevEnum.type == MOJOARCHIVE_ENTRY_FILE))
    {
        char *fullpath = (char *) xmalloc(strlen(inst->base) +
                                          strlen(ar->prevEnum.filename) + 2);
        sprintf(fullpath, "%s/%s", inst->base, ar->prevEnum.filename);
        retval = MojoInput_newFromFile(fullpath);
        free(fullpath);
    } // if

    return retval;
} // MojoArchive_dir_openCurrentEntry


static void MojoArchive_dir_close(MojoArchive *ar)
{
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;
    freeDirStack(&inst->dirs);
    free(inst->base);
    free(inst);
    MojoArchive_resetEntry(&ar->prevEnum);
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

            // !!! FIXME: failing this, maybe default.mojosetup?
            // !!! FIXME:  maybe a command line?
        } // if

        free(basepath);   // appBinaryPath caller free()s this string.
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


// This stub is here if we didn't compile in libfetch...
#if !SUPPORT_URL_HTTP && !SUPPORT_URL_FTP
MojoInput *MojoInput_fromURL(const char *url)
{
    // !!! FIXME: localization.
    logError(_("No networking support in this build."));
    return NULL;
} // MojoInput_fromURL
#endif

// end of fileio.c ...

