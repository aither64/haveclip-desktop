#include "NodeDialog.h"
#include "ui_NodeDialog.h"

#include "Node.h"

NodeDialog::NodeDialog(QWidget *parent) :
        QDialog(parent),
	ui(new Ui::NodeDialog)
{
	ui->setupUi(this);

	setWindowTitle(tr("Add node"));
}

NodeDialog::NodeDialog(Node &n, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NodeDialog),
	m_node(n)
{
	ui->setupUi(this);

	setWindowTitle(tr("Edit node %1").arg(n.name()));

	ui->nameLineEdit->setText(n.name());
	ui->hostLineEdit->setText(n.host());
	ui->portLineEdit->setText(QString::number(n.port()));
}

NodeDialog::~NodeDialog()
{
	delete ui;
}

Node& NodeDialog::node()
{
	m_node.setName( ui->nameLineEdit->text() );
	m_node.setHost( ui->hostLineEdit->text() );
	m_node.setPort( ui->portLineEdit->text().toUShort() );

	return m_node;
}
