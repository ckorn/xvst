#ifndef DOWNLOADLOGIMPL_H
#define DOWNLOADLOGIMPL_H
//
#include <QtGui>
//
#include "ui_downloadlog.h"
//
#include "../sessionmanager.h"
#include "../videoinformation.h"
//
class DownloadLogImpl : public QDialog, public Ui::DownloadLog
{
Q_OBJECT
	public:
		DownloadLogImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
		/*! Display the downloads log */
		void buildLog(QList<LogEntry> logEntries, VideoInformation *videoInformation);
};
#endif





