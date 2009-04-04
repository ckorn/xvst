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

#include "http.h"

// ArrayAvg class

ArrayAvg::ArrayAvg(const int maxItems)
{
	this->maxItems = maxItems;
	reset();
}

ArrayAvg::~ArrayAvg()
{
	arrayAvg.clear();
}

float ArrayAvg::add(const float value)
{
	if (arrayAvg.count() >= maxItems)
		arrayAvg.removeAt(0);

	arrayAvg.append(value);

	return avg();
}

float ArrayAvg::avg()
{
	float total = 0.0;

	for (int n = 0; n < arrayAvg.count(); n++)
		total += arrayAvg.at(n);

	if (arrayAvg.count() > 0)
		return total / arrayAvg.count();
	else
		return 0.0;
}

void ArrayAvg::reset()
{
	arrayAvg.clear();
}

// Cookie class

Cookie::Cookie(QString cookieInf)
{
	// parese the cookie info
	cookieInf += ";";
	// the first token is the cookie boddy
	cookieBoddy = getToken(cookieInf, ";", 0);
	// get the expiration date
	expires = copyBetween(cookieInf, "expires=", ";");
	// get the domain
	domain = copyBetween(cookieInf, "domain=", ";");
	// get the path
	path = copyBetween(cookieInf, "path=", ";");
}

Cookie::Cookie(const Cookie &other): QObject()
{
	// copy internal cookie data from other cookie
	cookieBoddy = other.cookieBoddy;
	expires = other.expires;
	domain = other.domain;
	path = other.path;
}

QString Cookie::getCookieBoddy()
{
	return cookieBoddy;
}

QString Cookie::getExpires()
{
	return expires;
}

QString Cookie::getDomain()
{
	return domain;
}

QString Cookie::getPath()
{
	return path;
}

// Cookie controller class

CookieController::CookieController()
{
	cookies = new QList<Cookie*>;
}

CookieController::CookieController(const CookieController &other): QObject()
{
	cookies = new QList<Cookie*>;
	// copy all cookies from another cookie controller
	copyCookies(other);
}

CookieController::~CookieController()
{
	clear();
	delete cookies;
}

void CookieController::addCookie(QString cookie)
{
	cookies->append(new Cookie(cookie));
}

void CookieController::copyCookies(const CookieController &other)
{
	for (int n = 0; n < cookies->count(); n++)
	{
		Cookie *cookie = other.cookies->at(n);
		cookies->append(new Cookie(*cookie));
	}
}

void CookieController::clear()
{
	while (!cookies->isEmpty())
		delete cookies->takeFirst();
}

QString CookieController::getCookies(QUrl URL)
{
	QString result = "";

	// get cookies
	for (int n = 0; n < cookies->count(); n++)
	{
		Cookie *cookie = cookies->at(n);
		QString host = URL.host();
		if (host.indexOf("www.") == 0) 
			host = host.remove(0, 3);
		else
			if (host.at(host.length() - 1) != '.')
				host = "." + host;

		// is a valid cookie?
		if (host.indexOf(cookie->getDomain()) != -1)
			if (cookie->getPath() != "/")
			{
				if (cookie->getPath() == URL.path())
					result += cookie->getCookieBoddy() + ";";
			}
			else
				result += cookie->getCookieBoddy() + ";";
	}
	// return cookies assigned to this URL
	return result;
}

// Http class

Http::Http(bool useInternalTimer)
{
	initClass(useInternalTimer);
}

Http::Http(const Http &other): QObject()
{
	initClass();
	// init internal timer
	useInternalTimer = other.useInternalTimer;
	// default max retries
	maxRetries = other.maxRetries;
	timeOut = other.timeOut;
	// copy destination file
	if (other.file != NULL)
	{
		if (file != NULL) delete file;
		file = new QFile(other.file->fileName());
	}
	// cancel or pause on finish?
	pauseOnDestroyF = other.pauseOnDestroyF;
	// copy stored cookies
	cookies->copyCookies(*other.cookies);
	// copy custom header parameters
	delete customHeaders;
	customHeaders = new QStringList(*other.customHeaders);
}

