# URL: https://github.com/strukturag/libheif
# License: GNU LGPL v3 - https://github.com/strukturag/libheif/blob/master/COPYING

include($${PWD}/../../Features.pri)

!disable_libheif {

    DEFINES += HAS_LIBHEIF

    !system_libheif {

        THIRDPARTY_LIBDE265_PATH = $${PWD}/libheif-1.4.0

        INCLUDEPATH += $${THIRDPARTY_LIBDE265_PATH}
        DEPENDPATH += $${THIRDPARTY_LIBDE265_PATH}

        DEFINES += LIBHEIF_STATIC_BUILD

        OUT_LIB_TARGET = tp_libheif
        OUT_LIB_DIR = $${OUT_PWD}/../ThirdParty/libheif
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
#        PRE_TARGETDEPS += $${OUT_LIB_DIR}/$${OUT_LIB_NAME}

    } else {

        *msvc*: LIBS += libheif.lib
        else: LIBS += -lheif

    }

}

