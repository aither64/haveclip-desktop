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

#include <QDebug>
#include <QFileDialog>
#include <QClipboard>
#include <QMessageBox>

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "CertificateTrustDialog.h"
#include "NodeModel.h"
#include "NodeAddWizard.h"
#include "NodeDialog.h"
#include "Node.h"
#include "Network/ConnectionManager.h"

SettingsDialog::SettingsDialog(QSettings *settings, ConnectionManager *conman, QWidget *parent) :
        QDialog(parent),
	ui(new Ui::SettingsDialog),
	settings(settings),
	conman(conman)
{
	ui->setupUi(this);

	connect(ui->nodeAddButton, SIGNAL(clicked()), this, SLOT(addNode()));
	connect(ui->nodeEditButton, SIGNAL(clicked()), this, SLOT(editNode()));
	connect(ui->nodeRemoveButton, SIGNAL(clicked()), this, SLOT(deleteNode()));

	nodeModel = new NodeModel(settings, this);
	ui->nodeListView->setModel(nodeModel);

	connect(ui->nodeListView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editNode(QModelIndex)));

	// History
	ui->historyGroupBox->setChecked( settings->value("History/Enable", true).toBool() );
	ui->historySizeSpinBox->setValue( settings->value("History/Size", 10).toInt() );
	ui->historySaveCheckBox->setChecked( settings->value("History/Save", true).toBool() );

	if(qApp->clipboard()->supportsSelection())
	{
		ui->synchronizeComboBox->setCurrentIndex(settings->value("Sync/Synchronize", ClipboardManager::Both).toInt());

	} else {
		ui->syncGroupBox->hide();
	}

	// Encryption
	ui->encryptionComboBox->setCurrentIndex(settings->value("Connection/Encryption", ConnectionManager::None).toInt());
	ui->certificateLineEdit->setText(settings->value("Connection/Certificate", "certs/haveclip.crt").toString());
	ui->keyLineEdit->setText(settings->value("Connection/PrivateKey", "certs/haveclip.key").toString());

	connect(ui->certificateButton, SIGNAL(clicked()), this, SLOT(setCertificatePath()));
	connect(ui->keyButton, SIGNAL(clicked()), this, SLOT(setPrivateKeyPath()));
	connect(ui->certificateLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setFingerprint()));
	connect(ui->genCertButton, SIGNAL(clicked()), this, SLOT(generateCertificate()));

	setFingerprint();

	// Connection
	ui->hostLineEdit->setText( settings->value("Connection/Host", "0.0.0.0").toString() );
	ui->portSpinBox->setValue( settings->value("Connection/Port", 9999).toInt() );
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

QList<Node*> SettingsDialog::nodes()
{
	return nodeModel->nodes();
}

bool SettingsDialog::historyEnabled()
{
	return ui->historyGroupBox->isChecked();
}

int SettingsDialog::historySize()
{
	return ui->historySizeSpinBox->value();
}

bool SettingsDialog::saveHistory()
{
	return ui->historySaveCheckBox->isChecked();
}

ClipboardManager::SynchronizeMode SettingsDialog::synchronizationMode()
{
	return (ClipboardManager::SynchronizeMode) ui->synchronizeComboBox->currentIndex();
}

void SettingsDialog::addNode()
{
	NodeAddWizard *wizard = new NodeAddWizard(conman, this);

	if(wizard->exec() == QDialog::Accepted)
	{
		nodeModel->addNode(new Node(wizard->node()));
	}

	wizard->deleteLater();
}

void SettingsDialog::editNode(const QModelIndex &index)
{
	Node *n = nodeModel->nodeForIndex(index.isValid() ? index : ui->nodeListView->currentIndex());

	if(!n)
		return;

	NodeDialog *dlg = new NodeDialog(n, this);

	if(dlg->exec() == QDialog::Accepted)
	{
		nodeModel->updateNode(dlg->node());
	}

	dlg->deleteLater();
}

void SettingsDialog::deleteNode()
{
	nodeModel->removeNode(ui->nodeListView->currentIndex());
}

QString SettingsDialog::host()
{
	return ui->hostLineEdit->text();
}

quint16 SettingsDialog::port()
{
	return ui->portSpinBox->value();
}

ConnectionManager::Encryption SettingsDialog::encryption()
{
	return (ConnectionManager::Encryption) ui->encryptionComboBox->currentIndex();
}

void SettingsDialog::setCertificatePath()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Select certificate file"), "", tr("Certificates (*.crt *.pem)"));

	if(!path.isEmpty())
		ui->certificateLineEdit->setText(path);
}

void SettingsDialog::setPrivateKeyPath()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Select private key file"), "", tr("Private keys (*.key *.pem)"));

	if(!path.isEmpty())
		ui->keyLineEdit->setText(path);
}

QString SettingsDialog::certificate()
{
	return ui->certificateLineEdit->text();
}

QString SettingsDialog::privateKey()
{
	return ui->keyLineEdit->text();
}

void SettingsDialog::setFingerprint()
{
	QString path = ui->certificateLineEdit->text();

	if(!QFile::exists(path))
		return;

	QList<QSslCertificate> certs = QSslCertificate::fromPath(path);

	if(certs.isEmpty())
		ui->shaFingerLabel->setText(tr("Certificate does not exist or is not valid"));
	else
		ui->shaFingerLabel->setText(CertificateTrustDialog::formatDigest(certs.first().digest(QCryptographicHash::Sha1)));
}

void SettingsDialog::generateCertificate()
{
	certGenerator = new CertificateGenerator(this);

	connect(certGenerator, SIGNAL(finished()), this, SLOT(certificateReady()));

	certGenerator->generate();
}

void SettingsDialog::certificateReady()
{
	certGenerator->savePrivateKeyToFile(ui->keyLineEdit->text()); // FIXME
	certGenerator->saveCertificateToFile(ui->certificateLineEdit->text()); // FIXME

	certGenerator->deleteLater();
	setFingerprint();
}

void SettingsDialog::certificateFailed()
{
	QMessageBox::critical(
		this,
		tr("Unable to generate certificate"),
		tr("You are missing the OpenSSL plugin for QCA. Please"
		   "install it and try again.")
	);

}
