# URL: https://github.com/memononen/nanosvg
# License: zlib License - https://github.com/memononen/nanosvg/blob/master/LICENSE.txt

TEMPLATE = lib
CONFIG += staticlib
TARGET = tp_NanoSVG

QT -= core gui

CONFIG -= warn_on
CONFIG += exceptions_off warn_off

THIRDPARTY_NANOSVG_PATH = $${PWD}/nanosvg-25241c5
THIRDPARTY_NANOSVG_IMPL_PATH = $${PWD}/implementation

INCLUDEPATH += $${THIRDPARTY_NANOSVG_PATH}/src

include(../CommonSettings.pri)

HEADERS += \
    $$files($${THIRDPARTY_NANOSVG_PATH}/src/*.h)

SOURCES += \
    $$files($${THIRDPARTY_NANOSVG_IMPL_PATH}/*.cpp)

TR_EXCLUDE += $${THIRDPARTY_NANOSVG_PATH}/* $${THIRDPARTY_NANOSVG_IMPL_PATH}/*

