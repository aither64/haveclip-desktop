QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = ../bin/haveclip

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
    src/NodeAddWizard/NodeDiscoveryModel.h

FORMS += src/AboutDialog.ui \
         src/CertificateTrustDialog.ui \
	 src/SettingsDialog.ui \
    src/NodeDialog.ui \
    src/NodeAddWizard/SearchPage.ui \
    src/NodeAddWizard/IntroductionPage.ui \
    src/NodeAddWizard/VerificationPage.ui \
    src/NodeAddWizard/PairedPage.ui \
    src/SecurityCodePrompt.ui

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
    src/NodeAddWizard/NodeDiscoveryModel.cpp

RESOURCES += HaveClip.qrc

win32:RC_FILE = src/HaveClip.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../haveclip-core/release/bin/ -lhaveclipcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../haveclip-core/debug/bin/ -lhaveclipcore
else:unix: LIBS += -L$$OUT_PWD/../haveclip-core/bin/ -lhaveclipcore

INCLUDEPATH += $$PWD/../haveclip-core/src
DEPENDPATH += $$PWD/../haveclip-core/src

unix|win32: LIBS += -lqca
