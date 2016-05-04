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

#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <QAbstractListModel>
#include <QSettings>

#include "Node.h"

class NodeModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit NodeModel(QObject *parent = 0);
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	void removeNode(Node &n);
	void removeNode(QModelIndex i);
	QList<Node>& nodes();
	Node nodeForIndex(QModelIndex index);

public slots:
	void addNode(const Node &n);
	void updateNode(const Node &n);
	void resetModel();

private:
	QList<Node> m_nodes;

};

#endif // NODEMODEL_H
