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

#include "SecurityCodePrompt.h"
#include "ui_SecurityCodePrompt.h"

#include <QMessageBox>

#include "Network/ConnectionManager.h"
#include "Node.h"

SecurityCodePrompt::SecurityCodePrompt(const Node &n, ConnectionManager *conman, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SecurityCodePrompt),
	m_conman(conman)
{
	ui->setupUi(this);

	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ui->verifyButton, SIGNAL(clicked()), this, SLOT(verifyCode()));
	connect(m_conman, SIGNAL(verificationFinished(ConnectionManager::CodeValidity)), this, SLOT(verificationFinish(ConnectionManager::CodeValidity)));
	connect(m_conman, SIGNAL(verificationFailed(Communicator::CommunicationStatus)), this, SLOT(verificationFailed(Communicator::CommunicationStatus)));

	ui->infoLabel->setText( ui->infoLabel->text().arg(n.name()).arg(n.host()).arg(n.port()) );
	ui->errorLabel->hide();
}

SecurityCodePrompt::~SecurityCodePrompt()
{
	delete ui;
}

void SecurityCodePrompt::verifyCode()
{
	ui->verifyButton->setText(tr("Verifying..."));
	ui->verifyButton->setEnabled(false);

	m_conman->provideSecurityCode(ui->codeLineEdit->text());
}

void SecurityCodePrompt::verificationFinish(ConnectionManager::CodeValidity validity)
{
	if(validity == ConnectionManager::Valid)
	{
		accept();

	} else if(validity == ConnectionManager::Refused) {
		QMessageBox::warning(
			this,
			tr("Verification failed"),
			tr("You have run out of tries. Please repeat the verification process.")
		);

		reject();

	} else {
		ui->errorLabel->show();
		ui->verifyButton->setText(tr("Verify"));
		ui->verifyButton->setEnabled(true);
	}
}

void SecurityCodePrompt::verificationFailed(Communicator::CommunicationStatus status)
{
	QMessageBox::critical(
		this,
		tr("Connection failed"),
		tr("Unable to establish connection: %1").arg(Communicator::statusToString(status))
	);

	reject();
}
