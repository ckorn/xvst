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
#include "forms/loadingimpl.h"
#include "forms/mainformimpl.h"
//
#include "options.h"
#include "languages.h"
#include "http.h"
#include "programversion.h"
//
#ifdef STATIC_BUILD
	#include <QtPlugin>
	Q_IMPORT_PLUGIN(qico)
	Q_IMPORT_PLUGIN(qgif)
	Q_IMPORT_PLUGIN(qjpeg)
#endif

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	// display loading dialog
	LoadingImpl::instance()->show();

	// get language file
	ProgramOptions *programOptions = ProgramOptions::instance();
	programOptions->load();
	QString qm = LanguageManager::get_qm_languageFile(programOptions->getLanguageFile(true));

	// install translator
	QTranslator translator;
	translator.load(qm);
	app.installTranslator(&translator);

	// display loading plugins
	LoadingImpl::instance()->setMessage(LoadingImpl::tr("Loading plugins... please wait..."));
	qApp->processEvents();

	// create and display main window
	MainFormImpl win;
	win.show();

	// hide loading window
	if (LoadingImpl::instance() != NULL)
	{
		LoadingImpl::instance()->setMessage(LoadingImpl::tr("Plugins loaded"));
		LoadingImpl::instance()->finished();
	}

	// set http user agent
	Http::setUserAgent("xVST-" + PROGRAM_VERSION_SHORT);

	// run program
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	return app.exec();
}
//
