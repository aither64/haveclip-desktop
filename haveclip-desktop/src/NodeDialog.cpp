#include "NodeDialog.h"
#include "ui_NodeDialog.h"

#include <QDateTime>

#include "Network/ConnectionManager.h"
#include "Node.h"
#include "NodeAddWizard.h"
#include "CertificateInfo.h"

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

	CertificateInfo info(m_node.certificate());

	// issued to
	ui->toCommonNameLabel->setText( info.commonName() );
	ui->toOrgLabel->setText( info.organization() );
	ui->toOrgUnitLabel->setText( info.organizationUnit() );
	ui->serialLabel->setText( info.serialNumber() );

	// validity
	ui->issuedOnLabel->setText( info.issuedOn().toString("d/M/yyyy") );
	ui->expiresLabel->setText( info.expiryDate().toString("d/M/yyyy") );

	// fingerprints
	ui->sha1FingerLabel->setText( info.sha1Digest() );
	ui->md5FingerLabel->setText( info.md5Digest() );
}
