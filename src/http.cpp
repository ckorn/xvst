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

#include "http.h"

Http::Http()
{
	// http protocol
	http = new QHttp(this);
	// download speed avg calculator
	downloadSpeedAvg = new ArrayAvg(10);
	// init internal timmer
	internalTimer = 0;
	// destination file
	file = NULL;
	// connect signals
	connect(http, SIGNAL(dataReadProgress(int, int)), this, SLOT(dataReadProgress(int, int)));
	connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)));
	connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this,
	        SLOT(responseHeaderReceived(const QHttpResponseHeader &)));
	connect(http, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
}

Http::~Http()
{
	if (http->hasPendingRequests())
		abort();

	delete downloadSpeedAvg;
	delete http;
}

bool Http::isObjectMoved(int statusCode)
{
	return	statusCode == 301 || statusCode == 302 ||
	       statusCode == 303 || statusCode == 307;
}

void Http::initData()
{
	// init http variables
	totalDownload = 0;
	currDownload = 0;
	prevDownload = 0;
	downloadSpeed = 0;
	timeRemaining = 0;
	fileSize = 0;
	userAborted = false;
	canDownload = false;
	notLength = false;
	autoJump = true;
}

void Http::jumpToURL(QUrl url)
{
	// http mode
	QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
	// set http host
	http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
	// set http user and password (only if is requiered)
	if (!url.userName().isEmpty())
		http->setUser(url.userName(), url.password());
	// get url
	if (file != NULL)
		httpGetId = http->get(getPathAndQuery(url), file); //http->get(url.toEncoded(), file);
	else
		httpGetId = http->get(getPathAndQuery(url)); //http->get(url.toEncoded());
}

int Http::download(const QUrl URL, const QDir destination, QString fileName)
{
	if (!isDownloading())
		if (URL.isValid())
		{
			if (!destination.exists())
				if (!destination.mkpath(destination.absolutePath()))
					return UNABLE_CREATE_DIR;
			// set a default name (if is empty)
			if (fileName.isEmpty())
				fileName = "download.file";
			// set destination file name
			fileName = cleanFileName(fileName);
			fileName = uniqueFileName(destination.absolutePath() + "/" + fileName).absoluteFilePath();
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
			// start internal timer
			if (internalTimer != 0) this->killTimer(internalTimer);
			internalTimer = this->startTimer(1000);
			// make the first jump
			jumpToURL(URL);
			// ok
			return QHttp::NoError;
		}
		else
			return INVALID_URL;
	else
		return ALREADY_DOWNLOADING;
}

QString Http::downloadWebpage(const QUrl URL, bool isUtf8)
{
	QString result = "";

	if (!isDownloading())
		if (URL.isValid())
		{
			// init http variables
			initData();
			// set the sync flag active
			syncFlag = true;
			// make the first jump
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
			this->autoJump = autoJump;
			// set the sync flag active
			syncFlag = true;
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

void Http::cancel()
{
	if (isDownloading())
	{
		userAborted = true;
		http->abort();
	}
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

bool Http::isDownloading()
{
	return 	http->state() != QHttp::Unconnected &&
	        http->state() != QHttp::Connected &&
	        http->state() != QHttp::Closing;
}

void Http::dataReadProgress(int done, int total)
{
	totalDownload = total;
	currDownload = done;

	emit downloadEvent(done, total);
}

void Http::requestFinished(int id, bool error)
{
	if (id == httpGetId)
	{
		if (file != NULL)
		{
			if (error)
			{
				// delete file
				file->remove();
				// what error?
				if (http->error() == QHttp::Aborted && userAborted) // cancel
					emit downloadCanceled();
				else // others
					emit downloadError(http->error());
			}
			else // no error, but...
				if (file->size() != fileSize && !notLength)
				{
					file->remove();
					emit downloadError(INVALID_FILE_SIZE);
				}
				else
					emit downloadFinished(destFile);
			// finish timer
			this->killTimer(internalTimer);
			internalTimer = 0;
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
}

void Http::responseHeaderReceived(const QHttpResponseHeader &resp)
{
	// get the file size
	if (resp.hasContentLength())
		fileSize = resp.value("content-length").toInt();
	else
		notLength = true;

	if (isObjectMoved(resp.statusCode()) && autoJump)
		jumpToURL(QUrl(resp.value("location")));
	else
		if (resp.statusCode() != 200)
			http->abort();
		else
			canDownload = true;
}

void Http::stateChanged(int state)
{
	if (state == QHttp::Reading && canDownload)
		emit downloadStarted();
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
