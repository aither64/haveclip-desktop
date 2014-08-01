#!/bin/bash
#
# Usage: ./deploy_mac.sh <app_bundle>
#
# This script does:
#  1) run macdeployqt
#  2) copy libs to Frameworks
#  3) fix QCA library path in MacOS/haveclip
#  4) fix library paths in Frameworks/libhaveclipcore.1.dylib and libqca.2.dylib
#  5) add QCA OSSL plugin to PlugIns/crypto/libqca-ossl.so, fix library paths

## Editable
MACDEPLOYQT="macdeployqt-4.8"
APP="$1"

HAVECLIP_CORE="libhaveclipcore.1.dylib"
QCA_LIB="libqca.2.dylib"
QCA_PLUGIN_SRC="/Developer/Applications/Qt/plugins/crypto"
QCA_LIB_LOCATION="/Library/Frameworks/$QCA_LIB"
QCA_PLUGINS="ossl"
## --

if [ "$1" == "" ] ; then
	echo "Usage: ./deploy_mac.sh <app_bundle>"
	exit 1
fi

# 1)
$MACDEPLOYQT "$APP"

cd "$APP/Contents" || exit 1

# 2)
cp "$QCA_LIB_LOCATION" Frameworks/
cp ../../../haveclip-core/bin/$HAVECLIP_CORE Frameworks/

# 3)
install_name_tool -change "$QCA_LIB_LOCATION" @executable_path/../Frameworks/$QCA_LIB MacOs/haveclip

# 4)
cd Frameworks
install_name_tool -change "$QCA_LIB_LOCATION" @executable_path/../Frameworks/$QCA_LIB $HAVECLIP_CORE
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $HAVECLIP_CORE
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $HAVECLIP_CORE
install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork $HAVECLIP_CORE

install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $QCA_LIB
install_name_tool -id $QCA_LIB $QCA_LIB

cd ..

# 5)
mkdir -p PlugIns/crypto
cd PlugIns/crypto

for plugin in $QCA_PLUGINS ; do
	plugin_file="libqca-$plugin.so"
	
	cp "$QCA_PLUGIN_SRC/$plugin_file" ./
	
	install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $plugin_file
	install_name_tool -change "$QCA_LIB_LOCATION" @executable_path/../Frameworks/$QCA_LIB $plugin_file
done

cd ../..

echo "Done"

