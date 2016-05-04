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

#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QWizardPage>
#include <QModelIndex>

class ConnectionManager;
class NodeDiscoveryModel;

namespace Ui {
class SearchPage;
}

class SearchPage : public QWizardPage
{
	Q_OBJECT

public:
	explicit SearchPage(ConnectionManager *conman, QWidget *parent = 0);
	~SearchPage();
	virtual bool isComplete() const;

private slots:
	void selectNode(const QModelIndex &index);
	void checkComplete();

private:
	Ui::SearchPage *ui;
	NodeDiscoveryModel *m_discoveryModel;
	bool m_complete;
};

#endif // SEARCHPAGE_H
