#ifndef NODEADDWIZARD_H
#define NODEADDWIZARD_H

#include <QWizard>

#include "Node.h"

class ConnectionManager;
class IntroductionPage;

class NodeAddWizard : public QWizard
{
	Q_OBJECT
public:
	enum Pages {
		SearchPageId = 0,
		IntroductionPageId,
		VerificationPageId,
		PairedPageId
	};

	enum Mode {
		SearchMode,
		VerifyMode
	};

	explicit NodeAddWizard(Mode mode, ConnectionManager *conman, QWidget *parent = 0);
	Node& node();
	void setNode(Node &n);
	void verifyNode(Node &n);

public slots:
	virtual void reject();

private slots:
	void pageChange(int id);
	void buttonClick(int which);

private:
	Mode m_mode;
	ConnectionManager *m_conman;
	Node m_node;
	IntroductionPage *m_introPage;

	int idOf(int page);
};

#endif // NODEADDWIZARD_H
