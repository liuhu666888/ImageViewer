/*
   Copyright (C) 2017-2019 Peter S. Zhigalov <peter.zhigalov@gmail.com>

   This file is part of the `ImageViewer' program.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "AboutDialog.h"
#include "AboutDialog_p.h"

#include <cstring>

#include <QApplication>
#include <QSysInfo>
#include <QRegExp>

#include "Utils/InfoUtils.h"
#include "Decoders/DecodersManager.h"

#if defined (HAS_LIBPNG)
#include <png.h>
#endif
#if defined (HAS_LIBJPEG)
#include <jpeglib.h>
#endif
#if defined (HAS_OPENJPEG)
#include <openjpeg.h>
#endif
#if defined (HAS_LIBJASPER)
#include "Workarounds/BeginIgnoreShiftNegative.h"
#include <jasper/jasper.h>
#include "Workarounds/EndIgnoreShiftNegative.h"
#endif
#if defined (HAS_LIBMNG)
#include <libmng.h>
#endif
#if defined (HAS_LIBTIFF)
#include <tiffio.h>
#endif
#if defined (HAS_LIBWEBP)
#include <webp/decode.h>
#endif
#if defined (HAS_GIFLIB)
#include <gif_lib.h>
#endif
#if defined (HAS_LIBRAW)
#include "Workarounds/BeginIgnoreDeprecated.h"
#include <libraw/libraw_version.h>
#include "Workarounds/EndIgnoreDeprecated.h"
#endif
#if defined (HAS_LIBHEIF)
#include <libheif/heif.h>
#endif
#if defined (HAS_OPENEXR)
#include <OpenEXR/OpenEXRConfig.h>
#endif
#if defined (HAS_JBIGKIT)
#include <jbig.h>
#endif
#if defined (HAS_EXIV2)
#include "Workarounds/BeginIgnoreDeprecated.h"
#include <exiv2/exiv2.hpp>
#include "Workarounds/EndIgnoreDeprecated.h"
#endif
#if defined (HAS_LCMS2)
#include <lcms2.h>
#endif
#if defined (HAS_ZLIB)
#include <zlib.h>
#endif
#if defined (HAS_ZSTD)
#include <zstd.h>
#endif
#if defined (HAS_XZUTILS)
#include <lzma.h>
#endif
#if defined (HAS_LIBEXPAT)
#include <expat.h>
#endif
#if defined (HAS_FREETYPE)
#include <ft2build.h>
#include FT_FREETYPE_H
#endif
#if defined (HAS_LIBDE256)
#include <libde265/de265.h>
#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

namespace {

const char *ABOUT_PIXMAP_PATH     = ":/icon/icon_64.png";
const char *ABOUT_URL_STRING      = "https://github.com/AlienCowEatCake/ImageViewer";
const char *ABOUT_LICENSE_STRIG   = "<a href=\"http://www.gnu.org/copyleft/gpl.html\">GNU GPL v3</a>";
const char *ABOUT_YEAR_STRING     = "2017-2019";
const char *ABOUT_AUTHOR_STRING   = QT_TRANSLATE_NOOP("AboutDialog", "Peter S. Zhigalov");
const char *ABOUT_EMAIL_STRING    = "peter.zhigalov@gmail.com";

QString letterByNumFrom0(int num)
{
    static const QString alphabet = QString::fromLatin1("abcdefghijklmnopqrstuvwxyz");
    if(num < 0 || num > 26)
        return QString();
    return QString(alphabet[num]);
}

QString letterByNumFrom1(int num)
{
    return letterByNumFrom0(num - 1);
}

QString formatItem(const QString &title, const QString &name, const QString &version, const QString &url, const QString& license, const QString& licenseUrl)
{
    QString result;
    if(!title.isEmpty())
        result.append(QString::fromLatin1("<i>%1</i><br>").arg(title));
    result.append(QString::fromLatin1("<b>%1</b>").arg(name));
    if(!version.isEmpty())
        result.append(QString::fromLatin1(", version %1").arg(version));
    if(!url.isEmpty())
        result.append(QString::fromLatin1("<br><a href=\"%1\">%1</a>").arg(url));
    if(!license.isEmpty() && !licenseUrl.isEmpty())
        result.append(QString::fromLatin1("<br>License: <a href=\"%2\" title=\"%2\">%1</a>").arg(license).arg(licenseUrl));
    else if(!license.isEmpty())
        result.append(QString::fromLatin1("<br>License: %1").arg(license));
    else if(!licenseUrl.isEmpty())
        result.append(QString::fromLatin1("<br>License: <a href=\"%1\">%1</a>").arg(licenseUrl));
    result.append(QString::fromLatin1("<br><br>"));
    return result;
}

QString getTextBrowserContent()
{
    QString result;

    result.append(QString::fromLatin1("<b>%1</b> %2<br>")
                  .arg(QString::fromLatin1("System:"))
                  .arg(InfoUtils::GetSystemDescription())
                  );

    result.append(QString::fromLatin1("<b>%1</b> %2<br>")
                  .arg(QString::fromLatin1("Compiler:"))
                  .arg(InfoUtils::GetCompilerDescription())
                  );

    QStringList availableDecoders = DecodersManager::getInstance().registeredDecoders();
    for(QStringList::Iterator it = availableDecoders.begin(); it != availableDecoders.end(); ++it)
    {
        const QString decoderPrefix = QString::fromLatin1("Decoder");
        if(it->startsWith(decoderPrefix))
            *it = it->mid(decoderPrefix.length()).toLower();
        else
            *it = it->toLower();
    }
    availableDecoders.sort();

    result.append(QString::fromLatin1("<b>%1</b> %2<br><br>")
            .arg(QString::fromLatin1("Available image decoders:"))
            .arg(availableDecoders.join(QString::fromLatin1(", "))));

    result.append(formatItem(
                      QString::fromLatin1("This software uses the Qt framework"),
                      QString::fromLatin1("qt"),
                      QString::fromLatin1(QT_VERSION_STR),
                      QString::fromLatin1("https://www.qt.io/"),
                      QString::fromLatin1("GNU LGPL v3 or Commercial"),
                      QString::fromLatin1("https://www.qt.io/licensing/")
                      ));

#if defined (HAS_LIBPNG)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the PNG reference library"),
                      QString::fromLatin1("libpng"),
                      QString::fromLatin1("%1.%2.%3").arg(PNG_LIBPNG_VER_MAJOR).arg(PNG_LIBPNG_VER_MINOR).arg(PNG_LIBPNG_VER_RELEASE),
                      QString::fromLatin1("http://www.libpng.org/pub/png/libpng.html"),
                      QString::fromLatin1("Zlib License"),
                      QString::fromLatin1("http://www.libpng.org/pub/png/src/libpng-LICENSE.txt")
                      ));
#if defined (PNG_APNG_SUPPORTED)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the APNG patch for libpng"),
                      QString::fromLatin1("libpng-apng"),
                      QString::fromLatin1("%1.%2.%3").arg(PNG_LIBPNG_VER_MAJOR).arg(PNG_LIBPNG_VER_MINOR).arg(PNG_LIBPNG_VER_RELEASE),
                      QString::fromLatin1("https://sourceforge.net/projects/libpng-apng/"),
                      QString::fromLatin1("Zlib License"),
                      QString::fromLatin1("https://opensource.org/licenses/Zlib")
                      ));
#endif
#endif

#if defined (HAS_LIBJPEG)
#if defined (LIBJPEG_TURBO_VERSION)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the libjpeg-turbo library"),
                      QString::fromLatin1("libjpeg-turbo"),
                      QString::fromLatin1(TOSTRING(LIBJPEG_TURBO_VERSION)),
                      QString::fromLatin1("https://www.libjpeg-turbo.org/"),
                      QString::fromLatin1("IJG License + Modified 3-clause BSD License + Zlib License"),
                      QString::fromLatin1("https://github.com/libjpeg-turbo/libjpeg-turbo/blob/master/LICENSE.md")
                      ));
#else
    result.append(formatItem(
                      QString::fromLatin1("This software is based in part on the work of the Independent JPEG Group"),
                      QString::fromLatin1("libjpeg"),
                      QString::number(JPEG_LIB_VERSION_MAJOR) + letterByNumFrom1(JPEG_LIB_VERSION_MINOR),
                      QString::fromLatin1("http://www.ijg.org/"),
                      QString::fromLatin1("Libjpeg License"),
                      QString::fromLatin1("https://jpegclub.org/reference/libjpeg-license/")
                      ));
#endif
#endif
    Q_UNUSED(letterByNumFrom1(0));

#if defined (HAS_OPENJPEG)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the OpenJPEG library"),
                      QString::fromLatin1("libopenjp2"),
                      QString::fromLatin1("%1.%2.%3").arg(OPJ_VERSION_MAJOR).arg(OPJ_VERSION_MINOR).arg(OPJ_VERSION_BUILD),
                      QString::fromLatin1("http://www.openjpeg.org/"),
                      QString::fromLatin1("2-clause BSD License"),
                      QString::fromLatin1("https://github.com/uclouvain/openjpeg/blob/master/LICENSE")
                      ));
#endif

#if defined (HAS_LIBJASPER)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the JasPer image processing/coding tool kit"),
                      QString::fromLatin1("libjasper"),
                      strcmp(JAS_VERSION, "unknown") ? QString::fromLatin1(JAS_VERSION) : QString(),
                      QString::fromLatin1("https://www.ece.uvic.ca/~frodo/jasper/"),
                      QString::fromLatin1("JasPer License v2.0"),
                      QString::fromLatin1("https://www.ece.uvic.ca/~frodo/jasper/LICENSE")
                      ));
#endif

#if defined (HAS_LIBMNG)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the Multiple-image Network Graphics (MNG) reference library"),
                      QString::fromLatin1("libmng"),
                      QString::fromLatin1("%1.%2.%3").arg(MNG_VERSION_MAJOR).arg(MNG_VERSION_MINOR).arg(MNG_VERSION_RELEASE),
                      QString::fromLatin1("https://sourceforge.net/projects/libmng/"),
                      QString::fromLatin1("Zlib License"),
                      QString::fromLatin1("https://opensource.org/licenses/Zlib")
                      ));
#endif

#if defined (HAS_LIBTIFF)
    QRegExp tiffVersionRegExp(QString::fromLatin1("(\\d*\\.\\d*)(\\.\\d*)?"));
    result.append(formatItem(
                      QString::fromLatin1("This software uses the LibTIFF library"),
                      QString::fromLatin1("libtiff"),
                      (tiffVersionRegExp.indexIn(QString::fromLatin1(TIFFGetVersion())) >= 0) ? tiffVersionRegExp.cap(0) : QString(),
                      QString::fromLatin1("http://www.simplesystems.org/libtiff/"),
                      QString::fromLatin1("LibTiff License"),
                      QString::fromLatin1("https://gitlab.com/libtiff/libtiff/blob/master/COPYRIGHT")
                      ));
#endif

#if defined (HAS_LIBWEBP)
    const QString webpVersion = QString::fromLatin1("%1").arg(QString::number(WebPGetDecoderVersion(), 16), 6, QChar::fromLatin1('0'));
    result.append(formatItem(
                      QString::fromLatin1("This software uses the WebP image library"),
                      QString::fromLatin1("libwebp"),
                      QString::fromLatin1("%1.%2.%3").arg(webpVersion.mid(0, 2).toInt()).arg(webpVersion.mid(2, 2).toInt()).arg(webpVersion.mid(4, 2).toInt()),
                      QString::fromLatin1("https://www.webmproject.org/"),
                      QString::fromLatin1("3-clause BSD License"),
                      QString::fromLatin1("https://chromium.googlesource.com/webm/libwebp/+/refs/heads/master/COPYING")
                      ));
#endif

#if defined (HAS_LIBBPG)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the libbpg library"),
                      QString::fromLatin1("libbpg"),
                      QString(),
                      QString::fromLatin1("https://bellard.org/bpg/"),
                      QString::fromLatin1("MIT License + GNU LGPL v2.1"),
                      QString()
                      ));
#endif

#if defined (HAS_LIBWMF)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the libwmf library"),
                      QString::fromLatin1("libwmf"),
                      QString(),
                      QString::fromLatin1("http://wvware.sourceforge.net/libwmf.html"),
                      QString::fromLatin1("GNU GPL v2"),
                      QString::fromLatin1("https://www.gnu.org/licenses/old-licenses/gpl-2.0.html")
                      ));
#endif

#if defined (HAS_GIFLIB)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the GIFLIB library"),
                      QString::fromLatin1("giflib"),
#if defined (GIFLIB_MAJOR) && defined (GIFLIB_MINOR) && defined (GIFLIB_RELEASE)
                      QString::fromLatin1("%1.%2.%3").arg(GIFLIB_MAJOR).arg(GIFLIB_MINOR).arg(GIFLIB_RELEASE),
#else
                      QString(),
#endif
                      QString::fromLatin1("http://giflib.sourceforge.net/"),
                      QString::fromLatin1("MIT License"),
                      QString::fromLatin1("https://sourceforge.net/p/giflib/code/ci/master/tree/COPYING")
                      ));
#endif

#if defined (HAS_LIBRAW)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the LibRaw library"),
                      QString::fromLatin1("libraw"),
                      QString::fromLatin1(LIBRAW_VERSION_STR),
                      QString::fromLatin1("https://www.libraw.org/"),
                      QString::fromLatin1("GNU LGPL v2.1 or CDDL v1.0"),
                      QString::fromLatin1("https://github.com/LibRaw/LibRaw/blob/master/COPYRIGHT")
                      ));
#endif

#if defined (HAS_LIBHEIF)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the libheif library"),
                      QString::fromLatin1("libheif"),
                      QString::fromLatin1(LIBHEIF_VERSION),
                      QString::fromLatin1("https://github.com/strukturag/libheif"),
                      QString::fromLatin1("GNU LGPL v3"),
                      QString::fromLatin1("https://github.com/strukturag/libheif/blob/master/COPYING")
                      ));
#endif

#if defined (HAS_OPENEXR)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the OpenEXR library"),
                      QString::fromLatin1("openexr"),
                      QString::fromLatin1(OPENEXR_VERSION_STRING),
                      QString::fromLatin1("https://www.openexr.com/"),
                      QString::fromLatin1("3-clause BSD License"),
                      QString::fromLatin1("https://github.com/openexr/openexr/blob/master/LICENSE.md")
                      ));
#endif

#if defined (HAS_JBIGKIT)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the JBIG-KIT library"),
                      QString::fromLatin1("jbigkit"),
                      QString::fromLatin1(JBG_VERSION),
                      QString::fromLatin1("https://www.cl.cam.ac.uk/~mgk25/jbigkit/"),
                      QString::fromLatin1("GNU GPL v2 or Commercial"),
                      QString::fromLatin1("https://www.cl.cam.ac.uk/~mgk25/jbigkit/#licensing")
                      ));
#endif

#if defined (HAS_LIBEXIF)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the libexif C EXIF library"),
                      QString::fromLatin1("libexif"),
                      QString(),
                      QString::fromLatin1("https://libexif.github.io/"),
                      QString::fromLatin1("GNU LGPL v2.1"),
                      QString::fromLatin1("https://github.com/libexif/libexif/blob/master/COPYING")
                      ));
#endif

#if defined (HAS_EXIV2)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the Exiv2 C++ metadata library"),
                      QString::fromLatin1("exiv2"),
                      QString::fromLatin1(Exiv2::versionString().c_str()),
                      QString::fromLatin1("http://www.exiv2.org/"),
                      QString::fromLatin1("GNU GPL v2 or later"),
                      QString::fromLatin1("https://www.gnu.org/licenses/old-licenses/gpl-2.0.html")
                      ));
#endif

#if defined (HAS_LCMS2)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the Little CMS 2 library"),
                      QString::fromLatin1("lcms2"),
                      QString::fromLatin1("%1.%2").arg(LCMS_VERSION / 1000).arg(LCMS_VERSION % 100 / 10),
                      QString::fromLatin1("http://www.littlecms.com/"),
                      QString::fromLatin1("MIT License"),
                      QString::fromLatin1("https://opensource.org/licenses/mit-license")
                      ));
#endif

#if defined (HAS_ZLIB)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the zlib library"),
                      QString::fromLatin1("zlib"),
                      QString::fromLatin1(ZLIB_VERSION),
                      QString::fromLatin1("http://www.zlib.net/"),
                      QString::fromLatin1("Zlib License"),
                      QString::fromLatin1("http://www.zlib.net/zlib_license.html")
                      ));
#endif

#if defined (HAS_ZSTD)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the Zstandard library"),
                      QString::fromLatin1("zstd"),
                      QString::fromLatin1("%1.%2.%3").arg(ZSTD_VERSION_MAJOR).arg(ZSTD_VERSION_MINOR).arg(ZSTD_VERSION_RELEASE),
                      QString::fromLatin1("https://facebook.github.io/zstd/"),
                      QString::fromLatin1("3-clause BSD License"),
                      QString::fromLatin1("https://github.com/facebook/zstd/blob/dev/LICENSE")
                      ));
#endif

#if defined (HAS_XZUTILS)
    result.append(formatItem(
                      QString::fromLatin1("This software uses part of the XZ Utils"),
                      QString::fromLatin1("xz"),
                      QString::fromLatin1(lzma_version_string()),
                      QString::fromLatin1("https://tukaani.org/xz/"),
                      QString::fromLatin1("Public Domain"),
                      QString::fromLatin1("https://git.tukaani.org/?p=xz.git;a=blob;f=COPYING")
                      ));
#endif

#if defined (HAS_LIBEXPAT)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the Expat, a C library for parsing XML"),
                      QString::fromLatin1("libexpat"),
                      QString::fromLatin1("%1.%2.%3").arg(XML_MAJOR_VERSION).arg(XML_MINOR_VERSION).arg(XML_MICRO_VERSION),
                      QString::fromLatin1("https://libexpat.github.io/"),
                      QString::fromLatin1("MIT License"),
                      QString::fromLatin1("https://github.com/libexpat/libexpat/blob/master/expat/COPYING")
                      ));
#endif

#if defined (HAS_FREETYPE)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the FreeType library"),
                      QString::fromLatin1("freetype"),
                      QString::fromLatin1("%1.%2.%3").arg(FREETYPE_MAJOR).arg(FREETYPE_MINOR).arg(FREETYPE_PATCH),
                      QString::fromLatin1("https://freetype.org/"),
                      QString::fromLatin1("FreeType License or GNU GPL v2"),
                      QString::fromLatin1("https://www.freetype.org/license.html")
                      ));
#endif

#if defined (HAS_LIBDE256)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the libde265 library"),
                      QString::fromLatin1("libde265"),
                      QString::fromLatin1(LIBDE265_VERSION),
                      QString::fromLatin1("https://www.libde265.org/"),
                      QString::fromLatin1("GNU LGPL v3"),
                      QString::fromLatin1("https://github.com/strukturag/libde265/blob/master/COPYING")
                      ));
#endif

#if defined (HAS_QTEXTENDED)
    result.append(formatItem(
                      QString::fromLatin1("This software uses part of the Qt Extended library"),
                      QString::fromLatin1("qt-extended"),
                      QString::fromLatin1("4.4.3"),
                      QString::fromLatin1("https://sourceforge.net/projects/qpe/files/QPE/qtopia/"),
                      QString::fromLatin1("GNU GPL v2"),
                      QString::fromLatin1("https://www.gnu.org/licenses/old-licenses/gpl-2.0.html")
                      ));
#endif

#if defined (HAS_QTIMAGEFORMATS)
    result.append(formatItem(
                      QString::fromLatin1("This software uses the additional Image Format plugins for Qt"),
                      QString::fromLatin1("qtimageformats"),
                      QString(),
                      QString::fromLatin1("https://github.com/qt/qtimageformats"),
                      QString::fromLatin1("GNU GPL v2 or GNU LGPL v3 or Commercial"),
                      QString::fromLatin1("https://www.qt.io/licensing/")
                      ));
#endif

#if defined (HAS_STB)
    result.append(formatItem(
                      QString::fromLatin1("This software uses part of the STB library"),
                      QString::fromLatin1("stb"),
                      QString(),
                      QString::fromLatin1("https://github.com/nothings/stb"),
                      QString::fromLatin1("Public Domain or MIT License"),
                      QString::fromLatin1("https://github.com/nothings/stb/blob/master/README.md")
                      ));
#endif

#if defined (HAS_NANOSVG)
    result.append(formatItem(
                      QString::fromLatin1("This software uses Nano SVG parser and rasterizer"),
                      QString::fromLatin1("nanosvg"),
                      QString(),
                      QString::fromLatin1("https://github.com/memononen/nanosvg"),
                      QString::fromLatin1("Zlib License"),
                      QString::fromLatin1("https://github.com/memononen/nanosvg/blob/master/LICENSE.txt")
                      ));
#endif

    return result;
}

} // namespace

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(new UI(this))
{
    connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(close()));

    m_ui->iconLabel->setPixmap(qApp->translate("AboutDialog", ABOUT_PIXMAP_PATH));
    setWindowTitle(qApp->translate("AboutDialog", "About"));
    m_ui->textLabel->setText(QString::fromLatin1(
                                 "<h3>%1 v%2</h3>"
                                 "<a href=\"%3\">%3</a><br>"
                                 "%4: %5<br><br>"
                                 "Copyright &copy; %6<br>"
                                 "%7 &lt;<a href=\"mailto:%8\">%8</a>&gt;"
                                 )
                             .arg(qApp->applicationName())
                             .arg(qApp->applicationVersion())
                             .arg(qApp->translate("AboutDialog", ABOUT_URL_STRING))
                             .arg(qApp->translate("AboutDialog", "License"))
                             .arg(qApp->translate("AboutDialog", ABOUT_LICENSE_STRIG))
                             .arg(qApp->translate("AboutDialog", ABOUT_YEAR_STRING))
                             .arg(qApp->translate("AboutDialog", ABOUT_AUTHOR_STRING))
                             .arg(qApp->translate("AboutDialog", ABOUT_EMAIL_STRING))
                             );
    m_ui->textBrowser->setHtml(getTextBrowserContent());

    ensurePolished();
    adjustSize();
    setFixedSize(minimumSize());

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint |
#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))
                   Qt::WindowCloseButtonHint |
#endif
                   Qt::WindowSystemMenuHint | Qt::MSWindowsFixedSizeDialogHint);
    setWindowModality(Qt::ApplicationModal);
}

AboutDialog::~AboutDialog()
{}

void AboutDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    m_ui->textBrowser->setFixedWidth(width() - m_ui->centralWidget->layoutMarginLeft() - m_ui->centralWidget->layoutMarginRight());
}
