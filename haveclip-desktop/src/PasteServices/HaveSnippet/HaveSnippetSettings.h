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
