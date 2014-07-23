#include "NodeDialog.h"
#include "ui_NodeDialog.h"

#include <QDateTime>

#include "Network/ConnectionManager.h"
#include "Node.h"
#include "NodeAddWizard.h"

NodeDialog::NodeDialog(Node &n, ConnectionManager *conman, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NodeDialog),
	m_node(n),
	m_conman(conman)
{
	ui->setupUi(this);

	setWindowTitle(tr("Edit node %1").arg(n.name()));

	connect(ui->verifyButton, SIGNAL(clicked()), this, SLOT(verifyNode()));

	showInfo();
}

NodeDialog::~NodeDialog()
{
	delete ui;
}

Node& NodeDialog::node()
{
	m_node.setName( ui->nameLineEdit->text() );
	m_node.setHost( ui->hostLineEdit->text() );
	m_node.setPort( ui->portLineEdit->text().toUShort() );

	m_node.setSendEnabled( ui->sendCheckBox->isChecked() );
	m_node.setReceiveEnabled( ui->receiveCheckBox->isChecked() );

	return m_node;
}

QString NodeDialog::formatDigest(QByteArray raw)
{
	QString digest = QString(raw.toHex()).toUpper();

	for(int i = 2; i < digest.size(); i+=3)
		digest.insert(i, ":");

	return digest;
}

void NodeDialog::verifyNode()
{
	NodeAddWizard *w = new NodeAddWizard(NodeAddWizard::VerifyMode, m_conman, this);
	w->verifyNode(m_node);

	if(w->exec() == QDialog::Accepted)
	{
		m_node.update(w->node());
		showInfo();
	}

	w->deleteLater();
}

void NodeDialog::showInfo()
{
	ui->nameLineEdit->setText(m_node.name());
	ui->hostLineEdit->setText(m_node.host());
	ui->portLineEdit->setText(QString::number(m_node.port()));

	ui->sendCheckBox->setChecked(m_node.isSendEnabled());
	ui->receiveCheckBox->setChecked(m_node.isReceiveEnabled());

	QSslCertificate cert = m_node.certificate();

	// issued to
	ui->toCommonNameLabel->setText( subjectInfo(cert, QSslCertificate::CommonName) );
	ui->toOrgLabel->setText( subjectInfo(cert, QSslCertificate::Organization) );
	ui->toOrgUnitLabel->setText( subjectInfo(cert, QSslCertificate::OrganizationalUnitName) );
	ui->serialLabel->setText( formatDigest(cert.serialNumber()) );

	// validity
	ui->issuedOnLabel->setText( cert.effectiveDate().toString("d/M/yyyy") );
	ui->expiresLabel->setText( cert.expiryDate().toString("d/M/yyyy") );

	// fingerprints
	ui->sha1FingerLabel->setText( formatDigest(cert.digest(QCryptographicHash::Sha1)) );
	ui->md5FingerLabel->setText( formatDigest(cert.digest(QCryptographicHash::Md5)) );
}

QString NodeDialog::subjectInfo(QSslCertificate &cert, QSslCertificate::SubjectInfo info)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	return cert.subjectInfo(info).join(" ");
#else
	return cert.subjectInfo(info);
#endif
}
