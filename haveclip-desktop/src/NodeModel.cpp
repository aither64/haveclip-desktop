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

#include "NodeModel.h"

#include <QStringList>

#include "Settings.h"
#include "Node.h"

NodeModel::NodeModel(QObject *parent) :
        QAbstractListModel(parent)
{
	Settings *s = Settings::get();

	connect(s, SIGNAL(nodeAdded(Node)), this, SLOT(addNode(Node)));
	connect(s, SIGNAL(nodeUpdated(Node)), this, SLOT(updateNode(Node)));
}

int NodeModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return m_nodes.count();
}

QVariant NodeModel::data(const QModelIndex &index, int role) const
{
	const int row = index.row();

	if(row > m_nodes.count())
		return QVariant();

	switch(role)
	{
	case Qt::DisplayRole:
		return m_nodes[row].name();

	default:
		return QVariant();
	}
}

void NodeModel::removeNode(QModelIndex i)
{
	if(!i.isValid())
		return;

	const int r = i.row();

	beginRemoveRows(QModelIndex(), r, r);

	m_nodes.removeAt(r);

	endRemoveRows();
}

QList<Node>& NodeModel::nodes()
{
	return m_nodes;
}

Node NodeModel::nodeForIndex(QModelIndex index)
{
	if(!index.isValid())
		return Node();

	return m_nodes[index.row()];
}

void NodeModel::addNode(const Node &n)
{
	beginInsertRows(QModelIndex(), m_nodes.count(), m_nodes.count());

	m_nodes << n;

	endInsertRows();
}

void NodeModel::updateNode(const Node &n)
{
	int cnt = m_nodes.count();

	for(int i = 0; i < cnt; i++)
	{
		if(m_nodes[i].id() == n.id())
		{
			m_nodes[i] = n;

			QModelIndex modelIndex = index(i, 0);
			emit dataChanged(modelIndex, modelIndex);

			return;
		}
	}
}

void NodeModel::resetModel()
{
	bool signal = m_nodes.isEmpty();

	if(signal)
		beginRemoveRows(QModelIndex(), 0, m_nodes.count()-1);

	m_nodes.clear();

	if(signal)
		endRemoveRows();

	m_nodes = Settings::get()->nodes();

	beginResetModel();
	endResetModel();
}
