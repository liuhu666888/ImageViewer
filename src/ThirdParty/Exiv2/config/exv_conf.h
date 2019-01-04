// File generated by cmake from cmake/config.h.cmake.

#ifndef _EXV_CONF_H_
#define _EXV_CONF_H_

// Defined if you want to use libssh for SshIO.
/* #undef EXV_USE_SSH */

// Define to 1 if you want to use libcurl in httpIO.
/* #undef EXV_USE_CURL */

// Define if you require webready support.
/* #undef EXV_ENABLE_WEBREADY */

// Define if you have the `gmtime_r' function.
/* #undef EXV_HAVE_GMTIME_R */

// Define if you have the <libintl.h> header file.
/* #undef EXV_HAVE_LIBINTL_H */

// Define if you want translation of program messages to the user's native language
/* #undef EXV_ENABLE_NLS */

// Define if you want video support.
/* #undef EXV_ENABLE_VIDEO */

// Define if you have the strerror_r function.
/* #undef EXV_HAVE_STRERROR_R */

// Define to enable the Windows unicode path support.
/* #undef EXV_UNICODE_PATH */

/* Define to `const' or to empty, depending on the second argument of `iconv'. */
/* #undef ICONV_ACCEPTS_CONST_INPUT */
#if defined ICONV_ACCEPTS_CONST_INPUT
#define EXV_ICONV_CONST const
#else
#define EXV_ICONV_CONST
#endif

// Define if you have the <regex.h> header file.
/* #undef EXV_HAVE_REGEX_H */

// Define if you have the mmap function.
#define EXV_HAVE_MMAP

// Define if you have the munmap function.
#define EXV_HAVE_MUNMAP

/* Define if you have the <unistd.h> header file. */
#define EXV_HAVE_UNISTD_H

// Define if you have the <sys/mman.h> header file.
#define EXV_HAVE_SYS_MMAN_H

// Define if you have are using the zlib library.
#if defined (HAS_ZLIB)
#define EXV_HAVE_LIBZ
#endif

/* Define if you have (Exiv2/xmpsdk) Adobe XMP Toolkit. */
#define EXV_HAVE_XMP_TOOLKIT

/* Define to the full name of this package. */
#define EXV_PACKAGE_NAME "exiv2"

/* Define to the full name and version of this package. */
#define EXV_PACKAGE_STRING "exiv2 0.27.0"

/* Define to the version of this package. */
#define EXV_PACKAGE_VERSION "0.27.0"

#define EXIV2_MAJOR_VERSION (0)
#define EXIV2_MINOR_VERSION (27)
#define EXIV2_PATCH_VERSION (0)
#define EXIV2_TWEAK_VERSION ()

// Definition to enable translation of Nikon lens names.
#define EXV_HAVE_LENSDATA

// Define if you have the iconv function.
#define EXV_HAVE_ICONV

// Definition to enable conversion of UCS2 encoded Windows tags to UTF-8.
#define EXV_HAVE_PRINTUCS2

#endif /* !_EXV_CONF_H_ */
