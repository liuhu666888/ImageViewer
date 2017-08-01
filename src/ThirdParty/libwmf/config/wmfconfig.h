/* wmfconfig.h.  Generated by configure.  */
/* wmfconfig.h.in.  Generated from configure.ac by autoheader.  */


#ifndef LIBWMF_CONFIG_H
#define LIBWMF_CONFIG_H


/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */

/* define if you have expat */
/* #undef HAVE_EXPAT */

/* Define to 1 if you have the `fprintf' function. */
#define HAVE_FPRINTF 1

/* Define to 1 if you have the `fscanf' function. */
#define HAVE_FSCANF 1

/* define if you have libgd */
#define HAVE_GD 1

/* Define to 1 if you have the <inttypes.h> header file. */
/* #undef HAVE_INTTYPES_H */

/* define if you have libjpeg */
#define HAVE_LIBJPEG 1

/* define if you have libplot */
/* #undef HAVE_LIBPLOT */

/* define if you have libpng */
#define HAVE_LIBPNG 1

/* define if you have libxml2 */
/* #undef HAVE_LIBXML2 */

/* Define to 1 if you have the <memory.h> header file. */
/* #undef HAVE_MEMORY_H */

/* Define to 1 if you have the `printf' function. */
#define HAVE_PRINTF 1

/* Define to 1 if you have the `scanf' function. */
#define HAVE_SCANF 1

/* define if you have snprintf */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the `sprintf' function. */
#define HAVE_SPRINTF 1

/* Define to 1 if you have the `sscanf' function. */
#define HAVE_SSCANF 1

/* Define to 1 if you have the <stdint.h> header file. */
/* #undef HAVE_STDINT_H */

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
/* #undef HAVE_STRINGS_H */

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the <sys/stat.h> header file. */
/* #undef HAVE_SYS_STAT_H */

/* Define to 1 if you have the <sys/types.h> header file. */
/* #undef HAVE_SYS_TYPES_H */

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* define if you have unistd.h */
#if !defined (_WIN32)
#define HAVE_UNISTD_H 1
#endif

/* Define to 1 if you have the `vfprintf' function. */
#define HAVE_VFPRINTF 1

/* define if you have vfscanf */
#define HAVE_VFSCANF 1

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the `vscanf' function. */
#define HAVE_VSCANF 1

/* define if you have vsnprintf */
#define HAVE_VSNPRINTF 1

/* Define to 1 if you have the `vsprintf' function. */
#define HAVE_VSPRINTF 1

/* Define to 1 if you have the `vsscanf' function. */
#define HAVE_VSSCANF 1

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "wvware-devel@sourceforge.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "libwmf"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libwmf 0.2.8"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libwmf"

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.2.8"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* If any *printf or *scanf functions are missing, cover them with trio
   functions */
/* #undef TRIO_REPLACE_STDIO */

/* defining this will disable egs, fig, svg & foreign layers */
/* #undef WITHOUT_LAYERS */

/* Define to 1 if the X Window System is missing or not being used. */
#define X_DISPLAY_MISSING 1

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `long' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `unsigned' if <sys/types.h> does not define. */
/* #undef size_t */

/* define to your implementation of snprintf */
/* #undef snprintf */

/* define to your implementation of vsnprintf */
/* #undef vsnprintf */


/* custom fonts provider */
#include "fontsprovider/fontsprovider.h"
#define WMF_FONTDIR         ProvideWmfFontdir()
#define WMF_GS_FONTDIR      ProvideWmfGsFontdir()
#define WMF_SYS_FONTMAP     ProvideWmfSysFontmap()
#define WMF_XTRA_FONTMAP    ProvideWmfXtraFontmap()
#define WMF_GS_FONTMAP      ProvideWmfGsFontmap()

#endif /* ! LIBWMF_CONFIG_H */

