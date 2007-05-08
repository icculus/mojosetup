// !!! FIXME: TODO...install signal handlers to catch crashes and try to push
// !!! FIXME:  them through fatal() so we can uninstall if possible.

#if PLATFORM_MACOSX
#include <Carbon/Carbon.h>
#undef true
#undef false
#endif

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/utsname.h>
#include <sys/mount.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#if MOJOSETUP_HAVE_SYS_UCRED_H
#  ifdef MOJOSETUP_HAVE_MNTENT_H
#    undef MOJOSETUP_HAVE_MNTENT_H /* don't do both... */
#  endif
#  include <sys/ucred.h>
#endif

#if MOJOSETUP_HAVE_MNTENT_H
#  include <mntent.h>
#endif

#if PLATFORM_BEOS
#define DLOPEN_ARGS 0
void *beos_dlopen(const char *fname, int unused);
void *beos_dlsym(void *lib, const char *sym);
void beos_dlclose(void *lib);
void beos_usleep(unsigned long ticks);
#define dlopen beos_dlopen
#define dlsym beos_dlsym
#define dlclose beos_dlclose
#define usleep beos_usleep
#else
#include <dlfcn.h>
#define DLOPEN_ARGS (RTLD_NOW | RTLD_GLOBAL)
#endif

#include "../platform.h"

static struct timeval startup_time;

static char *getCurrentWorkingDir(void)
{
    char *retval = NULL;
    size_t len;

    // loop a few times in case we don't have a large enough buffer.
    for (len = 128; len <= (16*1024); len *= 2)
    {
        retval = (char *) xrealloc(retval, len);
        if (getcwd(retval, len-1) != NULL)
        {
            size_t slen = strlen(retval);
            if (retval[slen-1] != '/')   // make sure this ends with '/' ...
            {
                retval[slen] = '/';
                retval[slen+1] = '\0';
            } // if
            return retval;
        } // if
    } // for

    free(retval);
    return NULL;
} // getCurrentWorkingDir


static void *guaranteeAllocation(void *ptr, size_t len, size_t *_alloclen)
{
    void *retval = NULL;
    size_t alloclen = *_alloclen;
    if (alloclen > len)
        return ptr;

    if (!alloclen)
        alloclen = 1;

    while (alloclen <= len)
        alloclen *= 2;

    retval = xrealloc(ptr, alloclen);
    if (retval != NULL)
        *_alloclen = alloclen;

    return retval;
} // guaranteeAllocation


