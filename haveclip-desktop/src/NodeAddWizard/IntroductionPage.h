#ifndef INTRODUCTIONPAGE_H
#define INTRODUCTIONPAGE_H

#include <QWizardPage>

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

private slots:
	void introductionComplete();

private:
	Ui::IntroductionPage *ui;
	ConnectionManager *m_conman;
};

#endif // INTRODUCTIONPAGE_H
