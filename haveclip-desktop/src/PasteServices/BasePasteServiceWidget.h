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

#ifndef BASEPASTESERVICEWIDGET_H
#define BASEPASTESERVICEWIDGET_H

#include <QWidget>
#include "PasteServices/BasePasteService.h"

class BasePasteServiceWidget : public QWidget
{
	Q_OBJECT
public:
	enum Mode {
		Settings,
		Paste
	};

	explicit BasePasteServiceWidget(Mode mode, QWidget *parent = 0);
	virtual void load(QSettings *settings) = 0;
	virtual void load(BasePasteService *service) = 0;
	virtual QHash<QString, QVariant> settings() = 0;
	
signals:
	
public slots:
	
private:
	Mode mode;
};

#endif // BASEPASTESERVICEWIDGET_H
