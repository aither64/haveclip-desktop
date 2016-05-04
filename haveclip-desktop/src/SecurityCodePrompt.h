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

#ifndef SECURITYCODEPROMPT_H
#define SECURITYCODEPROMPT_H

#include <QDialog>

#include "Network/Communicator.h"
#include "Network/ConnectionManager.h"

class Node;

namespace Ui {
class SecurityCodePrompt;
}

class SecurityCodePrompt : public QDialog
{
	Q_OBJECT

public:
	explicit SecurityCodePrompt(const Node &n, ConnectionManager *conman, QWidget *parent = 0);
	~SecurityCodePrompt();

private slots:
	void verifyCode();
	void verificationFinish(ConnectionManager::CodeValidity validity);
	void verificationFailed(Communicator::CommunicationStatus status);

private:
	Ui::SecurityCodePrompt *ui;
	ConnectionManager *m_conman;
};

#endif // SECURITYCODEPROMPT_H
