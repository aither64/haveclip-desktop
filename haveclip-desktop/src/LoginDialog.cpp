/*
  HaveClip

  Copyright (C) 2013 Jakub Skokan <aither@havefun.cz>

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

#include "LoginDialog.h"
#include "ui_LoginDialog.h"

LoginDialog::LoginDialog(QString username, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::LoginDialog)
{
	ui->setupUi(this);

	ui->usernameLineEdit->setText(username);
	ui->errorLabel->hide();
}

LoginDialog::~LoginDialog()
{
	delete ui;
}

void LoginDialog::setError(QString err)
{
	ui->errorLabel->setText(err);
	ui->errorLabel->show();
}

QString LoginDialog::username()
{
	return ui->usernameLineEdit->text();
}

QString LoginDialog::password()
{
	return ui->passwordLineEdit->text();
}
