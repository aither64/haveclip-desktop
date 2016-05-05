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

#ifndef CERTIFICATEGENERATORDIALOG_H
#define CERTIFICATEGENERATORDIALOG_H

#include <QDialog>
#include "CertificateGenerator.h"

namespace Ui {
class CertificateGeneratorDialog;
}

class CertificateGeneratorDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CertificateGeneratorDialog(QWidget *parent = 0);
	~CertificateGeneratorDialog();
	void savePrivateKey(QString where);
	void saveCertificate(QString where);

public slots:
	void generate();

protected:
	virtual void closeEvent(QCloseEvent *event);

private slots:
	void generationFailed(CertificateGenerator::ErrorType type, const QString &err);

private:
	Ui::CertificateGeneratorDialog *ui;
	CertificateGenerator *certGenerator;
};

#endif // CERTIFICATEGENERATORDIALOG_H