// This is a mess, but I'm not sure it can be done more cleanly.
static char *realpathInternal(char *path, const char *cwd, int linkloop)
{
    char *linkname = NULL;
    char *retval = NULL;
    size_t len = 0;
    size_t alloclen = 0;

    if (*path == '/')   // absolute path.
    {
        retval = xstrdup("/");
        path++;
        len = 1;
    } // if
    else   // relative path.
    {
        if (cwd != NULL)
            retval = xstrdup(cwd);
        else
        {
            if ((retval = getCurrentWorkingDir()) == NULL)
                return NULL;
        } // else
        len = strlen(retval);
    } // else

    while (true)
    {
        struct stat statbuf;
        size_t newlen;

        char *nextpath = strchr(path, '/');
        if (nextpath != NULL)
            *nextpath = '\0';

        newlen = strlen(path);
        retval = guaranteeAllocation(retval, len + newlen + 2, &alloclen);
        strcpy(retval + len, path);

        if (*path == '\0')
            retval[--len] = '\0';  // chop ending "/" bit, it gets readded later.

        else if (strcmp(path, ".") == 0)
        {
            retval[--len] = '\0';  // chop ending "/." bit
            newlen = 0;
        } // else if

        else if (strcmp(path, "..") == 0)
        {
            char *ptr;
            retval[--len] = '\0';  // chop ending "/.." bit
            ptr = strrchr(retval, '/');
            if ((ptr == NULL) || (ptr == retval))
            {
                strcpy(retval, "/");
                len = 0;
            } // if
            else
            {
                *ptr = '\0';
                len -= (size_t) ((retval+len)-ptr);
            } // else
            newlen = 0;
        } // else if

        // it may be a symlink...check it.
        else if (lstat(retval, &statbuf) == -1)
            goto realpathInternal_failed;

        else if (S_ISLNK(statbuf.st_mode))
        {
            char *newresolve = NULL;
            int br = 0;

            if (linkloop > 255)
                goto realpathInternal_failed;

            linkname = (char *) xmalloc(statbuf.st_size + 1);
            br = readlink(retval, linkname, statbuf.st_size);
            if (br < 0)
                goto realpathInternal_failed;

            // readlink() doesn't null-terminate!
            linkname[br] = '\0';

            // chop off symlink name for its cwd.
            retval[len] = '\0';

            // resolve the link...
            newresolve = realpathInternal(linkname, retval, linkloop + 1);
            if (newresolve == NULL)
                goto realpathInternal_failed;

            len = strlen(newresolve);
            retval = guaranteeAllocation(retval, len + 2, &alloclen);
            strcpy(retval, newresolve);
            free(newresolve);
            free(linkname);
        } // else if

        else
        {
            len += newlen;
        } // else

        if (nextpath == NULL)
            break;  // holy crap we're done!
        else  // append a '/' before the next path element.
        {
            path = nextpath + 1;
            retval[len++] = '/';
            retval[len] = '\0';
        } // else
    } // while

    // Shrink string if we're using more memory than necessary...
    if (alloclen > len+1)
        retval = (char *) xrealloc(retval, len+1);

    return retval;

realpathInternal_failed:
    free(linkname);
    free(retval);
    return NULL;
} // realpathInternal


// Rolling my own realpath, even if the runtime has one, since apparently
//  the spec is a little flakey, and it can overflow PATH_MAX. On BeOS <= 5,
//  we'd have to resort to BPath to do this, too, and I'd rather avoid the C++
//  dependencies and headers.
char *MojoPlatform_realpath(const char *_path)
{
    char *path = xstrdup(_path);
    char *retval = realpathInternal(path, NULL, 0);
    free(path);
    return retval;
} // MojoPlatform_realpath


// (Stolen from physicsfs: http://icculus.org/physfs/ ...)
/*
 * See where program (bin) resides in the $PATH. Returns a copy of the first
 *  element in $PATH that contains it, or NULL if it doesn't exist or there
 *  were other problems.
 *
 * You are expected to free() the return value when you're done with it.
 */
static char *findBinaryInPath(const char *bin)
{
    const char *_envr = getenv("PATH");
    size_t alloc_size = 0;
    char *envr = NULL;
    char *exe = NULL;
    char *start = NULL;
    char *ptr = NULL;

    if ((_envr == NULL) || (bin == NULL))
        return NULL;

    envr = (char *) alloca(strlen(_envr) + 1);
    strcpy(envr, _envr);
    start = envr;

    do
    {
        size_t size;
        ptr = strchr(start, ':');  // find next $PATH separator.
        if (ptr)
            *ptr = '\0';

        size = strlen(start) + strlen(bin) + 2;
        if (size > alloc_size)
        {
            char *x = (char *) xrealloc(exe, size);
            alloc_size = size;
            exe = x;
        } // if

        // build full binary path...
        strcpy(exe, start);
        if ((exe[0] == '\0') || (exe[strlen(exe) - 1] != '/'))
            strcat(exe, "/");
        strcat(exe, bin);

        if (access(exe, X_OK) == 0)  // Exists as executable? We're done.
        {
            strcpy(exe, start);  // i'm lazy. piss off.
            return(exe);
        } // if

        start = ptr + 1;  // start points to beginning of next element.
    } while (ptr != NULL);

    if (exe != NULL)
        free(exe);

    return(NULL);  // doesn't exist in path.
} // findBinaryInPath


