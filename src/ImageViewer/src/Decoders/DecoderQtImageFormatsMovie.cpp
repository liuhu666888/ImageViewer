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

#include "DecoderQtImageFormatsMovie.h"

#include <QGraphicsProxyWidget>
#include <QFileInfo>

#if defined (HAS_THIRDPARTY_QTIMAGEFORMATS)
#include "QtImageFormatsMovie.h"
#include "QtImageFormatsMovieLabel.h"
#endif

#include "DecoderAutoRegistrator.h"

#define DECODER_QTIMAREFORMATS_MOVIE_PRIORITY 170

namespace {

DecoderAutoRegistrator registrator(new DecoderQtImageFormatsMovie);

} // namespace

QString DecoderQtImageFormatsMovie::name() const
{
    return QString::fromLatin1("DecoderQtImageFormatsMovie");
}

QList<DecoderFormatInfo> DecoderQtImageFormatsMovie::supportedFormats() const
{
#if defined (HAS_THIRDPARTY_QTIMAGEFORMATS)
    const QList<QByteArray> readerFormats = QtImageFormatsMovie::supportedFormats();
    QList<DecoderFormatInfo> result;
    for(QList<QByteArray>::ConstIterator it = readerFormats.constBegin(); it != readerFormats.constEnd(); ++it)
    {
        const QString format = QString::fromLatin1(*it).toLower();
        DecoderFormatInfo info;
        info.decoderPriority = DECODER_QTIMAREFORMATS_MOVIE_PRIORITY;
        info.format = format;
        result.append(info);
    }
    return result;
#else
    return QList<DecoderFormatInfo>();
#endif
}

QGraphicsItem *DecoderQtImageFormatsMovie::loadImage(const QString &filePath)
{
    const QFileInfo fileInfo(filePath);
    if(!fileInfo.exists() || !fileInfo.isReadable())
        return NULL;
#if defined (HAS_THIRDPARTY_QTIMAGEFORMATS)
    QtImageFormatsMovie *movie = new QtImageFormatsMovie(filePath);
    if(!movie->isValid())
    {
        movie->deleteLater();
        return NULL;
    }
    QtImageFormatsMovieLabel *movieLabel = new QtImageFormatsMovieLabel();
    movieLabel->setAttribute(Qt::WA_NoSystemBackground, true);
    movieLabel->setMovie(movie);
    movie->setParent(movieLabel);
    movie->start();
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
    proxy->setWidget(movieLabel);
    return proxy;
#else
    return NULL;
#endif
}