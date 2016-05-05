/*
  HaveClip

  Copyright (C) 2013-2016 Jakub Skokan <aither@havefun.cz>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

NodeAddWizard::NodeAddWizard(NodeAddWizard::Mode mode, ConnectionManager *conman, QWidget *parent) :
	QWizard(parent),
	m_mode(mode),
	m_conman(conman)
{
	setWindowTitle(tr("Add device"));

	if(mode == SearchMode)
		addPage(new SearchPage(m_conman));

	addPage(m_introPage = new IntroductionPage(m_conman));
	addPage(new VerificationPage(m_conman));
	addPage(new PairedPage);

	setOption(QWizard::NoBackButtonOnLastPage, true);

	if(mode == SearchMode)
	{
		setButtonText(QWizard::CustomButton1, tr("&Search local network"));

		pageChange(SearchPageId);
	}

	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(pageChange(int)));
	connect(this, SIGNAL(customButtonClicked(int)), this, SLOT(buttonClick(int)));
}

Node& NodeAddWizard::node()
{
	return m_node;
}

void NodeAddWizard::setNode(Node &n)
{
	m_node = n;
}

void NodeAddWizard::verifyNode(Node &n)
{
	m_node = n;
	m_introPage->setNode(n);
}

void NodeAddWizard::reject()
{
	m_conman->cancelVerification();

	QWizard::reject();
}

void NodeAddWizard::pageChange(int id)
{
	id = idOf(id);

	QList<QWizard::WizardButton> layout;

	switch(id)
	{
	case SearchPageId:
		setOption(QWizard::HaveCustomButton1, true);
		setButtonText(QWizard::NextButton, tr("&Verify connection"));

		layout << QWizard::CustomButton1 << QWizard::Stretch << QWizard::BackButton
		       << QWizard::NextButton << QWizard::CancelButton;
		break;

	case IntroductionPageId:
	case VerificationPageId:
		setOption(QWizard::HaveCustomButton1, false);
		setButtonText(QWizard::NextButton, tr("&Next"));
		layout << QWizard::Stretch << QWizard::NextButton << QWizard::CancelButton;
		break;

	case PairedPageId:
		layout << QWizard::Stretch << QWizard::FinishButton;
		break;

	default:
		break;
	}

	setButtonLayout(layout);
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

int NodeAddWizard::idOf(int page)
{
	if(m_mode == VerifyMode)
		return page+1;

	return page;
}
