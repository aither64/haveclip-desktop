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

#include "CertificateGeneratorDialog.h"
#include "ui_CertificateGeneratorDialog.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>

#include "Settings.h"

CertificateGeneratorDialog::CertificateGeneratorDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::CertificateGeneratorDialog)
{
	ui->setupUi(this);

	certGenerator = new CertificateGenerator(this);

	certGenerator->setCommonName(Settings::get()->networkName());

	connect(certGenerator, SIGNAL(finished()), this, SLOT(accept()));
	connect(certGenerator, SIGNAL(errorOccurred(CertificateGenerator::ErrorType,QString)),
		this, SLOT(generationFailed(CertificateGenerator::ErrorType,QString)));

	QTimer::singleShot(0, this, SLOT(generate()));
}

CertificateGeneratorDialog::~CertificateGeneratorDialog()
{
	delete ui;
}

void CertificateGeneratorDialog::generate()
{
	certGenerator->generate();
}

void CertificateGeneratorDialog::savePrivateKey(QString where)
{
	certGenerator->savePrivateKeyToFile(where);
}

void CertificateGeneratorDialog::saveCertificate(QString where)
{
	certGenerator->saveCertificateToFile(where);
}

void CertificateGeneratorDialog::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event);

	//event->ignore();
}

void CertificateGeneratorDialog::generationFailed(CertificateGenerator::ErrorType type, const QString &err)
{
	QString msg;

	switch(type)
	{
	case CertificateGenerator::MkpathFailed:
		msg = tr("Cannot create directory '%1'.").arg(err);
		break;

	case CertificateGenerator::SaveFailed:
		msg = tr("Failed to create file '%1'.").arg(err);
		break;

	default:
		msg = tr("Unknown error.");
	}

	QMessageBox::critical(
		this,
		tr("Unable to generate certificate"),
		msg
	);

	reject();
}
