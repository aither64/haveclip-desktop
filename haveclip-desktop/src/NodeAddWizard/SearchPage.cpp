#include "SearchPage.h"
#include "ui_SearchPage.h"

#include "Network/ConnectionManager.h"
#include "NodeDiscoveryModel.h"
#include "Node.h"

SearchPage::SearchPage(ConnectionManager *conman, QWidget *parent) :
        QWizardPage(parent),
	ui(new Ui::SearchPage),
	m_complete(false)
{
	ui->setupUi(this);

	registerField("node_host", ui->hostLineEdit);
	registerField("node_port", ui->portLineEdit);

	connect(ui->hostLineEdit, SIGNAL(textEdited(QString)), this, SLOT(checkComplete()));
	connect(ui->portLineEdit, SIGNAL(textEdited(QString)), this, SLOT(checkComplete()));

	connect(ui->nodeTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectNode(QModelIndex)));
	connect(ui->nodeTreeView, SIGNAL(activated(QModelIndex)), this, SLOT(selectNode(QModelIndex)));

	m_discoveryModel = new NodeDiscoveryModel(conman->autoDiscovery(), this);

	ui->nodeTreeView->setModel(m_discoveryModel);
}

SearchPage::~SearchPage()
{
	delete ui;
}

bool SearchPage::isComplete() const
{
	return m_complete;
}

void SearchPage::selectNode(const QModelIndex &index)
{
	if(!index.isValid())
		return;

	Node n = m_discoveryModel->nodeAt(index);

	if(!n.isCompatible())
		return;

	ui->hostLineEdit->setText(n.host());
	ui->portLineEdit->setText(QString::number(n.port()));

	checkComplete();
}

void SearchPage::checkComplete()
{
	bool c = !ui->hostLineEdit->text().isEmpty() && !ui->portLineEdit->text().isEmpty();

	if(c != m_complete)
	{
		m_complete = c;
		emit completeChanged();
	}
}