char *MojoPlatform_appBinaryPath(void)
{
    const char *argv0 = GArgv[0];
    char *retval = NULL;

    if (strchr(argv0, '/') != NULL)  
        retval = MojoPlatform_realpath(argv0); // argv[0] contains a path?
    else  // slow path...have to search the whole $PATH for this one...
    {
        char *found = findBinaryInPath(argv0);
        if (found)
            retval = MojoPlatform_realpath(found);
        free(found);
    } // else

    return retval;
} // MojoPlatform_appBinaryPath


// This implementation is a bit naive.
boolean MojoPlatform_locale(char *buf, size_t len)
{
    boolean retval = false;
    const char *envr = getenv("LANG");
    if (envr != NULL)
    {
        char *ptr = NULL;
        xstrncpy(buf, envr, len);
        ptr = strchr(buf, '.');  // chop off encoding if explicitly listed.
        if (ptr != NULL)
            *ptr = '\0';
        retval = true;
    } // if

    #if PLATFORM_MACOSX
    else if (CFLocaleCreateCanonicalLocaleIdentifierFromString == NULL)
        retval = false; // !!! FIXME: 10.2 compatibility?

    else if (CFLocaleCreateCanonicalLocaleIdentifierFromString != NULL)
    {
        CFPropertyListRef languages = CFPreferencesCopyAppValue(
                                            CFSTR("AppleLanguages"),
                                            kCFPreferencesCurrentApplication);
        if (languages != NULL)
        {
            CFStringRef primary = CFArrayGetValueAtIndex(languages, 0);
            if (primary != NULL)
            {
                CFStringRef locale =
                        CFLocaleCreateCanonicalLocaleIdentifierFromString(
                                                kCFAllocatorDefault, primary);                if (locale != NULL)
                if (locale != NULL)
                {
                    CFStringGetCString(locale, buf, len, kCFStringEncodingUTF8);
                    CFRelease(locale);
                    retval = true;
                } // if
            } // if
            CFRelease(languages);
        } // if
    } // else if
    #endif

    return retval;
} // MojoPlatform_locale


boolean MojoPlatform_osType(char *buf, size_t len)
{
#if PLATFORM_MACOSX
    xstrncpy(buf, "macosx", len);
#elif PLATFORM_BEOS
    xstrncpy(buf, "beos", len);   // !!! FIXME: zeta? haiku?
#elif defined(linux) || defined(__linux) || defined(__linux__)
    xstrncpy(buf, "linux", len);
#elif defined(__FreeBSD__) || defined(__DragonFly__)
    xstrncpy(buf, "freebsd", len);
#elif defined(__NetBSD__)
    xstrncpy(buf, "netbsd", len);
#elif defined(__OpenBSD__)
    xstrncpy(buf, "openbsd", len);
#elif defined(bsdi) || defined(__bsdi) || defined(__bsdi__)
    xstrncpy(buf, "bsdi", len);
#elif defined(_AIX)
    xstrncpy(buf, "aix", len);
#elif defined(hpux) || defined(__hpux) || defined(__hpux__)
    xstrncpy(buf, "hpux", len);
#elif defined(sgi) || defined(__sgi) || defined(__sgi__) || defined(_SGI_SOURCE)
    xstrncpy(buf, "irix", len);
#else
#   error Please define your platform.
#endif

    return true;
} // MojoPlatform_ostype


boolean MojoPlatform_osVersion(char *buf, size_t len)
{
#if PLATFORM_MACOSX
    long ver = 0x0000;
	if (Gestalt(gestaltSystemVersion, &ver) == noErr)
    {
        char str[16];
        snprintf(str, sizeof (str), "%X", (int) ver);
        snprintf(buf, len, "%c%c.%c.%c", str[0], str[1], str[2], str[3]);
        return true;
    } // if
#else
    // This information may or may not actually MEAN anything. On BeOS, it's
    //  useful, but on other things, like Linux, it'll give you the kernel
    //  version, which doesn't necessarily help.
    struct utsname un;
    if (uname(&un) == 0)
    {
        xstrncpy(buf, un.release, len);
        return true;
    } // if
#endif

    return false;
} // MojoPlatform_osversion


