#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QWizardPage>
#include <QModelIndex>

class ConnectionManager;
class NodeDiscoveryModel;

namespace Ui {
class SearchPage;
}

class SearchPage : public QWizardPage
{
	Q_OBJECT

public:
	explicit SearchPage(ConnectionManager *conman, QWidget *parent = 0);
	~SearchPage();
	virtual bool isComplete() const;

private slots:
	void selectNode(const QModelIndex &index);
	void checkComplete();

private:
	Ui::SearchPage *ui;
	NodeDiscoveryModel *m_discoveryModel;
	bool m_complete;
};

#endif // SEARCHPAGE_H
