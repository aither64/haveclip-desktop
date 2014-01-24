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

#ifndef HAVESNIPPETSETTINGS_H
#define HAVESNIPPETSETTINGS_H

#include "../BasePasteServiceWidget.h"

#include <QNetworkAccessManager>
#include <QStringListModel>

namespace Ui {
class HaveSnippetSettings;
}

class HaveSnippetSettings : public BasePasteServiceWidget
{
	Q_OBJECT
	
public:
	explicit HaveSnippetSettings(BasePasteServiceWidget::Mode mode, QWidget *parent = 0);
	~HaveSnippetSettings();
	void load(QSettings *settings);
	void load(BasePasteService *service);
	QHash<QString, QVariant> settings();

signals:

public slots:
	void refreshData();

private:
	Ui::HaveSnippetSettings *ui;
	QNetworkAccessManager *manager;
	QNetworkReply *languagesReply;
	QNetworkReply *tagsReply;
	QList<QVariant> m_languages;
	QStringList m_tags;
	QString m_language;
	QStringListModel *tagListModel;

	int expireIndexFromDuration(int d);
	void fetchLanguages();
	void fetchTags();
	QString clearTags(QString raw);
	QUrl addApikey(QUrl url);

private slots:
	void languagesFetched();
	void tagsFetched();
};

#endif // HAVESNIPPETSETTINGS_H
