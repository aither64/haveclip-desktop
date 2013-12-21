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

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "CertificateTrustDialog.h"

#include "PasteServices/BasePasteService.h"
#include "PasteServices/PasteServiceEditDialog.h"

#include "PasteServices/HaveSnippet/HaveSnippet.h"
#include "PasteServices/Stikked/Stikked.h"
#include "PasteServices/Pastebin/Pastebin.h"

SettingsDialog::SettingsDialog(QSettings *settings, QWidget *parent) :
        QDialog(parent),
	ui(new Ui::SettingsDialog),
	settings(settings)
{
	ui->setupUi(this);

	connect(ui->nodeAddButton, SIGNAL(clicked()), this, SLOT(addNode()));
	connect(ui->nodeEditButton, SIGNAL(clicked()), this, SLOT(editNode()));
	connect(ui->nodeRemoveButton, SIGNAL(clicked()), this, SLOT(deleteNode()));

	// Pool
	foreach(QString n, settings->value("Pool/Nodes").toStringList())
	{
		QListWidgetItem *it = new QListWidgetItem(n);
		it->setFlags(it->flags() | Qt::ItemIsEditable);
		ui->nodeListWidget->addItem(it);
	}

	// History
	ui->historyGroupBox->setChecked( settings->value("History/Enable", true).toBool() );
	ui->historySizeSpinBox->setValue( settings->value("History/Size", 10).toInt() );
	ui->historySaveCheckBox->setChecked( settings->value("History/Save", true).toBool() );

	if(qApp->clipboard()->supportsSelection())
	{
		if( (ClipboardManager::SelectionMode) settings->value("Selection/Mode", ClipboardManager::Separate).toInt() == ClipboardManager::Separate)
			ui->keepSelectionSeparateRadioButton->setChecked(true);
		else
			ui->uniteSelectionRadioButton->setChecked(true);

		ui->synchronizeComboBox->setCurrentIndex(settings->value("Sync/Synchronize", ClipboardManager::Both).toInt());

	} else {
		ui->selectionGroupBox->hide();
		ui->syncGroupBox->hide();
	}

	// Encryption
	ui->encryptionComboBox->setCurrentIndex(settings->value("Connection/Encryption", ClipboardManager::None).toInt());
	ui->certificateLineEdit->setText(settings->value("Connection/Certificate", "certs/haveclip.crt").toString());
	ui->keyLineEdit->setText(settings->value("Connection/PrivateKey", "certs/haveclip.key").toString());

	connect(ui->certificateButton, SIGNAL(clicked()), this, SLOT(setCertificatePath()));
	connect(ui->keyButton, SIGNAL(clicked()), this, SLOT(setPrivateKeyPath()));
	connect(ui->certificateLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setFingerprint()));

	setFingerprint();

	// Connection
	ui->hostLineEdit->setText( settings->value("Connection/Host", "0.0.0.0").toString() );
	ui->portSpinBox->setValue( settings->value("Connection/Port", 9999).toInt() );

	ui->passwordLineEdit->setText( settings->value("AccessPolicy/Password").toString() );

	// Paste services
	connect(ui->pasteAddButton, SIGNAL(clicked()), this, SLOT(addPasteService()));
	connect(ui->pasteEditButton, SIGNAL(clicked()), this, SLOT(editPasteService()));
	connect(ui->pasteRemoveButton, SIGNAL(clicked()), this, SLOT(deletePasteService()));
	connect(ui->pasteServiceListWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editPasteService()));
	connect(ui->pasteDownButton, SIGNAL(clicked()), this, SLOT(moveDown()));
	connect(ui->pasteUpButton, SIGNAL(clicked()), this, SLOT(moveUp()));

	settings->beginGroup("PasteServices");

	foreach(QString index, settings->childGroups())
	{
		settings->beginGroup(index);

		BasePasteService *s;

		switch(settings->value("Type").toInt())
		{
		case BasePasteService::HaveSnippet:
			s = new HaveSnippet(settings, this);
			break;
		case BasePasteService::Stikked:
			s = new Stikked(settings, this);
			break;
		case BasePasteService::Pastebin:
			s = new Pastebin(settings, this);
			break;
		default:
			continue;
		}

		m_services << s;

		ui->pasteServiceListWidget->addItem(settings->value("Label").toString());

		settings->endGroup();
	}

	settings->endGroup();

}

SettingsDialog::~SettingsDialog()
{
	delete ui;

	qDeleteAll(m_services);
}