void MojoPlatform_sleep(uint32 ticks)
{
    usleep(ticks * 1000);
} // MojoPlatform_sleep


uint32 MojoPlatform_ticks(void)
{
    uint64 then_ms, now_ms;
    struct timeval now;
    gettimeofday(&now, NULL);
    then_ms = (((uint64) startup_time.tv_sec) * 1000) +
              (((uint64) startup_time.tv_usec) / 1000);
    now_ms = (((uint64) now.tv_sec) * 1000) + (((uint64) now.tv_usec) / 1000);
    return ((uint32) (now_ms - then_ms));
} // MojoPlatform_ticks


void MojoPlatform_die(void)
{
    _exit(86);
} // MojoPlatform_die


boolean MojoPlatform_unlink(const char *fname)
{
    boolean retval = false;
    struct stat statbuf;
    if (stat(fname, &statbuf) != -1)
    {
        if (S_ISDIR(statbuf.st_mode))
            retval = (rmdir(fname) == 0);
        else
            retval = (unlink(fname) == 0);
    } // if
    return retval;
} // MojoPlatform_unlink


boolean MojoPlatform_symlink(const char *src, const char *dst)
{
    return (symlink(dst, src) == 0);
} // MojoPlatform_symlink


boolean MojoPlatform_mkdir(const char *path)
{
    // !!! FIXME: error if already exists?
    return (mkdir(path, 0755) == 0);
} // MojoPlatform_mkdir


boolean MojoPlatform_rename(const char *src, const char *dst)
{
    return (rename(src, dst) == 0);
} // MojoPlatform_rename


boolean MojoPlatform_exists(const char *dir, const char *fname)
{
    boolean retval = false;
    if (fname == NULL)
        retval = (access(dir, F_OK) != -1);
    else
    {
        const size_t len = strlen(dir) + strlen(fname) + 2;
        char *buf = (char *) xmalloc(strlen(dir) + strlen(fname) + 2);
        snprintf(buf, len, "%s/%s", dir, fname);
        retval = (access(buf, F_OK) != -1);
        free(buf);
    } // else
    return retval;
} // MojoPlatform_exists


boolean MojoPlatform_perms(const char *fname, uint16 *p)
{
    boolean retval = false;
    struct stat statbuf;
    if (stat(fname, &statbuf) != -1)
    {
        *p = statbuf.st_mode;
        retval = true;
    } // if
    return retval;
} // MojoPlatform_perms


boolean MojoPlatform_chmod(const char *fname, uint16 p)
{
    return (chmod(fname, p) != -1);
} // MojoPlatform_chmod


char *MojoPlatform_findMedia(const char *uniquefile)
{
#if MOJOSETUP_HAVE_SYS_UCRED_H
    int i = 0;
    struct statfs *mntbufp = NULL;
    int mounts = getmntinfo(&mntbufp, MNT_WAIT);
    for (i = 0; i < mounts; i++)
    {
        const char *mnt = mntbufp[i].f_mntonname;
        if (MojoPlatform_exists(mnt, uniquefile))
            return xstrdup(mnt);
    } // for

#elif MOJOSETUP_HAVE_MNTENT_H
    FILE *mounts = setmntent("/etc/mtab", "r");
    if (mounts != NULL)
    {
        struct mntent *ent = NULL;
        while ((ent = getmntent(mounts)) != NULL)
        {
            const char *mnt = ent->mnt_dir;
            if (MojoPlatform_exists(mnt, uniquefile))
            {
                endmntent(mounts);
                return xstrdup(mnt);
            } // if
        } // while
        endmntent(mounts);
    } // if

#else
#   warning No mountpoint detection on this platform...
#endif

    return NULL;
} // MojoPlatform_findMedia


void MojoPlatform_log(const char *str)
{
    printf("%s\n", str);
} // MojoPlatform_log


