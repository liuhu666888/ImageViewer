# URL: http://www.exiv2.org/
# License: GNU GPL

TEMPLATE = lib
CONFIG += staticlib
TARGET = tp_exiv2

QT -= gui

CONFIG -= warn_on
CONFIG += warn_off

THIRDPARTY_EXIV2_PATH = $${PWD}/exiv2-0.27.0-Source
THIRDPARTY_EXIV2_CONFIG_PATH = $${PWD}/config

include(../CommonSettings.pri)
include(../zlib/zlib.pri)
include(../libexpat/libexpat.pri)

INCLUDEPATH = $${THIRDPARTY_EXIV2_CONFIG_PATH} $${THIRDPARTY_EXIV2_PATH}/include $${THIRDPARTY_EXIV2_PATH}/include/exiv2 $${THIRDPARTY_EXIV2_PATH}/xmpsdk/include $${INCLUDEPATH}

DEFINES += EXV_HAVE_STDINT_H exiv2lib_STATIC

win32 {
    DEFINES += NOMINMAX
}

*clang* {
    DEFINES *= _LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR
}

SOURCES += \
    $${THIRDPARTY_EXIV2_PATH}/src/canonmn_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/casiomn_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/cr2header_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/crwimage_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/fujimn_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/helper_functions.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/image_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/makernote_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/minoltamn_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/nikonmn_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/olympusmn_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/orfimage_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/panasonicmn_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/pentaxmn_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/rw2image_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/samsungmn_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/sigmamn_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/sonymn_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tags_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tiffcomposite_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tiffimage_int.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tiffvisitor_int.cpp \
\ #    $${THIRDPARTY_EXIV2_PATH}/src/asfvideo.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/basicio.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/bigtiffimage.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/bmpimage.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/convert.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/cr2image.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/crwimage.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/datasets.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/easyaccess.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/epsimage.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/error.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/exif.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/futils.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/gifimage.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/http.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/image.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/ini.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/iptc.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/jp2image.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/jpgimage.cpp \
\ #    $${THIRDPARTY_EXIV2_PATH}/src/matroskavideo.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/metadatum.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/mrwimage.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/orfimage.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/pgfimage.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/preview.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/properties.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/psdimage.cpp \
\ #    $${THIRDPARTY_EXIV2_PATH}/src/quicktimevideo.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/rafimage.cpp \
\ #    $${THIRDPARTY_EXIV2_PATH}/src/riffvideo.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/rw2image.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tags.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tgaimage.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tiffimage.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/types.cpp \
\ #    $${THIRDPARTY_EXIV2_PATH}/src/utilsvideo.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/value.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/version.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/webpimage.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/xmp.cpp \
    $${THIRDPARTY_EXIV2_PATH}/src/xmpsidecar.cpp \

HEADERS += \
    $${THIRDPARTY_EXIV2_PATH}/src/canonmn_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/casiomn_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/cr2header_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/crwimage_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/fujimn_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/helper_functions.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/image_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/makernote_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/minoltamn_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/nikonmn_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/olympusmn_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/orfimage_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/panasonicmn_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/pentaxmn_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/rw2image_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/safe_op.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/samsungmn_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/sigmamn_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/sonymn_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tags_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tiffcomposite_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tiffimage_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tiffvisitor_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/tifffwd_int.hpp \
    $${THIRDPARTY_EXIV2_PATH}/src/timegm.h \
    $${THIRDPARTY_EXIV2_PATH}/src/unused.h \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/asfvideo.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/basicio.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/bmpimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/convert.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/cr2image.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/crwimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/datasets.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/easyaccess.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/epsimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/error.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/exif.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/futils.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/gifimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/http.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/image.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/ini.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/iptc.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/jp2image.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/jpgimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/matroskavideo.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/metadatum.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/mrwimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/orfimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/pgfimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/preview.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/properties.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/psdimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/quicktimevideo.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/rafimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/riffvideo.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/rw2image.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/tags.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/tgaimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/tiffimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/types.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/utilsvideo.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/value.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/version.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/webpimage.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/xmp_exiv2.hpp \
    $${THIRDPARTY_EXIV2_PATH}/include/exiv2/xmpsidecar.hpp \

!disable_libexpat {
    SOURCES += \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/ExpatAdapter.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/MD5.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/ParseRDF.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/UnicodeConversions.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/WXMPIterator.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/WXMPMeta.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/WXMPUtils.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/XML_Node.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/XMPCore_Impl.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/XMPIterator.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/XMPMeta-GetSet.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/XMPMeta-Parse.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/XMPMeta-Serialize.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/XMPMeta.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/XMPUtils-FileInfo.cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/src/XMPUtils.cpp
    HEADERS += \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/include/MD5.h \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/include/TXMPIterator.hpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/include/TXMPMeta.hpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/include/TXMPUtils.hpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/include/XMP_Const.h \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/include/XMP_Environment.h \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/include/XMP.incl_cpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/include/XMPSDK.hpp \
        $${THIRDPARTY_EXIV2_PATH}/xmpsdk/include/XMP_Version.h
}

!disable_zlib {
    SOURCES += \
        $${THIRDPARTY_EXIV2_PATH}/src/pngchunk_int.cpp \
        $${THIRDPARTY_EXIV2_PATH}/src/pngimage.cpp
    HEADERS += \
        $${THIRDPARTY_EXIV2_PATH}/include/exiv2/pngimage.hpp
}

win32 | haiku {
    *g++*|*clang* {
        SOURCES += \
            $${THIRDPARTY_EXIV2_PATH}/src/localtime.c
    }
}

TR_EXCLUDE += $${THIRDPARTY_EXIV2_PATH}/*

