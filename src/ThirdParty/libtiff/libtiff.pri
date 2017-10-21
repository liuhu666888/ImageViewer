# URL: http://www.simplesystems.org/libtiff/ + https://github.com/vadz/libtiff.git
# License: ???

include($${PWD}/../../Features.pri)

!disable_libtiff {

    DEFINES += HAS_LIBTIFF

    !system_libtiff {

        THIRDPARTY_LIBTIFF_PATH = $${PWD}/libtiff-e568614
        THIRDPARTY_LIBTIFF_CONFIG_PATH = $${PWD}/config

        INCLUDEPATH += $${THIRDPARTY_LIBTIFF_CONFIG_PATH} $${THIRDPARTY_LIBTIFF_PATH}/libtiff
        DEPENDPATH += $${THIRDPARTY_LIBTIFF_CONFIG_PATH} $${THIRDPARTY_LIBTIFF_PATH}/libtiff

        DEFINES += TIFF_PREFIX

        OUT_LIB_TARGET = tp_libtiff
        OUT_LIB_DIR = $${OUT_PWD}/../ThirdParty/libtiff
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

        *msvc*: LIBS += libtiff.lib
        else: LIBS += -ltiff

    }

}

