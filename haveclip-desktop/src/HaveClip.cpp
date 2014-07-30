/*
  HaveClip

  Copyright (C) 2013 Jakub Skokan <aither@havefun.cz>

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

#include "HaveClip.h"

#include <QStringList>
#include <QUrl>
#include <QImage>
#include <QColor>
#include <QMenu>
#include <QLabel>
#include <QTextDocument>
#include <QMessageBox>
#include <QApplication>

#ifdef INCLUDE_SERIAL_MODE
#include <QxtGui/QxtGlobalShortcut>
#endif

#include "Settings.h"
#include "SettingsDialog.h"
#include "AboutDialog.h"
#include "CertificateTrustDialog.h"
#include "Node.h"
#include "SecurityCodePrompt.h"
#include "CertificateGeneratorDialog.h"

HaveClip::HaveClip(QObject *parent) :
	QObject(parent)
{
	Settings *s = Settings::create(this);

	connect(s, SIGNAL(firstStart()), this, SLOT(onFirstStart()));

	s->init();

	manager = new ClipboardManager(this);

	connect(manager->history(), SIGNAL(historyChanged()), this, SLOT(updateHistory()));
	connect(manager->connectionManager(), SIGNAL(untrustedCertificateError(Node,QList<QSslError>)), this, SLOT(determineCertificateTrust(Node,QList<QSslError>)));
	connect(manager->connectionManager(), SIGNAL(sslFatalError(QList<QSslError>)), this, SLOT(sslFatalError(QList<QSslError>)));
	connect(manager->connectionManager(), SIGNAL(verificationRequested(Node)), this, SLOT(verificationRequest(Node)));

	historySignalMapper = new QSignalMapper(this);

	connect(historySignalMapper, SIGNAL(mapped(QObject*)), this, SLOT(historyActionClicked(QObject*)));

	// Tray
	trayIcon = new QSystemTrayIcon(QIcon(":/gfx/HaveClip_128.png"), this);
	trayIcon->setToolTip(tr("HaveClip"));

#ifndef Q_OS_MAC
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
#endif

	historyMenu = new QMenu(tr("History"));
	historySeparator = historyMenu->addSeparator();

	menu = new QMenu;

#if defined Q_OS_MAC
	menu->addMenu(historyMenu);
	menu->addSeparator();
#endif

	QAction *a = menu->addAction(tr("&Enable clipboard synchronization"));
	a->setCheckable(true);
	a->setChecked(manager->isSyncEnabled());
	connect(a, SIGNAL(toggled(bool)), this, SLOT(toggleSharedClipboard(bool)));

	clipSndAction = menu->addAction(tr("Enable clipboard se&nding"));
	clipSndAction->setCheckable(true);
	clipSndAction->setChecked(manager->isSendingEnabled());
	clipSndAction->setEnabled(manager->isSyncEnabled());
	connect(clipSndAction, SIGNAL(toggled(bool)), this, SLOT(toggleSend(bool)));

	clipRecvAction = menu->addAction(tr("Enable clipboard &receiving"));
	clipRecvAction->setCheckable(true);
	clipRecvAction->setChecked(manager->isReceivingEnabled());
	clipRecvAction->setEnabled(manager->isSyncEnabled());
	connect(clipRecvAction, SIGNAL(toggled(bool)), this, SLOT(toggleReceive(bool)));

	menu->addSeparator();

	menu->addAction(tr("Synchronize clipboard"), this, SLOT(synchronizeClipboard()));

	menuSeparator = menu->addSeparator();

	menu->addAction(tr("&Settings"), this, SLOT(showSettings()));
	menu->addAction(tr("&About..."), this, SLOT(showAbout()));
	menu->addAction(tr("&Quit"), qApp, SLOT(quit()));

	trayIcon->setContextMenu(menu);
	trayIcon->show();

	qApp->setQuitOnLastWindowClosed(false);
	qApp->setWindowIcon(QIcon(":/gfx/HaveClip_128.png"));

	manager->start();
}

HaveClip::~HaveClip()
{
	delete menu;
	delete historyMenu;
}

void HaveClip::updateHistory()
{
	updateHistoryContextMenu();
	updateToolTip();
}

void HaveClip::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	if(reason != QSystemTrayIcon::Trigger)
		return;

	historyMenu->exec(trayIcon->geometry().bottomLeft());
}

void HaveClip::toggleSharedClipboard(bool enabled)
{
	clipSndAction->setEnabled(enabled);
	clipRecvAction->setEnabled(enabled);

	Settings::get()->setSyncEnabled(enabled);
}

void HaveClip::toggleSend(bool enabled)
{
	Settings::get()->setSendEnabled(enabled);
}

void HaveClip::toggleReceive(bool enabled)
{
	Settings::get()->setRecvEnabled(enabled);
}

void HaveClip::updateHistoryContextMenu()
{
	QHashIterator<QAction*, ClipboardItem*> i(historyHash);

	while(i.hasNext())
	{
		i.next();

		historySignalMapper->removeMappings(i.key());
		historyMenu->removeAction(i.key());
		historyHash.remove(i.key());
		i.key()->deleteLater();
	}

	historyMenu->clear();

	if(!manager->history()->isEnabled())
		return;

	history = manager->history()->items();

	QAction *lastAction = 0;

	foreach(ClipboardContainer *cont, history)
	{
		switch(cont->type())
		{
		case ClipboardItem::BasicItem: {
			ClipboardItem *it = cont->item();
			QAction *act = new QAction(it->title, this);

			if(!it->icon.isNull())
				act->setIcon(it->icon);
			else if(it->mode == ClipboardItem::Selection)
				act->setIcon(QIcon(":/gfx/icons/selection.svg"));

			else if(it->mode == ClipboardItem::Clipboard || it->mode == ClipboardItem::ClipboardAndSelection)
				act->setIcon(QIcon(":/gfx/icons/clipboard.svg"));

			connect(act, SIGNAL(triggered()), historySignalMapper, SLOT(map()));
			historySignalMapper->setMapping(act, act);

			historyMenu->insertAction(lastAction ? lastAction : historySeparator, act);

			historyHash.insert(act, it);

			lastAction = act;

			break;
		}
		}
	}
}

void HaveClip::updateToolTip()
{
	QString tip;

#if defined Q_OS_LINUX
	tip = "<p>%1</p>";
	tip += "<pre>" + manager->history()->currentItem()->excerpt + "</pre>";
#else
	tip = "%1";
#endif

	trayIcon->setToolTip(tip.arg(tr("HaveClip")));
}

void HaveClip::onFirstStart()
{
	qDebug() << "Generating certificate on first start";

	CertificateGeneratorDialog genDlg;

	if(genDlg.exec() == QDialog::Accepted)
	{
		genDlg.savePrivateKey(Settings::get()->privateKeyPath());
		genDlg.saveCertificate(Settings::get()->certificatePath());
	}
}

void HaveClip::historyActionClicked(QObject *obj)
{
	QAction *act = static_cast<QAction*>(obj);

	if(historyHash.contains(act))
	{
		manager->jumpTo(historyHash[act]);
		updateHistoryContextMenu();
	}
}

void HaveClip::showSettings()
{
	SettingsDialog *dlg = new SettingsDialog(manager->connectionManager());

	if(dlg->exec() == QDialog::Accepted)
	{
		dlg->apply();

		Settings::get()->save();
	}

	dlg->deleteLater();
}

void HaveClip::showAbout()
{
	AboutDialog *dlg = new AboutDialog;
	dlg->exec();
	dlg->deleteLater();
}

void HaveClip::determineCertificateTrust(const Node &node, const QList<QSslError> errors)
{
	CertificateTrustDialog *dlg = new CertificateTrustDialog(node, errors);

	if(dlg->exec() == QDialog::Accepted)
	{
//		QSslCertificate cert = errors.first().certificate();
//		node->certificate = cert;

//		if(dlg->remember())
//			settings->setValue("Node:" + node->toString() + "/Certificate", QString(cert.toPem()));

		// FIXME

//		manager->distributeCurrentClipboard();
	}

	dlg->deleteLater();
}

void HaveClip::sslFatalError(const QList<QSslError> errors)
{
	QString errs;

	foreach(QSslError e, errors)
		errs += e.errorString() + "\n";

	QMessageBox::warning(0, tr("SSL fatal error"), tr("Unable to establish secure connection:\n\n") + errs);
}

void HaveClip::verificationRequest(const Node &n)
{
	qDebug() << "Verification requested" << n.name() << n.host() << n.port();

	SecurityCodePrompt *prompt = new SecurityCodePrompt(n, manager->connectionManager());

	if(prompt->exec() == QDialog::Accepted)
	{
		//
	}

	prompt->deleteLater();
}

void HaveClip::synchronizeClipboard()
{
	manager->distributeCurrentClipboard();
}
