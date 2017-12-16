/*
   Copyright (C) 2017 Peter S. Zhigalov <peter.zhigalov@gmail.com>

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

#if !defined (QTUTILS_OBJECTIVECUTILS_MAC_H_INCLUDED)
#define QTUTILS_OBJECTIVECUTILS_MAC_H_INCLUDED

#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

#include <QtGlobal>
#include <QVariant>
#include <QSize>

#define AUTORELEASE_POOL ::ObjCUtils::AutoReleasePool pool; (void)(pool)

namespace ObjCUtils {

class AutoReleasePool
{
    Q_DISABLE_COPY(AutoReleasePool)

public:
    AutoReleasePool();
    ~AutoReleasePool();

private:
    NSAutoreleasePool *m_pool;
};


template <typename T>
class CFRAII
{
public:
    inline CFRAII(const T &dataRef = static_cast<T>(0))
        : m_dataRef(dataRef)
    {}

    inline CFRAII(const CFRAII &other)
        : m_dataRef(other.m_dataRef)
    {
        if(m_dataRef)
            CFRetain(m_dataRef);
    }

    inline ~CFRAII()
    {
        if(m_dataRef)
            CFRelease(m_dataRef);
    }

    inline CFRAII operator = (const CFRAII &other)
    {
        if(this == &other)
            return;
        if(other.m_dataRef)
            CFRetain(other.m_dataRef);
        CFTypeRef oldDataRef = m_dataRef;
        m_dataRef = other.m_dataRef;
        if(oldDataRef)
            CFRelease(oldDataRef);
        return *this;
    }

    inline operator T ()
    {
        return m_dataRef;
    }

    inline T *operator & ()
    {
        return &m_dataRef;
    }

private:
    T m_dataRef;
};


QVariant QVariantFromObject(const id obj);
id QVariantToObject(const QVariant &variant);

QVariantMap QVariantMapFromNSDictionary(const NSDictionary *dict);
NSDictionary *QVariantMapToNSDictionary(const QVariantMap &map);

QVariantList QVariantListFromNSArray(const NSArray *array);
NSArray *QVariantListToNSArray(const QVariantList &list);

QString QStringFromNSString(const NSString *string);
NSString *QStringToNSString(const QString &string);

QByteArray QByteArrayFromNSData(const NSData *data);
NSData *QByteArrayToNSData(const QByteArray &array);

QDateTime QDateTimeFromNSDate(const NSDate *date);
NSDate *QDateTimeToNSDate(const QDateTime &dateTime);

QUrl QUrlFromNSURL(const NSURL *url);
NSURL *QUrlToNSURL(const QUrl &url);

QPixmap QPixmapFromNSImage(const NSImage *image);
NSImage *QPixmapToNSImage(const QPixmap &pixmap, const QSize& sizeInPoints = QSize());

QRectF QRectFFromNSRect(const NSRect &rect);
NSRect QRectFToNSRect(const QRectF &rect);

QSizeF QSizeFFromNSSize(const NSSize &size);
NSSize QSizeFToNSSize(const QSizeF &size);

QPointF QPointFFromNSPoint(const NSPoint &point);
NSPoint QPointFToNSPoint(const QPointF &point);

} // namespace ObjCUtils

#endif // QTUTILS_OBJECTIVECUTILS_MAC_H_INCLUDED

