# URL: https://github.com/memononen/nanosvg
# License: zlib License - https://github.com/memononen/nanosvg/blob/master/LICENSE.txt

include($${PWD}/../../Features.pri)

!disable_nanosvg {

    THIRDPARTY_NANOSVG_PATH = $${PWD}/nanosvg-25241c5

    INCLUDEPATH += $${THIRDPARTY_NANOSVG_PATH}/src
    DEPENDPATH += $${THIRDPARTY_NANOSVG_PATH}/src
    DEFINES += HAS_NANOSVG

    OUT_LIB_TARGET = tp_NanoSVG
    OUT_LIB_DIR = $${OUT_PWD}/../ThirdParty/NanoSVG
    OUT_LIB_NAME =
    OUT_LIB_LINK =
    win32 {
        CONFIG(release, debug|release) {
            OUT_LIB_DIR = $${OUT_LIB_DIR}/release
        } else:CONFIG(debug, debug|release) {
            OUT_LIB_DIR = $${OUT_LIB_DIR}/debug
        }
        *g++*|*clang* {
            OUT_LIB_NAME = lib$${OUT_LIB_TARGET}.a
            OUT_LIB_LINK = -l$${OUT_LIB_TARGET}
        } else {
            OUT_LIB_NAME = $${OUT_LIB_TARGET}.lib
            OUT_LIB_LINK = $${OUT_LIB_NAME}
        }
    } else {
        OUT_LIB_DIR = $${OUT_LIB_DIR}
        OUT_LIB_NAME = lib$${OUT_LIB_TARGET}.a
        OUT_LIB_LINK = -l$${OUT_LIB_TARGET}
    }
    LIBS += -L$${OUT_LIB_DIR} $${OUT_LIB_LINK}
    PRE_TARGETDEPS += $${OUT_LIB_DIR}/$${OUT_LIB_NAME}

}

