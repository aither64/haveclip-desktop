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

#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "HaveClip.h"

AboutDialog::AboutDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AboutDialog)
{
	ui->setupUi(this);

	ui->textLabel->setText(tr(
		"<h1>HaveClip</h1>"
		"<p>Version %1</p>"
		"<p>A simple clipboard synchronization tool.</p>"
		"<h2>Authors:</h2>"
		"<ul><li>Developed by Jakub Skokan &lt;<a href=\"mailto:aither@havefun.cz\">aither@havefun.cz</a>&gt;</li>"
		"<li>Icon created by Aleš Kocur &lt;<a href=\"mailto:kafe@havefun.cz\">kafe@havefun.cz</a>&gt;</li></ul>"
	).arg(VERSION));
}

AboutDialog::~AboutDialog()
{
	delete ui;
}
