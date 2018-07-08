/*
   Copyright (C) 2017-2018 Peter S. Zhigalov <peter.zhigalov@gmail.com>

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

#include "SettingsDialog.h"
#include "SettingsDialog_p.h"

#include <QColorDialog>

#include "Decoders/DecodersManager.h"

namespace {

const char CHECKBOX_PROPERTY_DECODER_NAME[] = "Decoder";

} // namespace

struct SettingsDialog::Impl
{
    Impl(SettingsDialog *widget, GUISettings *settings)
        : settingsDialog(widget)
        , ui(settingsDialog->m_ui.data())
        , settings(settings)
    {
        onNormalBackgroundColorChanged(settings->normalBackgroundColor());
        onFullScreenBackgroundColorChanged(settings->fullScreenBackgroundColor());
        QObject::connect(ui->normalBackgroundColorButton, SIGNAL(clicked()), settingsDialog, SLOT(onNormalColorDialogRequested()));
        QObject::connect(ui->fullScreenBackgroundColorButton, SIGNAL(clicked()), settingsDialog, SLOT(onFullScreenColorDialogRequested()));
        QObject::connect(ui->buttonBox, SIGNAL(rejected()), settingsDialog, SLOT(close()));
        QObject::connect(ui->buttonBox, SIGNAL(accepted()), settingsDialog, SLOT(onSettingsAccepted()));
        QObject::connect(settings, SIGNAL(normalBackgroundColorChanged(const QColor&)), settingsDialog, SLOT(onNormalBackgroundColorChanged(const QColor&)));
        QObject::connect(settings, SIGNAL(fullScreenBackgroundColorChanged(const QColor&)), settingsDialog, SLOT(onFullScreenBackgroundColorChanged(const QColor&)));
    }

    void fillDecoders()
    {
        QVBoxLayout *enabledDecodersLayout = new QVBoxLayout(ui->enabledDecodersFrame);
        DecodersManager &decodersManager = DecodersManager::getInstance();
        const QStringList blacklistedDecoders = decodersManager.blackListedDecoders();
        QStringList registeredDecoders = decodersManager.registeredDecoders();
        registeredDecoders.sort();
        for(QStringList::ConstIterator it = registeredDecoders.constBegin(), itEnd = registeredDecoders.constEnd(); it != itEnd; ++it)
        {
            QString humanReadableName = *it;
            const QString decoderPrefix = QString::fromLatin1("Decoder");
            if(humanReadableName.startsWith(decoderPrefix))
                humanReadableName = humanReadableName.mid(decoderPrefix.length());
            QCheckBox *checkBox = new QCheckBox(humanReadableName, ui->enabledDecodersFrame);
            enabledDecodersLayout->addWidget(checkBox);
            checkBox->setProperty(CHECKBOX_PROPERTY_DECODER_NAME, *it);
            checkBox->setChecked(!blacklistedDecoders.contains(*it));
        }
        ui->enabledDecodersScrollArea->setWidget(ui->enabledDecodersFrame);
    }

    void onSettingsAccepted()
    {
        settings->setAskBeforeDelete(ui->askBeforeDeleteCheckbox->isChecked());
        settings->setMoveToTrash(ui->moveToTrashCheckbox->isChecked());
        settings->setSmoothTransformation(ui->smoothTransformationCheckbox->isChecked());
        settings->setNormalBackgroundColor(normalBackground);
        settings->setFullScreenBackgroundColor(fullScreenBackground);
        settings->setSlideShowInterval(ui->slideShowSpinBox->value());
        settings->setWheelMode(ui->wheelZoomRadioButton->isChecked() ? ImageViewerWidget::WHEEL_ZOOM : ImageViewerWidget::WHEEL_SCROLL);

        const QList<QCheckBox*> enabledDecodersCheckboxes = ui->enabledDecodersFrame->findChildren<QCheckBox*>();
        QStringList blacklistedDecoders;
        for(QList<QCheckBox*>::ConstIterator it = enabledDecodersCheckboxes.constBegin(), itEnd = enabledDecodersCheckboxes.constEnd(); it != itEnd; ++it)
        {
            const QCheckBox *checkBox = *it;
            if(checkBox->checkState() != Qt::Unchecked)
                continue;
            const QString decoderName = checkBox->property(CHECKBOX_PROPERTY_DECODER_NAME).toString();
            if(!decoderName.isEmpty())
                blacklistedDecoders.append(decoderName);
        }
        DecodersManager::getInstance().setBlackListedDecoders(blacklistedDecoders);

        settingsDialog->accept();
    }

    void onNormalBackgroundColorChanged(const QColor &color)
    {
        normalBackground = color;
        ui->updateNormalBackgroundColorButton(color);
    }

    void onFullScreenBackgroundColorChanged(const QColor &color)
    {
        fullScreenBackground = color;
        ui->updateFullScreenBackgroundColorButton(color);
    }

    void onColorDialogRequested(const QColor &oldColor, void (SettingsDialog::*callback)(const QColor&), const char *slot)
    {
        /// @todo Нужен нормальный кроссплатформенный ColorDialog!
#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))
    #if !defined (Q_OS_MAC)
        Q_UNUSED(slot);
        QColorDialog dialog(settingsDialog);
        dialog.setOption(QColorDialog::ShowAlphaChannel, true);
        dialog.setCurrentColor(oldColor);
        dialog.setWindowTitle(tr("Select Background Color"));
        dialog.exec();
        const QColor newColor = dialog.currentColor();
        if(newColor.isValid() && newColor != oldColor)
            (settingsDialog->*callback)(newColor);
    #else
        Q_UNUSED(callback);
        QColorDialog dialog(settingsDialog);
        dialog.setOption(QColorDialog::NoButtons);
        dialog.setOption(QColorDialog::ShowAlphaChannel, true);
        dialog.setCurrentColor(oldColor);
        QObject::connect(&dialog, SIGNAL(currentColorChanged(const QColor&)), settingsDialog, slot);
        dialog.exec();
    #endif
#else
        Q_UNUSED(slot);
        bool ok = true;
        const QRgb newRgba = QColorDialog::getRgba(oldColor.rgba(), &ok, settingsDialog);
        if(!ok)
            return;
        const QColor newColor(qRed(newRgba), qGreen(newRgba), qBlue(newRgba), qAlpha(newRgba));
        if(newColor.isValid() && newColor != oldColor)
            (settingsDialog->*callback)(newColor);
#endif
    }

    SettingsDialog *settingsDialog;
    SettingsDialog::UI *ui;
    GUISettings *settings;
    QColor normalBackground;
    QColor fullScreenBackground;
};

SettingsDialog::SettingsDialog(GUISettings *settings, QWidget *parent)
    : QDialog(parent)
    , m_ui(new UI(this, settings))
    , m_impl(new Impl(this, settings))
{
    setWindowTitle(qApp->translate("SettingsDialog", "Preferences"));

    ensurePolished();
    adjustSize();
    setFixedSize(minimumSize());

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint |
#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))
                   Qt::WindowCloseButtonHint |
#endif
                   Qt::WindowSystemMenuHint | Qt::MSWindowsFixedSizeDialogHint);
    setWindowModality(Qt::ApplicationModal);

    m_impl->fillDecoders();
}

SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    adjustSize();
}

void SettingsDialog::onSettingsAccepted()
{
    m_impl->onSettingsAccepted();
}

void SettingsDialog::onNormalBackgroundColorChanged(const QColor &color)
{
    m_impl->onNormalBackgroundColorChanged(color);
}

void SettingsDialog::onFullScreenBackgroundColorChanged(const QColor &color)
{
    m_impl->onFullScreenBackgroundColorChanged(color);
}

void SettingsDialog::onNormalColorDialogRequested()
{
    m_impl->onColorDialogRequested(m_impl->settings->normalBackgroundColor(),
                                   &SettingsDialog::onNormalBackgroundColorChanged,
                                   SLOT(onNormalBackgroundColorChanged(const QColor&)));
}

void SettingsDialog::onFullScreenColorDialogRequested()
{
    m_impl->onColorDialogRequested(m_impl->settings->fullScreenBackgroundColor(),
                                   &SettingsDialog::onFullScreenBackgroundColorChanged,
                                   SLOT(onFullScreenBackgroundColorChanged(const QColor&)));
}
