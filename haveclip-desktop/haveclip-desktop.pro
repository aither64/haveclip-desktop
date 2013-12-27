QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix: CONFIG += qxt
unix: QXT += core gui

TEMPLATE = app
TARGET = ../bin/haveclip

DEPENDPATH += . \
              src \
              src/PasteServices \
              src/PasteServices/HaveSnippet \
              src/PasteServices/Pastebin \
              src/PasteServices/Stikked
INCLUDEPATH += . \
               src \
               src/PasteServices \
               src/PasteServices/HaveSnippet \
               src/PasteServices/Stikked \
               src/PasteServices/Pastebin

# Input
HEADERS += src/AboutDialog.h \
           src/AutoCompleteLineEdit.h \
           src/CertificateTrustDialog.h \
           src/HaveClip.h \
           src/LoginDialog.h \
           src/SettingsDialog.h \
           src/PasteServices/BasePasteServiceWidget.h \
           src/PasteServices/PasteDialog.h \
           src/PasteServices/PasteServiceEditDialog.h \
           src/PasteServices/HaveSnippet/HaveSnippetSettings.h \
           src/PasteServices/Pastebin/PastebinSettings.h \
           src/PasteServices/Stikked/StikkedSettings.h

FORMS += src/AboutDialog.ui \
         src/CertificateTrustDialog.ui \
         src/LoginDialog.ui \
         src/SettingsDialog.ui \
         src/PasteServices/PasteDialog.ui \
         src/PasteServices/PasteServiceEditDialog.ui \
         src/PasteServices/HaveSnippet/HaveSnippetSettings.ui \
         src/PasteServices/Pastebin/PastebinSettings.ui \
         src/PasteServices/Stikked/StikkedSettings.ui

SOURCES += src/AboutDialog.cpp \
           src/AutoCompleteLineEdit.cpp \
           src/CertificateTrustDialog.cpp \
           src/HaveClip.cpp \
           src/LoginDialog.cpp \
           src/SettingsDialog.cpp \
           src/PasteServices/BasePasteServiceWidget.cpp \
           src/PasteServices/PasteDialog.cpp \
           src/PasteServices/PasteServiceEditDialog.cpp \
           src/PasteServices/HaveSnippet/HaveSnippetSettings.cpp \
           src/PasteServices/Pastebin/PastebinSettings.cpp \
           src/PasteServices/Stikked/StikkedSettings.cpp \
    src/Main.cpp

RESOURCES += HaveClip.qrc

win32:RC_FILE = src/HaveClip.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../haveclip-core/release/bin/ -lhaveclipcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../haveclip-core/debug/bin/ -lhaveclipcore
else:unix: LIBS += -L$$OUT_PWD/../haveclip-core/bin/ -lhaveclipcore

INCLUDEPATH += $$PWD/../haveclip-core/src
DEPENDPATH += $$PWD/../haveclip-core/src
