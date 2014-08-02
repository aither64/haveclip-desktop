#ifndef VERIFICATIONPAGE_H
#define VERIFICATIONPAGE_H

#include <QWizardPage>

#include "Network/ConnectionManager.h"

namespace Ui {
class VerificationPage;
}

class VerificationPage : public QWizardPage
{
	Q_OBJECT

public:
	explicit VerificationPage(ConnectionManager *conman, QWidget *parent = 0);
	~VerificationPage();
	virtual void initializePage();
	virtual bool isComplete() const;

private slots:
	void verificationComplete(ConnectionManager::CodeValidity validity);

private:
	Ui::VerificationPage *ui;
	ConnectionManager *m_conman;
};

#endif // VERIFICATIONPAGE_H
