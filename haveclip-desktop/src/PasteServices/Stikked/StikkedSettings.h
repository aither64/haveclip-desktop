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

#ifndef STIKKEDSETTINGS_H
#define STIKKEDSETTINGS_H

#include <QHash>
#include "../BasePasteServiceWidget.h"

namespace Ui {
class StikkedSettings;
}

class StikkedSettings : public BasePasteServiceWidget
{
	Q_OBJECT
public:
	explicit StikkedSettings(BasePasteServiceWidget::Mode mode, QWidget *parent = 0);
	~StikkedSettings();
	void load(QSettings *settings);
	void load(BasePasteService *service);
	QHash<QString, QVariant> settings();
	
signals:
	
public slots:

private:
	Ui::StikkedSettings *ui;
	
	int expireIndexFromDuration(int d);
};

#endif // STIKKEDSETTINGS_H
