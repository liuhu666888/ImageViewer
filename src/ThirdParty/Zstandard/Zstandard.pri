# URL: https://github.com/facebook/zstd
# License: 3-clause BSD License - https://github.com/facebook/zstd/blob/dev/LICENSE

include($${PWD}/../../Features.pri)

!disable_zstd {

    DEFINES += HAS_ZSTD

    !system_zstd {

        THIRDPARTY_ZSTD_PATH = $${PWD}/zstd-1.4.4

        INCLUDEPATH += $${THIRDPARTY_ZSTD_PATH}/lib
        DEPENDPATH += $${THIRDPARTY_ZSTD_PATH}/lib

        OUT_LIB_TARGET = tp_zstd
        OUT_LIB_DIR = $${OUT_PWD}/../ThirdParty/Zstandard
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

        *msvc*: LIBS += libzstd.lib
        else: LIBS += -lzstd

    }

}

