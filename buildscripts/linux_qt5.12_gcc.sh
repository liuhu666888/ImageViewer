#!/bin/bash -e
PROJECT="ImageViewer"
BUILDDIR="build_linux_qt5.12_gcc"
SUFFIX="_qt5.12_$(gcc -dumpmachine)"
APP_PATH="src/${PROJECT}"
DESKTOP_PATH="src/${PROJECT}/resources/platform/linux/${PROJECT}.desktop"
ICON_PATH="src/${PROJECT}/resources/icon/icon.svg"

QTDIR="/opt/Qt5.12.6/5.12.6/gcc_64"
CMD_QMAKE="${QTDIR}/bin/qmake"
CMD_DEPLOY="/opt/linuxdeployqt-6-x86_64.AppImage"
CMD_APPIMAGETOOL="/opt/appimagetool-x86_64.AppImage"

cd "$(dirname $0)"/..
rm -rf "${BUILDDIR}"
mkdir -p "${BUILDDIR}"
cd "${BUILDDIR}"
${CMD_QMAKE} -r CONFIG+="release" CONFIG+=c++11 CONFIG+="enable_update_checking" "../${PROJECT}.pro"
make
strip --strip-all "${APP_PATH}/${PROJECT}"

rm -rf "AppDir"
mkdir -p "AppDir/usr/bin" "AppDir/usr/lib" "AppDir/usr/share/applications" "AppDir/usr/share/icons/hicolor/scalable/apps"
cp -a "${APP_PATH}/${PROJECT}" "AppDir/usr/bin/"
cp -a "../${DESKTOP_PATH}" "AppDir/usr/share/applications/${PROJECT}.desktop"
cp -a "../${ICON_PATH}" "AppDir/usr/share/icons/hicolor/scalable/apps/${PROJECT}.svg"
"${CMD_DEPLOY}" "AppDir/usr/share/applications/${PROJECT}.desktop" -always-overwrite -qmake="${CMD_QMAKE}"
"${CMD_APPIMAGETOOL}" --no-appstream "AppDir" ../"${PROJECT}${SUFFIX}.AppImage"
cd ..
