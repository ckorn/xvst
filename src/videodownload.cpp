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

#include "videodownload.h"

VideoDownload::VideoDownload(QString saveTo)
{
	setObjectName("VideoDownload");
	// init http
	http = new Http;
	// init data
	videoItem = NULL;
	setDownloadDir(saveTo);
	// connect signals
	connect(http, SIGNAL(downloadStarted()), this, SLOT(downloadStarted()));
	connect(http, SIGNAL(downloadFinished(const QFileInfo)), this, SLOT(downloadFinished(const QFileInfo)));
	connect(http, SIGNAL(downloadCanceled()), this, SLOT(downloadCanceled()));
	connect(http, SIGNAL(downloadError(int)), this, SLOT(downloadError(int)));
	connect(http, SIGNAL(downloadEvent(int, int)), this, SLOT(downloadEvent(int, int)));
}

VideoDownload::~VideoDownload()
{
	if (http->isDownloading())
		http->cancel();

	delete http;
}

void VideoDownload::downloadVideo(VideoItem *videoItem)
{
	if (videoItem == NULL || !canStartDownload() || this->videoItem != NULL) return;
	// assign data
	this->videoItem = videoItem;
	videoItem->lock (this);
	videoItem->setAsDownloading(this);
	videoItem->setProgress(0, this);
	// start download video
	if (int er = http->download(QUrl(videoItem->getVideoInformation().URL),
	                            QDir(downloadDir),
	                            videoItem->getVideoFile()) != 0)
		downloadError(er);
}

void VideoDownload::cancelDownload()
{
	http->cancel();
}

bool VideoDownload::canStartDownload()
{
	return !http->isDownloading();
}

bool VideoDownload::isDownloading()
{
	return http->isDownloading();
}

QString VideoDownload::getDownloadDir()
{
	return downloadDir;
}

void VideoDownload::setDownloadDir(QString downloadDir)
{
	this->downloadDir = downloadDir;
}

void VideoDownload::downloadStarted()
{
	workStarted();
}

void VideoDownload::downloadFinished(const QFileInfo destFile)
{
	videoItem->setVideoFile(destFile.absoluteFilePath(), this);
	videoItem->setAsDownloaded(this);
	workFinished();
}

void VideoDownload::downloadCanceled()
{
	videoItem->setAsCanceled(this);
	workFinished();
}

void VideoDownload::downloadError(int error)
{	
	videoItem->setAsError(this);
	workFinished();
}

void VideoDownload::downloadEvent(int pos, int max)
{
	videoItem->setProgress(calculePercent(pos, max), this);
	videoItem->setVideoSize(max, this);
	videoItem->setDownloadSpeed(http->getDownloadSpeed(), this);
	videoItem->setTimeRemaining(http->getTimeRemaining(), this);
	// emit signal
	emit videoItemUpdated(videoItem);
}

void VideoDownload::workStarted()
{
	emit downloadStarted(videoItem);
}

void VideoDownload::workFinished()
{
	videoItem->unlock(this);
	// emit signal
	emit downloadFinished(videoItem);
	// 
	this->videoItem = NULL;
}