QStringList SettingsDialog::nodes()
{
	QStringList ret;

	for(int i = 0; i < ui->nodeListWidget->count(); i++)
		ret << ui->nodeListWidget->item(i)->text();

	return ret;
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

ClipboardManager::SelectionMode SettingsDialog::selectionMode()
{
	return ui->keepSelectionSeparateRadioButton->isChecked() ? ClipboardManager::Separate : ClipboardManager::United;
}

ClipboardManager::SynchronizeMode SettingsDialog::synchronizationMode()
{
	return (ClipboardManager::SynchronizeMode) ui->synchronizeComboBox->currentIndex();
}

void SettingsDialog::addNode()
{
	QListWidgetItem *it = new QListWidgetItem(NODE_ADD_STR);
	it->setFlags(it->flags() | Qt::ItemIsEditable);
	ui->nodeListWidget->addItem(it);
	ui->nodeListWidget->editItem(it);
	it->setSelected(true);
}

void SettingsDialog::editNode()
{
	QList<QListWidgetItem*> items = ui->nodeListWidget->selectedItems();

	if(items.count() > 0)
		ui->nodeListWidget->editItem(items.first());
}

void SettingsDialog::deleteNode()
{
	foreach(QListWidgetItem *it, ui->nodeListWidget->selectedItems())
		delete ui->nodeListWidget->takeItem( ui->nodeListWidget->row(it) );
}

QString SettingsDialog::host()
{
	return ui->hostLineEdit->text();
}

quint16 SettingsDialog::port()
{
	return ui->portSpinBox->value();
}

QString SettingsDialog::password()
{
	return ui->passwordLineEdit->text();
}

ClipboardManager::Encryption SettingsDialog::encryption()
{
	return (ClipboardManager::Encryption) ui->encryptionComboBox->currentIndex();
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

QList<BasePasteService*> SettingsDialog::pasteServices()
{
	return m_services;
}

void SettingsDialog::addPasteService()
{
	PasteServiceEditDialog *dlg = new PasteServiceEditDialog(PasteServiceEditDialog::Add, 0, this);

	if(dlg->exec() == QDialog::Accepted)
	{
		BasePasteService *s;

		switch(dlg->type())
		{
		case BasePasteService::HaveSnippet:
			s = new HaveSnippet(settings);
			break;
		case BasePasteService::Stikked:
			s = new Stikked(settings);
			break;
		case BasePasteService::Pastebin:
			s = new Pastebin(settings);
			break;
		default:
			return;
		}

		s->applySettings(dlg->settings());

		ui->pasteServiceListWidget->addItem(s->label());
		m_services << s;
	}

	dlg->deleteLater();
}

void SettingsDialog::editPasteService()
{
	int i = ui->pasteServiceListWidget->currentRow();
	PasteServiceEditDialog *dlg = new PasteServiceEditDialog(PasteServiceEditDialog::Edit, m_services[i], this);

	if(dlg->exec() == QDialog::Accepted)
	{
		if(m_services[i]->type() == dlg->type()) {
			m_services[i]->applySettings(dlg->settings());
			ui->pasteServiceListWidget->item(i)->setText(m_services[i]->label());

		} else {
			BasePasteService *s;

			switch(dlg->type())
			{
			case BasePasteService::HaveSnippet:
				s = new HaveSnippet();
				break;

			case BasePasteService::Stikked:
				s = new Stikked();
				break;
			case BasePasteService::Pastebin:
				s = new Pastebin();
				break;
			default:
				return;
			}

			s->applySettings(dlg->settings());

			delete m_services[i];
			m_services[i] = s;
		}
	}

	dlg->deleteLater();
}

void SettingsDialog::deletePasteService()
{
	int i = ui->pasteServiceListWidget->currentRow();
	delete ui->pasteServiceListWidget->item(i);
	delete m_services.takeAt(i);
}

void SettingsDialog::moveUp()
{
	int current = ui->pasteServiceListWidget->currentRow();

	if(current <= 0)
		return;

	ui->pasteServiceListWidget->insertItem(current-1, ui->pasteServiceListWidget->takeItem(current));
	m_services.insert(current-1, m_services.takeAt(current));

	ui->pasteServiceListWidget->setCurrentRow(current-1);
}

void SettingsDialog::moveDown()
{
	int current = ui->pasteServiceListWidget->currentRow();

	if(current == -1 || current+1 == ui->pasteServiceListWidget->count())
		return;

	ui->pasteServiceListWidget->insertItem(current+1, ui->pasteServiceListWidget->takeItem(current));
	m_services.insert(current+1, m_services.takeAt(current));

	ui->pasteServiceListWidget->setCurrentRow(current-1);
}
