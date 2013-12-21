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

#ifndef PASTESERVICEEDITDIALOG_H
#define PASTESERVICEEDITDIALOG_H

#include <QDialog>

#include "PasteServices/BasePasteService.h"

namespace Ui {
class PasteServiceEditDialog;
}

class PasteServiceEditDialog : public QDialog
{
	Q_OBJECT
	
public:
	enum Action {
		Add,
		Edit
	};

	explicit PasteServiceEditDialog(Action action, BasePasteService *service, QWidget *parent = 0);
	~PasteServiceEditDialog();
	BasePasteService::PasteService type();
	QString label();
	QHash<QString, QVariant> settings();
	
private:
	Ui::PasteServiceEditDialog *ui;
	BasePasteService* m_service;
	QList<BasePasteService*> services;
};

#endif // PASTESERVICEEDITDIALOG_H
