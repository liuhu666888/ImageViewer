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

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import <WebKit/WebKit.h>

#include "MacWebKitRasterizerGraphicsItem.h"

#include <cmath>
#include <algorithm>
#include <limits>

#include <QtGlobal>
#include <QFileInfo>
#include <QUrl>
#include <QPixmap>
#include <QPainter>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QSysInfo>
#include <QDebug>

#include "MacImageUtils.h"

//#define MAC_WEBKIT_RASTERIZER_GRAPHICS_ITEM_DEBUG

// ====================================================================================================

namespace {

const qreal MAX_PIXMAP_DIMENSION = 16384;
const qreal DEFAULT_WIDTH = 640;
const qreal DEFAULT_HEIGHT = 480;

QRectF QRectFIntegerized(const QRectF rect)
{
    const qreal left = std::floor(rect.left());
    const qreal top = std::floor(rect.top());
    const qreal width = std::ceil(rect.width() + std::abs(rect.left() - left));
    const qreal height = std::ceil(rect.height() + std::abs(rect.top() - top));
    return QRectF(left, top, width, height);
}

QRectF QRectFFromNSRect(const NSRect &rect)
{
    return QRectF(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
}

NSRect QRectFToNSRect(const QRectF &rect)
{
    return NSMakeRect(rect.x(), rect.y(), rect.width(), rect.height());
}

QRectF DOMNodeActualBoundingBox(DOMNode *node)
{
    QRectF result;
    DOMNodeList *childNodes = [node childNodes];
    for(unsigned int i = 0; i < [childNodes length]; i++)
    {
        DOMNode *childNode = [childNodes item: i];
        QRectF childRect = QRectFFromNSRect([childNode boundingBox]);
        if(!childRect.isValid())
            childRect = DOMNodeActualBoundingBox(childNode);
        if(!childRect.isValid())
            continue;
        if(!result.isValid())
            result = childRect;
        else
            result = result.united(childRect);
    }
    return result;
}

QRectF WebFrameDocumentBounds(WebFrame *frame)
{
    return QRectFFromNSRect([[[frame frameView] documentView] bounds]);
}

QRectF SVGViewBoxAttribute(WebView *webView)
{
    const NSString *str = [webView stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getAttribute('viewBox');"];
    const QStringList vb = QString::fromUtf8([str UTF8String]).split(QRegExp(QString::fromLatin1("\\s")));
    return (vb.size() == 4 ? QRectF(vb.at(0).toDouble(), vb.at(1).toDouble(), vb.at(2).toDouble(), vb.at(3).toDouble()) : QRectF());
}

QSizeF SVGSizeAttribute(WebView *webView)
{
    return QSizeF(
        static_cast<qreal>([[webView stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getAttribute('width');"] doubleValue]),
        static_cast<qreal>([[webView stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getAttribute('height');"] doubleValue])
    );
}

QRectF SVGBBox(WebView *webView)
{
    return QRectF(
        static_cast<qreal>([[webView stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getBBox().x;"] doubleValue]),
        static_cast<qreal>([[webView stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getBBox().y;"] doubleValue]),
        static_cast<qreal>([[webView stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getBBox().width;"] doubleValue]),
        static_cast<qreal>([[webView stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getBBox().height;"] doubleValue])
    );
}

QRectF SVGBoundingClientRect(WebView *webView)
{
    if(QSysInfo::MacintoshVersion >= QSysInfo::MV_10_9) /// @todo Работает в 10.8.5, нужно проверить ранние версии 10.8
    {
        return QRectF(
            static_cast<qreal>([[webView stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getBoundingClientRect().x;"] doubleValue]),
            static_cast<qreal>([[webView stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getBoundingClientRect().y;"] doubleValue]),
            static_cast<qreal>([[webView stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getBoundingClientRect().width;"] doubleValue]),
            static_cast<qreal>([[webView stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getBoundingClientRect().height;"] doubleValue])
        );
    }
    else
    {
        return WebFrameDocumentBounds([webView mainFrame]);
    }
}

QRectF SVGActualBoundingBox(WebView *webView)
{
    QRectF rect;
    if(QSysInfo::MacintoshVersion >= QSysInfo::MV_10_8) /// @todo Работает в 10.8.5, нужно проверить ранние версии 10.8
        rect = DOMNodeActualBoundingBox([webView mainFrameDocument]);
    else
        rect = QRectFIntegerized(SVGBBox(webView));
    if(rect.isValid())
        rect = rect.intersected(QRectF(0, 0, std::numeric_limits<qreal>::max(), std::numeric_limits<qreal>::max()));
    return rect;
}

} // namespace

// ====================================================================================================

@interface MacWebKitRasterizerViewDelegate : NSObject
-(id) initWithImpl: (MacWebKitRasterizerGraphicsItem::Impl*) impl;
@end

// ====================================================================================================

class MacWebKitRasterizerGraphicsItem::Impl
{
public:
    struct RasterizerCache
    {
        QPixmap pixmap;
        qreal scaleFactor;
    };

    enum ScaleMethod
    {
        SCALE_BY_RESIZE_FRAME,
        SCALE_BY_RESIZE_FRAME_AND_SCALE_CONTENT
    };

    Impl(const QUrl &url);
    Impl(const QByteArray &htmlData, MacWebKitRasterizerGraphicsItem::DataType dataType);
    ~Impl();

    MacWebKitRasterizerGraphicsItem::State state() const;
    void setState(MacWebKitRasterizerGraphicsItem::State state);

    void waitForLoad() const;
    QPixmap grapPixmap(qreal scaleFactor = 1);

    QRectF rect() const;
    void setRect(const QRectF &rect);

    qreal maxScaleFactor() const;
    void setMaxScaleFactor(qreal maxScaleFactor);

    ScaleMethod scaleMethod() const;
    void setScaleMethod(ScaleMethod scaleMethod);

    RasterizerCache &rasterizerCache();
    const RasterizerCache &rasterizerCache() const;

private:
    void init();

    WebView *m_view;
    QWidget *m_container;
    MacWebKitRasterizerViewDelegate *m_delegate;
    MacWebKitRasterizerGraphicsItem::State m_state;
    QRectF m_rect;
    qreal m_maxScaleFactor;
    ScaleMethod m_scaleMethod;
    RasterizerCache m_rasterizerCache;
};

// ====================================================================================================

MacWebKitRasterizerGraphicsItem::Impl::Impl(const QUrl &url)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    init();
    if(!url.isValid())
    {
        m_state = MacWebKitRasterizerGraphicsItem::STATE_FAILED;
    }
    else
    {
        NSURLRequest *request = [NSURLRequest requestWithURL: [NSURL URLWithString: [NSString stringWithUTF8String: url.toString().toUtf8().data()]]];
        [[m_view mainFrame] loadRequest: request];
        waitForLoad();
    }
    [pool release];
}

MacWebKitRasterizerGraphicsItem::Impl::Impl(const QByteArray &htmlData, MacWebKitRasterizerGraphicsItem::DataType dataType)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    init();
    if(htmlData.isEmpty())
    {
        m_state = MacWebKitRasterizerGraphicsItem::STATE_FAILED;
    }
    else
    {
        NSString *mimeType = nil;
        switch(dataType)
        {
        case DATA_TYPE_HTML:
            mimeType = @"text/html";
            break;
        case DATA_TYPE_XHTML:
            mimeType = @"application/xhtml+xml";
            break;
        case DATA_TYPE_XML:
            mimeType = @"application/xml";
            break;
        case DATA_TYPE_SVG:
            mimeType = @"image/svg+xml";
            break;
        default:
            break;
        }
        NSData *data = [NSData dataWithBytes: const_cast<void*>(static_cast<const void*>(htmlData.constData())) length: static_cast<NSUInteger>(htmlData.size())];
        [[m_view mainFrame] loadData: data MIMEType: mimeType textEncodingName: nil baseURL: nil];
        waitForLoad();
    }
    [pool release];
}

MacWebKitRasterizerGraphicsItem::Impl::~Impl()
{
    [m_view setFrameLoadDelegate: nil];
    [m_delegate release];
    [m_view removeFromSuperview];
    [m_view release];
    m_container->deleteLater();
}

MacWebKitRasterizerGraphicsItem::State MacWebKitRasterizerGraphicsItem::Impl::state() const
{
    return m_state;
}

void MacWebKitRasterizerGraphicsItem::Impl::setState(MacWebKitRasterizerGraphicsItem::State state)
{
    m_state = state;
}

void MacWebKitRasterizerGraphicsItem::Impl::waitForLoad() const
{
    while(m_state == MacWebKitRasterizerGraphicsItem::STATE_LOADING)
        [[NSRunLoop currentRunLoop] runMode: NSDefaultRunLoopMode beforeDate: [NSDate distantFuture]];
}

QPixmap MacWebKitRasterizerGraphicsItem::Impl::grapPixmap(qreal scaleFactor)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    const QRectF scaledRect = QRectFIntegerized(QRectF(m_rect.topLeft() * scaleFactor, m_rect.size() * scaleFactor));
    const QRectF scaledPage = QRectFIntegerized(scaledRect.united(QRectF(0, 0, 1, 1)));

    switch(scaleMethod())
    {
    case SCALE_BY_RESIZE_FRAME_AND_SCALE_CONTENT:
    {
        const QString zoomScript = QString::fromLatin1("document.documentElement.style.zoom = '%1'");
        const double oldScaleFactor = QString::fromUtf8([[m_view stringByEvaluatingJavaScriptFromString: @"document.documentElement.style.zoom;"] UTF8String]).toDouble();
        if(oldScaleFactor > scaleFactor)
        {
            [m_view stringByEvaluatingJavaScriptFromString: [NSString stringWithUTF8String: zoomScript.arg(scaleFactor).toUtf8().data()]];
            [m_view setFrameSize: NSMakeSize(scaledPage.width(), scaledPage.height())];
        }
        else
        {
            [m_view setFrameSize: NSMakeSize(scaledPage.width(), scaledPage.height())];
            [m_view stringByEvaluatingJavaScriptFromString: [NSString stringWithUTF8String: zoomScript.arg(scaleFactor).toUtf8().data()]];
        }
        break;
    }
    case SCALE_BY_RESIZE_FRAME:
    {
        [m_view setFrameSize: NSMakeSize(scaledPage.width(), scaledPage.height())];
        break;
    }
    }

    NSView *webFrameViewDocView = [[[m_view mainFrame] frameView] documentView];
    const NSRect cacheRect = QRectFToNSRect(scaledPage);
    const NSInteger one = static_cast<NSInteger>(1);
    NSBitmapImageRep *bitmapRep = [[NSBitmapImageRep alloc]
            initWithBitmapDataPlanes: nil
                          pixelsWide: std::max(static_cast<NSInteger>(cacheRect.size.width), one)
                          pixelsHigh: std::max(static_cast<NSInteger>(cacheRect.size.height), one)
                       bitsPerSample: 8
                     samplesPerPixel: 4
                            hasAlpha: YES
                            isPlanar: NO
                      colorSpaceName: NSCalibratedRGBColorSpace
                        bitmapFormat: 0
                         bytesPerRow: 4 * std::max(static_cast<NSInteger>(cacheRect.size.width), one)
                        bitsPerPixel: 32];

    [NSGraphicsContext saveGraphicsState];
    NSGraphicsContext *graphicsContext = [NSGraphicsContext graphicsContextWithBitmapImageRep: bitmapRep];
    [NSGraphicsContext setCurrentContext: graphicsContext];
    [webFrameViewDocView displayRectIgnoringOpacity: cacheRect inContext: graphicsContext];
    [NSGraphicsContext restoreGraphicsState];

    NSImage *webImage = [[NSImage alloc] initWithSize: bitmapRep.size];
    [webImage addRepresentation: bitmapRep];
    QPixmap pixmap = MacImageUtils::QPixmapFromNSImage(webImage);
    [webImage release];
    [pool release];
    return pixmap.copy(scaledRect.toRect());
}

QRectF MacWebKitRasterizerGraphicsItem::Impl::rect() const
{
    return m_rect;
}

void MacWebKitRasterizerGraphicsItem::Impl::setRect(const QRectF &rect)
{
#if defined (MAC_WEBKIT_RASTERIZER_GRAPHICS_ITEM_DEBUG)
    qDebug() << __FUNCTION__ << "rect:" << rect;
#endif
    m_rect = rect;
}

qreal MacWebKitRasterizerGraphicsItem::Impl::maxScaleFactor() const
{
    return m_maxScaleFactor;
}

void MacWebKitRasterizerGraphicsItem::Impl::setMaxScaleFactor(qreal maxScaleFactor)
{
    m_maxScaleFactor = maxScaleFactor;
}

MacWebKitRasterizerGraphicsItem::Impl::ScaleMethod MacWebKitRasterizerGraphicsItem::Impl::scaleMethod() const
{
    return m_scaleMethod;
}

void MacWebKitRasterizerGraphicsItem::Impl::setScaleMethod(MacWebKitRasterizerGraphicsItem::Impl::ScaleMethod scaleMethod)
{
#if defined (MAC_WEBKIT_RASTERIZER_GRAPHICS_ITEM_DEBUG)
    QString methodStr;
    switch(scaleMethod)
    {
#define METHOD_CASE(METHOD) \
    case METHOD: \
        methodStr = QString::fromLatin1(#METHOD); \
        break;
    METHOD_CASE(SCALE_BY_RESIZE_FRAME)
    METHOD_CASE(SCALE_BY_RESIZE_FRAME_AND_SCALE_CONTENT)
#undef METHOD_CASE
    }
    qDebug() << __FUNCTION__ << "scaleMethod:" << methodStr;
#endif
    m_scaleMethod = scaleMethod;
}

MacWebKitRasterizerGraphicsItem::Impl::RasterizerCache &MacWebKitRasterizerGraphicsItem::Impl::rasterizerCache()
{
    return m_rasterizerCache;
}

const MacWebKitRasterizerGraphicsItem::Impl::RasterizerCache &MacWebKitRasterizerGraphicsItem::Impl::rasterizerCache() const
{
    return m_rasterizerCache;
}

void MacWebKitRasterizerGraphicsItem::Impl::init()
{
    m_view = [[WebView alloc] initWithFrame: NSMakeRect(0, 0, 0, 0)];
    m_container = new QWidget;
    m_delegate = [[MacWebKitRasterizerViewDelegate alloc] initWithImpl: this];
    m_state = MacWebKitRasterizerGraphicsItem::STATE_LOADING;
    m_rasterizerCache.scaleFactor = 0;
    m_maxScaleFactor = 1;
    m_scaleMethod = SCALE_BY_RESIZE_FRAME;
    [m_view setFrameLoadDelegate: reinterpret_cast<id>(m_delegate)];
    [reinterpret_cast<NSView*>(m_container->winId()) addSubview: m_view];
    [m_view setDrawsBackground: NO];
}

// ====================================================================================================

@implementation MacWebKitRasterizerViewDelegate
{
    MacWebKitRasterizerGraphicsItem::Impl *m_impl;
}

-(id)                 initWithImpl: (MacWebKitRasterizerGraphicsItem::Impl*) impl
{
    self = [super init];
    if(self)
        m_impl = impl;
    return self;
}

-(void)                    webView: (WebView*)  view
             didFinishLoadForFrame: (WebFrame*) frame
{
#if defined (MAC_WEBKIT_RASTERIZER_GRAPHICS_ITEM_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    if(frame == [view mainFrame])
    {
        QRectF actualRect;
        if(QString::fromUtf8([[view stringByEvaluatingJavaScriptFromString: @"document.documentElement instanceof SVGElement;"] UTF8String]) == QString::fromLatin1("true"))
        {
            const QRectF viewBox = SVGViewBoxAttribute(view);
            const QRectF boundClientRect = SVGBoundingClientRect(view);

            /// @note WebKit рендерит с учетом смещения, нужен только размер
            if(boundClientRect.isValid())
                actualRect = QRectF(0, 0, boundClientRect.width(), boundClientRect.height());
            else if(viewBox.isValid())
                actualRect = QRectF(0, 0, viewBox.width(), viewBox.height());
            else
                actualRect = SVGActualBoundingBox(view);
            if(!actualRect.isValid())
                actualRect = WebFrameDocumentBounds(frame);

#if defined (MAC_WEBKIT_RASTERIZER_GRAPHICS_ITEM_DEBUG)
            qDebug() << "***** ----------------------------------------";
            qDebug() << "***** Detected SVG document";
            qDebug() << "***** ----------------------------------------";
            qDebug() << "***** viewBox attribute:" << SVGViewBoxAttribute(view);
            qDebug() << "***** size attribute:" << SVGSizeAttribute(view);
            qDebug() << "***** getBBox() value:" << SVGBBox(view);
            qDebug() << "***** getBoundingClientRect() value:" << SVGBoundingClientRect(view);
            qDebug() << "***** DOM boundingBox:" << DOMNodeActualBoundingBox([view mainFrameDocument]);
            qDebug() << "***** documentView bounds:" << WebFrameDocumentBounds(frame);
            qDebug() << "***** ----------------------------------------";
            qDebug() << "***** actual rect:" << actualRect;
            qDebug() << "***** ----------------------------------------";
#endif
            MacWebKitRasterizerGraphicsItem::Impl::ScaleMethod scaleMethod;
            if(SVGSizeAttribute(view).isEmpty() && viewBox.isValid())
                scaleMethod = MacWebKitRasterizerGraphicsItem::Impl::SCALE_BY_RESIZE_FRAME;
            else
                scaleMethod = MacWebKitRasterizerGraphicsItem::Impl::SCALE_BY_RESIZE_FRAME_AND_SCALE_CONTENT;
            m_impl->setScaleMethod(scaleMethod);
        }
        else
        {
            /// @todo Тут, возможно, следует сделать более гибкий алгоритм
            actualRect = WebFrameDocumentBounds(frame);
            actualRect = actualRect.united(QRectF(0, 0, DEFAULT_WIDTH, 1));

#if defined (MAC_WEBKIT_RASTERIZER_GRAPHICS_ITEM_DEBUG)
            qDebug() << "***** ----------------------------------------";
            qDebug() << "***** Detected HTML document";
            qDebug() << "***** ----------------------------------------";
            qDebug() << "***** DOM boundingBox:" << DOMNodeActualBoundingBox([view mainFrameDocument]);
            qDebug() << "***** documentView bounds:" << WebFrameDocumentBounds(frame);
            qDebug() << "***** ----------------------------------------";
            qDebug() << "***** actual rect:" << actualRect;
            qDebug() << "***** ----------------------------------------";
#endif
            m_impl->setScaleMethod(MacWebKitRasterizerGraphicsItem::Impl::SCALE_BY_RESIZE_FRAME_AND_SCALE_CONTENT);
        }

        if(!actualRect.isValid())
        {
            const qreal width = std::max(actualRect.width(), DEFAULT_WIDTH);
            const qreal height = std::max(actualRect.height(), DEFAULT_HEIGHT);
            const qreal top = (actualRect.top() + actualRect.height() > height ? actualRect.top() : 0);
            const qreal left = (actualRect.left() + actualRect.width() > width ? actualRect.left() : 0);
            actualRect = QRectF(left, top, width, height);
        }

        m_impl->setRect(actualRect);
        m_impl->setMaxScaleFactor(std::min(MAX_PIXMAP_DIMENSION / actualRect.width(), MAX_PIXMAP_DIMENSION / actualRect.height()));
        m_impl->setState(MacWebKitRasterizerGraphicsItem::STATE_SUCCEED);
    }
    [pool release];
}

- (void)                   webView: (WebView*)  view
   didFailProvisionalLoadWithError: (NSError*)  error
                          forFrame: (WebFrame*) frame
{
#if defined (MAC_WEBKIT_RASTERIZER_GRAPHICS_ITEM_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    if(frame == [view mainFrame])
    {
        qWarning() << QString::fromUtf8([[error description] UTF8String]);
        m_impl->setState(MacWebKitRasterizerGraphicsItem::STATE_FAILED);
    }
    [pool release];
}

@end

// ====================================================================================================

MacWebKitRasterizerGraphicsItem::MacWebKitRasterizerGraphicsItem(const QUrl &url, QGraphicsItem *parentItem)
    : QGraphicsObject(parentItem)
    , m_impl(new Impl(url))
{}

MacWebKitRasterizerGraphicsItem::MacWebKitRasterizerGraphicsItem(const QByteArray &htmlData, DataType dataType, QGraphicsItem *parentItem)
    : QGraphicsObject(parentItem)
    , m_impl(new Impl(htmlData, dataType))
{}

MacWebKitRasterizerGraphicsItem::~MacWebKitRasterizerGraphicsItem()
{}

QRectF MacWebKitRasterizerGraphicsItem::boundingRect() const
{
    return m_impl->rect();
}

void MacWebKitRasterizerGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
#if defined (MAC_WEBKIT_RASTERIZER_GRAPHICS_ITEM_DEBUG)
    painter->setBrush(Qt::transparent);
    painter->setPen(Qt::red);
    painter->drawRect(boundingRect());
#endif
    QPixmap &pixmap = m_impl->rasterizerCache().pixmap;
    const QRectF identityRect = QRectF(0, 0, 1, 1);
    const QRectF deviceTransformedRect = painter->deviceTransform().mapRect(identityRect);
    const qreal newScaleFactor = std::min(std::max(deviceTransformedRect.width(), deviceTransformedRect.height()), m_impl->maxScaleFactor());
    qreal &scaleFactor = m_impl->rasterizerCache().scaleFactor;
    if(std::abs(newScaleFactor - scaleFactor) / std::max(newScaleFactor, scaleFactor) > 1e-2)
    {
        scaleFactor = newScaleFactor;
        pixmap = m_impl->grapPixmap(newScaleFactor);
    }
    painter->drawPixmap(boundingRect(), pixmap, QRectF(pixmap.rect()));
}

MacWebKitRasterizerGraphicsItem::State MacWebKitRasterizerGraphicsItem::state() const
{
    return m_impl->state();
}

// ====================================================================================================

