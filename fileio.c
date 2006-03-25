#include "fileio.h"

// !!! FIXME: don't have this here. (need unlink for now).
#include <unistd.h>

boolean mojoInputToPhysicalFile(MojoInput *in, const char *fname)
{
    FILE *out = NULL;
    boolean iofailure = false;
    char buf[1024];
    size_t br;

    STUBBED("mkdir first?");

    if (in == NULL)
        return false;

    STUBBED("fopen?");
    unlink(fname);
    out = fopen(fname, "wb");
    if (out == NULL)
        return false;

    while (!iofailure)
    {
        br = in->read(in, buf, sizeof (buf));
        STUBBED("how to detect read failures?");
        if (br == 0)  // we're done!
            break;
        else if (br < 0)
            iofailure = true;
        else
        {
            if (fwrite(buf, br, 1, out) != 1)
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


// MojoArchives from directories on the OS filesystem.

// !!! FIXME: abstract the unixy bits into the platform/ dir.
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

typedef struct
{
    DIR *dir;
    char *path;
} MojoArchiveDirInstance;

static boolean MojoArchive_dir_enumerate(MojoArchive *ar, const char *path)
{
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;
    if (inst->dir != NULL)
        closedir(inst->dir);

    free(ar->lastEnumInfo.filename);
    ar->lastEnumInfo.filename = NULL;
    ar->lastEnumInfo.type = MOJOARCHIVE_ENTRY_UNKNOWN;
    ar->lastEnumInfo.filesize = 0;

    inst->path = xrealloc(inst->path, strlen(path) + 1);
    strcpy(inst->path, path);

    inst->dir = opendir(path);
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

    fullpath = (char *) alloca(strlen(dent->d_name) + strlen(inst->path) + 2);
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

    ar->lastEnumInfo.filename = xmalloc(strlen(dent->d_name) + 1);
    strcpy(ar->lastEnumInfo.filename, dent->d_name);
    return &ar->lastEnumInfo;
} // MojoArchive_dir_enumNext


static MojoInput *MojoArchive_dir_openCurrentEntry(MojoArchive *ar)
{
    STUBBED("writeme");
    return NULL;
} // MojoArchive_dir_openCurrentEntry


static void MojoArchive_dir_close(MojoArchive *ar)
{
    MojoArchiveDirInstance *inst = (MojoArchiveDirInstance *) ar->opaque;
    if (inst->dir != NULL)
        closedir(inst->dir);
    free(inst->path);
    free(ar->lastEnumInfo.filename);
    free(ar->opaque);
    free(ar);
} // MojoArchive_dir_close


MojoArchive *MojoArchive_newFromDirectory(const char *dirname)
{
    MojoArchive *ar = (MojoArchive *) xmalloc(sizeof (MojoArchive));
    ar->enumerate = MojoArchive_dir_enumerate;
    ar->enumNext = MojoArchive_dir_enumNext;
    ar->openCurrentEntry = MojoArchive_dir_openCurrentEntry;
    ar->close = MojoArchive_dir_close;
    ar->opaque = xmalloc(sizeof (MojoArchiveDirInstance));
    return ar;
} // MojoArchive_newFromDirectory

// end of fileio.c ...

