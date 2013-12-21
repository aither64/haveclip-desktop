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

#ifndef CERTIFICATETRUSTDIALOG_H
#define CERTIFICATETRUSTDIALOG_H

#include <QDialog>
#include <QSslError>

#include "ClipboardManager.h"

namespace Ui {
class CertificateTrustDialog;
}

class CertificateTrustDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit CertificateTrustDialog(ClipboardManager::Node *node, const QList<QSslError> &errors, QWidget *parent = 0);
	explicit CertificateTrustDialog(BasePasteService *service, const QList<QSslError> &errors, QWidget *parent = 0);
	~CertificateTrustDialog();
	void init(QString to, const QList<QSslError> &errors);
	bool remember();
	static QString formatDigest(QByteArray raw);

private:
	Ui::CertificateTrustDialog *ui;

	QString subjectInfo(QSslCertificate &cert, QSslCertificate::SubjectInfo info);
	QString issuerInfo(QSslCertificate &cert, QSslCertificate::SubjectInfo info);
};

#endif // CERTIFICATETRUSTDIALOG_H
