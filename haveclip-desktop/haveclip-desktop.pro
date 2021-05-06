QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
!mac:TARGET = ../bin/haveclip
mac:TARGET = HaveClip
VERSION = 0.15.0

target.path = /usr/bin/
INSTALLS += target

DEPENDPATH += . \
	      src
INCLUDEPATH += . \
	       src

# Input
HEADERS += src/AboutDialog.h \
           src/CertificateTrustDialog.h \
           src/HaveClip.h \
	   src/SettingsDialog.h \
    src/NodeDialog.h \
    src/NodeModel.h \
    src/NodeAddWizard.h \
    src/NodeAddWizard/SearchPage.h \
    src/NodeAddWizard/IntroductionPage.h \
    src/NodeAddWizard/VerificationPage.h \
    src/NodeAddWizard/PairedPage.h \
    src/SecurityCodePrompt.h \
    src/NodeAddWizard/NodeDiscoveryModel.h \
    src/CertificateGeneratorDialog.h

FORMS += src/AboutDialog.ui \
         src/CertificateTrustDialog.ui \
	 src/SettingsDialog.ui \
    src/NodeDialog.ui \
    src/NodeAddWizard/SearchPage.ui \
    src/NodeAddWizard/IntroductionPage.ui \
    src/NodeAddWizard/VerificationPage.ui \
    src/NodeAddWizard/PairedPage.ui \
    src/SecurityCodePrompt.ui \
    src/CertificateGeneratorDialog.ui

SOURCES += src/AboutDialog.cpp \
           src/CertificateTrustDialog.cpp \
           src/HaveClip.cpp \
	   src/SettingsDialog.cpp \
	   src/Main.cpp \
    src/NodeDialog.cpp \
    src/NodeModel.cpp \
    src/NodeAddWizard.cpp \
    src/NodeAddWizard/SearchPage.cpp \
    src/NodeAddWizard/IntroductionPage.cpp \
    src/NodeAddWizard/VerificationPage.cpp \
    src/NodeAddWizard/PairedPage.cpp \
    src/SecurityCodePrompt.cpp \
    src/NodeAddWizard/NodeDiscoveryModel.cpp \
    src/CertificateGeneratorDialog.cpp

RESOURCES += HaveClip.qrc

win32:RC_FILE = src/HaveClip.rc

win32 {
    CONFIG(release, debug|release): _DEBUG_RELEASE_DIR = release/
    else:                           _DEBUG_RELEASE_DIR = debug/
} # else:unix:                      _DEBUG_RELEASE_DIR =

LIBS += -L$$OUT_PWD/../haveclip-core/$${_DEBUG_RELEASE_DIR} -lhaveclipcore

INCLUDEPATH += $$PWD/../haveclip-core/src
DEPENDPATH += $$PWD/../haveclip-core/src

PRE_TARGETDEPS += $$OUT_PWD/../haveclip-core/$${_DEBUG_RELEASE_DIR}$${QMAKE_PREFIX_STATICLIB}haveclipcore.$${QMAKE_EXTENSION_STATICLIB}


unix:!mac: LIBS += -lX11

# https://doc.qt.io/qt-5.12/qmake-advanced-usage.html#library-dependencies
CONFIG += link_prl

mac {
    ICON=gfx/HaveClip.icns

    QMAKE_TARGET_BUNDLE_PREFIX = "cz.havefun." # hard-coded in "HaveClip.in.plist" (see details in it)
    QMAKE_INFO_PLIST = HaveClip.in.plist
    # Add dependencies (prerequisites) to the "Info.plist" target to remake it when they change.
    info_plist.target = $${TARGET}.app/Contents/Info.plist
    info_plist.depends = $$_PRO_FILE_ $$PWD/$$QMAKE_INFO_PLIST
    QMAKE_EXTRA_TARGETS += info_plist


    # Code signing
    # ============
    # alternative solutions: see the commit message or PR...
    # Add the custom command to the existing standard "all" target
    # (see the generated "haveclip-desktop/Makefile").
    all.commands = codesign -f -s - $${TARGET}.app 2>&1  # `2>&1` (redirect 2-stderr to 1-stdout)
                                                         # to convert "replacing existing signature" error into the message in Compile Output and Issues
    QMAKE_EXTRA_TARGETS += all
}
