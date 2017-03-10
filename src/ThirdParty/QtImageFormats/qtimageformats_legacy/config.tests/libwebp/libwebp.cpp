/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the config.tests in the Qt ImageFormats module.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <webp/decode.h>
#include <webp/encode.h>

#if WEBP_ABI_IS_INCOMPATIBLE(WEBP_DECODER_ABI_VERSION, 0x0203) || WEBP_ABI_IS_INCOMPATIBLE(WEBP_ENCODER_ABI_VERSION, 0x0202)
#error "Incompatible libwebp version"
#endif

int main(int, char **)
{
    WebPDecoderConfig config;
    WebPDecBuffer *output_buffer = &config.output;
    WebPBitstreamFeatures *bitstream = &config.input;
    WebPPicture picture;
    picture.use_argb = 0;
    WebPConfig config2;
    config2.lossless = 0;

    return 0;
}
