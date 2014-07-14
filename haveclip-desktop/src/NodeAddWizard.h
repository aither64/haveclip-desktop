#ifndef NODEADDWIZARD_H
#define NODEADDWIZARD_H

#include <QWizard>

class Node;
class ConnectionManager;

class NodeAddWizard : public QWizard
{
	Q_OBJECT
public:
	enum Pages {
		Search = 0,
		Introduction,
		Verification,
		Paired
	};

	explicit NodeAddWizard(ConnectionManager *conman, QWidget *parent = 0);
	Node* node();
	void setNode(Node *n);

public slots:
	virtual void reject();

private slots:
	void pageChange(int id);
	void buttonClick(int which);

private:
	ConnectionManager *m_conman;
	Node *m_node;
};

#endif // NODEADDWIZARD_H
