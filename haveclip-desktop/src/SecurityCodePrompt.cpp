#include "SecurityCodePrompt.h"
#include "ui_SecurityCodePrompt.h"

#include "Network/ConnectionManager.h"
#include "Node.h"

SecurityCodePrompt::SecurityCodePrompt(Node *n, ConnectionManager *conman, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SecurityCodePrompt),
	m_conman(conman)
{
	ui->setupUi(this);

	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ui->verifyButton, SIGNAL(clicked()), this, SLOT(verifyCode()));
	connect(m_conman, SIGNAL(verificationFinished(bool)), this, SLOT(verificationFinish(bool)));

	ui->infoLabel->setText( ui->infoLabel->text().arg(n->name()).arg(n->host()).arg(n->port()) );
}

SecurityCodePrompt::~SecurityCodePrompt()
{
	delete ui;
}

void SecurityCodePrompt::verifyCode()
{
	m_conman->provideSecurityCode(ui->codeLineEdit->text());
}

void SecurityCodePrompt::verificationFinish(bool ok)
{
	if(ok)
		accept();
}
