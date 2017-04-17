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

#include "GUISettings.h"
#include <limits>
#include "Utils/SettingsWrapper.h"

namespace {

ImageViewerWidget::ZoomMode zoomModeFromVariant(const QVariant &variant, ImageViewerWidget::ZoomMode defaultValue)
{
    bool ok;
    int value = variant.toInt(&ok);
    if(!ok)
        return defaultValue;
    switch(value)
    {
    case ImageViewerWidget::ZOOM_IDENTITY:
        return ImageViewerWidget::ZOOM_IDENTITY;
    case ImageViewerWidget::ZOOM_FIT_TO_WINDOW:
        return ImageViewerWidget::ZOOM_FIT_TO_WINDOW;
    case ImageViewerWidget::ZOOM_CUSTOM:
        return ImageViewerWidget::ZOOM_CUSTOM;
    default:
        break;
    }
    return defaultValue;
}

QVariant zoomModeToVariant(ImageViewerWidget::ZoomMode mode)
{
    return mode;
}

const QString ASK_BEFORE_DELETE_KEY             = QString::fromLatin1("AskBeforeDelete");
const QString MOVE_TO_TRASH_KEY                 = QString::fromLatin1("MoveToTrash");
const QString ZOOM_MODE_KEY                     = QString::fromLatin1("ZoomMode");
const QString ZOOM_LEVEL_KEY                    = QString::fromLatin1("ZoomLevel");
const QString NORMAL_BACKGROUND_COLOR_KEY       = QString::fromLatin1("NormalBackgroundColor");
const QString FULLSCREEN_BACKGROUND_COLOR_KEY   = QString::fromLatin1("FullScreenBackgroundColor");
const QString LAST_OPENED_PATH_KEY              = QString::fromLatin1("LastOpenedPath");
const QString SMOOTH_TRANSFORMATION_KEY         = QString::fromLatin1("SmoothTransformation");
const QString MAIN_WINDOW_GEOMETRY_KEY          = QString::fromLatin1("MainWindowGeometry");
const QString MAIN_WINDOW_STATE_KEY             = QString::fromLatin1("MainWindowState");
const QString SLIDESHOW_INTERVAL_KEY            = QString::fromLatin1("SlideShowInterval");

} // namespace

struct GUISettings::Impl
{
    Impl()
        : settings(QString::fromLatin1("GUISettings"))
    {}
    SettingsWrapper settings;
};

GUISettings::GUISettings(QObject *parent)
    : QObject(parent)
    , m_impl(new Impl)
{}

GUISettings::~GUISettings()
{}

bool GUISettings::askBeforeDelete() const
{
    const bool defaultValue = true;
    QVariant value = m_impl->settings.value(ASK_BEFORE_DELETE_KEY, defaultValue);
    return value.isValid() && value.canConvert(QVariant::Bool) ? value.toBool() : defaultValue;
}

void GUISettings::setAskBeforeDelete(bool enabled)
{
    const bool oldValue = askBeforeDelete();
    m_impl->settings.setValue(ASK_BEFORE_DELETE_KEY, enabled);
    if(enabled != oldValue)
        emit askBeforeDeleteChanged(enabled);
}

bool GUISettings::moveToTrash() const
{
    const bool defaultValue = true;
    QVariant value = m_impl->settings.value(MOVE_TO_TRASH_KEY, defaultValue);
    return value.isValid() && value.canConvert(QVariant::Bool) ? value.toBool() : defaultValue;
}

void GUISettings::setMoveToTrash(bool enabled)
{
    const bool oldValue = moveToTrash();
    m_impl->settings.setValue(MOVE_TO_TRASH_KEY, enabled);
    if(enabled != oldValue)
        emit moveToTrashChanged(enabled);
}

ImageViewerWidget::ZoomMode GUISettings::zoomMode() const
{
    const ImageViewerWidget::ZoomMode defaultMode = ImageViewerWidget::ZOOM_FIT_TO_WINDOW;
    const QVariant defaultValue = zoomModeToVariant(defaultMode);
    QVariant value = m_impl->settings.value(ZOOM_MODE_KEY, defaultValue);
    return zoomModeFromVariant((value.isValid() ? value : defaultValue), defaultMode);
}

void GUISettings::setZoomMode(ImageViewerWidget::ZoomMode mode)
{
    const ImageViewerWidget::ZoomMode oldValue = zoomMode();
    m_impl->settings.setValue(ZOOM_MODE_KEY, zoomModeToVariant(mode));
    if(mode != oldValue)
        emit zoomModeChanged(mode);
}

qreal GUISettings::zoomLevel() const
{
    const qreal defaultValue = 1;
    QVariant variant = m_impl->settings.value(ZOOM_LEVEL_KEY);
    if(!variant.isValid() || !variant.canConvert(QVariant::Double))
        return defaultValue;
    bool ok;
    qreal value = variant.toDouble(&ok);
    return (ok && value > 0 && value < std::numeric_limits<qreal>::max()) ? static_cast<qreal>(value) : defaultValue;
}

