#!/bin/bash
#
# Usage: ./deploy_mac.sh <app_bundle>
#
# This script does:
#  1) run macdeployqt
#  2) copy libhaveclipcore.1.dylib to Frameworks
#  3) fix QCA library path in MacOS/haveclip
#  4) fix QCA library path in Frameworks/libhaveclipcore.1.dylib
#  5) add QCA OSSL plugin to PlugIns/crypto/libqca-ossl.so

## Editable
MACDEPLOYQT="macdeployqt-4.8"
APP="$1"

QCA_PLUGIN_SRC="/Developer/Applications/Qt/plugins/crypto"
QCA_LIB_LOCATION="/Library/Frameworks/libqca.2.dylib"
## --

if [ "$1" == "" ] ; then
	echo "Usage: ./deploy_mac.sh <app_bundle>"
	exit 1
fi

# 1)
$MACDEPLOYQT "$APP"

cd "$APP/Contents" || exit 1

# 2)
cp ../../../haveclip-core/bin/libhaveclipcore.1.dylib Frameworks/

# 3)
install_name_tool -change "$QCA_LIB_LOCATION" @executable_path/../Frameworks/libqca.2.dylib MacOs/haveclip

# 4)
install_name_tool -change "$QCA_LIB_LOCATION" @executable_path/../Frameworks/libqca.2.dylib Frameworks/libhaveclipcore.1.dylib

# 5)
mkdir -p PlugIns/crypto
cp "$QCA_PLUGIN_SRC/libqca-ossl.so" PlugIns/crypto/

echo "Done"
