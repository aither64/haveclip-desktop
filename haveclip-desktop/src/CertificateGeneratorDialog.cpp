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
	connect(certGenerator, SIGNAL(errorOccured(CertificateGenerator::ErrorType,QString)),
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
