/*
   Copyright (C) 2017 Peter S. Zhigalov <peter.zhigalov@gmail.com>

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

#include "MainWindow.h"
#include "MainWindow_p.h"

#include <cassert>

#include <QApplication>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QStringList>
#include <QProcess>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QLabel>
#include <QDir>
#include <QVector>
#include <QtAlgorithms>
#include <QFileSystemWatcher>
#include <QTimer>

#include "Utils/SettingsWrapper.h"
#include "Utils/Workarounds.h"
#include "Utils/FileUtils.h"
#include "Utils/ImageSaver.h"

#include "Decoders/DecodersManager.h"
#include "GUISettings.h"
#include "SettingsDialog.h"
#include "AboutDialog.h"

namespace {

bool numericLessThan(const QString &s1, const QString &s2)
{
    const QString sl1 = s1.toLower(), sl2 = s2.toLower();
    QString::ConstIterator it1 = sl1.constBegin(), it2 = sl2.constBegin();
    for(; it1 != sl1.constEnd() && it2 != sl2.constEnd(); ++it1, ++it2)
    {
        QChar c1 = *it1, c2 = *it2;
        if(c1.isNumber() && c2.isNumber())
        {
            QString num1, num2;
            while(c1.isNumber())
            {
                num1.append(c1);
                if((++it1) == sl1.constEnd())
                    break;
                c1 = *it1;
            }
            while(c2.isNumber())
            {
                num2.append(c2);
                if((++it2) == sl2.constEnd())
                    break;
                c2 = *it2;
            }
            if(num1 != num2)
            {
                return num1.toLongLong() < num2.toLongLong();
            }
            else
            {
                if(it1 == sl1.constEnd() || it2 == sl2.constEnd())
                    break;
                --it1;
                --it2;
            }
        }
        else if(c1 != c2)
        {
            return c1 < c2;
        }
    }
    if(it1 == sl1.constEnd() && it2 != sl2.constEnd())
        return true;
    if(it1 != sl1.constEnd() && it2 == sl2.constEnd())
        return false;
    return s1 < s2;
}

} // namespace

struct MainWindow::Impl
{
    Impl(MainWindow *mainWindow)
        : mainWindow(mainWindow)
        , ui(new UI(mainWindow))
        , settings(new GUISettings(mainWindow))
        , supportedFormats(DecodersManager::getInstance().supportedFormatsWithWildcards())
        , isFullScreenMode(false)
        , imageSaver(mainWindow)
        , watcher(mainWindow)
        , slideShowTimer(mainWindow)
        , isSlideShowMode(false)
    {
        onFileClosed();
    }

    QStringList supportedFilesInDirectory(const QDir &dir) const
    {
        QStringList list = dir.entryList(supportedFormats, QDir::Files | QDir::Readable, QDir::NoSort);
        qSort(list.begin(), list.end(), &numericLessThan);
        return list;
    }

    bool isFileOpened() const
    {
        return ui->imageViewerWidget->imageSize().isValid();
    }

    void updateDirectoryInfo(const QString &path, bool force = false)
    {
        const QFileInfo fileInfo = QFileInfo(path);
        const QDir dir = fileInfo.dir();
        const QString directoryPath = dir.absolutePath();
        const QString fileName = fileInfo.fileName();
        if(directoryPath != currentDirectory || force)
        {
            currentDirectory = directoryPath;
            filesInCurrentDirectory.clear();
            currentIndexInDirectory = -1;
            const QStringList list = supportedFilesInDirectory(dir);
            for(QStringList::ConstIterator it = list.constBegin(); it != list.constEnd(); ++it)
            {
                const QString &name = *it;
                filesInCurrentDirectory.append(name);
                if(currentIndexInDirectory == -1 && name == fileName)
                    currentIndexInDirectory = filesInCurrentDirectory.size() - 1;
            }
            const QStringList currentDirectories = watcher.directories();
            if(currentDirectories.isEmpty() || currentDirectories.first() != directoryPath)
            {
                if(!currentDirectories.isEmpty())
                    watcher.removePaths(currentDirectories);
                if(currentIndexInDirectory >= 0)
                    watcher.addPath(directoryPath);
            }
        }
        else
        {
            if(currentIndexInDirectory > 0 && filesInCurrentDirectory[currentIndexInDirectory - 1] == fileName)
                currentIndexInDirectory--;
            else if(currentIndexInDirectory < filesInCurrentDirectory.size() - 1 && filesInCurrentDirectory[currentIndexInDirectory + 1] == fileName)
                currentIndexInDirectory++;
            else
                currentIndexInDirectory = filesInCurrentDirectory.indexOf(fileName);
        }

        if(currentIndexInDirectory < 0)
        {
            filesInCurrentDirectory.clear();
            currentDirectory.clear();
            ui->navigateNext->setEnabled(false);
            ui->navigatePrevious->setEnabled(false);
            ui->startSlideShow->setEnabled(false);
            ui->actionNavigateNext->setEnabled(false);
            ui->actionNavigatePrevious->setEnabled(false);
            ui->actionStartSlideShow->setEnabled(false);
        }
        else
        {
            ui->navigateNext->setEnabled(true);
            ui->navigatePrevious->setEnabled(true);
            ui->startSlideShow->setEnabled(true);
            ui->actionNavigateNext->setEnabled(true);
            ui->actionNavigatePrevious->setEnabled(true);
            ui->actionStartSlideShow->setEnabled(true);
        }
        const bool deletionEnabled = fileInfo.exists() && fileInfo.isWritable() && isFileOpened();
        ui->deleteFile->setEnabled(deletionEnabled);
        ui->actionDeleteFile->setEnabled(deletionEnabled);
    }

    void onFileClosed()
    {
        const QStringList currentDirectories = watcher.directories();
        if(!currentDirectories.isEmpty())
            watcher.removePaths(currentDirectories);
        filesInCurrentDirectory.clear();
        currentDirectory.clear();
        currentIndexInDirectory = -1;
        ui->navigateNext->setEnabled(false);
        ui->navigatePrevious->setEnabled(false);
        ui->startSlideShow->setEnabled(false);
        ui->deleteFile->setEnabled(false);
        ui->actionNavigateNext->setEnabled(false);
        ui->actionNavigatePrevious->setEnabled(false);
        ui->actionStartSlideShow->setEnabled(false);
        ui->actionDeleteFile->setEnabled(false);
    }

    void syncFullScreen()
    {
        if((isFullScreenMode && mainWindow->isFullScreen()) || (!isFullScreenMode && !mainWindow->isFullScreen()))
            return;

        const bool toFullScreenMode = mainWindow->isFullScreen();
        isFullScreenMode = toFullScreenMode;
        if(toFullScreenMode)
        {
            ui->menubar->setVisible(false);
            ui->toolbar->setVisible(false);
        }
        else
        {
            ui->menubar->setVisible(settings->menuBarVisible());
            ui->toolbar->setVisible(settings->toolBarVisible());
        }
        ui->zoomFullScreen->setChecked(toFullScreenMode);
        ui->actionZoomFullScreen->setChecked(toFullScreenMode);
        mainWindow->updateBackgroundColor();
    }

    MainWindow *mainWindow;
    QScopedPointer<UI> ui;
    GUISettings *settings;
    const QStringList supportedFormats;
    bool isFullScreenMode;

    QString currentDirectory;
    QVector<QString> filesInCurrentDirectory;
    int currentIndexInDirectory;

    ImageSaver imageSaver;
    QFileSystemWatcher watcher;

    QTimer slideShowTimer;
    bool isSlideShowMode;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_impl(new Impl(this))
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAcceptDrops(true);

    QScopedPointer<UI> &ui = m_impl->ui;
    ImageViewerWidget *imageViewerWidget = ui->imageViewerWidget;

    connect(imageViewerWidget, SIGNAL(zoomLevelChanged(qreal)), this, SLOT(updateWindowTitle()));

    connect(ui->navigatePrevious, SIGNAL(clicked()), this, SLOT(onOpenPreviousRequested()));
    connect(ui->navigateNext, SIGNAL(clicked()), this, SLOT(onOpenNextRequested()));
    connect(ui->startSlideShow, SIGNAL(clicked()), this, SLOT(switchSlideShowMode()));
    connect(ui->zoomIn, SIGNAL(clicked()), imageViewerWidget, SLOT(zoomIn()));
    connect(ui->zoomOut, SIGNAL(clicked()), imageViewerWidget, SLOT(zoomOut()));
    connect(ui->zoomFitToWindow, SIGNAL(clicked()), this, SLOT(onZoomFitToWindowClicked()));
    connect(ui->zoomOriginalSize, SIGNAL(clicked()), this, SLOT(onZoomOriginalSizeClicked()));
    connect(ui->zoomFullScreen, SIGNAL(clicked()), this, SLOT(switchFullScreenMode()));
    connect(ui->rotateCounterclockwise, SIGNAL(clicked()), imageViewerWidget, SLOT(rotateCounterclockwise()));
    connect(ui->rotateClockwise, SIGNAL(clicked()), imageViewerWidget, SLOT(rotateClockwise()));
    connect(ui->flipHorizontal, SIGNAL(clicked()), imageViewerWidget, SLOT(flipHorizontal()));
    connect(ui->flipVertical, SIGNAL(clicked()), imageViewerWidget, SLOT(flipVertical()));
    connect(ui->openFile, SIGNAL(clicked()), this, SLOT(onOpenFileWithDialogRequested()));
    connect(ui->saveFileAs, SIGNAL(clicked()), this, SLOT(onSaveAsRequested()));
    connect(ui->deleteFile, SIGNAL(clicked()), this, SLOT(onDeleteFileRequested()));
    connect(ui->preferences, SIGNAL(clicked()), this, SLOT(showPreferences()));
    connect(ui->exit, SIGNAL(clicked()), this, SLOT(onExitRequested()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFileWithDialogRequested()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(onSaveAsRequested()));
    connect(ui->actionNavigatePrevious, SIGNAL(triggered()), this, SLOT(onOpenPreviousRequested()));
    connect(ui->actionNavigateNext, SIGNAL(triggered()), this, SLOT(onOpenNextRequested()));
    connect(ui->actionStartSlideShow, SIGNAL(triggered()), this, SLOT(switchSlideShowMode()));
    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(onExitRequested()));
    connect(ui->actionRotateCounterclockwise, SIGNAL(triggered()), imageViewerWidget, SLOT(rotateCounterclockwise()));
    connect(ui->actionRotateClockwise, SIGNAL(triggered()), imageViewerWidget, SLOT(rotateClockwise()));
    connect(ui->actionFlipHorizontal, SIGNAL(triggered()), imageViewerWidget, SLOT(flipHorizontal()));
    connect(ui->actionFlipVertical, SIGNAL(triggered()), imageViewerWidget, SLOT(flipVertical()));
    connect(ui->actionDeleteFile, SIGNAL(triggered()), this, SLOT(onDeleteFileRequested()));
    connect(ui->actionZoomIn, SIGNAL(triggered()), imageViewerWidget, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), imageViewerWidget, SLOT(zoomOut()));
    connect(ui->actionZoomReset, SIGNAL(triggered()), imageViewerWidget, SLOT(resetZoom()));
    connect(ui->actionZoomFitToWindow, SIGNAL(triggered()), this, SLOT(onZoomFitToWindowClicked()));
    connect(ui->actionZoomOriginalSize, SIGNAL(triggered()), this, SLOT(onZoomOriginalSizeClicked()));
    connect(ui->actionZoomFullScreen, SIGNAL(triggered()), this, SLOT(switchFullScreenMode()));
    connect(ui->actionShowMenuBar, SIGNAL(triggered()), this, SLOT(switchShowMenuBar()));
    connect(ui->actionShowToolBar, SIGNAL(triggered()), this, SLOT(switchShowToolBar()));
    connect(ui->actionEnglish, SIGNAL(triggered()), this, SLOT(onActionEnglishTriggered()));
    connect(ui->actionRussian, SIGNAL(triggered()), this, SLOT(onActionRussianTriggered()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    GUISettings *settings = m_impl->settings;

    connect(settings, SIGNAL(normalBackgroundColorChanged(const QColor&)), this, SLOT(updateBackgroundColor()));
    connect(settings, SIGNAL(fullScreenBackgroundColorChanged(const QColor&)), this, SLOT(updateBackgroundColor()));
    connect(settings, SIGNAL(smoothTransformationChanged(bool)), imageViewerWidget, SLOT(setSmoothTransformation(bool)));
    connect(settings, SIGNAL(slideShowIntervalChanged(int)), this, SLOT(updateSlideShowInterval()));
    connect(settings, SIGNAL(wheelModeChanged(ImageViewerWidget::WheelMode)), imageViewerWidget, SLOT(setWheelMode(ImageViewerWidget::WheelMode)));

    connect(&m_impl->watcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(onDirectoryChanged()));

    connect(&m_impl->slideShowTimer, SIGNAL(timeout()), this, SLOT(onOpenNextRequested()));

    imageViewerWidget->setZoomLevel(settings->zoomLevel());
    imageViewerWidget->setBackgroundColor(settings->normalBackgroundColor());
    imageViewerWidget->setSmoothTransformation(settings->smoothTransformation());
    imageViewerWidget->setWheelMode(settings->wheelMode());
    onZoomModeChanged(settings->zoomMode());
    updateSlideShowInterval();
    setLanguage();

    ui->menubar->setVisible(settings->menuBarVisible());
    ui->actionShowMenuBar->setChecked(settings->menuBarVisible());
    ui->toolbar->setVisible(settings->toolBarVisible());
    ui->actionShowToolBar->setChecked(settings->toolBarVisible());

    restoreState(settings->mainWindowState());
    restoreGeometry(settings->mainWindowGeometry());
}

MainWindow::~MainWindow()
{
    m_impl->settings->setZoomLevel(m_impl->ui->imageViewerWidget->zoomLevel());
    qApp->quit();
}

void MainWindow::setLanguage(const QString &newLanguage)
{
    // Отображение название языка -> соответствующая ему менюшка
    static QMap<QString, QAction*> languagesMap;
    if(languagesMap.isEmpty())
    {
        languagesMap[QString::fromLatin1("en")] = m_impl->ui->actionEnglish;
        languagesMap[QString::fromLatin1("ru")] = m_impl->ui->actionRussian;
    }

    // Определим системную локаль
    static QString systemLang;
    if(systemLang.isEmpty())
    {
        const QString systemLocale = QLocale::system().name().toLower();
        for(QMap<QString, QAction*>::Iterator it = languagesMap.begin(); it != languagesMap.end(); ++it)
        {
            if(systemLocale.startsWith(it.key()))
            {
                systemLang = it.key();
                break;
            }
        }
        if(systemLang.isEmpty())
            systemLang = QString::fromLatin1("en");
    }

    // Посмотрим в настройки, не сохранен ли случайно в них язык
    SettingsWrapper settings;
    QString language = newLanguage;
    if(language.isEmpty())
    {
        const QVariant rawValue = settings.value(QString::fromLatin1("Language"), systemLang);
        const QString value = rawValue.isValid() ? rawValue.toString() : systemLang;
        language = languagesMap.find(value) != languagesMap.end() ? value : systemLang;
    }
    else
    {
        settings.setValue(QString::fromLatin1("Language"), language);
    }

    // Удалим старый перевод, установим новый
    static QTranslator qtTranslator;
    static QTranslator appTranslator;
    static QTranslator utilsTranslator;
    if(!qtTranslator.isEmpty())
        qApp->removeTranslator(&qtTranslator);
    if(!appTranslator.isEmpty())
        qApp->removeTranslator(&appTranslator);
    if(!utilsTranslator.isEmpty())
        qApp->removeTranslator(&utilsTranslator);
    qtTranslator.load(QString::fromLatin1("qt_%1").arg(language), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    appTranslator.load(QString::fromLatin1(":/translations/imageviewer_%1").arg(language));
    utilsTranslator.load(QString::fromLatin1(":/translations/qtutils_%1").arg(language));
    qApp->installTranslator(&qtTranslator);
    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&utilsTranslator);

    // Пофиксим шрифты
    Workarounds::FontsFix(language);

    // Проставим галочку на нужном нам языке и снимем с остальных
    languagesMap[language]->setChecked(true);

    // Перегрузим ресурсы в окнах
    m_impl->ui->retranslate();
    updateWindowTitle();
}

void MainWindow::openNewWindow(const QString &filename)
{
    const QStringList args = filename.isEmpty() ? QStringList() : QStringList(filename);
    QProcess::startDetached(QApplication::applicationFilePath(), args, QDir::currentPath());
}

void MainWindow::updateWindowTitle()
{
    QString label;
    if(m_impl->isFileOpened())
    {
        const QSize imageSize = m_impl->ui->imageViewerWidget->imageSize();
        label = QFileInfo(m_impl->settings->lastOpenedPath()).fileName();
        label.append(QString::fromLatin1(" (%1x%2) %3% - %4")
                     .arg(imageSize.width())
                     .arg(imageSize.height())
                     .arg(static_cast<quint64>(m_impl->ui->imageViewerWidget->zoomLevel() * 100))
                     .arg(qApp->applicationName()));
    }
    else if(m_impl->currentIndexInDirectory >= 0)
    {
        label = QFileInfo(m_impl->filesInCurrentDirectory[m_impl->currentIndexInDirectory]).fileName();
        label.append(QString::fromLatin1(" (%1) - %2")
                     .arg(tr("Error"))
                     .arg(qApp->applicationName()));
    }
    else
    {
        label = qApp->applicationName();
    }
    if(m_impl->currentIndexInDirectory >= 0)
    {
        label.prepend(QString::fromLatin1("[%1/%2] ")
                      .arg(m_impl->currentIndexInDirectory + 1)
                      .arg(m_impl->filesInCurrentDirectory.size()));
    }
    setWindowTitle(label);
}

void MainWindow::showAbout()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::showPreferences()
{
    SettingsDialog dialog(m_impl->settings, this);
    dialog.exec();
}

void MainWindow::switchFullScreenMode()
{
    const bool toFullScreenMode = !m_impl->isFullScreenMode;
    GUISettings *settings = m_impl->settings;
    if(toFullScreenMode)
    {
        settings->setMainWindowGeometry(saveGeometry());
        settings->setMainWindowState(saveState());
        showFullScreen();
    }
    else
    {
        showNormal();
        restoreState(settings->mainWindowState());
        restoreGeometry(settings->mainWindowGeometry());
    }
    m_impl->syncFullScreen();
}

void MainWindow::switchSlideShowMode()
{
    m_impl->isSlideShowMode = !m_impl->isSlideShowMode;
    m_impl->ui->setSlideShowMode(m_impl->isSlideShowMode);
    if(m_impl->isSlideShowMode)
        m_impl->slideShowTimer.start();
    else
        m_impl->slideShowTimer.stop();
}

void MainWindow::switchShowMenuBar()
{
    const bool newValue = !m_impl->settings->menuBarVisible();
    m_impl->settings->setMenuBarVisible(newValue);
    m_impl->ui->actionShowMenuBar->setChecked(newValue);
    if(!m_impl->isFullScreenMode)
        m_impl->ui->menubar->setVisible(newValue);
}

void MainWindow::switchShowToolBar()
{
    const bool newValue = !m_impl->settings->toolBarVisible();
    m_impl->settings->setToolBarVisible(newValue);
    m_impl->ui->actionShowToolBar->setChecked(newValue);
    if(!m_impl->isFullScreenMode)
        m_impl->ui->toolbar->setVisible(newValue);
}

void MainWindow::onOpenPreviousRequested()
{
    int bestMatchedIndex;
    if(m_impl->currentIndexInDirectory > 0)
        bestMatchedIndex = m_impl->currentIndexInDirectory - 1;
    else if(m_impl->filesInCurrentDirectory.size() > 0)
        bestMatchedIndex = m_impl->filesInCurrentDirectory.size() - 1;
    else
        return;
    onOpenFileRequested(QDir(m_impl->currentDirectory).absoluteFilePath(m_impl->filesInCurrentDirectory[bestMatchedIndex]));
}

void MainWindow::onOpenNextRequested()
{
    int bestMatchedIndex;
    if(m_impl->currentIndexInDirectory >= 0 && m_impl->currentIndexInDirectory < m_impl->filesInCurrentDirectory.size() - 1)
        bestMatchedIndex = m_impl->currentIndexInDirectory + 1;
    else if(m_impl->filesInCurrentDirectory.size() > 0)
        bestMatchedIndex = 0;
    else
        return;
    onOpenFileRequested(QDir(m_impl->currentDirectory).absoluteFilePath(m_impl->filesInCurrentDirectory[bestMatchedIndex]));
}

void MainWindow::onOpenFirstRequested()
{
    if(m_impl->filesInCurrentDirectory.size() == 0)
        return;
    onOpenFileRequested(QDir(m_impl->currentDirectory).absoluteFilePath(m_impl->filesInCurrentDirectory[0]));
}

void MainWindow::onOpenLastRequested()
{
    if(m_impl->filesInCurrentDirectory.size() == 0)
        return;
    onOpenFileRequested(QDir(m_impl->currentDirectory).absoluteFilePath(m_impl->filesInCurrentDirectory[m_impl->filesInCurrentDirectory.size() - 1]));
}

void MainWindow::onZoomModeChanged(ImageViewerWidget::ZoomMode mode)
{
    const bool modeIsFitToWindow = mode == ImageViewerWidget::ZOOM_FIT_TO_WINDOW;
    const bool modeIsOriginalSize = mode == ImageViewerWidget::ZOOM_IDENTITY;
    m_impl->ui->imageViewerWidget->setZoomMode(mode);
    m_impl->ui->zoomFitToWindow->setChecked(modeIsFitToWindow);
    m_impl->ui->zoomOriginalSize->setChecked(modeIsOriginalSize);
    m_impl->ui->actionZoomFitToWindow->setChecked(modeIsFitToWindow);
    m_impl->ui->actionZoomOriginalSize->setChecked(modeIsOriginalSize);
    m_impl->settings->setZoomMode(mode);
    updateWindowTitle();
}

void MainWindow::onOpenFileRequested(const QString &filePath)
{
    QGraphicsItem *item = DecodersManager::getInstance().loadImage(filePath);
    if(item)
    {
        m_impl->ui->imageViewerWidget->setZoomMode(m_impl->settings->zoomMode());
        m_impl->ui->imageViewerWidget->setGraphicsItem(item);
        m_impl->ui->setImageControlsEnabled(true);
        m_impl->settings->setLastOpenedPath(filePath);
    }
    else
    {
        m_impl->ui->imageViewerWidget->clear();
        m_impl->ui->setImageControlsEnabled(false);
        QMessageBox::critical(this, tr("Error"), tr("Failed to open file \"%1\"").arg(filePath));
    }
    m_impl->updateDirectoryInfo(filePath);
    updateWindowTitle();
}

void MainWindow::onOpenPathRequested(const QString &path)
{
    const QFileInfo info(path);
    if(info.isFile())
    {
        onOpenFileRequested(path);
    }
    else
    {
        const QDir dir(info.absoluteFilePath());
        const QStringList files = m_impl->supportedFilesInDirectory(dir);
        if(!files.empty())
            onOpenFileRequested(dir.absoluteFilePath(files.first()));
    }
}

void MainWindow::onOpenFileWithDialogRequested()
{
    const QString formatString = QString::fromLatin1("%2 (%1);;%3 (*.*)")
            .arg(m_impl->supportedFormats.join(QString::fromLatin1(" ")))
            .arg(tr("All Supported Images")).arg(tr("All Files"));
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), m_impl->settings->lastOpenedPath(), formatString);
    if(fileName.isEmpty())
        return;
    onOpenFileRequested(fileName);
}

void MainWindow::onSaveAsRequested()
{
    if(!m_impl->isFileOpened())
        return;
    const QString openedPath = m_impl->settings->lastOpenedPath();
    m_impl->imageSaver.setDefaultFilePath(openedPath);
    m_impl->imageSaver.save(m_impl->ui->imageViewerWidget->grabImage(), openedPath);
}

void MainWindow::onDeleteFileRequested()
{
    if(!m_impl->isFileOpened())
        return;

    GUISettings *settings = m_impl->settings;

    if(settings->askBeforeDelete())
    {
        if(QMessageBox::warning(this, tr("Delete File"), tr("Are you sure you want to delete current file?"),
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            return;
    }

    if(settings->moveToTrash())
    {
        QString errorDescription;
        if(!FileUtils::MoveToTrash(settings->lastOpenedPath(), &errorDescription))
        {
            QMessageBox::critical(this, tr("Error"), errorDescription);
            return;
        }
    }
    else
    {
        QFile file(settings->lastOpenedPath());
        if(!file.remove())
        {
            QMessageBox::critical(this, tr("Error"), tr("Failed to delete file \"%1\"").arg(settings->lastOpenedPath()));
            return;
        }
    }

    m_impl->ui->imageViewerWidget->clear();
    m_impl->ui->setImageControlsEnabled(false);
    if(m_impl->currentIndexInDirectory >= 0)
        m_impl->filesInCurrentDirectory.erase(m_impl->filesInCurrentDirectory.begin() + m_impl->currentIndexInDirectory);
    if(m_impl->filesInCurrentDirectory.size() > 0)
    {
        m_impl->currentIndexInDirectory--;
        onOpenNextRequested();
    }
    else
    {
        m_impl->onFileClosed();
        updateWindowTitle();
    }
}

void MainWindow::onExitRequested()
{
    close();
}

void MainWindow::onZoomFitToWindowClicked()
{
    ImageViewerWidget::ZoomMode mode;
    if(m_impl->settings->zoomMode() != ImageViewerWidget::ZOOM_FIT_TO_WINDOW)
        mode = ImageViewerWidget::ZOOM_FIT_TO_WINDOW;
    else
        mode = ImageViewerWidget::ZOOM_CUSTOM;
    onZoomModeChanged(mode);
}

void MainWindow::onZoomOriginalSizeClicked()
{
    ImageViewerWidget::ZoomMode mode;
    if(m_impl->settings->zoomMode() != ImageViewerWidget::ZOOM_IDENTITY)
        mode = ImageViewerWidget::ZOOM_IDENTITY;
    else
        mode = ImageViewerWidget::ZOOM_CUSTOM;
    onZoomModeChanged(mode);
}

void MainWindow::onActionEnglishTriggered()
{
    setLanguage(QString::fromLatin1("en"));
}

void MainWindow::onActionRussianTriggered()
{
    setLanguage(QString::fromLatin1("ru"));
}

void MainWindow::onDirectoryChanged()
{
    m_impl->updateDirectoryInfo(m_impl->settings->lastOpenedPath(), true);
    updateWindowTitle();
}

void MainWindow::updateSlideShowInterval()
{
    m_impl->slideShowTimer.setInterval(m_impl->settings->slideShowInterval() * 1000);
}

void MainWindow::updateBackgroundColor()
{
    const QColor color = m_impl->isFullScreenMode
            ? m_impl->settings->fullScreenBackgroundColor()
            : m_impl->settings->normalBackgroundColor();
    m_impl->ui->imageViewerWidget->setBackgroundColor(color);
}

void MainWindow::changeEvent(QEvent *event)
{
    switch(event->type())
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    case QEvent::ThemeChange:
#endif
    case QEvent::StyleChange:
    case QEvent::PaletteChange:
        m_impl->ui->updateIcons();
        break;
    case QEvent::WindowStateChange:
        m_impl->syncFullScreen();
        break;
    default:
        break;
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!m_impl->isFullScreenMode)
    {
        m_impl->settings->setMainWindowGeometry(saveGeometry());
        m_impl->settings->setMainWindowState(saveState());
    }
    QMainWindow::closeEvent(event);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    if(mimeData->hasUrls())
    {
        const QList<QUrl> urlList = mimeData->urls();
        QList<QUrl>::ConstIterator it = urlList.constBegin();
        const QFileInfo info((it++)->toLocalFile());
        onOpenPathRequested(info.absoluteFilePath());
        for(; it != urlList.constEnd(); ++it)
        {
            const QFileInfo info(it->toLocalFile());
            if(info.isDir())
            {
                const QStringList files = m_impl->supportedFilesInDirectory(info.dir());
                if(!files.empty())
                    openNewWindow(info.dir().absoluteFilePath(files.first()));
            }
            else
            {
                openNewWindow(info.absoluteFilePath());
            }
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Home:
        onOpenFirstRequested();
        break;
    case Qt::Key_End:
        onOpenLastRequested();
        break;
    case Qt::Key_Escape:
        if(m_impl->isFullScreenMode)
            switchFullScreenMode();
        break;
    case Qt::Key_Up:
        m_impl->ui->imageViewerWidget->scrollUp();
        break;
    case Qt::Key_Down:
        m_impl->ui->imageViewerWidget->scrollDown();
        break;
    case Qt::Key_Left:
        m_impl->ui->imageViewerWidget->scrollLeft();
        break;
    case Qt::Key_Right:
        m_impl->ui->imageViewerWidget->scrollRight();
        break;
    default:
        break;
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    m_impl->ui->contextMenu->exec(event->globalPos());
    QMainWindow::contextMenuEvent(event);
}
