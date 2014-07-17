#ifndef NODEDIALOG_H
#define NODEDIALOG_H

#include <QDialog>

#include "Node.h"

class ConnectionManager;

namespace Ui {
class NodeDialog;
}

class NodeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NodeDialog(Node &n, ConnectionManager *conman, QWidget *parent);
	~NodeDialog();
	Node& node();
	static QString formatDigest(QByteArray raw);

private slots:
	void verifyNode();

private:
	Ui::NodeDialog *ui;
	Node m_node;
	ConnectionManager *m_conman;

	void showInfo();
	QString subjectInfo(QSslCertificate &cert, QSslCertificate::SubjectInfo info);
};

#endif // NODEDIALOG_H
