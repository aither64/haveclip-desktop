#include "HaveSnippetSettings.h"
#include "ui_HaveSnippetSettings.h"

#include "PasteServices/HaveSnippet//HaveSnippet.h"

#include <QDebug>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QUrlQuery>
#endif

HaveSnippetSettings::HaveSnippetSettings(BasePasteServiceWidget::Mode mode, QWidget *parent) :
	BasePasteServiceWidget(mode, parent),
	ui(new Ui::HaveSnippetSettings),
	languagesReply(0),
	tagsReply(0)
{
	ui->setupUi(this);

	switch(mode)
	{
	case BasePasteServiceWidget::Settings:
		break;
	case BasePasteServiceWidget::Paste:
		ui->apiUrlLabel->hide();
		ui->apiUrlLineEdit->hide();
		ui->apiKeyLabel->hide();
		ui->apiKeyLineEdit->hide();
		break;
	}

	manager = new QNetworkAccessManager(this);

	tagListModel = new QStringListModel(QStringList(), this);
	ui->tagsLineEdit->setModel(tagListModel);

	connect(ui->apiUrlLineEdit, SIGNAL(editingFinished()), this, SLOT(refreshData()));

	m_language = "text";

//	for(int i = 0; Stikked::m_languages[i].name != 0; i++)
//		ui->langComboBox->addItem(Stikked::m_languages[i].label);
}

HaveSnippetSettings::~HaveSnippetSettings()
{
	delete ui;
}

void HaveSnippetSettings::load(QSettings *settings)
{
	ui->apiUrlLineEdit->setText(settings->value("Url", "http://your.sticked/api/create").toString());
	ui->nameLineEdit->setText(settings->value("Name").toString());
	ui->titleLineEdit->setText(settings->value("Title").toString());
	ui->shortDescriptionLineEdit->setText(settings->value("ShortDescription").toString());
	m_language = settings->value("Language", "text").toString();
	ui->fullDescriptionTextEdit->setPlainText(settings->value("FullDescription").toString());
	ui->tagsLineEdit->setText(settings->value("Tags").toString());
	ui->accessibilityComboBox->setCurrentIndex(settings->value("Accessibility", 1).toInt());
	ui->expirationComboBox->setCurrentIndex(expireIndexFromDuration(settings->value("Expiration", 0).toInt()));
	ui->apiKeyLineEdit->setText(settings->value("ApiKey").toString());

	refreshData();
}

void HaveSnippetSettings::load(BasePasteService *service)
{
	HaveSnippet *snippet = static_cast<HaveSnippet*>(service);

	ui->apiUrlLineEdit->setText(snippet->url());
	ui->nameLineEdit->setText(snippet->name());
	ui->titleLineEdit->setText(snippet->title());
	ui->shortDescriptionLineEdit->setText(snippet->shortDescription());
	m_language = snippet->lang();
	ui->fullDescriptionTextEdit->setPlainText(snippet->fullDescription());
	ui->tagsLineEdit->setText(snippet->tags());
	ui->accessibilityComboBox->setCurrentIndex(snippet->accessibility());
	ui->expirationComboBox->setCurrentIndex(expireIndexFromDuration(snippet->expiration()));
	ui->apiKeyLineEdit->setText(snippet->apiKey());

	refreshData();
}

