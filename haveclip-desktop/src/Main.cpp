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

#include <QApplication>
#include <QTextCodec>

#ifdef Q_OS_LINUX
#include <signal.h>
#endif

#include "HaveClip.h"

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

	QApplication a(argc, argv);

#ifdef Q_OS_LINUX
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = ClipboardManager::gracefullyExit;
	sigaction(SIGTERM, &sa, NULL);
#endif

	HaveClip hc;
	
	return a.exec();
}
