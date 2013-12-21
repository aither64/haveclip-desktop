#ifndef AUTOCOMPLETELINEEDIT_H
#define AUTOCOMPLETELINEEDIT_H

#include <QLineEdit>
#include <QCompleter>

class AutoCompleteLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	explicit AutoCompleteLineEdit(QAbstractItemModel *model = 0, QWidget *parent = 0);
	explicit AutoCompleteLineEdit(QWidget *parent = 0);
	void setModel(QAbstractItemModel *model);
	void setSeparator(QString s);
	
signals:
	
private slots:
	void insertCompletion(const QString& completion);

private:
	QCompleter *m_completer;
	QString m_separator;

	void keyPressEvent(QKeyEvent *event);
	QString textUnderCursor() const;

};

#endif // AUTOCOMPLETELINEEDIT_H
