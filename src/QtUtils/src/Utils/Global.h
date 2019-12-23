/*
   Copyright (C) 2018-2019 Peter S. Zhigalov <peter.zhigalov@gmail.com>

   This file is part of the `QtUtils' library.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined (QTUTILS_GLOBAL_H_INCLUDED)
#define QTUTILS_GLOBAL_H_INCLUDED

#include <cstddef>

#include <QtGlobal>

#if (QT_VERSION < QT_VERSION_CHECK(4, 6, 0))

static inline bool qFuzzyIsNull(double d)
{
    return qAbs(d) <= 0.000000000001;
}

static inline bool qFuzzyIsNull(float f)
{
    return qAbs(f) <= 0.00001f;
}

#endif

#if !defined (QT_HAS_ATTRIBUTE)
#   if defined (__has_attribute)
#       define QT_HAS_ATTRIBUTE(x) __has_attribute(x)
#   else
#       define QT_HAS_ATTRIBUTE(x) 0
#   endif
#endif

#if !defined (QT_HAS_CPP_ATTRIBUTE)
#   if defined (__has_cpp_attribute)
#       define QT_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#   else
#       define QT_HAS_CPP_ATTRIBUTE(x) 0
#   endif
#endif

#if !defined (Q_FALLTHROUGH)
#   if defined (__cplusplus)
#       if QT_HAS_CPP_ATTRIBUTE(clang::fallthrough)
#           define Q_FALLTHROUGH() [[clang::fallthrough]]
#       elif QT_HAS_CPP_ATTRIBUTE(gnu::fallthrough)
#           define Q_FALLTHROUGH() [[gnu::fallthrough]]
#       elif QT_HAS_CPP_ATTRIBUTE(fallthrough)
#           define Q_FALLTHROUGH() [[fallthrough]]
#       endif
#   endif
#   if !defined (Q_FALLTHROUGH)
#       if (defined (Q_CC_GNU) && ((Q_CC_GNU + 0) >= 700)) && !defined (Q_CC_INTEL)
#           define Q_FALLTHROUGH() __attribute__((fallthrough))
#       else
#           define Q_FALLTHROUGH() (void)0
#       endif
#   endif
#endif

#if !defined (Q_DECL_OVERRIDE)
#   if defined (__cplusplus) && (__cplusplus >= 201103L)
#       define Q_DECL_OVERRIDE override
#   else
#       define Q_DECL_OVERRIDE
#   endif
#endif

#if !defined (Q_DECL_FINAL)
#   if defined (__cplusplus) && (__cplusplus >= 201103L)
#       define Q_DECL_FINAL final
#   else
#       define Q_DECL_FINAL
#   endif
#endif

#if !defined (Q_NULLPTR)
#   if defined (__cplusplus) && (__cplusplus >= 201103L)
#       define Q_NULLPTR nullptr
#   else
#       define Q_NULLPTR NULL
#   endif
#endif

#endif // QTUTILS_GLOBAL_H_INCLUDED

