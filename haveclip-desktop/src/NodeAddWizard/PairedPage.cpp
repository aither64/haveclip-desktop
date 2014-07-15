#include "PairedPage.h"
#include "ui_PairedPage.h"

PairedPage::PairedPage(QWidget *parent) :
        QWizardPage(parent),
        ui(new Ui::PairedPage)
{
	ui->setupUi(this);
}

PairedPage::~PairedPage()
{
	delete ui;
}
