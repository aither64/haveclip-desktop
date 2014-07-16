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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QListWidgetItem>
#include <QHash>

#include "ClipboardManager.h"
#include "CertificateGenerator.h"

#define NODE_ADD_STR tr("IP address:port")

class Node;
class NodeModel;
class ConnectionManager;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit SettingsDialog(ConnectionManager *conman, QWidget *parent = 0);
	~SettingsDialog();
	QList<Node>& nodes();
	bool historyEnabled();
	int historySize();
	bool saveHistory();
	ClipboardManager::SynchronizeMode synchronizationMode();
	QString host();
	quint16 port();
	ConnectionManager::Encryption encryption();
	QString certificate();
	QString privateKey();
	bool allowAutoDiscover();
	QString networkName();
	
private:
	Ui::SettingsDialog *ui;
	ConnectionManager *conman;
	CertificateGenerator *certGenerator;
	NodeModel *nodeModel;

private slots:
	void addNode();
	void editNode(const QModelIndex &index = QModelIndex());
	void deleteNode();
	void setCertificatePath();
	void setPrivateKeyPath();
	void setFingerprint();
	void generateCertificate();
	void certificateReady();
	void certificateFailed();
};

#endif // SETTINGSDIALOG_H
