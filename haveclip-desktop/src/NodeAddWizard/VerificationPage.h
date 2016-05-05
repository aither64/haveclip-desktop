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

#ifndef VERIFICATIONPAGE_H
#define VERIFICATIONPAGE_H

#include <QWizardPage>

#include "Network/ConnectionManager.h"

namespace Ui {
class VerificationPage;
}

class VerificationPage : public QWizardPage
{
	Q_OBJECT

public:
	explicit VerificationPage(ConnectionManager *conman, QWidget *parent = 0);
	~VerificationPage();
	virtual void initializePage();
	virtual bool isComplete() const;

private slots:
	void verificationComplete(ConnectionManager::CodeValidity validity);

private:
	Ui::VerificationPage *ui;
	ConnectionManager *m_conman;
};

#endif // VERIFICATIONPAGE_H
