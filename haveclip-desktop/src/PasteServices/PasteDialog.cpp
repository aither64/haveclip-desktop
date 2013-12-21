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

#include "PasteDialog.h"
#include "ui_PasteDialog.h"

#include "PasteServices/HaveSnippet/HaveSnippet.h"
#include "HaveSnippet/HaveSnippetSettings.h"
#include "PasteServices/Stikked/Stikked.h"
#include "Stikked/StikkedSettings.h"
#include "PasteServices/Pastebin/Pastebin.h"
#include "Pastebin/PastebinSettings.h"

PasteDialog::PasteDialog(QString data, BasePasteService *service, QWidget *parent) :
        QDialog(parent),
	ui(new Ui::PasteDialog),
	service(service)
{
	ui->setupUi(this);

	switch(service->type())
	{
	case BasePasteService::HaveSnippet:
		serviceWidget = new HaveSnippetSettings(BasePasteServiceWidget::Paste, this);
		serviceWidget->load(service);
		break;
	case BasePasteService::Stikked:
		serviceWidget = new StikkedSettings(BasePasteServiceWidget::Paste, this);
		serviceWidget->load(service);
		break;
	case BasePasteService::Pastebin:
		serviceWidget = new PastebinSettings(BasePasteServiceWidget::Paste, this);
		serviceWidget->load(service);
		break;
	default:
		return;
	}

	ui->pasteOptionsVerticalLayout->addWidget(serviceWidget);

	ui->plainTextEdit->setPlainText(data);
}

PasteDialog::~PasteDialog()
{
	delete ui;
}

QString PasteDialog::dataToPaste()
{
	return ui->plainTextEdit->toPlainText();
}

QHash<QString, QVariant> PasteDialog::pasteServiceSettings()
{
	return serviceWidget->settings();
}
