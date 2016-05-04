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

#ifndef INTRODUCTIONPAGE_H
#define INTRODUCTIONPAGE_H

#include <QWizardPage>

#include "Node.h"
#include "Network/Communicator.h"

class ConnectionManager;

namespace Ui {
class IntroductionPage;
}

class IntroductionPage : public QWizardPage
{
	Q_OBJECT

public:
	explicit IntroductionPage(ConnectionManager *conman, QWidget *parent = 0);
	~IntroductionPage();
	virtual void initializePage();
	virtual bool isComplete() const;
	void setNode(Node &n);

private slots:
	void introductionComplete();
	void introductionFailure(Communicator::CommunicationStatus status);

private:
	Ui::IntroductionPage *ui;
	ConnectionManager *m_conman;
	Node m_node;
};

#endif // INTRODUCTIONPAGE_H
