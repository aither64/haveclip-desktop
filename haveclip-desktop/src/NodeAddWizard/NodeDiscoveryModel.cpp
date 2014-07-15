#include "NodeDiscoveryModel.h"

#include "Network/AutoDiscovery.h"
#include "Node.h"

NodeDiscoveryModel::NodeDiscoveryModel(AutoDiscovery *discovery, QObject *parent) :
	QAbstractListModel(parent),
	m_discovery(discovery)
{
	connect(m_discovery, SIGNAL(aboutToDiscover()), this, SLOT(resetDiscovery()));
	connect(m_discovery, SIGNAL(peerDiscovered(Node*)), this, SLOT(addNode(Node*)));
}

int NodeDiscoveryModel::columnCount(const QModelIndex &parent) const
{
	return ColumnCount;
}

int NodeDiscoveryModel::rowCount(const QModelIndex &parent) const
{
	return m_nodes.count();
}

QVariant NodeDiscoveryModel::data(const QModelIndex &index, int role) const
{
	if(role != Qt::DisplayRole)
		return QVariant();

	const int row = index.row();
	const int col = index.column();

	switch(col)
	{
	case Name:
		return m_nodes[row]->name();

	case Compatible:
		return m_nodes[row]->isCompatible() ? tr("compatible") : tr("not compatible");

	case HostAddress:
		return m_nodes[row]->host();

	case HostPort:
		return m_nodes[row]->port();

	default:
		break;
	}

	return QVariant();
}

QVariant NodeDiscoveryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation == Qt::Vertical || role != Qt::DisplayRole)
		return QVariant();

	switch(section)
	{
	case Name:
		return tr("Name");

	case Compatible:
		return tr("Compatibility");

	case HostAddress:
		return tr("Address");

	case HostPort:
		return tr("Port");

	default:
		break;
	}

	return QVariant();
}

Node* NodeDiscoveryModel::nodeAt(const QModelIndex &index)
{
	return m_nodes[index.row()];
}

void NodeDiscoveryModel::resetDiscovery()
{
	if(m_nodes.empty())
		return;

	beginRemoveRows(QModelIndex(), 0, m_nodes.count() - 1);

	m_nodes.clear();

	endRemoveRows();
}

void NodeDiscoveryModel::addNode(Node *n)
{
	int cnt = m_nodes.count();

	beginInsertRows(QModelIndex(), cnt, cnt);

	m_nodes << n;

	endInsertRows();
}
