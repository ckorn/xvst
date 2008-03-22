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
	INVALID_FILE_SIZE,			//24
	MISSING_RESUME_FILE,		//25
	UNABLE_RESUME_DOWNLOAD,		//26
	UNABLE_APPEND_FILE			//27
};

enum StopReason
{
	NO_STOPPED,
	DOWNLOAD_FINISHED,
	USER_CANCELLED,
	USER_PAUSED,
	TIME_OUT
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

class Cookie : public QObject
{
Q_OBJECT
	private:
		QString cookieBoddy;	//!< Cookie boddy
		QString expires;		//!< Cookie expiration date
		QString domain;			//!< Cookie working domain
		QString path;			//!< Cookie working path
	public:
		/*! Class constructor */
		Cookie(QString cookieInf);
		/*! Get the cookie boddy */
		QString getCookieBoddy();
		/*! Get the cookie expiration date */
		QString getExpires();
		/*! Get the working domain */
		QString getDomain();
		/*! Get the cookie working path */
		QString getPath();
};

class CookieController : public QObject
{
Q_OBJECT
	private:
		QList<Cookie*> *cookies;
	public:
		/*! Class constructor */
		CookieController();
		/*! Class dsetructor */
		~CookieController();
		/*! Add a new cookie */
		void addCookie(QString cookie);
		/*! Clear all stored cookies */
		void clear();
		/*! Get host cookies */
		QString getCookies(QUrl URL);
};

class Http : public QObject
{
Q_OBJECT
	private:
		QHttp *http;				//!< http protocol
		ArrayAvg *downloadSpeedAvg;	//!< download speed avg calculator
		CookieController *cookies;	//!< cookies manager (controller)
		int internalTimer;			//!< internal timer id
		bool useInternalTimer;		//!< flag for know if is being used an internal timer
		QFile *file;				//!< destination file
		int httpGetId;				//!< current download id
		StopReason stopReason;		//!< flag for know if the user aborted
		bool pauseOnDestroyF;		//!< should pause the download instead of cancel it?
		int timeRemaining;			//!< time remaining in seconds
		int downloadSpeed;			//!< download speed in bytes
		int totalDownload;			//!< virtual total to download
		int currDownload;			//!< current downloaded bytes
		int prevDownload;			//!< previous downloaded bytes
		int realStartSize;			//!< real dwonloaded data (important for resuming downloads)
		int realTotalSize;			//!< real file size (important for resuming downloads)
		QFileInfo destFile;			//!< destination file information
		QUrl oriURL;				//!< First url (original)
		int fileSize;				//!< total file size
		bool notLength;				//!< Content length found?
		bool autoJump;				//!< should jump to next url? only for head requests
		bool resuming;				//!< flag for know if is being resumed
		bool autoRestartOnFail;		//!< restart the download on fail?
		bool restartDownload;		//!< flog for know if should restart again
		bool syncFlag;				//!< sync. flag
		bool postMethodFlag;		//!< post method flag
		QString data;				//!< internal downloaded data
		QString parameters;			//!< internal post parameters
		int maxRetries;				//!< maximum retries for session (each download is a session)
		int retriesCount;			//!< current retries in this session (each download is a session)
		bool startedDownload;		//!< flag for know if is downloading (or just redirecting...
		int stepID;					//!< this number is to know if we are doing the same work (usefull for the time out)
		int launchedStepID;			//!< the launche step id (used for compare with stepID)
		int timeOut;				//!< when the connection is considered "time out" in miliseconds (we will ot wait infinite...)
		/*! Init the internal http data */
		void initData();
		/*! Init retries data */
		void initRetriesData();
		/*! Start timer */
		void initTimer();
		/*! Stop timer */
		void deinitTimer();
		/*! Get if response is object moved */
		bool isObjectMoved(int statusCode);
		/*! Jump to url */
		void jumpToURL(QUrl url);
	protected:
		/*! internal timer event */
		void timerEvent(QTimerEvent *event);
	public:
		/*! class constructor */
		Http(bool useInternalTimer = true);
		/*! class destructor */
		~Http();
		/*! Start a new asynchronously download */
		int download(const QUrl URL, const QDir destination, QString fileName = "", bool autoName = true);
		/*! Resume a previous asynchronously download */
		int resume(const QUrl URL, QString fileName, bool autoRestartOnFail = true);
		/*! Pause the current asynchronously download */
		void pause();
		/*! Cancel current download */
		void cancel();
		/*! Download a Webpage synchronously (return the webpage content) */
		QString downloadWebpage(const QUrl URL, bool isUtf8 = true);
		/*! Download a Webpage synchronously: post mode (return the webpage content) */
		QString downloadWebpagePost(const QUrl URL, QString parameters, bool isUtf8 = true);
		/*! Get only the response header */
		QHttpResponseHeader head(const QUrl URL, bool autoJump = false);
		/*! Add custom cookie */
		void addCookie(QString cookie);
		/*! Clear the stored cookies */
		void clearCookies();
		/*! Pause on destroy the Http class (only if is downloading) */
		void pauseOnDestroy(bool pauseOnDestroyF = true);
		/*! Get if is downloading */
		bool isDownloading();
		/*! Get file size */
		int getFileSize();
		/*! Get download speed */
		int getDownloadSpeed();
		/*! Get time remaining */
		int getTimeRemaining();
		/*! Get the destination file name */
		QFileInfo getDestiationFile();
		/*! Set the max retries */
		void setMaxRetries(int value);
		/*! Set the time out in seconds */
		void setTimeOut(int value);
	private slots:
		/*! when the http protocol read data */
		void dataReadProgress(int done, int total);
		/*! the http finished a request */
		void requestFinished(int id, bool error);
		/*! when the http received the response header */
		void responseHeaderReceived(const QHttpResponseHeader &resp);
		/*! current http state changed */
		void stateChanged(int state);
		/*! restart the download signal */
		void restartDownloadSignal();
		/*! time out checker signal */
		void timeOutCheckout();
	signals:
		/*! when a download started */
		void downloadStarted();
		/*! when a download finished */
		void downloadFinished(const QFileInfo destFile);
		/*! when a download has been paused */
		void downloadPaused(const QFileInfo destFile);
		/*! when a download has been resumed */
		void downloadResumed();
		/*! when a download file has been canceled */
		void downloadCanceled();
		/*! an error ocurred during the download process */
		void downloadError(int error);
		/*! when the http read data */
		void downloadEvent(int pos, int max);
};

#endif // __HTTP_H__