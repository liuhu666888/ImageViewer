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

#if !defined(GUISETTINGS_H_INCLUDED)
#define GUISETTINGS_H_INCLUDED

#include <QObject>
#include <QString>
#include <QColor>
#include "Utils/ScopedPointer.h"
#include "ImageViewerWidget.h"

class GUISettings : public QObject
{
    Q_OBJECT

signals:
    void askBeforeDeleteChanged(bool enabled);
    void moveToTrashChanged(bool enabled);
    void zoomModeChanged(ImageViewerWidget::ZoomMode mode);
    void zoomLevelChanged(qreal level);
    void backgroundColorChanged(const QColor &color);
    void lastOpenedPathChanged(const QString &path);

public:
    GUISettings(QObject *parent = NULL);
    ~GUISettings();

    bool askBeforeDelete() const;
    void setAskBeforeDelete(bool enabled);

    bool moveToTrash() const;
    void setMoveToTrash(bool enabled);

    ImageViewerWidget::ZoomMode zoomMode() const;
    void setZoomMode(ImageViewerWidget::ZoomMode mode);

    qreal zoomLevel() const;
    void setZoomLevel(qreal level);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QString lastOpenedPath() const;
    void setLastOpenedPath(const QString &path);

private:
    struct Impl;
    QScopedPointer<Impl> m_impl;
};

#endif // GUISETTINGS_H_INCLUDED