Http::~Http()
{
	if (pauseOnDestroyF) pause(); else cancel();

	delete customHeaders;
	delete downloadSpeedAvg;
	delete cookies;
	delete http;
}

Http &Http::operator=(const Http &other)
{
	// init internal timer
	useInternalTimer = other.useInternalTimer;
	// default max retries
	maxRetries = other.maxRetries;
	timeOut = other.timeOut;
	// copy destination file
	if (other.file != NULL)
	{
		if (file != NULL) delete file;
		file = new QFile(other.file->fileName());
	}
	// cancel or pause on finish?
	pauseOnDestroyF = other.pauseOnDestroyF;
	// copy stored cookies
	cookies->copyCookies(*other.cookies);
	// copy custom header parameters
	delete customHeaders;
	customHeaders = new QStringList(*other.customHeaders);
	// return my self updated
	return *this;
}

void Http::initClass(bool useInternalTimer)
{
	setObjectName("Http");
	// http protocol
	http = new QHttp(this);
	// cookies controller
	cookies = new CookieController;
	// download speed avg calculator
	downloadSpeedAvg = new ArrayAvg(100);
	// init internal timer
	internalTimer = 0;
	this->useInternalTimer = useInternalTimer;
	// default max retries
	maxRetries = 1;//5;
	initRetriesData();
	setTimeOut(20);	// 8 seconds
	// destination file
	file = NULL;
	// cancel or pause on finish?
	pauseOnDestroy(false);
	// init custom header parameters
	customHeaders = new QStringList;
	// init data
	initData();
	// connect signals
	connect(http, SIGNAL(dataReadProgress(int, int)), this, SLOT(dataReadProgress(int, int)));
	connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)));
	connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this,
			SLOT(responseHeaderReceived(const QHttpResponseHeader &)));
	connect(http, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
	connect(&tmrTimeOut, SIGNAL(timeout()), this, SLOT(timeOutCheckout()));
}

void Http::initData()
{
	stopReason = NO_STOPPED;

	timeRemaining = 0;
	downloadSpeed = 0;

	totalDownload = 0;
	currDownload = 0;
	prevDownload = 0;

	realStartSize = 0;
	realTotalSize = 0;

	fileSize = 0;
	notLength = false;

	oriURL.clear();

	autoJump = true;
	resuming = false;
	autoRestartOnFail = false;
	restartDownload = false;

	postMethodFlag = false;
	syncFlag = false;
	data = "";
	parameters = "";

	startedDownload = false;
}

void Http::initRetriesData()
{
	retriesCount = 0;
}

void Http::initTimer()
{
	if (useInternalTimer)
	{
		// start internal timer
		if (internalTimer != 0) 
			this->killTimer(internalTimer);

		internalTimer = this->startTimer(1000);
	}
}

void Http::deinitTimer()
{
	if (useInternalTimer && internalTimer != 0)
	{
		// terminate internal timer
		this->killTimer(internalTimer);
		internalTimer = 0;
	}
}

bool Http::isObjectMoved(int statusCode)
{
	return statusCode == 301 || statusCode == 302 ||
	       statusCode == 303 || statusCode == 307;
}

