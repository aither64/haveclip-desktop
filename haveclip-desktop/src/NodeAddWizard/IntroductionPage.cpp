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
