#ifndef NODEDISCOVERYMODEL_H
#define NODEDISCOVERYMODEL_H

#include <QAbstractListModel>

#include "Node.h"

class AutoDiscovery;

class NodeDiscoveryModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum Columns {
		Name = 0,
		Compatible,
		HostAddress,
		HostPort,
		ColumnCount
	};

	explicit NodeDiscoveryModel(AutoDiscovery *discovery, QObject *parent = 0);

	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	Node nodeAt(const QModelIndex &index);

signals:

private slots:
	void resetDiscovery();
	void addNode(const Node &n);

private:
	AutoDiscovery *m_discovery;
	QList<Node> m_nodes;
};

#endif // NODEDISCOVERYMODEL_H
