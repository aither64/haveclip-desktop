#include "IntroductionPage.h"
#include "ui_IntroductionPage.h"

#include <QMessageBox>

#include "Network/ConnectionManager.h"
#include "Node.h"

IntroductionPage::IntroductionPage(ConnectionManager *conman, QWidget *parent) :
        QWizardPage(parent),
	ui(new Ui::IntroductionPage),
	m_conman(conman)
{
	ui->setupUi(this);

	connect(m_conman, SIGNAL(introductionFinished()), this, SLOT(introductionComplete()));
	connect(m_conman, SIGNAL(introductionFailed(Communicator::CommunicationStatus)), this, SLOT(introductionFailure(Communicator::CommunicationStatus)));
}

IntroductionPage::~IntroductionPage()
{
	delete ui;
}

void IntroductionPage::initializePage()
{
	if(m_node.host().isEmpty())
	{
		m_node.setHost(field("node_host").toString());
		m_node.setPort(field("node_port").toString().toUShort());
	}

	m_conman->verifyConnection(m_node);
}

bool IntroductionPage::isComplete() const
{
	return false;
}

void IntroductionPage::setNode(Node &n)
{
	m_node = n;
}

void IntroductionPage::introductionComplete()
{
	wizard()->next();
}

void IntroductionPage::introductionFailure(Communicator::CommunicationStatus status)
{
	QMessageBox::critical(
		this,
		tr("Connection failed"),
		tr("Unable to establish connection: %1").arg(Communicator::statusToString(status))
	);

	wizard()->back();
}
