/*
   Copyright (C) 2017 Peter S. Zhigalov <peter.zhigalov@gmail.com>

   This file is part of the `ImageViewer' program.

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

#include <cmath>
#include <cstring>
#include <cassert>
#include <algorithm>

#include <libwmf/api.h>
#include <libwmf/gd.h>

#include <QFileInfo>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QByteArray>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

#include "../IDecoder.h"
#include "Internal/DecoderAutoRegistrator.h"

namespace
{

// ====================================================================================================

const qreal MAX_IMAGE_DIMENSION = 16384;
const qreal MIN_IMAGE_DIMENSION = 1;

int sink(void *context, char *buffer, int length)
{
    QByteArray *imageData = reinterpret_cast<QByteArray*>(context);
    imageData->append(buffer, length);
    return imageData->size();
}

const char* wmfErrorToString(wmf_error_t error)
{
    switch(error)
    {
    case wmf_E_None:
        return "No error.";
    case wmf_E_InsMem:
        return "An attempt to allocate memory has failed.";
    case wmf_E_BadFile:
        return "Attempt to open an unreadable file, or to read from an unopened file.";
    case wmf_E_BadFormat:
        return "The metafile, if indeed it is a metafile, has been corrupted.";
    case wmf_E_EOF:
        return "An unexpected end-of-file has been reached.";
    case wmf_E_DeviceError:
        return "Device-layer error.";
    case wmf_E_Glitch:
        return "Programmer's error. Sorry.";
    case wmf_E_Assert:
        return "Internally forced error.";
    case wmf_E_UserExit:
        return "The status function has returned non-zero; exit is premature.";
    }
    assert(false);
    return "Unknown error.";
}

// ====================================================================================================

class WmfGraphicsItem : public QGraphicsItem
{
public:
    WmfGraphicsItem(const QString &filePath)
        : m_isValid(false)
        , m_API(NULL)
        , m_ddata(NULL)
        , m_minScaleFactor(1)
        , m_maxScaleFactor(1)
        , m_cachedScaleFactor(0)
    {
        memset(&m_bbox, 0, sizeof(wmfD_Rect));

        QFile inFile(filePath);
        if(!inFile.open(QIODevice::ReadOnly))
        {
            qWarning() << "Can't open" << filePath;
            return;
        }
        m_inBuffer = inFile.readAll();
        unsigned char *bufferData = reinterpret_cast<unsigned char*>(m_inBuffer.data());
        const long bufferSize = static_cast<long>(m_inBuffer.size());

        wmfAPI_Options m_options;
        memset(&m_options, 0, sizeof(wmfAPI_Options));
        m_options.function = wmf_gd_function;

        wmf_error_t error;

        unsigned long flags = WMF_OPT_FUNCTION | WMF_OPT_IGNORE_NONFATAL;
        error = wmf_api_create(&m_API, flags, &m_options);
        if(error != wmf_E_None)
        {
            qWarning() << "Can't create wmf API:" << wmfErrorToString(error);
            if(m_API)
                wmf_api_destroy(m_API);
            return;
        }

        m_ddata = WMF_GD_GetData(m_API);

        if((m_ddata->flags & WMF_GD_SUPPORTS_PNG) == 0)
        {
            qWarning() << "libwmf should be built with GD + PNG support";
            wmf_api_destroy(m_API);
            return;
        }

        error = wmf_mem_open(m_API, bufferData, bufferSize);
        if(error != wmf_E_None)
        {
            qWarning() << "Can't open memory:" << wmfErrorToString(error);
            wmf_api_destroy(m_API);
            return;
        }

        error = wmf_scan(m_API, 0, &m_bbox);
        if(error != wmf_E_None)
        {
            qWarning() << "Can't scan wmf:" << wmfErrorToString(error);
            wmf_mem_close(m_API);
            wmf_api_destroy(m_API);
            return;
        }

        m_isValid = true;
        const QRectF actualRect = boundingRect();
        m_minScaleFactor = std::max(MIN_IMAGE_DIMENSION / actualRect.width(), MIN_IMAGE_DIMENSION / actualRect.height());
        m_maxScaleFactor = std::min(MAX_IMAGE_DIMENSION / actualRect.width(), MAX_IMAGE_DIMENSION / actualRect.height());
    }

    ~WmfGraphicsItem()
    {
        if(!isValid())
            return;
        wmf_mem_close(m_API);
        wmf_api_destroy(m_API);
    }

    bool isValid() const
    {
        return m_isValid;
    }

    QRectF boundingRect() const
    {
        const qreal left   = static_cast<qreal>(m_bbox.TL.x);
        const qreal top    = static_cast<qreal>(m_bbox.TL.y);
        const qreal width  = static_cast<qreal>(m_bbox.BR.x - m_bbox.TL.x);
        const qreal height = static_cast<qreal>(m_bbox.BR.y - m_bbox.TL.y);
        return QRectF(left, top, width, height);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = NULL)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        if(!isValid())
            return;

        const QRectF identityRect = QRectF(0, 0, 1, 1);
        const QRectF deviceTransformedRect = painter->deviceTransform().mapRect(identityRect);
        const qreal deviceScaleFactor = std::max(deviceTransformedRect.width(), deviceTransformedRect.height());
        const qreal actualScaleFactor = std::min(std::max(m_minScaleFactor, deviceScaleFactor), m_maxScaleFactor);
        if(std::abs(actualScaleFactor - m_cachedScaleFactor) / actualScaleFactor > 1e-2)
        {
            const bool previousPixmapIsValid = !m_cachedPixmap.isNull();
            m_cachedPixmap = QPixmap();
            m_cachedPixmap = getPixmap(static_cast<float>(actualScaleFactor));
            if(m_cachedPixmap.isNull() && previousPixmapIsValid)
            {
                m_maxScaleFactor = m_cachedScaleFactor;
                m_cachedPixmap = getPixmap(static_cast<float>(m_cachedScaleFactor));
            }
            else
            {
                m_cachedScaleFactor = actualScaleFactor;
            }
        }
        painter->drawPixmap(boundingRect(), m_cachedPixmap, m_cachedPixmap.rect());
    }

protected:
    QPixmap getPixmap(const float scaleFactor)
    {
        QByteArray imageData;

        m_ddata->type = wmf_gd_png;
        m_ddata->flags |= WMF_GD_OUTPUT_MEMORY | WMF_GD_OWN_BUFFER;
        m_ddata->sink.function = &sink;
        m_ddata->sink.context = reinterpret_cast<void*>(&imageData);
        m_ddata->file = NULL;

        m_ddata->bbox.TL.x = m_bbox.TL.x * scaleFactor;
        m_ddata->bbox.TL.y = m_bbox.TL.y * scaleFactor;
        m_ddata->bbox.BR.x = m_bbox.BR.x * scaleFactor;
        m_ddata->bbox.BR.y = m_bbox.BR.y * scaleFactor;

        m_ddata->width  = static_cast<unsigned int>(std::ceil(m_ddata->bbox.BR.x - m_ddata->bbox.TL.x));
        m_ddata->height = static_cast<unsigned int>(std::ceil(m_ddata->bbox.BR.y - m_ddata->bbox.TL.y));

        const wmf_error_t error = wmf_play(m_API, 0, &m_ddata->bbox);
        if(error != wmf_E_None)
        {
            qWarning() << "Can't play wmf:" << wmfErrorToString(error);
            return QPixmap();
        }

        return QPixmap::fromImage(QImage::fromData(imageData));
    }

private:
    Q_DISABLE_COPY(WmfGraphicsItem)

    bool m_isValid;
    QByteArray m_inBuffer;
    wmfAPI *m_API;
    wmf_gd_t *m_ddata;
    wmfD_Rect m_bbox;
    qreal m_minScaleFactor;
    qreal m_maxScaleFactor;

    QPixmap m_cachedPixmap;
    qreal m_cachedScaleFactor;
};

// ====================================================================================================

class DecoderLibWmf : public IDecoder
{
public:
    QString name() const
    {
        return QString::fromLatin1("DecoderLibWmf");
    }

    QStringList supportedFormats() const
    {
        return QStringList()
                << QString::fromLatin1("wmf");
    }

    QStringList advancedFormats() const
    {
        return QStringList();
    }

    QGraphicsItem *loadImage(const QString &filePath)
    {
        const QFileInfo fileInfo(filePath);
        if(!fileInfo.exists() || !fileInfo.isReadable())
            return NULL;

        WmfGraphicsItem *item = new WmfGraphicsItem(filePath);
        if(!item->isValid())
        {
            delete item;
            return NULL;
        }
        return item;
    }
};

DecoderAutoRegistrator registrator(new DecoderLibWmf);

// ====================================================================================================

} // namespace
