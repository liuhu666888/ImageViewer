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

#include "DecoderMacWebKit.h"

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>
#import <AppKit/AppKit.h>
#import <WebKit/WebKit.h>

#include <QtGlobal>
#include <QGraphicsObject>
#include <QFileInfo>
#include <QUrl>
#include <QPixmap>
#include <QPainter>
#include <QStringList>
#include <QRegExp>
#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))
#include <QMacCocoaViewContainer>
#endif
#include <QDebug>

#include "Internal/DecoderAutoRegistrator.h"
#include "Internal/MacImageUtils.h"

//#if defined (QT_DEBUG)
//#define DECODER_MAC_WEBKIT_PRIORITY -1
//#else
#define DECODER_MAC_WEBKIT_PRIORITY 200
//#endif

namespace {

DecoderAutoRegistrator registrator(new DecoderMacWebKit);

class MacWebKitRasterizerGraphicsItem;

} // namespace

@interface MacWebKitRasterizerViewDelegate : NSObject
-(id) initWithGraphicsItem: (MacWebKitRasterizerGraphicsItem*) graphicsItem;
@end

namespace {

class MacWebKitRasterizerGraphicsItem : public QGraphicsObject
{
    Q_INTERFACES(QGraphicsItem)

public:
    enum State
    {
        STATE_LOADING,
        STATE_FAILED,
        STATE_SUCCEED
    };

    MacWebKitRasterizerGraphicsItem(const QString &fileName, QGraphicsItem *parentItem = NULL);
    ~MacWebKitRasterizerGraphicsItem();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = NULL);

    State state() const;
    void setState(State state);

    QPixmap grapPixmap();

    void initGeometry(const QSizeF &size, const QRectF &viewBox);

private:
    WebView *m_view;
    QMacCocoaViewContainer *m_container;
    MacWebKitRasterizerViewDelegate *m_delegate;
    State m_state;
    QSizeF m_size;
    QRectF m_viewBox;
};

MacWebKitRasterizerGraphicsItem::MacWebKitRasterizerGraphicsItem(const QString &fileName, QGraphicsItem *parentItem)
    : QGraphicsObject(parentItem)
    , m_view([[WebView alloc] initWithFrame: NSMakeRect(0, 0, 0, 0)])
    , m_container(new QMacCocoaViewContainer(m_view))
    , m_delegate([[MacWebKitRasterizerViewDelegate alloc] initWithGraphicsItem: this])
    , m_state(STATE_LOADING)
{
    [m_view setFrameLoadDelegate    : (id <WebFrameLoadDelegate>)   m_delegate];
    [m_view setPolicyDelegate       : (id <WebPolicyDelegate>)      m_delegate];
    [m_view setUIDelegate           : (id <WebUIDelegate>)          m_delegate];
    [m_view setEditingDelegate      : (id <WebEditingDelegate>)     m_delegate];

    [m_view setDrawsBackground: NO];
    NSURLRequest *request = [NSURLRequest requestWithURL: QUrl::fromLocalFile(QFileInfo(fileName).absoluteFilePath()).toNSURL()];
    [[m_view mainFrame] loadRequest: request];
    while(m_state == STATE_LOADING)
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
}

MacWebKitRasterizerGraphicsItem::~MacWebKitRasterizerGraphicsItem()
{
    m_container->setCocoaView(nil);
    [m_view setFrameLoadDelegate    : (id <WebFrameLoadDelegate>)   nil];
    [m_view setPolicyDelegate       : (id <WebPolicyDelegate>)      nil];
    [m_view setUIDelegate           : (id <WebUIDelegate>)          nil];
    [m_view setEditingDelegate      : (id <WebEditingDelegate>)     nil];
    [m_delegate release];
    [m_view release];
    m_container->deleteLater();
}

QRectF MacWebKitRasterizerGraphicsItem::boundingRect() const
{
//    NSView *webFrameViewDocView = [[[m_view mainFrame] frameView] documentView];
//    NSRect cacheRect = [webFrameViewDocView bounds];
//    return QRectF(cacheRect.origin.x, cacheRect.origin.y, cacheRect.size.width, cacheRect.size.height);
    return m_viewBox;
}

void MacWebKitRasterizerGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(boundingRect(), grapPixmap(), boundingRect());
}

MacWebKitRasterizerGraphicsItem::State MacWebKitRasterizerGraphicsItem::state() const
{
    return m_state;
}

void MacWebKitRasterizerGraphicsItem::setState(State state)
{
    m_state = state;
}

