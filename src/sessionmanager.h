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
