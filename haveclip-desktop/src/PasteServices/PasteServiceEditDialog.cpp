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

#include "PasteServiceEditDialog.h"
#include "ui_PasteServiceEditDialog.h"

#include "BasePasteServiceWidget.h"
#include "PasteServices/HaveSnippet/HaveSnippet.h"
#include "HaveSnippet/HaveSnippetSettings.h"
#include "PasteServices/Stikked/Stikked.h"
#include "Stikked/StikkedSettings.h"
#include "PasteServices/Pastebin/Pastebin.h"
#include "Pastebin/PastebinSettings.h"

PasteServiceEditDialog::PasteServiceEditDialog(PasteServiceEditDialog::Action action, BasePasteService *service, QWidget *parent) :
        QDialog(parent),
	ui(new Ui::PasteServiceEditDialog)
{
	ui->setupUi(this);

	ui->pasteServiceComboBox->setCurrentIndex(service ? service->type() : 0);

	if(action == Edit)
		ui->labelLineEdit->setText(service->label());

	for(int i = 0; i < BasePasteService::PasteServiceCount; i++)
	{
		BasePasteServiceWidget *w;

		switch(i)
		{
		case BasePasteService::HaveSnippet:
			w = new HaveSnippetSettings(BasePasteServiceWidget::Settings, this);
			break;
		case BasePasteService::Stikked:
			w = new StikkedSettings(BasePasteServiceWidget::Settings, this);
			break;
		case BasePasteService::Pastebin:
			w = new PastebinSettings(BasePasteServiceWidget::Settings, this);
			break;
		default:
			continue;
		}

		if(action == Edit && i == service->type())
			w->load(service);

		ui->pasteStackedWidget->addWidget(w);
	}

	ui->pasteStackedWidget->setCurrentIndex(ui->pasteServiceComboBox->currentIndex());
}

PasteServiceEditDialog::~PasteServiceEditDialog()
{
	delete ui;
}

BasePasteService::PasteService PasteServiceEditDialog::type()
{
	return (BasePasteService::PasteService) ui->pasteServiceComboBox->currentIndex();
}

QString PasteServiceEditDialog::label()
{
	return ui->labelLineEdit->text();
}

QHash<QString, QVariant> PasteServiceEditDialog::settings()
{
	QHash<QString, QVariant> tmp = static_cast<BasePasteServiceWidget*>(ui->pasteStackedWidget->currentWidget())->settings();

	tmp["Label"] = label();

	return tmp;
}
