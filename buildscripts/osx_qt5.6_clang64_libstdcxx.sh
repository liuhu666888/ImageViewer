#!/bin/bash -e
PROJECT=ImageViewer
BUILDDIR=build_osx_qt5.6_clang64_libstdcxx
APPNAME="Image Viewer"
DMGNAME="${PROJECT}_qt5.6_clang64_libstdcxx"
OUT_PATH="src/${PROJECT}"
ALL_SDK_VERSIONS="$(xcodebuild -showsdks | grep '\-sdk macosx' | sed 's|.*-sdk macosx||')"
for SDK_VERSION in ${ALL_SDK_VERSIONS} ; do
	SDK_PATH="$(xcode-select -p)/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${SDK_VERSION}.sdk"
	if [[ $(find "${SDK_PATH}/usr/lib" -name 'libstdc++*' -maxdepth 1 | wc -l | xargs) > 0 ]] ; then
		MAC_SDK="macosx${SDK_VERSION}"
	fi
done

QT_PATH="/opt/Qt/5.6.3/clang_64_libstdc++_sdk10.10"
QTPLUGINS_PATH="${QT_PATH}/plugins"
CMD_QMAKE="${QT_PATH}/bin/qmake"
CMD_DEPLOY="${QT_PATH}/bin/macdeployqt"

echo "Using MAC_SDK=${MAC_SDK}"

cd "$(dirname $0)"/..
rm -rf "${BUILDDIR}"
mkdir -p "${BUILDDIR}"
cd "${BUILDDIR}"
BUILD_PATH="${PWD}"
${CMD_QMAKE} -r CONFIG+="release" LIBS+=-dead_strip QMAKE_MAC_SDK=${MAC_SDK} "../${PROJECT}.pro"
make -j3
cd "${OUT_PATH}"
plutil -replace LSMinimumSystemVersion -string "10.6" "${APPNAME}.app/Contents/Info.plist"
RES_PATH="${APPNAME}.app/Contents/Resources"
rm -f "${RES_PATH}/empty.lproj"
mkdir -p "${RES_PATH}/en.lproj" "${RES_PATH}/ru.lproj"
PLUGINS_PATH="${APPNAME}.app/Contents/PlugIns"
mkdir -p "${PLUGINS_PATH}/iconengines"
for iconengines_plugin in libqsvgicon.dylib ; do
	cp -a "${QTPLUGINS_PATH}/iconengines/${iconengines_plugin}" "${PLUGINS_PATH}/iconengines/"
done
${CMD_DEPLOY} "${APPNAME}.app" -verbose=2
cd "${BUILD_PATH}"

INSTALL_PATH="${PWD}/install"
ARTIFACTS_PATH="${PWD}/artifacts"
mkdir -p "${INSTALL_PATH}" "${ARTIFACTS_PATH}"
mv "${OUT_PATH}/${APPNAME}.app" "${INSTALL_PATH}/"
cd "${INSTALL_PATH}"
ln -s /Applications ./Applications
find "${APPNAME}.app/Contents/PlugIns" -name "*_debug.dylib" -delete
cd "${BUILD_PATH}"
hdiutil create -format UDBZ -fs HFS+ -srcfolder "${INSTALL_PATH}" -volname "${APPNAME}" "${ARTIFACTS_PATH}/${DMGNAME}.dmg"

