/*
   Copyright (C) 2017-2018 Peter S. Zhigalov <peter.zhigalov@gmail.com>

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

#include <QGraphicsProxyWidget>
#include <QFileInfo>

#include "QtImageFormatsMovie.h"

#include "../IDecoder.h"
#include "Internal/DecoderAutoRegistrator.h"
#include "Internal/GraphicsItemsFactory.h"
#include "Internal/Animation/MovieAnimationProvider.h"

namespace {

class DecoderQtImageFormatsMovie : public IDecoder
{
public:
    QString name() const
    {
        return QString::fromLatin1("DecoderQtImageFormatsMovie");
    }

    QStringList supportedFormats() const
    {
        const QList<QByteArray> readerFormats = QtImageFormatsMovie::supportedFormats();
        QStringList result;
        for(QList<QByteArray>::ConstIterator it = readerFormats.constBegin(); it != readerFormats.constEnd(); ++it)
            result.append(QString::fromLatin1(*it).toLower());
        return result;
    }

    QStringList advancedFormats() const
    {
        return QStringList();
    }

    bool isAvailable() const
    {
        return true;
    }

    QGraphicsItem *loadImage(const QString &filePath)
    {
        const QFileInfo fileInfo(filePath);
        if(!fileInfo.exists() || !fileInfo.isReadable())
            return NULL;
        QtImageFormatsMovie *movie = new QtImageFormatsMovie(filePath);
        if(!movie->isValid() || movie->frameCount() == 1)
        {
            movie->deleteLater();
            return NULL;
        }
        return GraphicsItemsFactory::instance().createAnimatedItem(new MovieAnimationProvider<QtImageFormatsMovie>(movie));
    }
};

DecoderAutoRegistrator registrator(new DecoderQtImageFormatsMovie);

} // namespace
