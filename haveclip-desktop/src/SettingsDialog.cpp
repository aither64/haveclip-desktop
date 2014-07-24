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

#include "Settings.h"
#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "CertificateTrustDialog.h"
#include "NodeModel.h"
#include "NodeAddWizard.h"
#include "NodeDialog.h"
#include "Node.h"
#include "Network/ConnectionManager.h"

SettingsDialog::SettingsDialog(ConnectionManager *conman, QWidget *parent) :
        QDialog(parent),
	ui(new Ui::SettingsDialog),
	conman(conman)
{
	ui->setupUi(this);

	connect(ui->nodeAddButton, SIGNAL(clicked()), this, SLOT(addNode()));
	connect(ui->nodeEditButton, SIGNAL(clicked()), this, SLOT(editNode()));
	connect(ui->nodeRemoveButton, SIGNAL(clicked()), this, SLOT(deleteNode()));

	nodeModel = new NodeModel(this);
	ui->nodeListView->setModel(nodeModel);

	connect(ui->nodeListView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editNode(QModelIndex)));

	Settings *s = Settings::get();

	// History
	ui->historyGroupBox->setChecked( s->isHistoryEnabled());
	ui->historySizeSpinBox->setValue( s->historySize() );
	ui->historySaveCheckBox->setChecked( s->saveHistory() );

	if(qApp->clipboard()->supportsSelection())
	{
		ui->synchronizeComboBox->setCurrentIndex(s->syncMode());

	} else {
		ui->syncGroupBox->hide();
	}

	// Encryption
	ui->encryptionComboBox->setCurrentIndex(s->encryption());
	ui->certificateLineEdit->setText(s->certificatePath());
	ui->keyLineEdit->setText(s->privateKeyPath());

	connect(ui->certificateButton, SIGNAL(clicked()), this, SLOT(setCertificatePath()));
	connect(ui->keyButton, SIGNAL(clicked()), this, SLOT(setPrivateKeyPath()));
	connect(ui->certificateLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setFingerprint()));
	connect(ui->genCertButton, SIGNAL(clicked()), this, SLOT(generateCertificate()));

	setFingerprint();

	// Connection
	ui->hostLineEdit->setText( s->host() );
	ui->portSpinBox->setValue( s->port() );

	// Auto discovery
	ui->allowDiscoveryCheckBox->setChecked( s->allowAutoDiscovery() );
	ui->networkNameLineEdit->setText( s->networkName() );
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::apply()
{
	Settings *s = Settings::get();

	s->setHistoryEnabled( ui->historyGroupBox->isChecked() );
	s->setHistorySize( ui->historySizeSpinBox->value() );
	s->setSaveHistory( ui->historySaveCheckBox->isChecked() );

	s->setSyncMode( (ClipboardManager::SynchronizeMode) ui->synchronizeComboBox->currentIndex() );

	s->setNodes( nodeModel->nodes() );
	s->setHostAndPort(
		ui->hostLineEdit->text(),
		ui->portSpinBox->value()
	);

	s->setAllowAutoDiscovery( ui->allowDiscoveryCheckBox->isChecked() );
	s->setNetworkName( ui->networkNameLineEdit->text() );

	s->setEncryption( (Communicator::Encryption) ui->encryptionComboBox->currentIndex() );
	s->setCertificatePath( ui->certificateLineEdit->text() );
	s->setPrivateKeyPath( ui->keyLineEdit->text() );
}

void SettingsDialog::addNode()
{
	NodeAddWizard *wizard = new NodeAddWizard(NodeAddWizard::SearchMode, conman, this);
	wizard->exec();
	wizard->deleteLater();
}

void SettingsDialog::editNode(const QModelIndex &index)
{
	Node n = nodeModel->nodeForIndex(index.isValid() ? index : ui->nodeListView->currentIndex());

	NodeDialog *dlg = new NodeDialog(n, conman, this);

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
