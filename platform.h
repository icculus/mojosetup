#ifndef _INCL_PLATFORM_H_
#define _INCL_PLATFORM_H_

#include "universal.h"

#ifdef __cplusplus
extern "C" {
#endif

// this is called by your mainline.
int MojoSetup_main(int argc, char **argv);

// Caller should _NOT_ free returned string...it's calculated on the first
//  call and reused for future calls.
// !!! FIXME: is that a good idea?
const char *MojoPlatform_appBinaryPath(void);

// Get the current locale, in the format "xx_YY" where "xx" is the language
//  (en, fr, de...) and "_YY" is the country. (_US, _CA, etc). The country
//  can be omitted. Don't include encoding, it's always UTF-8 at this time.
// Return true if locale is known, false otherwise.
boolean MojoPlatform_locale(char *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif

// end of platform.h ...

