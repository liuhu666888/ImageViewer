#
#  Copyright (C) 2017 Peter S. Zhigalov <peter.zhigalov@gmail.com>
#
#  This file is part of the `ImageViewer' program.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

TEMPLATE = app
TARGET = ImageViewer

INCLUDEPATH += src

QT += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += warn_on

DEFINES += QT_NO_CAST_FROM_ASCII

include(../Features.pri)
include(../QtUtils/QtUtils.pri)
include(../ThirdParty/ThirdParty.pri)

*g++*|*clang* {
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE *= -O3
    QMAKE_CXXFLAGS_RELEASE *= -DNDEBUG
    QMAKE_CXXFLAGS_RELEASE *= -DQT_NO_DEBUG_OUTPUT
}

*msvc* {
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE *= -Ox
    QMAKE_CXXFLAGS_RELEASE -= -GS
    QMAKE_CXXFLAGS_RELEASE *= -GS-
    QMAKE_CXXFLAGS_RELEASE *= -DQT_NO_DEBUG_OUTPUT
    DEFINES += _CRT_SECURE_NO_WARNINGS
    DEFINES += _CRT_SECURE_NO_DEPRECATE
    DEFINES += _USE_MATH_DEFINES
}

win32 {
    DEFINES += NOMINMAX
}

SOURCES += \
    src/GUI/GUISettings.cpp \
    src/GUI/MainWindow.cpp \
    src/GUI/ImageViewerWidget.cpp \
    src/GUI/SettingsDialog.cpp \
    src/GUI/AboutDialog.cpp \
    src/Decoders/DecodersManager.cpp \
    src/Decoders/DecoderQImage.cpp \
    src/Decoders/DecoderQMovie.cpp \
    src/Decoders/DecoderQtSVG.cpp \
    src/Decoders/Internal/Animation/AnimationObject.cpp \
    src/Decoders/Internal/Animation/AnimationUtils.cpp \
    src/Decoders/Internal/Animation/AnimationWidget.cpp \
    src/Decoders/Internal/Animation/FramesCompositor.cpp \
    src/Decoders/Internal/Animation/AbstractAnimationProvider.cpp \
    src/Decoders/Internal/ExifUtils.cpp \
    src/Decoders/Internal/CmsUtils.cpp \
    src/Decoders/Internal/ZLibUtils.cpp \
    src/main.cpp

HEADERS += \
    src/GUI/GUISettings.h \
    src/GUI/MainWindow.h \
    src/GUI/MainWindow_p.h \
    src/GUI/ImageViewerWidget.h \
    src/GUI/SettingsDialog.h \
    src/GUI/SettingsDialog_p.h \
    src/GUI/AboutDialog.h \
    src/GUI/AboutDialog_p.h \
    src/Decoders/DecodersManager.h \
    src/Decoders/IDecoder.h \
    src/Decoders/Internal/Animation/AnimationObject.h \
    src/Decoders/Internal/Animation/AnimationUtils.h \
    src/Decoders/Internal/Animation/AnimationWidget.h \
    src/Decoders/Internal/Animation/FramesCompositor.h \
    src/Decoders/Internal/Animation/IAnimationProvider.h \
    src/Decoders/Internal/Animation/AbstractAnimationProvider.h \
    src/Decoders/Internal/DecoderAutoRegistrator.h \
    src/Decoders/Internal/ExifUtils.h \
    src/Decoders/Internal/CmsUtils.h \
    src/Decoders/Internal/ZLibUtils.h

!disable_stb {
    SOURCES += \
        src/Decoders/DecoderSTB.cpp
}

!disable_qtimageformats {
    SOURCES += \
        src/Decoders/DecoderQtImageFormatsImage.cpp \
        src/Decoders/DecoderQtImageFormatsMovie.cpp
}

!disable_libjpeg {
    SOURCES += \
        src/Decoders/DecoderLibJpeg.cpp
}

!disable_libmng {
    SOURCES += \
        src/Decoders/DecoderLibMng.cpp
}

!disable_libpng {
    SOURCES += \
        src/Decoders/DecoderLibPng.cpp
}

!disable_libjasper {
    SOURCES += \
        src/Decoders/DecoderLibJasPer.cpp
}

!disable_libtiff {
    SOURCES += \
        src/Decoders/DecoderLibTiff.cpp
}

!disable_libwebp {
    SOURCES += \
        src/Decoders/DecoderLibWebP.cpp
}

!disable_libbpg {
    SOURCES += \
        src/Decoders/DecoderLibBpg.cpp
}

TRANSLATIONS += \
    resources/translations/imageviewer_en.ts \
    resources/translations/imageviewer_ru.ts

win32 {
    RC_FILE += resources/platform/windows/Resources.rc
    DEFINES += NOMINMAX
}

macx {
    greaterThan(QT_MAJOR_VERSION, 4): QT += macextras
    OBJECTIVE_SOURCES += \
        src/Decoders/Internal/MacImageUtils.mm \
        src/Decoders/Internal/MacWebKitRasterizerGraphicsItem.mm \
        src/Decoders/DecoderNSImage.mm \
        src/Decoders/DecoderMacWebKit.mm
    HEADERS += \
        src/Decoders/Internal/MacImageUtils.h \
        src/Decoders/Internal/MacWebKitRasterizerGraphicsItem.h
    LIBS += -framework AppKit
    LIBS += -framework Foundation
    LIBS += -framework WebKit

    QMAKE_INFO_PLIST = resources/platform/macosx/Info.plist
    ICON = resources/icon/icon.icns
    TARGET = "Image Viewer"
    QMAKE_CXXFLAGS += -Wno-invalid-constexpr
}

RESOURCES += \
    resources/translations/imageviewer_translations.qrc \
    resources/icon/icon.qrc \
    resources/style/style.qrc

QMAKE_RESOURCE_FLAGS += -threshold 0 -compress 9

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

# qmake CONFIG+=use_static_qgif
use_static_qgif {
    QTPLUGIN += qgif
    DEFINES += USE_STATIC_QGIF
}

# qmake CONFIG+=use_static_qmng
use_static_qmng {
    QTPLUGIN += qmng
    DEFINES += USE_STATIC_QMNG
}

# qmake CONFIG+=use_static_qsvg
use_static_qsvg {
    QTPLUGIN += qsvg
    DEFINES += USE_STATIC_QSVG
}
