/**
 * MojoSetup; a portable, flexible installation application.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

#ifndef _INCL_PLATFORM_H_
#define _INCL_PLATFORM_H_

#include "universal.h"

#ifdef __cplusplus
extern "C" {
#endif

// this is called by your mainline.
int MojoSetup_main(int argc, char **argv);

// Caller must free returned string!
char *MojoPlatform_appBinaryPath(void);

// Caller must free returned string!
char *MojoPlatform_currentWorkingDir(void);

// Caller must free returned string!
char *MojoPlatform_homedir(void);

uint32 MojoPlatform_ticks(void);

// Make current process kill itself immediately, without any sort of internal
//  cleanup, like atexit() handlers or static destructors...the OS will have
//  to sort out the freeing of any resources, and no more code in this
//  process than necessary should run. This function does not return. Try to
//  avoid calling this.
void MojoPlatform_die(void);

// Delete a file from the physical filesystem. This should remove empty
//  directories as well as files. Returns true on success, false on failure.
boolean MojoPlatform_unlink(const char *fname);

// Resolve symlinks, relative paths, etc. Caller free()'s buffer. Returns
//  NULL if path couldn't be resolved.
char *MojoPlatform_realpath(const char *path);

// Create a symlink in the physical filesystem. (src) is the NAME OF THE LINK
//  and (dst) is WHAT IT POINTS TO. This is backwards from the unix symlink()
//  syscall! Returns true if link was created, false otherwise.
boolean MojoPlatform_symlink(const char *src, const char *dst);

// !!! FIXME: comment me.
boolean MojoPlatform_mkdir(const char *path, uint16 perms);

// Move a file to a new name. This has to be a fast (if not atomic) operation,
//  so if it would require a legitimate copy to another filesystem or device,
//  this should fail, as the standard Unix rename() function does.
// Returns true on successful rename, false otherwise.
boolean MojoPlatform_rename(const char *src, const char *dst);

// !!! FIXME: comment me.
boolean MojoPlatform_exists(const char *dir, const char *fname);

// !!! FIXME: comment me.
boolean MojoPlatform_writable(const char *fname);

// !!! FIXME: comment me.
boolean MojoPlatform_isdir(const char *dir);

// !!! FIXME: comment me.
boolean MojoPlatform_perms(const char *fname, uint16 *p);

// !!! FIXME: comment me.
boolean MojoPlatform_chmod(const char *fname, uint16 p);

// !!! FIXME: comment me.
char *MojoPlatform_findMedia(const char *uniquefile);

// Convert a string into a permissions bitmask. On Unix, this is currently
//  expected to be an octal string like "0755", but may except other forms
//  in the future, and other platforms may need to interpret permissions
//  differently. (str) may be NULL for defaults, and is considered valid.
// If (str) is not valid, return a reasonable default and set (*valid) to
//  false. Otherwise, set (*valid) to true and return the converted value.
uint16 MojoPlatform_makePermissions(const char *str, boolean *valid);

// Return a default, sane set of permissions for a newly-created file.
uint16 MojoPlatform_defaultFilePerms(void);

// Return a default, sane set of permissions for a newly-created directory.
uint16 MojoPlatform_defaultDirPerms(void);

// Wrappers for Unix dlopen/dlsym/dlclose, sort of. Instead of a filename,
//  these take a memory buffer for the library. If you can't load this
//  directly in RAM, the platform should write it to a temporary file first,
//  and deal with cleanup in MojoPlatform_dlclose(). The memory buffer must be
//  dereferenced in MojoPlatform_dlopen(), as the caller may free() it upon
//  return. Everything else works like the usual Unix calls.
void *MojoPlatform_dlopen(const uint8 *img, size_t len);
void *MojoPlatform_dlsym(void *lib, const char *sym);
void MojoPlatform_dlclose(void *lib);

#if !SUPPORT_MULTIARCH
#define MojoPlatform_switchBin(img, len)
#else
void MojoPlatform_switchBin(const uint8 *img, size_t len);
#endif

// Put the calling process to sleep for at least (ticks) milliseconds.
//  This is meant to yield the CPU while spinning in a loop that is polling
//  for input, etc. Pumping the GUI event queue happens elsewhere, not here.
void MojoPlatform_sleep(uint32 ticks);

// Put a line of text to the the system log, whatever that might be on a
//  given platform. (str) is a complete line, but won't end with any newline
//  characters. You should supply if needed.
void MojoPlatform_log(const char *str);

// Get the current locale, in the format "xx_YY" where "xx" is the language
//  (en, fr, de...) and "_YY" is the country. (_US, _CA, etc). The country
//  can be omitted. Don't include encoding, it's always UTF-8 at this time.
// Return true if locale is known, false otherwise.
boolean MojoPlatform_locale(char *buf, size_t len);

boolean MojoPlatform_osType(char *buf, size_t len);
boolean MojoPlatform_osVersion(char *buf, size_t len);

// Basic platform detection.
#if PLATFORM_WINDOWS
#define PLATFORM_NAME "windows"
#elif PLATFORM_MACOSX
#define PLATFORM_NAME "macosx"
#elif PLATFORM_UNIX
#define PLATFORM_NAME "unix"
#elif PLATFORM_BEOS
#define PLATFORM_NAME "beos"
#else
#error Unknown platform.
#endif

// Basic architecture detection.

#if defined(__powerpc64__)
#define PLATFORM_ARCH "powerpc64"
#elif defined(__ppc__) || defined(__powerpc__) || defined(__POWERPC__)
#define PLATFORM_ARCH "powerpc"
#elif defined(__x86_64__) || defined(_M_X64)
#define PLATFORM_ARCH "x86-64"
#elif defined(__X86__) || defined(__i386__) || defined(i386) || defined (_M_IX86) || defined(__386__)
#define PLATFORM_ARCH "x86"
#else
#error Unknown processor architecture.
#endif

#ifdef __cplusplus
}
#endif

#endif

// end of platform.h ...

