#include "VerificationPage.h"
#include "ui_VerificationPage.h"

#include "Network/ConnectionManager.h"
#include "NodeAddWizard.h"
#include "Node.h"

VerificationPage::VerificationPage(ConnectionManager *conman, QWidget *parent) :
        QWizardPage(parent),
	ui(new Ui::VerificationPage),
	m_conman(conman)
{
	ui->setupUi(this);

	connect(m_conman, SIGNAL(verificationFinished(bool)), this, SLOT(verificationComplete(bool)));
}

VerificationPage::~VerificationPage()
{
	delete ui;
}

void VerificationPage::initializePage()
{
	ui->infoLabel->setText(
		ui->infoLabel->text().arg(m_conman->verifiedNode()->name())
				.arg(m_conman->verifiedNode()->host())
				.arg(m_conman->verifiedNode()->port())
	);

	ui->codeLabel->setText(QString("<h1>Security Code: %1</h1>").arg(m_conman->securityCode()));
}

bool VerificationPage::isComplete() const
{
	return false;
}

void VerificationPage::verificationComplete(bool ok)
{
	if(ok)
	{
		static_cast<NodeAddWizard*>(wizard())->setNode(m_conman->verifiedNode());
		wizard()->next();

	} else {
		qDebug() << "verification failed!";
	}
}
