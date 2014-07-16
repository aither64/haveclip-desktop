#include "NodeAddWizard.h"

#include <QVBoxLayout>
#include <QLabel>

#include "Node.h"
#include "Network/ConnectionManager.h"
#include "Network/AutoDiscovery.h"

#include "NodeAddWizard/SearchPage.h"
#include "NodeAddWizard/IntroductionPage.h"
#include "NodeAddWizard/VerificationPage.h"
#include "NodeAddWizard/PairedPage.h"

NodeAddWizard::NodeAddWizard(ConnectionManager *conman, QWidget *parent) :
	QWizard(parent),
	m_conman(conman)
{
	setWindowTitle(tr("Add node"));

	addPage(new SearchPage(m_conman));
	addPage(new IntroductionPage(m_conman));
	addPage(new VerificationPage(m_conman));
	addPage(new PairedPage);

	setOption(QWizard::NoBackButtonOnLastPage, true);
	setButtonText(QWizard::CustomButton1, tr("&Search local network"));

	pageChange(Search);

	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(pageChange(int)));
	connect(this, SIGNAL(customButtonClicked(int)), this, SLOT(buttonClick(int)));
}

Node NodeAddWizard::node()
{
	return m_node;
}

void NodeAddWizard::setNode(Node &n)
{
	m_node = n;
}

void NodeAddWizard::reject()
{
	m_conman->cancelVerification();

	QWizard::reject();
}

void NodeAddWizard::pageChange(int id)
{
	switch(id)
	{
	case Search:
		setOption(QWizard::HaveCustomButton1, true);
		setButtonText(QWizard::NextButton, tr("&Verify connection"));
		break;

	case Paired:
		setOption(QWizard::NoCancelButton, true);

	default:
		setOption(QWizard::HaveCustomButton1, false);
		setButtonText(QWizard::NextButton, tr("&Next"));
	}
}

void NodeAddWizard::buttonClick(int which)
{
	switch(which)
	{
	case QWizard::CustomButton1:
		m_conman->autoDiscovery()->discover();
		break;

	default:
		break;
	}
}
