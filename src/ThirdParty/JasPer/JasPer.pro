# URL: https://www.ece.uvic.ca/~frodo/jasper/ + https://github.com/mdadams/jasper
# License: https://www.ece.uvic.ca/~frodo/jasper/LICENSE

TEMPLATE = lib
CONFIG += staticlib
TARGET = tp_jasper

QT -= core gui

CONFIG -= warn_on
CONFIG += exceptions_off rtti_off warn_off

THIRDPARTY_JASPER_PATH = $${PWD}/jasper-573a6e4
THIRDPARTY_JASPER_CONFIG_PATH = $${PWD}/config

include(../CommonSettings.pri)
include(../libjpeg/libjpeg.pri)

INCLUDEPATH = $${THIRDPARTY_JASPER_CONFIG_PATH} $${THIRDPARTY_JASPER_PATH}/src/libjasper/include $${INCLUDEPATH}

*msvc*: DEFINES += JAS_WIN_MSVC_BUILD

SOURCES += \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_cm.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_debug.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_getopt.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_icc.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_iccdata.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_image.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_init.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_malloc.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_seq.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_stream.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_string.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_tmr.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_tvp.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/base/jas_version.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/bmp/bmp_cod.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/bmp/bmp_dec.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/bmp/bmp_enc.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jp2/jp2_cod.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jp2/jp2_dec.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jp2/jp2_enc.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_bs.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_cs.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_dec.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_enc.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_math.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_mct.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_mqcod.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_mqdec.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_mqenc.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_qmfb.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t1cod.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t1dec.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t1enc.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t2cod.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t2dec.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t2enc.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_tagtree.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_tsfb.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_util.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpg/jpg_val.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/mif/mif_cod.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/pgx/pgx_cod.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/pgx/pgx_dec.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/pgx/pgx_enc.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/pnm/pnm_cod.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/pnm/pnm_dec.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/pnm/pnm_enc.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/ras/ras_cod.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/ras/ras_dec.c \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/ras/ras_enc.c

!disable_libjpeg {
    SOURCES += \
        $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpg/jpg_dec.c \
        $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpg/jpg_enc.c
} else {
    SOURCES += \
        $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpg/jpg_dummy.c
}

HEADERS += \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/bmp/bmp_cod.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/bmp/bmp_enc.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jp2/jp2_cod.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jp2/jp2_dec.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_bs.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_cod.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_cs.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_dec.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_enc.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_fix.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_flt.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_math.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_mct.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_mqcod.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_mqdec.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_mqenc.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_qmfb.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t1cod.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t1dec.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t1enc.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t2cod.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t2dec.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_t2enc.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_tagtree.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_tsfb.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpc/jpc_util.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpg/jpg_cod.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpg/jpg_enc.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/jpg/jpg_jpeglib.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/mif/mif_cod.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/pgx/pgx_cod.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/pgx/pgx_enc.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/pnm/pnm_cod.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/pnm/pnm_enc.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/ras/ras_cod.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/ras/ras_enc.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_cm.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_debug.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_dll.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_fix.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_getopt.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_icc.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_image.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_init.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_malloc.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_math.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jasper.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_seq.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_stream.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_string.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_tmr.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_tvp.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_types.h \
    $${THIRDPARTY_JASPER_PATH}/src/libjasper/include/jasper/jas_version.h \
    $${THIRDPARTY_JASPER_CONFIG_PATH}/jasper/jas_config.h

TR_EXCLUDE += $${THIRDPARTY_JASPER_PATH}/*

