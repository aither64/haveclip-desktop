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

#ifndef NODEADDWIZARD_H
#define NODEADDWIZARD_H

#include <QWizard>

#include "Node.h"

class ConnectionManager;
class IntroductionPage;

class NodeAddWizard : public QWizard
{
	Q_OBJECT
public:
	enum Pages {
		SearchPageId = 0,
		IntroductionPageId,
		VerificationPageId,
		PairedPageId
	};

	enum Mode {
		SearchMode,
		VerifyMode
	};

	explicit NodeAddWizard(Mode mode, ConnectionManager *conman, QWidget *parent = 0);
	Node& node();
	void setNode(Node &n);
	void verifyNode(Node &n);

public slots:
	virtual void reject();

private slots:
	void pageChange(int id);
	void buttonClick(int which);

private:
	Mode m_mode;
	ConnectionManager *m_conman;
	Node m_node;
	IntroductionPage *m_introPage;

	int idOf(int page);
};

#endif // NODEADDWIZARD_H
