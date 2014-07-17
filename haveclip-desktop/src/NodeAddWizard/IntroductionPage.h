#ifndef INTRODUCTIONPAGE_H
#define INTRODUCTIONPAGE_H

#include <QWizardPage>

#include "Node.h"

class ConnectionManager;

namespace Ui {
class IntroductionPage;
}

class IntroductionPage : public QWizardPage
{
	Q_OBJECT

public:
	explicit IntroductionPage(ConnectionManager *conman, QWidget *parent = 0);
	~IntroductionPage();
	virtual void initializePage();
	virtual bool isComplete() const;
	void setNode(Node &n);

private slots:
	void introductionComplete();

private:
	Ui::IntroductionPage *ui;
	ConnectionManager *m_conman;
	Node m_node;
};

#endif // INTRODUCTIONPAGE_H
