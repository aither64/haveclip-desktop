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

#include "StikkedSettings.h"
#include "ui_StikkedSettings.h"

#include "PasteServices/Stikked/Stikked.h"

StikkedSettings::StikkedSettings(BasePasteServiceWidget::Mode mode, QWidget *parent) :
	BasePasteServiceWidget(mode, parent),
	ui(new Ui::StikkedSettings)
{
	ui->setupUi(this);

	switch(mode)
	{
	case BasePasteServiceWidget::Settings:
		break;
	case BasePasteServiceWidget::Paste:
		ui->apiUrlLabel->hide();
		ui->apiUrlLineEdit->hide();
		break;
	}

	for(int i = 0; Stikked::m_languages[i].name != 0; i++)
		ui->langComboBox->addItem(Stikked::m_languages[i].label);
}

StikkedSettings::~StikkedSettings()
{
	delete ui;
}

void StikkedSettings::load(QSettings *settings)
{
	ui->apiUrlLineEdit->setText(settings->value("Url", "http://your.sticked/api/create").toString());
	ui->nameLineEdit->setText(settings->value("Name").toString());
	ui->titleLineEdit->setText(settings->value("Title").toString());
	ui->privateCheckBox->setChecked(settings->value("Private", true).toBool());
	ui->langComboBox->setCurrentIndex(Stikked::langIndexFromName(Stikked::m_languages, settings->value("Language", "text").toString()));
	ui->expirationComboBox->setCurrentIndex(expireIndexFromDuration(settings->value("Expiration", 0).toInt()));
}

void StikkedSettings::load(BasePasteService *service)
{
	Stikked *stikked = static_cast<Stikked*>(service);

	ui->apiUrlLineEdit->setText(stikked->url());
	ui->nameLineEdit->setText(stikked->name());
	ui->titleLineEdit->setText(stikked->title());
	ui->privateCheckBox->setChecked(stikked->isPrivate());
	ui->langComboBox->setCurrentIndex(Stikked::langIndexFromName(Stikked::m_languages, stikked->lang()));
	ui->expirationComboBox->setCurrentIndex(expireIndexFromDuration(stikked->expiration()));
}

QHash<QString, QVariant> StikkedSettings::settings()
{
	QHash<QString, QVariant> s;

	s["Url"] = ui->apiUrlLineEdit->text();
	s["Name"] = ui->nameLineEdit->text();
	s["Title"] = ui->titleLineEdit->text();
	s["Private"] = ui->privateCheckBox->isChecked();
	s["Lang"] = Stikked::m_languages[ui->langComboBox->currentIndex()].name;

	int e;

	switch(ui->expirationComboBox->currentIndex())
	{
	case 0: // 30 minutes
		e = 30;
		break;
	case 1: // 1 hour
		e = 60;
		break;
	case 2: // 6 hours
		e = 6*60;
		break;
	case 3: // 12 hours
		e = 12*60;
		break;
	case 4: // 1 day
		e = 24*60;
		break;
	case 5: // 1 week
		e = 7*24*60;
		break;
	case 6: // 4 weeks
		e = 4*7*24*60;
		break;
	case 7: // forever
		e = 0;
		break;
	default:
		e = 60;
	}

	s["Expire"] = e;

	return s;
}

int StikkedSettings::expireIndexFromDuration(int d)
{
	switch(d)
	{
	case 30:
		return 0;
	case 60:
		return 1;
	case 6*60:
		return 2;
	case 12*60:
		return 3;
	case 24*60:
		return 4;
	case 7*24*60:
		return 5;
	case 4*7*24*60:
		return 6;
	case 0:
		return 7;
	default:
		return 1;
	}
}
