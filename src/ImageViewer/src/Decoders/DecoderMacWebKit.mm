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

#include <QFileInfo>
#include <QUrl>

#include "IDecoder.h"
#include "Internal/DecoderAutoRegistrator.h"
#include "Internal/MacWebKitRasterizerGraphicsItem.h"
#include "Internal/ZLibUtils.h"

//#if defined (QT_DEBUG)
//#define DECODER_MAC_WEBKIT_PRIORITY -1
//#else
#define DECODER_MAC_WEBKIT_PRIORITY 2000
//#endif

namespace {

class DecoderMacWebKit : public IDecoder
{
public:
    QString name() const
    {
        return QString::fromLatin1("DecoderMacWebKit");
    }

    QList<DecoderFormatInfo> supportedFormats() const
    {
        const QList<QByteArray> svgFormats = QList<QByteArray>()
                << "svg"
                << "svgz";
        QList<DecoderFormatInfo> result;
        for(QList<QByteArray>::ConstIterator it = svgFormats.constBegin(); it != svgFormats.constEnd(); ++it)
        {
            DecoderFormatInfo info;
            info.decoderPriority = DECODER_MAC_WEBKIT_PRIORITY;
            info.format = QString::fromLatin1(*it).toLower();
            result.append(info);
        }
        return result;
    }

    QGraphicsItem *loadImage(const QString &filePath)
    {
        const QFileInfo fileInfo(filePath);
        if(!fileInfo.exists() || !fileInfo.isReadable())
            return NULL;

        MacWebKitRasterizerGraphicsItem *result = NULL;
        if(fileInfo.suffix().toLower() == QString::fromLatin1("svgz"))
        {
            const QByteArray svgData = ZLibUtils::InflateFile(fileInfo.absoluteFilePath());
            if(!svgData.isEmpty())
                result = new MacWebKitRasterizerGraphicsItem(svgData, MacWebKitRasterizerGraphicsItem::DATA_TYPE_SVG);
        }
        else
        {
            result = new MacWebKitRasterizerGraphicsItem(QUrl(fileInfo.absoluteFilePath()));
        }

        if(result && result->state() != MacWebKitRasterizerGraphicsItem::STATE_SUCCEED)
        {
            result->deleteLater();
            result = NULL;
        }

        return result;
    }
};

DecoderAutoRegistrator registrator(new DecoderMacWebKit);

} // namespace