void Http::jumpToURL(QUrl url)
{	
	// check if the current url has a host (if not, assign the original url host)
	if (url.host().isEmpty())
	{
		QString tmpUrl = url.toString();
		if (!tmpUrl.isEmpty()) if (tmpUrl.at(0) != '/') tmpUrl = "/" + tmpUrl;
		// new url
		url = QUrl("http://" + oriURL.host() + tmpUrl);
	}

	// http mode
	QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
	
	// set http host
	http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
	
	// set http user and password (only if is requiered)
	if (!url.userName().isEmpty())
		http->setUser(url.userName(), url.password());

	// set the header method: POST or GET
	QString headerMethod = "GET";
	if (postMethodFlag) headerMethod = "POST";

	// build the request header
	QHttpRequestHeader header(headerMethod, getPathAndQuery(url));

	// set the host
	header.setValue("Host", url.host());

	// prepare parameters (they will only used for "POST" method)
	QByteArray paramsStr;

	// if is a "POST" method then, add some extra info
	if (postMethodFlag)
	{
		// add the parametes
		paramsStr.append(parameters);

		// set the content type and length
		header.setContentType("application/x-www-form-urlencoded"); // ** important **
		header.setContentLength(paramsStr.length());
	}

	// set connection: "keep alive"
	header.setValue("Connection", "Keep-Alive");

	// set Accept-Ranges: "bytes" (to indicate its acceptance of range requests for a resource)
	header.setValue("Accept-Ranges", "bytes");

	// add cookies
	QString cookiesToAdd = cookies->getCookies(url);
	if (!cookiesToAdd.isEmpty())
	{
		header.setValue("Cookie", cookiesToAdd);
		header.setValue("Cookie2", "$Version=1");
	}

	// if we are resuming a download...
	if (resuming && file != NULL)
		header.setValue("Range", QString("bytes=%1-").arg(file->size()));

	// add custom header parameters
	for (int n = 0; n < customHeaders->count(); n++)
	{
		QString key = getToken(customHeaders->at(n), "|", 0);
		QString value = getToken(customHeaders->at(n), "|", 1);
		// add it
		header.setValue(key, value);
	}

	// stop timer (no time out)
	tmrTimeOut.stop();

	// send the request header
	httpGetId = http->request(header, paramsStr, file);

	// time out controller
	// prepare the timer for possible timeout
	tmrTimeOut.start(timeOut);

	// post method off
	postMethodFlag = false;
}

int Http::download(const QUrl URL, QString destination, QString fileName, bool autoName)
{
	// clean destination dir
	destination = QDir::cleanPath(destination);

#ifdef Q_WS_WIN
	// fix Qt4.4.0 bug in windows
	if (destination.indexOf(":/") == -1) 
		destination.replace(":", ":/");
#endif

	// check if is already downloading another file
	if (isDownloading()) 
		return ALREADY_DOWNLOADING;

	// check if is a valid URL
	if (!validURL(URL.toString())) 
		return INVALID_URL;

	// create the destination path, if it don't exists
	if (!QDir(destination).exists())
		if (!QDir(destination).mkpath(destination))
			return UNABLE_CREATE_DIR;

	// set a default name (only if it is empty)
	if (fileName.isEmpty())
		fileName = "download.file";

	fileName = QFileInfo(fileName).fileName();

	// set destination file name
	fileName = cleanFileName(fileName);

	// get an unique file name for this download
	if (autoName) 
		fileName = uniqueFileName(destination + "/" + fileName);
	else
		fileName = destination + "/" + fileName;

	// create file
	file = new QFile(fileName);
	if (!file->open(QIODevice::WriteOnly))
	{
		delete file;
		file = NULL;
		return UNABLE_CREATE_FILE;
	}

	// set file info
	destFile = QFileInfo(fileName);

	// init http variables
	initData();
	oriURL = URL;

	// start internal timer
	initTimer();

	// +1 to retries count
	retriesCount++;

	// start the download process
	jumpToURL(URL);

	// ok
	return QHttp::NoError;
}

int Http::resume(const QUrl URL, QString fileName, bool autoRestartOnFail)
{
	// check if is already downloading another file
	if (isDownloading()) 
		return ALREADY_DOWNLOADING;

	// check if is a valid URL
	if (!validURL(URL.toString())) 
		return INVALID_URL;

	if (!QFile::exists(fileName))
		return MISSING_RESUME_FILE;

	// open the existent file in append mode
	file = new QFile(fileName);
	if (!file->open(QIODevice::Append))
	{
		delete file;
		file = NULL;
		return UNABLE_APPEND_FILE;
	}

	// set file info
	destFile = QFileInfo(fileName);

	// init http variables
	initData();
	resuming = true;
	oriURL = URL;
	this->autoRestartOnFail = autoRestartOnFail;

	// start internal timer
	initTimer();

	// +1 to retries count
	retriesCount++;

	// start the download process
	jumpToURL(URL);

	// ok
	return QHttp::NoError;
}

void Http::pause()
{
	if (isDownloading())
	{
		stopReason = USER_PAUSED;
		http->abort();
	}
}

