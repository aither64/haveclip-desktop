#ifndef NODEDIALOG_H
#define NODEDIALOG_H

#include <QDialog>

#include "Node.h"

namespace Ui {
class NodeDialog;
}

class NodeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NodeDialog(QWidget *parent = 0);
	explicit NodeDialog(Node &n, QWidget *parent);
	~NodeDialog();
	Node& node();

private:
	Ui::NodeDialog *ui;
	Node m_node;
};

#endif // NODEDIALOG_H
