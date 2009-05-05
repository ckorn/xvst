/*
*
* This file is part of xVideoServiceThief,
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 - 2008 Xesc & Technology
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with xVideoServiceThief. If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: Xesc <xeskuu.xvst@gmail.com>
* Program URL   : http://xviservicethief.sourceforge.net/
*
*/

#include <QtGui> //<QApplication>
//
#include "forms/mainformimpl.h"
//
#include "options.h"
#include "languages.h"
//

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

/*
#ifdef Q_OS_LINUX // modification made by "AzalSup"
	QString _homeDirectory  = getenv("HOME");
	_homeDirectory += "/.xVideoServiceThief";
	ProgramOptions *programOptions = new ProgramOptions(_homeDirectory);
#endif
#ifdef Q_WS_MAC
	QString preferencesPath = QString(QDir::homePath() + "/Library/Preferences");
	ProgramOptions *programOptions = new ProgramOptions(preferencesPath);
#endif
#ifdef Q_OS_WIN32
	QString programFiles = QString(getenv("APPDATA")) + "/xVideoServiceThief";
	// for old users, check if config.conf is present in xVST dir and copy it to new location
	if (QFile::exists(qApp->applicationDirPath() + "/config.conf") && 
		!QFile::exists(programFiles + "/config.conf"))
	{
		QDir().mkpath(programFiles);
		QFile conf(qApp->applicationDirPath() + "/config.conf");
		conf.copy(programFiles + "/config.conf");
	}
	// load options	
	ProgramOptions *programOptions = new ProgramOptions(programFiles);//qApp->applicationDirPath());
#endif

	// get language file
	programOptions->load();
	QString qm = LanguageManager::get_qm_languageFile(programOptions->getLanguageFile(true));
	delete programOptions;
*/

	// get language file
	ProgramOptions *programOptions = ProgramOptions::getProgramOptionsInstance();
	programOptions->load();
	QString qm = LanguageManager::get_qm_languageFile(programOptions->getLanguageFile(true));

	// install translator
	QTranslator translator;
	translator.load(qm);
	app.installTranslator(&translator);

	MainFormImpl win;
	win.show();
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	return app.exec();
}
//