void Http::cancel()
{
	if (isDownloading())
	{
		stopReason = USER_CANCELLED;
		http->abort();
	}
}

QString Http::downloadWebpage(const QUrl URL, bool isUtf8)
{
	QString result = "";

	if (!isDownloading())
		if (URL.isValid())
		{
			// init http variables
			initData();
			file = NULL;
			// set the sync flag active
			syncFlag = true;
			postMethodFlag = false;
			// do the first jump
			oriURL = URL;
			jumpToURL(URL);
			// wait while the webpage is being downloaded
			while (syncFlag)
				qApp->processEvents();
			// if is utf8 then convert the downloaded data to utf8
			if (isUtf8)
				result = QString::fromUtf8(data.toAscii());
			else
				result = data;
		}
	// final result (output)
	return result;
}

QString Http::downloadWebpagePost(const QUrl URL, QString parameters, bool isUtf8)
{
	QString result = "";

	if (!isDownloading())
		if (URL.isValid())
		{
			// init http variables
			initData();
			file = NULL;
			// set the sync flag active
			syncFlag = true;
			postMethodFlag = true;
			// set parameters
			this->parameters = parameters;
			// do the first jump
			oriURL = URL;
			jumpToURL(URL);
			// wait while the webpage is being downloaded
			while (syncFlag)
				qApp->processEvents();
			// if is utf8 then convert the downloaded data to utf8
			if (isUtf8)
				result = QString::fromUtf8(data.toAscii());
			else
				result = data;
		}
	// final result (output)
	return result;
}

QHttpResponseHeader Http::head(const QUrl URL, bool autoJump)
{
	QHttpResponseHeader result;

	if (!isDownloading())
		if (URL.isValid())
		{
			// init http variables
			initData();
			file = NULL;
			this->autoJump = autoJump;
			oriURL = URL;
			// set the sync flag active
			syncFlag = true;
			postMethodFlag = false;
			// get the head
			QHttp::ConnectionMode mode = QHttp::ConnectionModeHttp;
			http->setHost(URL.host(), mode, URL.port() == -1 ? 0 : URL.port());
			httpGetId = http->head(getPathAndQuery(URL)); //http->head(URL.toEncoded());
			// wait while the header is recived
			while (syncFlag)
				qApp->processEvents();
			// return the last response
			result = http->lastResponse();
		}
	return result;
}

void Http::addCookie(QString cookie)
{
	cookies->addCookie(cookie);
}

void Http::clearCookies()
{
	cookies->clear();
}

void Http::addHeaderParameter(QString key, QString value)
{
	customHeaders->append(key + "|" + value);
}

void Http::clearHeaderParameters()
{
	customHeaders->clear();
}

void Http::pauseOnDestroy(bool pauseOnDestroyF)
{
	this->pauseOnDestroyF = pauseOnDestroyF;
}

bool Http::isDownloading()
{
	return 	http->state() != QHttp::Unconnected &&
	        http->state() != QHttp::Connected &&
	        http->state() != QHttp::Closing;
}

int Http::getFileSize()
{
	return fileSize;
}

int Http::getDownloadSpeed()
{
	return downloadSpeed;
}

int Http::getTimeRemaining()
{
	return timeRemaining;
}

QFileInfo Http::getDestiationFile()
{
	return destFile;
}

void Http::setMaxRetries(int value)
{
	maxRetries = value;
}

void Http::setTimeOut(int value)
{
	timeOut = value * 1000;
}

void Http::dataReadProgress(int done, int total)
{
	if (!startedDownload) return;

	// if we are there, then reset the timeOut counter
	tmrTimeOut.stop();

	totalDownload = realTotalSize != 0 ? realTotalSize : total;
	currDownload = total != 0 ? realStartSize + done : 0;

	emit downloadEvent(currDownload, totalDownload);

	// start again the timeOut timer
	tmrTimeOut.start(timeOut);
}

