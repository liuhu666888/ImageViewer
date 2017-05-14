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

#include "DecodersManager.h"

#include <map>
#include <set>
#include <cassert>

#include <QGraphicsItem>
#include <QFileInfo>
#include <QDebug>
#include <QTime>
#include <QMap>

namespace {

struct DecoderWithPriority
{
    DecoderWithPriority(IDecoder *decoder = NULL, int priority = 0)
        : decoder(decoder)
        , priority(priority)
    {}

    bool operator < (const DecoderWithPriority &other) const
    {
        return priority > other.priority;
    }

    IDecoder *decoder;
    int priority;
};

int GetDecoderPriority(const IDecoder *decoder)
{
    static QMap<QString, int> decoderPriotities;
    if(decoderPriotities.isEmpty())
    {
#define PD(NAME, PRIORITY) decoderPriotities[QString::fromLatin1(NAME)] = PRIORITY
#if defined (QT_DEBUG)
#define PR(NAME, PRIORITY) decoderPriotities[QString::fromLatin1(NAME)] = -1
#else
#define PR(NAME, PRIORITY) decoderPriotities[QString::fromLatin1(NAME)] = PRIORITY
#endif
        /// @note Декодеры статических изображений
        PD("DecoderSTB"                 ,  100);    ///< Резервный декодер, так как мало что умеет.
        PD("DecoderQImage"              ,  200);    ///< Умеет все, что умеет Qt. Не поддерживает EXIF и ICCP.
        PD("DecoderQtImageFormatsImage" ,  300);    ///< Экзотические и deprecated декодеры Qt. Должен быть выше QImage.
        PD("DecoderLibJpeg"             ,  400);    ///< Умеет jpeg форматы. Поддерживает EXIF и ICCP. Должен быть выше QImage.
        PD("DecoderLibJasPer"           ,  410);    ///< Умеет формат JPEG 2000 и несколько побочных. Поддержка хуже, чем в QtImageFormatsImage, но имеет ряд дополнительных проверок от крашей.
        PD("DecoderLibTiff"             ,  420);    ///< Умеет формат tiff. Поддерживает EXIF(?) и ICCP. Должен быть выше QImage и QtImageFormatsImage.
        PR("DecoderNSImage"             ,  900);    ///< Умеет очень много разных форматов.
        /// @note Декодеры анимированных изображений
        PD("DecoderQMovie"              , 1100);    ///< Умеет анимированные gif.
        PD("DecoderLibMng"              , 1110);    ///< Умеет анимированные mng и jng. Поддержка mng хуже, чем в QtImageFormatsMovie.
        PD("DecoderQtImageFormatsMovie" , 1200);    ///< Умеет анимированные mng.
        PD("DecoderLibPng"              , 1300);    ///< Умеет анимированные png. Поддерживает EXIF и ICCP.
        PD("DecoderLibWebP"             , 1310);    ///< Умеет анимированные webp. Поддержка лучше, чем в QtImageFormatsMovie.
        PD("DecoderLibBpg"              , 1320);    ///< Умеет анимированные bpg. Поддерживает EXIF и ICCP.
        /// @note Декодеры векторных изображений
        PD("DecoderQtSVG"               , 2100);    ///< Умеет svg, но очень плохо.
        PD("DecoderMacWebKit"           , 2200);    ///< Умеет неинтерактивные svg.
#undef PD
#undef PR
    }

    QMap<QString, int>::ConstIterator it = decoderPriotities.find(decoder->name());
    if(it != decoderPriotities.end())
        return it.value();

    static int unknownDecoderPriority = 10000;
    decoderPriotities[decoder->name()] = unknownDecoderPriority;
    qWarning() << "Unknown priority for decoder" << decoder->name();
    return unknownDecoderPriority++;
}

} // namespace

struct DecodersManager::Impl
{
    Impl()
    {}

    void checkPendingDecoderRegistration()
    {
        if(!pendingDecoders.isEmpty())
        {
            for(QList<IDecoder*>::ConstIterator it = pendingDecoders.constBegin(); it != pendingDecoders.constEnd(); ++it)
            {
                IDecoder *decoder = *it;
                const int priority = GetDecoderPriority(decoder);
                if(priority >= 0)
                {
                    decoders.insert(decoder);
                    const QStringList supportedFormats = decoder->supportedFormats();
                    for(QStringList::ConstIterator jt = supportedFormats.constBegin(); jt != supportedFormats.constEnd(); ++jt)
                        formats[*jt].insert(DecoderWithPriority(decoder, priority));
                    qDebug() << "Decoder" << decoder->name() << "was registered for" << supportedFormats << " with priority =" << priority;
                }
                else
                {
                    qDebug() << "Decoder" << decoder->name() << "was NOT registered with priority =" << priority;
                }
            }
            pendingDecoders.clear();
        }
    }

