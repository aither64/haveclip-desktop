#include "IntroductionPage.h"
#include "ui_IntroductionPage.h"

#include "Network/ConnectionManager.h"
#include "Node.h"

IntroductionPage::IntroductionPage(ConnectionManager *conman, QWidget *parent) :
        QWizardPage(parent),
	ui(new Ui::IntroductionPage),
	m_conman(conman)
{
	ui->setupUi(this);

	connect(m_conman, SIGNAL(introductionFinished()), this, SLOT(introductionComplete()));
}

IntroductionPage::~IntroductionPage()
{
	delete ui;
}

void IntroductionPage::initializePage()
{
	Node n;
	n.setHost(field("node_host").toString());
	n.setPort(field("node_port").toString().toUShort());

	m_conman->verifyConnection(n);
}

bool IntroductionPage::isComplete() const
{
	return false;
}

void IntroductionPage::introductionComplete()
{
	wizard()->next();
}
