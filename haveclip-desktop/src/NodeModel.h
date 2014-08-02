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
