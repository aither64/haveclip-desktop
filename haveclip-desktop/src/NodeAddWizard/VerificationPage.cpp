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

#include "VerificationPage.h"
#include "ui_VerificationPage.h"

#include <QMessageBox>

#include "Network/ConnectionManager.h"
#include "NodeAddWizard.h"
#include "Node.h"

VerificationPage::VerificationPage(ConnectionManager *conman, QWidget *parent) :
        QWizardPage(parent),
	ui(new Ui::VerificationPage),
	m_conman(conman)
{
	ui->setupUi(this);

	connect(m_conman, SIGNAL(verificationFinished(ConnectionManager::CodeValidity)), this, SLOT(verificationComplete(ConnectionManager::CodeValidity)));
}

VerificationPage::~VerificationPage()
{
	delete ui;
}

void VerificationPage::initializePage()
{
	Node n = m_conman->verifiedNode();

	ui->infoLabel->setText(
		ui->infoLabel->text().arg(n.name())
				.arg(n.host())
				.arg(n.port())
	);

	ui->codeLabel->setText(QString("<h1>Security code: %1</h1>").arg(m_conman->securityCode()));
}

bool VerificationPage::isComplete() const
{
	return false;
}

void VerificationPage::verificationComplete(ConnectionManager::CodeValidity validity)
{
	if(validity == ConnectionManager::Valid)
	{
		static_cast<NodeAddWizard*>(wizard())->setNode(m_conman->verifiedNode()); // FIXME: the verified node is no longer there!
		wizard()->next();

	} else if(validity == ConnectionManager::Refused) {
		QMessageBox::warning(
			this,
			tr("Verification failed"),
			tr("You have run out of tries. Please repeat the verification process.")
		);

		wizard()->reject();

	} else {
		qDebug() << "verification failed!";
	}
}
