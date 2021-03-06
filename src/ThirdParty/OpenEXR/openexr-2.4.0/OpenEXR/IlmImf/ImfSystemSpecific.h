///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2012, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IMF_COMPILER_SPECIFIC_H
#define INCLUDED_IMF_COMPILER_SPECIFIC_H

#include "ImfNamespace.h"
#include "ImfSimd.h"
#if defined(_WIN32) && defined(__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
# include <errno.h>
# include <stdint.h>
#endif
#include <stdlib.h>
#include "ImfExport.h"

OPENEXR_IMF_INTERNAL_NAMESPACE_HEADER_ENTER


static unsigned long  systemEndianCheckValue   = 0x12345678;
static unsigned long* systemEndianCheckPointer = &systemEndianCheckValue;

// EXR files are little endian - check processor architecture is too
// (optimisation currently not supported for big endian machines)
static bool GLOBAL_SYSTEM_LITTLE_ENDIAN =
        (*(unsigned char*)systemEndianCheckPointer == 0x78 ? true : false);

inline void*
EXRAllocAligned (size_t size, size_t alignment)
{
    // GNUC is used for things like mingw to (cross-)compile for windows
#if defined(_WIN32) && defined(__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
# define EXR_NOT_POWER_OF_TWO(n) (((n) & ((n) - 1)))
# define EXR_UI(p) ((uintptr_t) (p))
# define EXR_PTR_ALIGN(p0, alignment) ((void*) ((EXR_UI(p0) + (alignment + sizeof(void*))) & (~EXR_UI(alignment - 1))))
# define EXR_ORIG_PTR(p) (*(((void**) (EXR_UI(p) & (~EXR_UI(sizeof(void*) - 1)))) - 1))
    if (EXR_NOT_POWER_OF_TWO (alignment))
    {
        errno = EINVAL;
        return NULL;
    }
    if (size == 0)
        return NULL;
    if (alignment < sizeof (void*))
        alignment = sizeof (void*);

    void* p0 = malloc (size + (alignment + sizeof (void*)));
    if (!p0)
        return NULL;
    void* p = EXR_PTR_ALIGN (p0, alignment);
    EXR_ORIG_PTR (p) = p0;
    return p;
#elif defined(_WIN32)
    return _aligned_malloc (size, alignment);
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
    return _mm_malloc (size, alignment);
#elif defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L)
    void* ptr = 0;
    // With fortify_source on, just doing the (void) cast trick
    // doesn't remove the unused result warning but we expect the
    // other mallocs to return null and to check the return value
    // of this function
    if ( posix_memalign (&ptr, alignment, size) )
        ptr = 0;
    return ptr;
#else
    return malloc(size);
#endif
}

inline void
EXRFreeAligned (void* ptr)
{
#if defined(_WIN32) && defined(__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
    if (ptr)
      free (EXR_ORIG_PTR (ptr));
# undef EXR_NOT_POWER_OF_TWO
# undef EXR_UI
# undef EXR_PTR_ALIGN
# undef EXR_ORIG_PTR
#elif defined(_WIN32)
    _aligned_free (ptr);
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) ||         \
    defined(__ECC)
    _mm_free (ptr);
#else
    free (ptr);
#endif
}

#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
// Causes issues on certain gcc versions
//#define EXR_FORCEINLINE inline __attribute__((always_inline))
#define EXR_FORCEINLINE inline
#define EXR_RESTRICT __restrict

#else

// generic compiler
#define EXR_FORCEINLINE inline
#define EXR_RESTRICT

#endif

// 
// Simple CPUID based runtime detection of various capabilities
//
class IMF_EXPORT CpuId
{
    public:
        CpuId();

        bool sse2;
        bool sse3;
        bool ssse3;
        bool sse4_1;
        bool sse4_2;
        bool avx;
        bool f16c;
};


OPENEXR_IMF_INTERNAL_NAMESPACE_HEADER_EXIT


#endif //include guard
