/*
*
* This file is part of xVideoServiceThief, 
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 Xesc & Technology
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Xesc <xeskuu@gmail.com>
* Program URL   : http://xviservicethief.sourceforge.net/
*
*/

#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H
//
#include <QtGui>
//
#include "options.h"
#include "videoitem.h"
#include "videolistcontroller.h"
//

static const QString SESSION_FILE = "/session.conf";
static const QString LOG_FILE = "/historic.log";

struct LogEntry
{
	QDateTime dateTime;	//!< date & time
	QString title;		//!< video title
	QString URL;		//!< video URL
};

class SessionManager : public QObject
{
Q_OBJECT
	private:
		ProgramOptions *programOptions;	//!< program options
	public:
		/*! Class constructor */
		SessionManager(ProgramOptions *programOptions);
		/*! Save the current session */
		void saveSession(VideoListController *videoListController);
		/*! Restore the last session */
		void loadSession(VideoListController *videoListController);
		/*! Add new entry to the log */
		void addToLog(VideoItem *videoItem);
		/*! Clear the current historic log */
		void clearLog();
		/*! Get all log entries */
		QList<LogEntry> getLogEntries();
};
#endif
