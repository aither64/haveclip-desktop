#ifndef SECURITYCODEPROMPT_H
#define SECURITYCODEPROMPT_H

#include <QDialog>

class ConnectionManager;
class Node;

namespace Ui {
class SecurityCodePrompt;
}

class SecurityCodePrompt : public QDialog
{
	Q_OBJECT

public:
	explicit SecurityCodePrompt(const Node &n, ConnectionManager *conman, QWidget *parent = 0);
	~SecurityCodePrompt();

private slots:
	void verifyCode();
	void verificationFinish(bool ok);

private:
	Ui::SecurityCodePrompt *ui;
	ConnectionManager *m_conman;
};

#endif // SECURITYCODEPROMPT_H
