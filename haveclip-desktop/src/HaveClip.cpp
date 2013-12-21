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

#ifdef INCLUDE_SERIAL_MODE
#include <qxt/QxtGui/QxtGlobalShortcut>
#endif

#include "SettingsDialog.h"
#include "AboutDialog.h"
#include "CertificateTrustDialog.h"
#include "LoginDialog.h"

#include "PasteServices/PasteDialog.h"
#include "PasteServices/Stikked/Stikked.h"
#include "PasteServices/Pastebin/Pastebin.h"

HaveClip::HaveClip(QObject *parent) :
	QObject(parent)
{
	manager = new ClipboardManager(this);
	settings = manager->settings();

	connect(manager->history(), SIGNAL(historyChanged()), this, SLOT(updateHistory()));
	connect(manager, SIGNAL(untrustedCertificateError(ClipboardManager::Node*,QList<QSslError>)), this, SLOT(determineCertificateTrust(ClipboardManager::Node*,QList<QSslError>)));
	connect(manager, SIGNAL(sslFatalError(QList<QSslError>)), this, SLOT(sslFatalError(QList<QSslError>)));

	historySignalMapper = new QSignalMapper(this);
	pasteSignalMapper = new QSignalMapper(this);
	pasteAdvSignalMapper = new QSignalMapper(this);

	connect(historySignalMapper, SIGNAL(mapped(QObject*)), this, SLOT(historyActionClicked(QObject*)));
	connect(pasteSignalMapper, SIGNAL(mapped(QObject*)), this, SLOT(simplePaste(QObject*)));
	connect(pasteAdvSignalMapper, SIGNAL(mapped(QObject*)), this, SLOT(advancedPaste(QObject*)));

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
	connect(clipSndAction, SIGNAL(toggled(bool)), manager, SLOT(toggleClipboardSending(bool)));

	clipRecvAction = menu->addAction(tr("Enable clipboard &receiving"));
	clipRecvAction->setCheckable(true);
	clipRecvAction->setChecked(manager->isReceivingEnabled());
	clipRecvAction->setEnabled(manager->isSyncEnabled());
	connect(clipRecvAction, SIGNAL(toggled(bool)), manager, SLOT(toggleClipboardReceiving(bool)));

#ifdef INCLUDE_SERIAL_MODE
	menu->addSeparator();
	serialModeAction = menu->addAction(tr("Begin serial mode"), this, SLOT(userToggleSerialMode()), QKeySequence("Ctrl+Alt+S"));

	connect(manager, SIGNAL(serialModeChanged(bool)), this, SLOT(toggleSerialMode(bool)));

	QxtGlobalShortcut *shortcut = new QxtGlobalShortcut(this);
	connect(shortcut, SIGNAL(activated()), this, SLOT(userToggleSerialModeShortcut()));
	shortcut->setShortcut(QKeySequence("Ctrl+Alt+S"));

	serialRestartMapper = new QSignalMapper(this);
	connect(serialRestartMapper, SIGNAL(mapped(int)), this, SLOT(restartSerialBatch(int)));

#endif // INCLUDE_SERIAL_MODE

	menu->addSeparator();

	menuSeparator = menu->addSeparator();

	loadPasteServices();

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

	manager->toggleSharedClipboard(enabled);
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
#ifdef INCLUDE_SERIAL_MODE
		case ClipboardItem::SerialBatch: {
			QMenu *menu = new QMenu(cont->title, historyMenu);

			foreach(ClipboardItem *child, cont->items())
			{
				QAction *act = menu->addAction(child->title);

				if(!child->icon.isNull())
					act->setIcon(child->icon);
				else if(child->mode == ClipboardItem::Selection)
					act->setIcon(QIcon(":/gfx/icons/selection.svg"));

				else if(child->mode == ClipboardItem::Clipboard || child->mode == ClipboardItem::ClipboardAndSelection)
					act->setIcon(QIcon(":/gfx/icons/clipboard.svg"));

				connect(act, SIGNAL(triggered()), historySignalMapper, SLOT(map()));
				historySignalMapper->setMapping(act, act);

				historyHash.insert(act, child);
			}

			menu->addSeparator();
			QAction *restart = menu->addAction(tr("Restart"));

			connect(restart, SIGNAL(triggered()), serialRestartMapper, SLOT(map()));
			serialRestartMapper->setMapping(restart, *((int*) &cont));

			lastAction = historyMenu->insertMenu(lastAction ? lastAction : historySeparator, menu);

			continue;
		}
#endif // INCLUDE_SERIAL_MODE

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

void HaveClip::historyActionClicked(QObject *obj)
{
	QAction *act = static_cast<QAction*>(obj);

	if(historyHash.contains(act))
	{
		manager->jumpTo(historyHash[act]);
		updateHistoryContextMenu();
	}
}

#ifdef INCLUDE_SERIAL_MODE
void HaveClip::userToggleSerialMode()
{
	manager->toggleSerialMode();

	toggleSerialMode(manager->isSerialModeEnabled());
}

void HaveClip::userToggleSerialModeShortcut()
{
	userToggleSerialMode();

	QString msg;

	if(manager->isSerialModeEnabled())
		msg = tr("Serial mode enabled.");

	else
		msg = tr("Serial mode disabled.");

	trayIcon->showMessage(tr("Serial mode"), msg, QSystemTrayIcon::Information, 4000);
}

void HaveClip::toggleSerialMode(bool enabled)
{
	if(enabled)
		serialModeAction->setText(tr("End serial mode"));

	else
		serialModeAction->setText(tr("Begin serial mode"));
}

void HaveClip::restartSerialBatch(int batch)
{
	manager->serialModeRestart((ClipboardContainer*) batch);
}

#endif // INCLUDE_SERIAL_MODE

void HaveClip::showSettings()
{
	SettingsDialog *dlg = new SettingsDialog(settings);

	if(dlg->exec() == QDialog::Accepted)
	{
		History *h = manager->history();

		h->setEnabled(dlg->historyEnabled());
		h->setStackSize(dlg->historySize());
		h->setSave(dlg->saveHistory());

		manager->setSelectionMode(dlg->selectionMode());
		manager->setSyncMode(dlg->synchronizationMode());

		manager->setNodes(dlg->nodes());

		manager->setListenHost(dlg->host(), dlg->port());
		manager->setEncryption(dlg->encryption());
		manager->setCertificate(dlg->certificate());
		manager->setPrivateKey(dlg->privateKey());

		manager->setPassword(dlg->password());

		manager->setPasteServices(dlg->pasteServices());

		manager->saveSettings();

		clearPasteServices();
		loadPasteServices();
	}

	dlg->deleteLater();
}

void HaveClip::showAbout()
{
	AboutDialog *dlg = new AboutDialog;
	dlg->exec();
	dlg->deleteLater();
}

void HaveClip::determineCertificateTrust(ClipboardManager::Node *node, const QList<QSslError> errors)
{
	CertificateTrustDialog *dlg = new CertificateTrustDialog(node, errors);

	if(dlg->exec() == QDialog::Accepted)
	{
		QSslCertificate cert = errors.first().certificate();
		node->certificate = cert;

		if(dlg->remember())
			settings->setValue("Node:" + node->toString() + "/Certificate", QString(cert.toPem()));

		manager->distributeCurrentClipboard();
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

void HaveClip::loadPasteServices()
{
	QList<BasePasteService*> services = manager->pasteServices();

	foreach(BasePasteService *s, services)
	{

		// FIXME: those should be connected in CLipboardManager
		connect(s, SIGNAL(authenticationRequired(BasePasteService*,QString,bool,QString)), this, SLOT(pasteServiceRequiresAuthentication(BasePasteService*,QString,bool,QString)));
		connect(s, SIGNAL(errorOccured(QString)), this, SLOT(pasteServiceError(QString)));
		connect(s, SIGNAL(untrustedCertificateError(BasePasteService*,QList<QSslError>)), this, SLOT(determineCertificateTrust(BasePasteService*,QList<QSslError>)));

		// Simple paste
		QAction *a = new QAction(tr("Paste to %1").arg(s->label()), this);

		pasteSignalMapper->setMapping(a, s);
		connect(a, SIGNAL(triggered()), pasteSignalMapper, SLOT(map()));

		menu->insertAction(menuSeparator, a);
		pasteActions << a;

		// Advanced paste
		a = new QAction(tr("Advanced paste to %1").arg(s->label()), this);

		pasteAdvSignalMapper->setMapping(a, s);
		connect(a, SIGNAL(triggered()), pasteAdvSignalMapper, SLOT(map()));

		menu->insertAction(menuSeparator, a);
		pasteActions << a;

		a = new QAction(this);
		a->setSeparator(true);
		menu->insertAction(menuSeparator, a);
		pasteActions << a;
	}
}

void HaveClip::clearPasteServices()
{
	foreach(QAction *a, pasteActions)
	{
		menu->removeAction(a);
		pasteSignalMapper->removeMappings(a);
		pasteAdvSignalMapper->removeMappings(a);
		a->deleteLater();
	}

	pasteActions.clear();
}

void HaveClip::simplePaste(QObject *obj)
{
	BasePasteService *service = static_cast<BasePasteService*>(obj);

	switch(service->type())
	{
	case BasePasteService::Stikked:

		break;
	}

	service->paste(manager->history()->currentItem()->toPlainText());
}

void HaveClip::advancedPaste(QObject *obj)
{
	BasePasteService *service = static_cast<BasePasteService*>(obj);

	PasteDialog *dlg = new PasteDialog(manager->history()->currentItem()->mimeData()->text(), service);

	if(dlg->exec() == QDialog::Accepted)
	{
		service->paste(dlg->pasteServiceSettings(), dlg->dataToPaste());
	}

	dlg->deleteLater();
}

void HaveClip::pasteServiceRequiresAuthentication(BasePasteService *service, QString username, bool failed, QString msg)
{
	LoginDialog *dlg = new LoginDialog(username);

	if(failed)
		dlg->setError(tr("Login failed: %1").arg(msg));

	if(dlg->exec() == QDialog::Accepted)
	{
		service->provideAuthentication(dlg->username(), dlg->password());
	}

	dlg->deleteLater();
}

void HaveClip::pasteServiceError(QString error)
{
	QMessageBox::warning(0, tr("Unable to paste"), tr("Paste failed.\n\nError occured: %1").arg(error));
}

void HaveClip::determineCertificateTrust(BasePasteService *service, const QList<QSslError> errors)
{
	CertificateTrustDialog *dlg = new CertificateTrustDialog(service, errors);

	if(dlg->exec() == QDialog::Accepted)
	{
		service->setCertificate(errors.first().certificate());

		manager->saveSettings(); // FIXME: it'd be enough to save just services

		service->retryPaste();
	}

	dlg->deleteLater();
}
