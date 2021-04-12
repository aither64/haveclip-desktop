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

#include <QApplication>
#include <QTextCodec>
#include <QLocalSocket>

#ifdef Q_OS_LINUX
#include <signal.h>
#endif

#include "HaveClip.h"
#include "../haveclip-core/src/Cli.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("HaveFun.cz");
	QCoreApplication::setOrganizationDomain("havefun.cz");
	QCoreApplication::setApplicationName("HaveClip");

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
#endif

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

#ifdef Q_OS_MAC
	// Prevent storage of the client cert/key in the "Login" keychain.
	// https://doc.qt.io/qt-5.12/qsslsocket.html#setLocalCertificate
	qputenv("QT_SSL_USE_TEMPORARY_KEYCHAIN", QByteArray::number(1));
#endif

	QApplication a(argc, argv);

    if (Cli::remoteConnect())
    {
        if (argc > 1)
            return Cli::exec();

        // FIXME: report that haveclip is already running?
        return 0;

    } else if (argc > 1) {
        // Has arguments but GUI isn't running.
        return 1;
    }

#ifdef Q_OS_LINUX
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = ClipboardManager::gracefullyExit;
	sigaction(SIGTERM, &sa, NULL);
#endif

	HaveClip hc;
	
	return a.exec();
}
