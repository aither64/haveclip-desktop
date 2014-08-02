#ifndef NODEDIALOG_H
#define NODEDIALOG_H

#include <QDialog>

#include "Node.h"

class ConnectionManager;

namespace Ui {
class NodeDialog;
}

class NodeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NodeDialog(Node &n, ConnectionManager *conman, QWidget *parent);
	~NodeDialog();
	Node& node();

private slots:
	void verifyNode();

private:
	Ui::NodeDialog *ui;
	Node m_node;
	ConnectionManager *m_conman;

	void showInfo();
};

#endif // NODEDIALOG_H
