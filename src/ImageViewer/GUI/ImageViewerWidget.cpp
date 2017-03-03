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

#include "ImageViewerWidget.h"

#include <QGraphicsScene>
#include <QGraphicsItem>

struct ImageViewerWidget::Impl
{
    Impl(ImageViewerWidget *widget)
        : imageViewerWidget(widget)
        , scene(new QGraphicsScene(widget))
        , currentGraphicsItem(NULL)
        , currentZoomMode(ZOOM_IDENTITY)
        , previousZoomMode(-1)
        , currentZoomLevel(1)
        , previousZoomLevel(-1)
        , currentRotationAngle(0)
    {
        widget->setScene(scene);
    }

    void updateTransformations()
    {
        if(!currentGraphicsItem)
            return;

        imageViewerWidget->resetTransform();
        imageViewerWidget->rotate(currentRotationAngle);
        switch(currentZoomMode)
        {
        case ZOOM_IDENTITY:
        {
            currentZoomLevel = 1;
            break;
        }
        case ZOOM_FIT_TO_WINDOW:
        {
            const QRectF boundingRect = currentGraphicsItem->boundingRect();
            const QSize imageSize = boundingRect.size().toSize();
            const QSize windowSize = imageViewerWidget->size();
            if(imageSize.width() > windowSize.width() || imageSize.height() > windowSize.height())
            {
                imageViewerWidget->fitInView(boundingRect, Qt::KeepAspectRatio);
                const qreal deltaWidth = qreal(windowSize.width()) / qreal(imageSize.width());
                const qreal deltaHeight = qreal(windowSize.height()) / qreal(imageSize.height());
                currentZoomLevel = qMin(deltaWidth, deltaHeight);
            }
            else
            {
                currentZoomLevel = 1;
            }
            break;
        }
        case ZOOM_CUSTOM:
        {
            imageViewerWidget->scale(currentZoomLevel, currentZoomLevel);
            break;
        }
        }

        if(previousZoomLevel != currentZoomLevel)
        {
            emit imageViewerWidget->zoomLevelChanged(currentZoomLevel);
            previousZoomLevel = currentZoomLevel;
        }
        if(previousZoomMode != currentZoomMode)
        {
            emit imageViewerWidget->zoomModeChanged(currentZoomMode);
            previousZoomMode = currentZoomMode;
        }
    }

    ImageViewerWidget *imageViewerWidget;
    QGraphicsScene *scene;
    QGraphicsItem *currentGraphicsItem;
    ZoomMode currentZoomMode;
    int previousZoomMode;
    qreal currentZoomLevel;
    qreal previousZoomLevel;
    qreal currentRotationAngle;
};

ImageViewerWidget::ImageViewerWidget(QWidget *parent)
    : QGraphicsView(parent)
    , m_impl(new Impl(this))
{
    setViewportMargins(0, 0, 0, 0);
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::TextAntialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

ImageViewerWidget::~ImageViewerWidget()
{}

void ImageViewerWidget::setZoomMode(ImageViewerWidget::ZoomMode mode, qreal zoomLevel)
{
    m_impl->currentZoomMode = mode;
    if(zoomLevel > 0)
        m_impl->currentZoomLevel = zoomLevel;
    m_impl->updateTransformations();
}

ImageViewerWidget::ZoomMode ImageViewerWidget::zoomMode() const
{
    return m_impl->currentZoomMode;
}

void ImageViewerWidget::setZoomLevel(qreal zoomLevel)
{
    setZoomMode(ZOOM_CUSTOM, zoomLevel);
}

qreal ImageViewerWidget::zoomLevel() const
{
    return m_impl->currentZoomLevel;
}

QSize ImageViewerWidget::imageSize() const
{
    if(!m_impl->currentGraphicsItem)
        return QSize();
    return m_impl->currentGraphicsItem->boundingRect().size().toSize();
}

void ImageViewerWidget::rotateClockwise()
{
    m_impl->currentRotationAngle += 90;
    if(m_impl->currentRotationAngle >= 360)
        m_impl->currentRotationAngle -= 360;
    m_impl->updateTransformations();
}

void ImageViewerWidget::rotateCounterclockwise()
{
    m_impl->currentRotationAngle -= 90;
    if(m_impl->currentRotationAngle <= -360)
        m_impl->currentRotationAngle += 360;
    m_impl->updateTransformations();
}

void ImageViewerWidget::zoomIn()
{
    setZoomLevel(m_impl->currentZoomLevel * 1.1);
}

void ImageViewerWidget::zoomOut()
{
    setZoomLevel(m_impl->currentZoomLevel / 1.1);
}

void ImageViewerWidget::setBackgroundColor(const QColor &color)
{
    setBackgroundBrush(QBrush(color));
}

void ImageViewerWidget::setGraphicsItem(QGraphicsItem *graphicsItem)
{
    clear();
    m_impl->currentGraphicsItem = graphicsItem;
    if(!graphicsItem)
        return;
    graphicsItem->setFlags(QGraphicsItem::ItemClipsToShape);
    graphicsItem->setCacheMode(QGraphicsItem::NoCache);
    scene()->addItem(graphicsItem);
    setSceneRect(graphicsItem->boundingRect());
    m_impl->updateTransformations();
}

void ImageViewerWidget::clear()
{
    m_impl->currentGraphicsItem = NULL;
    scene()->clear();
    resetTransform();
    resetMatrix();
    setSceneRect(0, 0, 1, 1);
}

void ImageViewerWidget::showEvent(QShowEvent *event)
{
    m_impl->updateTransformations();
    QGraphicsView::showEvent(event);
}

void ImageViewerWidget::resizeEvent(QResizeEvent *event)
{
    m_impl->updateTransformations();
    QGraphicsView::resizeEvent(event);
}