QHash<QString, QVariant> HaveSnippetSettings::settings()
{
	QHash<QString, QVariant> s;

	s["Url"] = ui->apiUrlLineEdit->text();
	s["Name"] = ui->nameLineEdit->text();
	s["Title"] = ui->titleLineEdit->text();
	s["ShortDescription"] = ui->shortDescriptionLineEdit->text();

	if(m_languages.size())
		s["Language"] = m_languages[ ui->langComboBox->currentIndex() ].toStringList()[0];
	else
		s["Language"] = "text";

	s["FullDescription"] = ui->fullDescriptionTextEdit->toPlainText();
	s["Tags"] = clearTags(ui->tagsLineEdit->text());
	s["Accessibility"] = ui->accessibilityComboBox->currentIndex();

	int e;

	switch(ui->expirationComboBox->currentIndex())
	{
	case 0: // 30 minutes
		e = 30;
		break;
	case 1: // 1 hour
		e = 60;
		break;
	case 2: // 12 hours
		e = 12*60;
		break;
	case 3: // 1 day
		e = 24*60;
		break;
	case 4: // 1 week
		e = 7*24*60;
		break;
	case 5: // 4 weeks
		e = 4*7*24*60;
		break;
	case 6: // forever
		e = 0;
		break;
	default:
		e = 60;
	}

	s["Expiration"] = e;
	s["ApiKey"] = ui->apiKeyLineEdit->text();
	s["LanguageList"] = m_languages;
	s["TagList"] = m_tags;

	return s;
}

void HaveSnippetSettings::refreshData()
{
	if(!languagesReply && !tagsReply)
	{
		fetchLanguages();
		fetchTags();
	}
}

int HaveSnippetSettings::expireIndexFromDuration(int d)
{
	switch(d)
	{
	case 30:
		return 0;
	case 60:
		return 1;
	case 12*60:
		return 2;
	case 24*60:
		return 3;
	case 7*24*60:
		return 4;
	case 4*7*24*60:
		return 5;
	case 0:
		return 6;
	default:
		return 0;
	}
}

void HaveSnippetSettings::fetchLanguages()
{
	QUrl url = addApikey(HaveSnippet::apiUrl(ui->apiUrlLineEdit->text(), "languages"));

	languagesReply = manager->get(QNetworkRequest(url));

	connect(languagesReply, SIGNAL(finished()), this, SLOT(languagesFetched()));
}

void HaveSnippetSettings::fetchTags()
{
	QUrl url = addApikey(HaveSnippet::apiUrl(ui->apiUrlLineEdit->text(), "tags"));

	tagsReply = manager->get(QNetworkRequest(url));

	connect(tagsReply, SIGNAL(finished()), this, SLOT(tagsFetched()));
}

void HaveSnippetSettings::languagesFetched()
{
	if(languagesReply->error() == QNetworkReply::NoError && languagesReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
	{
		QString ret = languagesReply->readAll().trimmed();
		QStringList languages = ret.split("\n");

		ui->langComboBox->clear();
		m_languages.clear();

		foreach(QString lang, languages)
		{
			int index = lang.indexOf('=');

			if(index == -1)
				continue;

			QStringList tmp;
			QString name = lang.left(index);
			QString label = lang.mid(index+1);

			tmp << name << label;
			m_languages << tmp;

			ui->langComboBox->addItem(label);

			if(name == m_language)
				ui->langComboBox->setCurrentIndex(ui->langComboBox->count() - 1);
		}
	}

	languagesReply->deleteLater();
	languagesReply = 0;
}

void HaveSnippetSettings::tagsFetched()
{
	if(tagsReply->error() == QNetworkReply::NoError && tagsReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
	{
		m_tags.clear();

		QString ret = tagsReply->readAll().trimmed();
		m_tags = ret.split("\n");

		tagListModel->setStringList(m_tags);
	}

	tagsReply->deleteLater();
	tagsReply = 0;
}

QString HaveSnippetSettings::clearTags(QString raw)
{
	QStringList cleaned;
	QStringList tags = raw.split(',');

	foreach(QString tag, tags)
		cleaned << tag.trimmed();

	return cleaned.join(",");
}

QUrl HaveSnippetSettings::addApikey(QUrl url)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	QUrlQuery tmp;
#else
	QUrl tmp = url;
#endif

	if(!ui->apiKeyLineEdit->text().isEmpty())
		tmp.addQueryItem("api_key", ui->apiKeyLineEdit->text());

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	QUrl ret = url;
	ret.setQuery(tmp);

	return ret;
#else
	return tmp;
#endif
}
