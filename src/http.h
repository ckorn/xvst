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

enum Error
{
	_UNABLE_CREATE_DIR = 20,	//20
	_UNABLE_CREATE_FILE,		//21
	_INVALID_URL,				//22
	_ALREADY_DOWNLOADING,		//23
	_INVALID_FILE_SIZE,			//24
	_MISSING_RESUME_FILE,		//25
	_UNABLE_RESUME_DOWNLOAD,	//26
	_UNABLE_APPEND_FILE			//27
};

enum StopReason
{
	NO_STOPPED = 100,			//100
	DOWNLOAD_FINISHED,			//101
	USER_CANCELLED,				//102
	USER_PAUSED,				//103
	TIME_OUT,					//104
	MAX_AUTO_JUMPS_REACHED		//105
};

class ArrayAvg : public QObject
{
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
	private:
		QString cookieInf;		//!< Cookie original information
		QString cookieBoddy;	//!< Cookie boddy
		QString expires;		//!< Cookie expiration date
		QString domain;			//!< Cookie working domain
		QString path;			//!< Cookie working path
	public:
		/*! Class constructor */
		Cookie(QString cookieInf);
		/*! Copy class constructor */
		Cookie(const Cookie&);
		/*! Get the original cookie */
		QString getCookieInf();
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
	private:
		QList<Cookie*> *cookies;
	public:
		/*! Class constructor */
		CookieController();
		/* Copy class constructor */
		CookieController(const CookieController&);
		/*! Class dsetructor */
		~CookieController();
		/*! Add a new cookie */
		void addCookie(QString cookie);
		/*! Copy all cookies from another CookieController */
		void copyCookies(const CookieController&);
		/*! Clear all stored cookies */
		void clear();
		/*! Get host cookies */
		QString getCookies(QUrl URL);
		/*! Get all cookies (if complete = true, return all information: host, expires...) */
		QString getCookies(bool complete, const QString separator = "|");
};

enum HttpMethod
{
	httpGet,
	httpPost,
	httpHead
};

static QString HTTP_GLOBAL_USER_AGENT = "";

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
		HttpMethod httpMethod;		//!< http request method: get, post or head
		QString data;				//!< internal downloaded data
		QString parameters;			//!< internal post parameters
		int maxRetries;				//!< maximum retries for session (each download is a session)
		int retriesCount;			//!< current retries in this session (each download is a session)
		int maxAutoJumps;			//!< maximum automatic jumps
		int autoJumps;				//!< current automatic jumps realized
		bool startedDownload;		//!< flag for know if is downloading (or just redirecting...
		QTimer tmrTimeOut;			//!< time out timer
		int timeOutIntervalCehck;	//!< time out interval checker
		int timeOut;				//!< when the connection is considered "time out" in miliseconds (we will ot wait infinite...)
		QStringList *customHeaders;	//!< list with custom header parameters
		bool skipExistentFiles;		//!< Skip files which are already present (downloaded)
		QString userAgent;			//!< "user agent" used for this http instance
		/*! Init Http class */
		void initClass(bool useInternalTimer = true);
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
		/*! copy class constructor */
		Http(const Http&);
		/*! class destructor */
		~Http();
		/*! Override = operator */
		Http &operator=(const Http &);
		/*! Start a new asynchronously download */
		int download(const QUrl URL, QString destination, QString fileName = "", bool autoName = true);
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
		QHttpResponseHeader head(const QUrl URL);
		/*! Add custom cookie */
		void addCookie(QString cookie);
		/*! Add custom cookies */
		void addCookies(QString cookies, const QString separator = "|");
		/*! Get all stored cookies as String */
		QString getCookies(const QString separator = "|");
		/*! Clear the stored cookies */
		void clearCookies();
		/*! Add custom header parameter */
		void addHeaderParameter(QString key, QString value);
		/*! Add custom header parameters */
		void addHeaderParameters(QString headers, const QString separator = "|");
		/*! Clear custom header parameters */
		void clearHeaderParameters();
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
		/*! Set the max automatic jumps */
		void setMaxAutoJumps(int value);
		/*! Set if should skip existent files (avoid download duplicated data) */
		void setSkipExistentFiles(bool value);
		/*! Get the last error generated (on start a new download, this value is reset) */
		int getLastError();
		/*! Get the last stop reason (on start a new download, this value is reset) */
		int getLastStopReason();
		/*! Set the user agent for this http instance (this override the global user agent) */
		void setUserAgent(QString value);
		/*! Set the global (default) HTTP user agent */
		static void setGlobalUserAgent(QString value);
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
