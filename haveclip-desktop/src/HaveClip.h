/*
  HaveClip

  Copyright (C) 2013 Jakub Skokan <aither@havefun.cz>

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

#ifndef HAVECLIP_H
#define HAVECLIP_H

#include <QSettings>
#include <QList>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QHash>
#include <QSignalMapper>
#include <QIcon>
#include <QSslError>

#include "ClipboardManager.h"
#include "PasteServices/BasePasteService.h"
#include "ClipboardItem.h"

class HaveClip : public QObject
{
	Q_OBJECT
public:
	explicit HaveClip(QObject *parent = 0);
	~HaveClip();
	
signals:
	
public slots:
	void updateHistory();

private:
	ClipboardManager *manager;
	QSettings *settings;
	QSystemTrayIcon *trayIcon;
	QMenu *menu;
	QMenu *historyMenu;
	QAction *historySeparator;
	QAction *menuSeparator;
	QAction *clipSndAction;
	QAction *clipRecvAction;
#ifdef INCLUDE_SERIAL_MODE
	QAction *serialModeAction;
	QSignalMapper *serialRestartMapper;
#endif
	QList<ClipboardContainer*> history;
	QHash<QAction*, ClipboardItem*> historyHash;
	QList<QAction*> pasteActions;
	QSignalMapper *historySignalMapper;
	QSignalMapper *pasteSignalMapper;
	QSignalMapper *pasteAdvSignalMapper;

	void updateHistoryContextMenu();
	void updateToolTip();
	void clearPasteServices();
	void loadPasteServices();

private slots:
	void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void toggleSharedClipboard(bool enabled);
	void historyActionClicked(QObject *obj);
#ifdef INCLUDE_SERIAL_MODE
	void userToggleSerialMode();
	void userToggleSerialModeShortcut();
	void toggleSerialMode(bool enabled);
	void restartSerialBatch(int batch);
#endif
	void showSettings();
	void showAbout();
	void determineCertificateTrust(ClipboardManager::Node *node, const QList<QSslError> errors);
	void determineCertificateTrust(BasePasteService *service, const QList<QSslError> errors);
	void sslFatalError(const QList<QSslError> errors);
	void simplePaste(QObject *obj);
	void advancedPaste(QObject *obj);
	void pasteServiceRequiresAuthentication(BasePasteService *service, QString username, bool failed, QString msg);
	void pasteServiceError(QString error);
};

#endif // HAVECLIP_H
