#ifndef PAIREDPAGE_H
#define PAIREDPAGE_H

#include <QWizardPage>

namespace Ui {
class PairedPage;
}

class PairedPage : public QWizardPage
{
	Q_OBJECT

public:
	explicit PairedPage(QWidget *parent = 0);
	~PairedPage();

private:
	Ui::PairedPage *ui;
};

#endif // PAIREDPAGE_H