void Http::requestFinished(int id, bool error)
{
	if (httpGetId != id) return;

	// stop timeout timer
	tmrTimeOut.stop();

	if (file != NULL)
	{
		if (error)
		{
			bool canRemove = true;
			
			if (http->error() == QHttp::Aborted && 
				(stopReason == USER_CANCELLED || stopReason == USER_PAUSED))
				switch (stopReason)
				{
					case USER_CANCELLED:
						emit downloadCanceled();
						break;
					case USER_PAUSED:
						canRemove = false;
						emit downloadPaused(destFile);
						break;
					case NO_STOPPED:
						break;
					case DOWNLOAD_FINISHED:
						break;
					case TIME_OUT:
						break;
				}
			else // others
			{
				// abort all (and clear pending requests)
				http->clearPendingRequests();
				// if is an "auto-abort" for restart the download then do not send the error signal
				if (restartDownload || (!restartDownload && retriesCount < maxRetries) ||
					(stopReason == TIME_OUT && retriesCount < maxRetries))
					QTimer::singleShot(500, this, SLOT(restartDownloadSignal()));
				else
					// send error signal
					emit downloadError(http->error());
			}
			// remove file?
			if (canRemove) file->remove();
		}
		else // no error, but...
			if (file->size() < fileSize && !notLength)
			{
				// remove the temporal file
				file->remove();
				// abort all (and clear pending requests)
				http->clearPendingRequests();
				// send the error signal
				emit downloadError(INVALID_FILE_SIZE);
			}
			else
			{
				initRetriesData();			
				emit downloadFinished(destFile);
			}

		// finish timer
		deinitTimer();
		
		// close file
		delete file;
		file = NULL;
	}
	else // no file assigned
	{
		if (error)
			data.clear();
		else
			data = http->readAll();
		// disable the sync flag
		syncFlag = false;
	}
}

void Http::responseHeaderReceived(const QHttpResponseHeader &resp)
{
	// get the file size
	if (resp.hasContentLength())
	{
		// remove the ";" char if exists from "Content-Length"
		QString preSize = resp.value("content-length").remove(";");
		// convert to int
		fileSize = preSize.toInt(&notLength);
		notLength = !notLength;
	}
	else
		notLength = true;

	// get range if we are resuming
	if (resuming)
	{
		realStartSize = copyBetween(resp.value("content-range"), "bytes ", "-").toInt();
		realTotalSize = getToken(resp.value("content-range"), "/", 1).toInt();

		prevDownload = realStartSize;
	}

	// get the server cookies
	QStringList cookiesList;
	cookiesList << resp.allValues("set-cookie");

	// add cookies
	for (int n = 0; n < cookiesList.count(); n++)
		cookies->addCookie(cookiesList.at(n));

	// check the status code, if is "object moved" then jump to new url
	if (isObjectMoved(resp.statusCode()) && autoJump)
		// jump to the redirected url (and clean it... if is necessary)
		jumpToURL(QUrl(cleanURL(resp.value("location"))));
	else
		// if the response is distinct to 200 and 206 or
		// the response is 200 and we are resuming then the server do not support resuming
		// files, so... start again the download...
		if ((resp.statusCode() != 200 && resp.statusCode() != 206) || 
			(resuming && resp.statusCode() == 200))
		{
			restartDownload = resuming && autoRestartOnFail;
			http->abort();
		}
		else
		{
			if (file != NULL) 
			{
				if (!resuming) 
					file->reset();
				else
					file->seek(realStartSize);
			}
			// send the download/resume signal
			if (resuming)
				emit downloadResumed();
			else
				emit downloadStarted();
			// ok, real download started...
			startedDownload = true;
		}
}

void Http::stateChanged(int /*state*/)
{
	// nothing to do...
}

void Http::restartDownloadSignal()
{
	download(oriURL, destFile.path(), destFile.fileName(), false);
}

void Http::timeOutCheckout()
{
	stopReason = TIME_OUT;
	http->abort();
}

void Http::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == internalTimer && http->state() == QHttp::Reading)
	{
		// download speed
		downloadSpeed = static_cast<int>(downloadSpeedAvg->add(currDownload - prevDownload));
		// time remaining
		if (downloadSpeed != 0)
			timeRemaining = (totalDownload - currDownload) / downloadSpeed;
		// reset data
		prevDownload = currDownload;
	}
}
