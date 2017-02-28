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

} // namespace

struct DecodersManager::Impl
{
    Impl()
        : defaultDecoder(NULL)
    {}

    void checkPendingDecoderRegistration()
    {
        if(!pendingDecoders.isEmpty())
        {
            for(QList<IDecoder*>::ConstIterator it = pendingDecoders.constBegin(); it != pendingDecoders.constEnd(); ++it)
            {
                IDecoder *decoder = *it;
                decoders.insert(decoder);
                const QList<DecoderFormatInfo> info = decoder->supportedFormats();
                QStringList debugFormatList;
                for(QList<DecoderFormatInfo>::ConstIterator it = info.constBegin(); it != info.constEnd(); ++it)
                {
                    formats[it->format].insert(DecoderWithPriority(decoder, it->decoderPriority));
                    debugFormatList.append(it->format);
                }
                qDebug() << "Decoder" << decoder->name() << "registered for" << debugFormatList;
            }
            pendingDecoders.clear();
        }
    }

    IDecoder *defaultDecoder;
    std::set<IDecoder*> decoders;
    std::map<QString, std::set<DecoderWithPriority> > formats;
    QList<IDecoder*> pendingDecoders;
};

DecodersManager::DecodersManager()
    : m_impl(new Impl())
{
    qDebug() << "DecodersManager created!";
}

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

void DecodersManager::registerDefaultDecoder(IDecoder *decoder)
{
    assert(!m_impl->defaultDecoder);
    registerDecoder(decoder);
    m_impl->defaultDecoder = decoder;
    qDebug() << "Decoder" << decoder->name() << "registered as DEFAULT!";
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

QGraphicsItem *DecodersManager::loadImage(const QString &filename)
{
    m_impl->checkPendingDecoderRegistration();
    const QFileInfo fileInfo(filename);
    if(!fileInfo.exists() || !fileInfo.isReadable())
    {
        qDebug() << "File" << filename << "is not exist or unreadable!";
        return NULL;
    }

    const QString extension = fileInfo.suffix().toLower();
    std::map<QString, std::set<DecoderWithPriority> >::const_iterator formatData = m_impl->formats.find(extension);
    if(formatData != m_impl->formats.end())
    {
        for(std::set<DecoderWithPriority>::const_iterator decoderData = formatData->second.begin(); decoderData != formatData->second.end(); ++decoderData)
        {
            IDecoder *decoder = decoderData->decoder;
            QGraphicsItem *item = decoder->loadImage(filename);
            if(item)
            {
                qDebug() << "Successfully opened" << filename << "with decoder" << decoder->name();
                return item;
            }
            else
            {
                qDebug() << "Failed to open" << filename << "with decoder" << decoder->name();
            }
        }
    }

    qDebug() << "Unknown format for file" << filename << "with extension" << extension;
    if(m_impl->defaultDecoder)
    {
        QGraphicsItem *item = m_impl->defaultDecoder->loadImage(filename);
        if(item)
        {
            qDebug() << "Successfully opened" << filename << "with decoder" << m_impl->defaultDecoder->name() << "(DEFAULT)";
            return item;
        }
    }
    return NULL;
}
