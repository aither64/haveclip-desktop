#include "NodeModel.h"

#include <QStringList>

#include "Settings.h"
#include "Node.h"

NodeModel::NodeModel(QObject *parent) :
        QAbstractListModel(parent)
{
	Settings *s = Settings::get();

	m_nodes = s->nodes();

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