QPixmap MacWebKitRasterizerGraphicsItem::grapPixmap()
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSView *webFrameViewDocView = [[[m_view mainFrame] frameView] documentView];
    NSRect cacheRect = [webFrameViewDocView bounds];
    NSBitmapImageRep *bitmapRep = [[NSBitmapImageRep alloc]
            initWithBitmapDataPlanes: nil
                          pixelsWide: static_cast<NSInteger>(cacheRect.size.width)
                          pixelsHigh: static_cast<NSInteger>(cacheRect.size.height)
                       bitsPerSample: 8
                     samplesPerPixel: 4
                            hasAlpha: YES
                            isPlanar: NO
                      colorSpaceName: NSCalibratedRGBColorSpace
                        bitmapFormat: 0
                         bytesPerRow: static_cast<NSInteger>(4 * cacheRect.size.width)
                        bitsPerPixel: 32];

    [NSGraphicsContext saveGraphicsState];
    NSGraphicsContext *graphicsContext = [NSGraphicsContext graphicsContextWithBitmapImageRep: bitmapRep];
    [NSGraphicsContext setCurrentContext: graphicsContext];
//    CGContextScaleCTM(graphicsContext.graphicsPort, scale, scale);
    [webFrameViewDocView displayRectIgnoringOpacity: cacheRect inContext: graphicsContext];
    [NSGraphicsContext restoreGraphicsState];

    NSImage *webImage = [[NSImage alloc] initWithSize: bitmapRep.size];
    [webImage addRepresentation: bitmapRep];
    QPixmap pixmap = MacImageUtils::QPixmapFromNSImage(webImage);
    [webImage release];
    [pool release];
    return pixmap;
}

void MacWebKitRasterizerGraphicsItem::initGeometry(const QSizeF &size, const QRectF &viewBox)
{
    if(!size.isEmpty() && viewBox.isValid())
    {
        m_size = size;
        m_viewBox = viewBox;
    }
    else if(!size.isEmpty() && !viewBox.isValid())
    {
        m_size = size;
        m_viewBox = QRectF(QPointF(0, 0), size);
    }
    else if(size.isEmpty() && viewBox.isValid())
    {
        m_size = viewBox.size();
        m_viewBox = viewBox;
    }
    else
    {
        NSRect rect = [[[[m_view mainFrame] frameView] documentView] bounds];
        m_viewBox = QRectF(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
        m_size = m_viewBox.size();
    }
    qDebug() << "size:" << size << " => " << m_size;
    qDebug() << "viewBox:" << viewBox << " => " << m_viewBox;
}

} // namespace

@implementation MacWebKitRasterizerViewDelegate
{
    MacWebKitRasterizerGraphicsItem *m_graphicsItem;
}

-(id)         initWithGraphicsItem: (MacWebKitRasterizerGraphicsItem*) graphicsItem
{
    self = [super init];
    if(self)
        m_graphicsItem = graphicsItem;
    return self;
}

-(void)                    webView: (WebView*)  sender
             didFinishLoadForFrame: (WebFrame*) frame
{
    qDebug() << __PRETTY_FUNCTION__;
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    if(frame == [sender mainFrame])
    {
        NSString *viewBox = [sender stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getAttribute('viewBox');"];
        NSString *height  = [sender stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getAttribute('height');"];
        NSString *width   = [sender stringByEvaluatingJavaScriptFromString: @"document.querySelector('svg').getAttribute('width');"];
        const QStringList vb = QString::fromNSString(viewBox).split(QRegExp(QString::fromLatin1("\\s")));
        const QRectF viewBoxRect = (vb.size() == 4 ? QRectF(vb.at(0).toDouble(), vb.at(1).toDouble(), vb.at(2).toDouble(), vb.at(3).toDouble()) : QRectF());
        m_graphicsItem->initGeometry(QSizeF(static_cast<qreal>([width doubleValue]), static_cast<qreal>([height doubleValue])), viewBoxRect);
        m_graphicsItem->setState(MacWebKitRasterizerGraphicsItem::STATE_SUCCEED);
    }
    [pool release];
}

- (void)                   webView: (WebView*)  sender
   didFailProvisionalLoadWithError: (NSError*)  error
                          forFrame: (WebFrame*) frame
{
    qDebug() << __PRETTY_FUNCTION__;
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    if(frame == [sender mainFrame])
    {
        qWarning() << QString::fromNSString([error description]);
        m_graphicsItem->setState(MacWebKitRasterizerGraphicsItem::STATE_FAILED);
    }
    [pool release];
}

@end

QString DecoderMacWebKit::name() const
{
    return QString::fromLatin1("DecoderMacWebKit");
}

QList<DecoderFormatInfo> DecoderMacWebKit::supportedFormats() const
{
#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))
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
#else
    return QList<DecoderFormatInfo>();
#endif
}

QGraphicsItem *DecoderMacWebKit::loadImage(const QString &filePath)
{
    const QFileInfo fileInfo(filePath);
    if(!fileInfo.exists() || !fileInfo.isReadable())
        return NULL;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    MacWebKitRasterizerGraphicsItem *result = new MacWebKitRasterizerGraphicsItem(filePath);
    if(result->state() != MacWebKitRasterizerGraphicsItem::STATE_SUCCEED)
    {
        result->deleteLater();
        result = NULL;
    }

    [pool release];

    return result;
}
