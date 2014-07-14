#include "SearchPage.h"
#include "ui_SearchPage.h"

SearchPage::SearchPage(QWidget *parent) :
        QWizardPage(parent),
	ui(new Ui::SearchPage),
	m_complete(false)
{
	ui->setupUi(this);

	registerField("node_host", ui->hostLineEdit);
	registerField("node_port", ui->portLineEdit);

	connect(ui->hostLineEdit, SIGNAL(textEdited(QString)), this, SLOT(checkComplete()));
	connect(ui->portLineEdit, SIGNAL(textEdited(QString)), this, SLOT(checkComplete()));
	connect(ui->nodeListView, SIGNAL(activated(QModelIndex)), this, SLOT(checkComplete()));
}

SearchPage::~SearchPage()
{
	delete ui;
}

bool SearchPage::isComplete() const
{
	return m_complete;
}

void SearchPage::checkComplete()
{
	bool c = (!ui->hostLineEdit->text().isEmpty() && !ui->portLineEdit->text().isEmpty()) || ui->nodeListView->currentIndex().isValid();

	if(c != m_complete)
	{
		m_complete = c;
		emit completeChanged();
	}
}
