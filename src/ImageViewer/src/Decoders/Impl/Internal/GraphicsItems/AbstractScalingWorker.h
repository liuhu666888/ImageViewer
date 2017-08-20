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

#if !defined(ABSTRACT_SCALING_WORKER_H_INCLUDED)
#define ABSTRACT_SCALING_WORKER_H_INCLUDED

#include <QObject>
#include <QPixmap>
#include <QMutex>

#include "Utils/ScopedPointer.h"

class AbstractScalingWorker : public QObject
{
    Q_OBJECT

public:
    AbstractScalingWorker(QObject *parent = NULL);
    ~AbstractScalingWorker();

    void setScaleFactor(const qreal newScaleFactor);
    qreal getScaleFactor() const;

    void lockScaledImage();
    void unlockScaledImage();

    bool hasScaledData() const;
    QPixmap getScaledPixmap() const;
    qreal getScaledScaleFactor() const;

public slots:
    void process();
    void abort();

signals:
    void started();
    void finished();
    void aborted();

protected:
    bool isAborted() const;
    virtual bool scaleImpl() = 0;

    qreal m_scaleFactor;
    struct ScaledImageData
    {
        QPixmap pixmap;
        qreal scaleFactor;
        ScaledImageData(const QPixmap& pixmap, const qreal scaleFactor);
    };
    QScopedPointer<ScaledImageData> m_scaledData;

private:
    bool m_workerAborted;
    QMutex m_scaledDataLock;
};

#endif // ABSTRACT_SCALING_WORKER_H_INCLUDED
