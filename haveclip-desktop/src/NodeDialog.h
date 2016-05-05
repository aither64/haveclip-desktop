/*
  HaveClip

  Copyright (C) 2013-2016 Jakub Skokan <aither@havefun.cz>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

private slots:
	void verifyNode();

private:
	Ui::NodeDialog *ui;
	Node m_node;
	ConnectionManager *m_conman;

	void showInfo();
};

#endif // NODEDIALOG_H
