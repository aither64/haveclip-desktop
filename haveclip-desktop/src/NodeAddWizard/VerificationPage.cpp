#include "VerificationPage.h"
#include "ui_VerificationPage.h"

#include <QMessageBox>

#include "Network/ConnectionManager.h"
#include "NodeAddWizard.h"
#include "Node.h"

VerificationPage::VerificationPage(ConnectionManager *conman, QWidget *parent) :
        QWizardPage(parent),
	ui(new Ui::VerificationPage),
	m_conman(conman)
{
	ui->setupUi(this);

	connect(m_conman, SIGNAL(verificationFinished(ConnectionManager::CodeValidity)), this, SLOT(verificationComplete(ConnectionManager::CodeValidity)));
}

VerificationPage::~VerificationPage()
{
	delete ui;
}

void VerificationPage::initializePage()
{
	Node n = m_conman->verifiedNode();

	ui->infoLabel->setText(
		ui->infoLabel->text().arg(n.name())
				.arg(n.host())
				.arg(n.port())
	);

	ui->codeLabel->setText(QString("<h1>Security code: %1</h1>").arg(m_conman->securityCode()));
}

bool VerificationPage::isComplete() const
{
	return false;
}

void VerificationPage::verificationComplete(ConnectionManager::CodeValidity validity)
{
	if(validity == ConnectionManager::Valid)
	{
		static_cast<NodeAddWizard*>(wizard())->setNode(m_conman->verifiedNode());
		wizard()->next();

	} else if(validity == ConnectionManager::Refused) {
		QMessageBox::warning(
			this,
			tr("Verification failed"),
			tr("You have run out of tries. Please repeat the verification process.")
		);

		wizard()->reject();

	} else {
		qDebug() << "verification failed!";
	}
}
