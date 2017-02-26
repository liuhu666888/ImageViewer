#-------------------------------------------------
#
# Project created by QtCreator 2017-02-25T14:20:14
#
#-------------------------------------------------

QT += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageViewer
TEMPLATE = app

INCLUDEPATH += src/ImageViewer

DEFINES += QT_NO_CAST_FROM_ASCII

include(src/QtUtils/QtUtils.pri)

SOURCES += \
    src/ImageViewer/GUI/MainWindow.cpp \
    src/ImageViewer/GUI/ImageViewerWidget.cpp \
    src/ImageViewer/Decoders/DecodersManager.cpp \
    src/ImageViewer/Decoders/DecoderQtPixmap.cpp \
    src/ImageViewer/Decoders/DecoderQtSVG.cpp \
    src/ImageViewer/Decoders/DecoderQtMovie.cpp \
    src/ImageViewer/main.cpp

HEADERS += \
    src/ImageViewer/GUI/MainWindow.h \
    src/ImageViewer/GUI/MainWindow_p.h \
    src/ImageViewer/GUI/ImageViewerWidget.h \
    src/ImageViewer/Decoders/DecodersManager.h \
    src/ImageViewer/Decoders/IDecoder.h \
    src/ImageViewer/Decoders/DecoderAutoRegistrator.h \
    src/ImageViewer/Decoders/DecoderQtPixmap.h \
    src/ImageViewer/Decoders/DecoderQtSVG.h \
    src/ImageViewer/Decoders/DecoderQtMovie.h

TRANSLATIONS += \
    src/ImageViewer/Resources/translations/en.ts \
    src/ImageViewer/Resources/translations/ru.ts

win32 {
    RC_FILE += src/ImageViewer/Resources/platform/windows/Resources.rc
    DEFINES += NOMINMAX
}

macx {
    QMAKE_INFO_PLIST = src/ImageViewer/Resources/platform/macosx/Info.plist
    ICON = src/ImageViewer/Resources/icon/icon.icns
    TARGET = "Image Viewer"
    QMAKE_CXXFLAGS += -Wno-invalid-constexpr
}

RESOURCES += \
    src/ImageViewer/Resources/translations/translations.qrc \
    src/ImageViewer/Resources/icon/icon.qrc

QMAKE_RESOURCE_FLAGS += -threshold 0 -compress 9

DESTDIR = .
OBJECTS_DIR = build/objects
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui

# qmake CONFIG+=use_static_qjpeg
use_static_qjpeg {
    QTPLUGIN += qjpeg
    DEFINES += USE_STATIC_QJPEG
}

# qmake CONFIG+=use_static_qtiff
use_static_qtiff {
    QTPLUGIN += qtiff
    DEFINES += USE_STATIC_QTIFF
}

# qmake CONFIG+=use_static_qico
use_static_qico {
    QTPLUGIN += qico
    DEFINES += USE_STATIC_QICO
}
