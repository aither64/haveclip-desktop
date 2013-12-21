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

#include <QDateTime>

#include "CertificateTrustDialog.h"
#include "ui_CertificateTrustDialog.h"

CertificateTrustDialog::CertificateTrustDialog(ClipboardManager::Node *node, const QList<QSslError> &errors, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CertificateTrustDialog)
{
	init(node->toString(), errors);
}

CertificateTrustDialog::CertificateTrustDialog(BasePasteService *service, const QList<QSslError> &errors, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CertificateTrustDialog)
{
	init(service->label(), errors);
}

CertificateTrustDialog::~CertificateTrustDialog()
{
	delete ui;
}

void CertificateTrustDialog::init(QString to, const QList<QSslError> &errors)
{
	ui->setupUi(this);

	QString errStr = ui->errorLabel->text().arg(to);
	errStr += "<br><br>";

	foreach(QSslError e, errors)
		errStr += e.errorString() + "<br>";

	ui->errorLabel->setText(errStr);

	QSslCertificate cert = errors.first().certificate();

	// issued to
	ui->toCommonNameLabel->setText( subjectInfo(cert, QSslCertificate::CommonName) );
	ui->toOrgLabel->setText( subjectInfo(cert, QSslCertificate::Organization) );
	ui->toOrgUnitLabel->setText( subjectInfo(cert, QSslCertificate::OrganizationalUnitName) );
	ui->serialLabel->setText( formatDigest(cert.serialNumber()) );

	// issuer
	ui->byCommonNameLabel->setText( issuerInfo(cert, QSslCertificate::CommonName) );
	ui->byOrgLabel->setText( issuerInfo(cert, QSslCertificate::Organization) );
	ui->byOrgUnitLabel->setText( issuerInfo(cert, QSslCertificate::OrganizationalUnitName) );

	// validity
	ui->issuedOnLabel->setText( cert.effectiveDate().toString("d/M/yyyy") );
	ui->expiresLabel->setText( cert.expiryDate().toString("d/M/yyyy") );

	// fingerprints
	ui->sha1FingerLabel->setText( formatDigest(cert.digest(QCryptographicHash::Sha1)) );
	ui->md5FingerLabel->setText( formatDigest(cert.digest(QCryptographicHash::Md5)) );
}

QString CertificateTrustDialog::formatDigest(QByteArray raw)
{
	QString digest = QString(raw.toHex()).toUpper();

	for(int i = 2; i < digest.size(); i+=3)
		digest.insert(i, ":");

	return digest;
}

bool CertificateTrustDialog::remember()
{
	return ui->rememberCheckBox->isChecked();
}

QString CertificateTrustDialog::subjectInfo(QSslCertificate &cert, QSslCertificate::SubjectInfo info)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	return cert.subjectInfo(info).join(" ");
#else
	return cert.subjectInfo(info);
#endif
}

QString CertificateTrustDialog::issuerInfo(QSslCertificate &cert, QSslCertificate::SubjectInfo info)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	return cert.issuerInfo(info).join(" ");
#else
	return cert.issuerInfo(info);
#endif
}