    std::set<IDecoder*> decoders;
    std::set<DecoderWithPriority> fallbackDecoders;
    std::map<QString, std::set<DecoderWithPriority> > formats;
    QList<IDecoder*> pendingDecoders;
};

DecodersManager::~DecodersManager()
{
    qDebug() << "DecodersManager destroyed!";
}

DecodersManager &DecodersManager::getInstance()
{
    static DecodersManager manager;
    return manager;
}

void DecodersManager::registerDecoder(IDecoder *decoder)
{
    m_impl->pendingDecoders.append(decoder);
    qDebug() << "Decoder" << decoder->name() << "was planned for delayed registration";
}

void DecodersManager::registerFallbackDecoder(IDecoder *decoder)
{
    const int fallbackPriority = GetDecoderPriority(decoder);
    m_impl->fallbackDecoders.insert(DecoderWithPriority(decoder, fallbackPriority));
    qDebug() << "Decoder" << decoder->name() << "was registered as FALLBACK with priority =" << fallbackPriority;
}

QStringList DecodersManager::registeredDecoders() const
{
    m_impl->checkPendingDecoderRegistration();
    QStringList result;
    for(std::set<IDecoder*>::const_iterator it = m_impl->decoders.begin(); it != m_impl->decoders.end(); ++it)
        result.append((*it)->name());
    return result;
}

QStringList DecodersManager::supportedFormats() const
{
    m_impl->checkPendingDecoderRegistration();
    QStringList result;
    for(std::map<QString, std::set<DecoderWithPriority> >::const_iterator it = m_impl->formats.begin(); it != m_impl->formats.end(); ++it)
        result.append(it->first);
    return result;
}

QStringList DecodersManager::supportedFormatsWithWildcards() const
{
    m_impl->checkPendingDecoderRegistration();
    QStringList result;
    for(std::map<QString, std::set<DecoderWithPriority> >::const_iterator it = m_impl->formats.begin(); it != m_impl->formats.end(); ++it)
        result.append(QString::fromLatin1("*.%1").arg(it->first));
    return result;
}

QGraphicsItem *DecodersManager::loadImage(const QString &filePath)
{
    m_impl->checkPendingDecoderRegistration();
    const QFileInfo fileInfo(filePath);
    if(!fileInfo.exists() || !fileInfo.isReadable())
    {
        qDebug() << "File" << filePath << "is not exist or unreadable!";
        return NULL;
    }

    std::set<IDecoder*> failedDecodres;

    const QString extension = fileInfo.suffix().toLower();
    std::map<QString, std::set<DecoderWithPriority> >::const_iterator formatData = m_impl->formats.find(extension);
    if(formatData != m_impl->formats.end())
    {
        for(std::set<DecoderWithPriority>::const_iterator decoderData = formatData->second.begin(); decoderData != formatData->second.end(); ++decoderData)
        {
            IDecoder *decoder = decoderData->decoder;
            QTime time;
            time.start();
            QGraphicsItem *item = decoder->loadImage(filePath);
            const int elapsed = time.elapsed();
            if(item)
            {
                qDebug() << "Successfully opened" << filePath << "with decoder" << decoder->name();
                qDebug() << "Elapsed time =" << elapsed << "ms";
                return item;
            }
            qDebug() << "Failed to open" << filePath << "with decoder" << decoder->name();
            qDebug() << "Elapsed time =" << elapsed << "ms";
            failedDecodres.insert(decoder);
        }
    }

    qDebug() << "Unknown format for file" << filePath << "with extension" << extension;
    for(std::set<DecoderWithPriority>::const_iterator decoderData = m_impl->fallbackDecoders.begin(); decoderData != m_impl->fallbackDecoders.end(); ++decoderData)
    {
        IDecoder *decoder = decoderData->decoder;
        if(failedDecodres.find(decoder) != failedDecodres.end())
            continue;
        QTime time;
        time.start();
        QGraphicsItem *item = decoder->loadImage(filePath);
        const int elapsed = time.elapsed();
        if(item)
        {
            qDebug() << "Successfully opened" << filePath << "with decoder" << decoder->name() << "(FALLBACK)";
            qDebug() << "Elapsed time =" << elapsed << "ms";
            return item;
        }
        qDebug() << "Failed to open" << filePath << "with decoder" << decoder->name() << "(FALLBACK)";
        qDebug() << "Elapsed time =" << elapsed << "ms";
        failedDecodres.insert(decoder);
    }
    return NULL;
}

DecodersManager::DecodersManager()
    : m_impl(new Impl())
{
    qDebug() << "DecodersManager created!";
}