void GUISettings::setZoomLevel(qreal level)
{
    const qreal oldValue = zoomLevel();
    m_impl->settings.setValue(ZOOM_LEVEL_KEY, level);
    if(level != oldValue)
        emit zoomLevelChanged(level);
}

QColor GUISettings::normalBackgroundColor() const
{
    const QColor defaultValue(255, 255, 255, 128);
    QVariant value = m_impl->settings.value(NORMAL_BACKGROUND_COLOR_KEY, defaultValue);
    return (value.isValid() && value.canConvert(QVariant::Color) ? value.value<QColor>() : defaultValue);
}

void GUISettings::setNormalBackgroundColor(const QColor &color)
{
    const QColor oldValue = normalBackgroundColor();
    m_impl->settings.setValue(NORMAL_BACKGROUND_COLOR_KEY, color);
    if(color != oldValue)
        emit normalBackgroundColorChanged(color);
}

QColor GUISettings::fullScreenBackgroundColor() const
{
    const QColor defaultValue(0, 0, 0, 128);
    QVariant value = m_impl->settings.value(FULLSCREEN_BACKGROUND_COLOR_KEY, defaultValue);
    return (value.isValid() && value.canConvert(QVariant::Color) ? value.value<QColor>() : defaultValue);
}

void GUISettings::setFullScreenBackgroundColor(const QColor &color)
{
    const QColor oldValue = fullScreenBackgroundColor();
    m_impl->settings.setValue(FULLSCREEN_BACKGROUND_COLOR_KEY, color);
    if(color != oldValue)
        emit fullScreenBackgroundColorChanged(color);
}

QString GUISettings::lastOpenedPath() const
{
    const QString defaultValue;
    QVariant value = m_impl->settings.value(LAST_OPENED_PATH_KEY, defaultValue);
    return (value.isValid() && value.canConvert(QVariant::String) ? value.toString() : defaultValue);
}

void GUISettings::setLastOpenedPath(const QString &path)
{
    const QString oldValue = lastOpenedPath();
    m_impl->settings.setValue(LAST_OPENED_PATH_KEY, path);
    if(path != oldValue)
        emit lastOpenedPathChanged(path);
}

bool GUISettings::smoothTransformation() const
{
    const bool defaultValue = true;
    QVariant value = m_impl->settings.value(SMOOTH_TRANSFORMATION_KEY, defaultValue);
    return value.isValid() && value.canConvert(QVariant::Bool) ? value.toBool() : defaultValue;
}

void GUISettings::setSmoothTransformation(bool enabled)
{
    const bool oldValue = smoothTransformation();
    m_impl->settings.setValue(SMOOTH_TRANSFORMATION_KEY, enabled);
    if(enabled != oldValue)
        emit smoothTransformationChanged(enabled);
}

QByteArray GUISettings::mainWindowGeometry() const
{
    QVariant value = m_impl->settings.value(MAIN_WINDOW_GEOMETRY_KEY);
    return value.isValid() && value.canConvert(QVariant::ByteArray) ? value.toByteArray() : QByteArray();
}

void GUISettings::setMainWindowGeometry(const QByteArray &geometry)
{
    const QByteArray oldValue = mainWindowGeometry();
    m_impl->settings.setValue(MAIN_WINDOW_GEOMETRY_KEY, geometry);
    if(geometry != oldValue)
        emit mainWindowGeometryChanged(geometry);
}

QByteArray GUISettings::mainWindowState() const
{
    QVariant value = m_impl->settings.value(MAIN_WINDOW_STATE_KEY);
    return value.isValid() && value.canConvert(QVariant::ByteArray) ? value.toByteArray() : QByteArray();
}

void GUISettings::setMainWindowState(const QByteArray &state)
{
    const QByteArray oldValue = mainWindowState();
    m_impl->settings.setValue(MAIN_WINDOW_STATE_KEY, state);
    if(state != oldValue)
        emit mainWindowStateChanged(state);
}

int GUISettings::slideShowInterval() const
{
    const int defaultValue = 5;
    QVariant variant = m_impl->settings.value(SLIDESHOW_INTERVAL_KEY);
    if(!variant.isValid() || !variant.canConvert(QVariant::Int))
        return defaultValue;
    bool ok;
    int value = variant.toInt(&ok);
    return (ok && value >= 1 && value <= 1000) ? value : defaultValue;
}

void GUISettings::setSlideShowInterval(int seconds)
{
    const int oldValue = slideShowInterval();
    m_impl->settings.setValue(SLIDESHOW_INTERVAL_KEY, seconds);
    if(seconds != oldValue)
        emit slideShowIntervalChanged(seconds);
}