static boolean testTmpDir(const char *dname, char *buf,
                          size_t len, const char *tmpl)
{
    boolean retval = false;
    if ( (dname != NULL) && (access(dname, R_OK | W_OK | X_OK) == 0) )
    {
        struct stat statbuf;
        if ( (stat(dname, &statbuf) == 0) && (S_ISDIR(statbuf.st_mode)) )
        {
            const size_t rc = snprintf(buf, len, "%s/%s", dname, tmpl);
            if (rc < len)
                retval = true;
        } // if
    } // if

    return retval;
} // testTmpDir


void *MojoPlatform_dlopen(const uint8 *img, size_t len)
{
    // Write the image to a temporary file, dlopen() it, and delete it
    //  immediately. The inode will be kept around by the Unix kernel until
    //  we either dlclose() it or the process terminates, but we don't have
    //  to worry about polluting the /tmp directory or cleaning this up later.
    // We'll try every reasonable temp directory location until we find one
    //  that works, in case (say) one lets us write a file, but there
    //  isn't enough space for the data.

    // /dev/shm may be able to avoid writing to physical media...try it first.
    const char *dirs[] = { "/dev/shm", getenv("TMPDIR"), P_tmpdir, "/tmp" };
    const char *tmpl = "mojosetup-gui-plugin-XXXXXX";
    char fname[PATH_MAX];
    void *retval = NULL;
    int i = 0;

    if (dlopen == NULL)   // weak symbol on older Mac OS X
        return NULL;

    #ifndef P_tmpdir  // glibc defines this, maybe others.
    #define P_tmpdir NULL
    #endif

    for (i = 0; (i < STATICARRAYLEN(dirs)) && (retval == NULL); i++)
    {
        if (testTmpDir(dirs[i], fname, len, tmpl))
        {
            const int fd = mkstemp(fname);
            if (fd != -1)
            {
                const size_t bw = write(fd, img, len);
                const int rc = close(fd);
                if ((bw == len) && (rc != -1))
                    retval = dlopen(fname, DLOPEN_ARGS);
                unlink(fname);
            } // if
        } // if
    } // for

    return retval;
} // MojoPlatform_dlopen


void *MojoPlatform_dlsym(void *lib, const char *sym)
{
    #if PLATFORM_MACOSX
    if (dlsym == NULL) return NULL;  // weak symbol on older Mac OS X
    #endif

    return dlsym(lib, sym);
} // MojoPlatform_dlsym


void MojoPlatform_dlclose(void *lib)
{
    #if PLATFORM_MACOSX
    if (dlclose == NULL) return;  // weak symbol on older Mac OS X
    #endif

    dlclose(lib);
} // MojoPlatform_dlclose



static void signal_catcher(int sig)
{
    static boolean first_shot = true;
    if (first_shot)
    {
        first_shot = false;
        logError("Caught signal #%d", sig);
    } // if
} // signal_catcher

static void crash_catcher(int sig)
{
    signal_catcher(sig);
    MojoSetup_crashed();
} // crash_catcher

static void termination_catcher(int sig)
{
    signal_catcher(sig);
    MojoSetup_terminated();
} // termination_catcher


static void install_signals(void)
{
    static int crash_sigs[] = { SIGSEGV, SIGILL, SIGBUS, SIGFPE, SIGTRAP };
    static int term_sigs[] = { SIGQUIT, SIGINT, SIGTERM, SIGHUP };
    static int ignore_sigs[] = { SIGPIPE };
    int i;

    for (i = 0; i < STATICARRAYLEN(crash_sigs); i++)
        signal(crash_sigs[i], crash_catcher);
    for (i = 0; i < STATICARRAYLEN(term_sigs); i++)
        signal(term_sigs[i], termination_catcher);
    for (i = 0; i < STATICARRAYLEN(ignore_sigs); i++)
        signal(ignore_sigs[i], SIG_IGN);
} // install_signals


int main(int argc, char **argv)
{
    gettimeofday(&startup_time, NULL);
    install_signals();
    return MojoSetup_main(argc, argv);
} // main

// end of unix.c ...
