QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
!mac:TARGET = ../bin/haveclip
mac:TARGET = HaveClip

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../haveclip-core/release/ -lhaveclipcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../haveclip-core/debug/ -lhaveclipcore
else:unix: LIBS += -L$$OUT_PWD/../haveclip-core/ -lhaveclipcore

INCLUDEPATH += $$PWD/../haveclip-core/src
DEPENDPATH += $$PWD/../haveclip-core/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../haveclip-core/release/libhaveclipcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../haveclip-core/debug/libhaveclipcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../haveclip-core/release/haveclipcore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../haveclip-core/debug/haveclipcore.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../haveclip-core/libhaveclipcore.a

unix {
    !mac {
        LIBS += -lX11
    }

    CONFIG += link_pkgconfig
    PKGCONFIG += openssl
}

mac:ICON=gfx/HaveClip.icns
