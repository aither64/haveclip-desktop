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
