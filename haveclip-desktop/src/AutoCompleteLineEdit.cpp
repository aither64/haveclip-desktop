#include "AutoCompleteLineEdit.h"

#include <QAbstractItemView>
#include <QKeyEvent>
#include <QDebug>

AutoCompleteLineEdit::AutoCompleteLineEdit(QAbstractItemModel *model, QWidget *parent) :
        QLineEdit(parent)
{
	m_completer = new QCompleter(this);
	m_separator = ",";

	connect(m_completer, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));

	if(model)
		setModel(model);
}

AutoCompleteLineEdit::AutoCompleteLineEdit(QWidget *parent) :
	AutoCompleteLineEdit(0, parent)
{

}

void AutoCompleteLineEdit::setModel(QAbstractItemModel *model)
{
	m_completer->setModel(model);
	m_completer->setWidget(this);
}

void AutoCompleteLineEdit::setSeparator(QString s)
{
	m_separator = s;
}

void AutoCompleteLineEdit::insertCompletion(const QString &completion)
{
	int len = completion.count() - m_completer->completionPrefix().count();
	int cursor = cursorPosition();

	setText(text().insert(cursor, completion.right(len)));
	setCursorPosition(cursor + len);
}

void AutoCompleteLineEdit::keyPressEvent(QKeyEvent *event)
{
	if(m_completer->popup()->isVisible())
	{
		switch(event->key())
		{
		case Qt::Key_Enter:
		case Qt::Key_Return:
		case Qt::Key_Escape:
		case Qt::Key_Tab:
		    event->ignore();
		    return;
		}
	}


	QLineEdit::keyPressEvent(event);

	QString completionPrefix = textUnderCursor();

	if(completionPrefix != m_completer->completionPrefix())
	{
		m_completer->setCompletionPrefix(completionPrefix);
		m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0,0));
	}

	if(!event->text().isEmpty() && !completionPrefix.isEmpty())
	{
		m_completer->complete();
	}

	if(completionPrefix.isEmpty())
		m_completer->popup()->hide();
}

QString AutoCompleteLineEdit::textUnderCursor() const
{
	QString str = text();
	int cnt = str.count();
	int cursor = cursorPosition();
	int start = str.lastIndexOf(m_separator, cnt == cursor ? -1 : -1 * (str.count() - cursorPosition() + m_separator.count()));

	if(start++ == -1)
		return str;

	int end = str.indexOf(m_separator, start);

	return str.mid(start, end == -1 ? -1 : end-start).trimmed();
}
