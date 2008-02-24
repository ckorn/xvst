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

#ifndef __HTTP_H__
#define __HTTP_H__

#include <QtGui>
#include <QtNetwork>
//
#include "tools.h"

enum Error
{
	UNABLE_CREATE_DIR	= 20,	//20
	UNABLE_CREATE_FILE,			//21
	INVALID_URL,				//22
	ALREADY_DOWNLOADING,		//23
	INVALID_FILE_SIZE			//24
};

class ArrayAvg : public QObject
{
Q_OBJECT
	private:
		QList<float> arrayAvg;	//!< array of floats
		int maxItems;			//!< max possible values
	public:
		/*! Class constructor */
		ArrayAvg(const int maxItems = 10);
		/*! Class dsetructor */
		~ArrayAvg();
		/*! Add new value */
		float add(const float value);
		/*! Get array avarage */
		float avg();
		/*! Reset array */
		void reset();
};

class Http : public QObject
{
Q_OBJECT
	private:
		QHttp *http;				//!< http protocol
		ArrayAvg *downloadSpeedAvg;	//!< download speed avg calculator
		QFile *file;				//!< destination file
		int httpGetId;				//!< current download id
		bool userAborted;			//!< flag for know if the user aborted
		bool useInternalTimer;		//!< can use an internal timer?
		int downloadSpeed;	//!< download speed in bytes
		int timeRemaining;	//!< time remaining in seconds
		int fileSize;		//!< total file size
		int internalTimer;	//!< internal timer id
		int totalDownload;	//!< virtual total to download
		int currDownload;	//!< current downloaded bytes
		int prevDownload;	//!< previous downloaded bytes
		QFileInfo destFile;	//!< destination file information
		bool canDownload;	//!< response result is 200 (ok)
		QString data;		//!< internal downloaded data
		QString parameters;	//!< internal post parameters
		bool autoJump;		//!< should jump to next url? only for head requests
		bool syncFlag;		//!< sync. flag
		bool postMethodFlag;//!< post method flag
		bool notLength;		//!< Content length found?
		QStringList cookies;//!< Stored cookies
		QUrl oriURL;		//!< First url
		/*! Get if response is object moved */
		bool isObjectMoved(int statusCode);
		/*! Init the internal http data */
		void initData();
		/*! Jump to url */
		void jumpToURL(QUrl url);
		/*! Start timer */
		void initTimer();
		/*! Stop timer */
		void deinitTimer();
	protected:
		/*! internal timer event */
		void timerEvent(QTimerEvent *event);
	public:
		/*! class constructor */
		Http(bool useInternalTimer = true);
		/*! class destructor */
		~Http();
		/*! Start a new asynchronously download */
		int download(const QUrl URL, const QDir destination, QString fileName = "");
		/*! Download a Webpage synchronously (return the webpage content) */
		QString downloadWebpage(const QUrl URL, bool isUtf8 = true);
		/*! Download a Webpage synchronously: post mode (return the webpage content) */
		QString downloadWebpagePost(const QUrl URL, QString parameters, bool isUtf8 = true);
		/*! Get only the response header */
		QHttpResponseHeader head(const QUrl URL, bool autoJump = false);
		/*! Cancel current download */
		void cancel();
		/*! Get file size */
		int getFileSize();
		/*! Get download speed */
		int getDownloadSpeed();
		/*! Get time remaining */
		int getTimeRemaining();
		/*! Get the destination file name */
		QFileInfo getDestiationFile();
		/*! Get if is downloading */
		bool isDownloading();
	private slots:
		/*! when the http protocol read data */
		void dataReadProgress(int done, int total);
		/*! the http finished a request */
		void requestFinished(int id, bool error);
		/*! when the http received the response header */
		void responseHeaderReceived(const QHttpResponseHeader &resp);
		/*! current http state changed */
		void stateChanged(int state);
	signals:
		/*! when a download started */
		void downloadStarted();
		/*! when a download finished */
		void downloadFinished(const QFileInfo destFile);
		/*! when a download file has been canceled */
		void downloadCanceled();
		/*! an error ocurred during the download process */
		void downloadError(int error);
		/*! when the http read data */
		void downloadEvent(int pos, int max);
};

#endif // __HTTP_H__
