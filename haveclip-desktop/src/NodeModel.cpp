#include "NodeModel.h"

#include <QStringList>

#include "Settings.h"
#include "Node.h"

NodeModel::NodeModel(QObject *parent) :
        QAbstractListModel(parent)
{
	m_nodes = Settings::get()->nodes();
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

void NodeModel::addNode(Node n)
{
	beginInsertRows(QModelIndex(), m_nodes.count(), m_nodes.count());

	m_nodes << n;

	endInsertRows();
}

void NodeModel::updateNode(Node &n)
{
//	QModelIndex i = index(m_nodes.indexOf(n), 0);

//	emit dataChanged(i, i);
	// FIXME
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
