#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QWizardPage>

namespace Ui {
class SearchPage;
}

class SearchPage : public QWizardPage
{
	Q_OBJECT

public:
	explicit SearchPage(QWidget *parent = 0);
	~SearchPage();
	virtual bool isComplete() const;

private slots:
	void checkComplete();

private:
	Ui::SearchPage *ui;
	bool m_complete;
};

#endif // SEARCHPAGE_H
