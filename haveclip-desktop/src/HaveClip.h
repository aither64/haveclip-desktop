/*
  HaveClip

  Copyright (C) 2013-2016 Jakub Skokan <aither@havefun.cz>

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

#include <QList>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QHash>
#include <QSignalMapper>
#include <QIcon>
#include <QSslError>

#include "ClipboardManager.h"
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
	QSystemTrayIcon *trayIcon;
	QMenu *menu;
	QMenu *historyMenu;
	QAction *historySeparator;
	QAction *menuSeparator;
	QAction *clipSndAction;
	QAction *clipRecvAction;
	QList<ClipboardContainer*> history;
	QHash<QAction*, ClipboardItem*> historyHash;
	QSignalMapper *historySignalMapper;

	void updateHistoryContextMenu();
	void updateToolTip();

private slots:
	void onFirstStart();
	void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void toggleSharedClipboard(bool enabled);
	void toggleSend(bool enabled);
	void toggleReceive(bool enabled);
	void historyActionClicked(QObject *obj);
	void showSettings();
	void showAbout();
	void determineCertificateTrust(const Node &node, const QList<QSslError> errors);
	void sslFatalError(const QList<QSslError> errors);
	void verificationRequest(const Node &n);
	void synchronizeClipboard();

};

#endif // HAVECLIP_H
