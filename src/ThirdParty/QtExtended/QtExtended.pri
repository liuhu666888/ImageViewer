# https://sourceforge.net/projects/qpe/files/QPE/qtopia/qt-extended-opensource-src-4.4.3.tar.gz/download

THIRDPARTY_QTEXTENDED_PATH = $${PWD}/src

CONFIG += has_thirdparty_qtextended
INCLUDEPATH += $${THIRDPARTY_QTEXTENDED_PATH}
DEPENDPATH += $${THIRDPARTY_QTEXTENDED_PATH}
DEFINES += HAS_THIRDPARTY_QTEXTENDED

QT += core

win32 {
    CONFIG(release, debug|release) {
        LIBS += -L$${OUT_PWD}/../ThirdParty/QtExtended/release
    } else:CONFIG(debug, debug|release) {
        LIBS += -L$${OUT_PWD}/../ThirdParty/QtExtended/debug
    }
    *g++*|*clang* {
        LIBS += -lQtExtended
    } else {
        LIBS += QtExtended.lib
    }
} else {
    LIBS += -L$${OUT_PWD}/../ThirdParty/QtExtended -lQtExtended
}

