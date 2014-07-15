#include "NodeModel.h"

#include <QStringList>

#include "Node.h"

NodeModel::NodeModel(QSettings *settings, QObject *parent) :
        QAbstractListModel(parent)
{
	settings->beginGroup("Pool/Nodes");

	Node *n;

	foreach(QString key, settings->childGroups())
	{
		settings->beginGroup(key);

		n = Node::load(settings);

		if(n) m_nodes << n;

		settings->endGroup();
	}

	settings->endGroup();
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
		return m_nodes[row]->name();

	default:
		return QVariant();
	}
}

void NodeModel::addNode(Node *n)
{
	beginInsertRows(QModelIndex(), m_nodes.count(), m_nodes.count());

	m_nodes << n;

	endInsertRows();
}

void NodeModel::updateNode(Node *n)
{
	QModelIndex i = index(m_nodes.indexOf(n), 0);

	emit dataChanged(i, i);
}

void NodeModel::removeNode(QModelIndex i)
{
	if(!i.isValid())
		return;

	const int r = i.row();

	beginRemoveRows(QModelIndex(), r, r);

	Node *n = m_nodes.takeAt(r);

	endRemoveRows();

	delete n;
}

QList<Node*> NodeModel::nodes()
{
	return m_nodes;
}

Node* NodeModel::nodeForIndex(QModelIndex index)
{
	if(!index.isValid())
		return 0;

	return m_nodes[index.row()];
}
