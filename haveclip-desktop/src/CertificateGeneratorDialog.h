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
