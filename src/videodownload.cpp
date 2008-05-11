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

#include "videodownload.h"

/* DownloadItem Class */

DownloadItem::DownloadItem(VideoDownload *parent, VideoItem *videoItem)
{
	setObjectName("DownloadItem");
	// assign main info
	this->parent = parent;
	this->videoItem = videoItem;
	// create the http object
	http = new Http;
	// connect signals
	connect(http, SIGNAL(downloadStarted()), this, SLOT(downloadStarted()));
	connect(http, SIGNAL(downloadPaused(const QFileInfo)), this, SLOT(downloadPaused(const QFileInfo)));
	connect(http, SIGNAL(downloadResumed()), this, SLOT(downloadResumed()));
	connect(http, SIGNAL(downloadFinished(const QFileInfo)), this, SLOT(downloadFinished(const QFileInfo)));
	connect(http, SIGNAL(downloadCanceled()), this, SLOT(downloadCanceled()));
	connect(http, SIGNAL(downloadError(int)), this, SLOT(downloadError(int)));
	connect(http, SIGNAL(downloadEvent(int, int)), this, SLOT(downloadEvent(int, int)));
}

DownloadItem::~DownloadItem()
{
	cancelDownload();

	delete http;
}

void DownloadItem::startDownload()
{
	// assign data
	this->videoItem = videoItem;
	videoItem->lock (this);
	videoItem->setAsDownloading(this);
	videoItem->setProgress(0, this);
	// start download
	int er = http->download(QUrl(videoItem->getVideoInformation().URL), 
		parent->getDownloadDir(), videoItem->getVideoFile());
	if (er != 0) downloadError(er);
}

void DownloadItem::pauseDownload()
{
	http->pause();
}

void DownloadItem::resumeDownload()
{
	// assign data
	this->videoItem = videoItem;
	videoItem->lock (this);
	videoItem->setAsDownloading(this);
	// resume download
	int er = http->resume(QUrl(videoItem->getVideoInformation().URL), videoItem->getVideoFile());
	if (er != 0) downloadError(er);
}

void DownloadItem::cancelDownload()
{
	http->cancel();
}

VideoItem* DownloadItem::getVideoItem()
{
	return videoItem;
}

void DownloadItem::downloadStarted()
{
	workStarted();
}

void DownloadItem::downloadPaused(const QFileInfo destFile)
{
	videoItem->setVideoFile(destFile.absoluteFilePath(), this);
	videoItem->setAsPaused(this);
	workFinished();
}

void DownloadItem::downloadResumed()
{
	workStarted();
}

void DownloadItem::downloadFinished(const QFileInfo destFile)
{
	videoItem->setVideoFile(destFile.absoluteFilePath(), this);
	videoItem->setAsDownloaded(this);
	workFinished();
}

void DownloadItem::downloadCanceled()
{
	videoItem->setAsCanceled(this);
	workFinished();
}

void DownloadItem::downloadError(int error)
{
	videoItem->setErrorCode(error, this);	
	videoItem->setAsError(this);
	workFinished();
}

void DownloadItem::downloadEvent(int pos, int max)
{
	videoItem->setProgress(calculePercent(pos, max), this);
	videoItem->setVideoSize(max, this);
	videoItem->setDownloadSpeed(http->getDownloadSpeed(), this);
	videoItem->setTimeRemaining(http->getTimeRemaining(), this);
	// emit signal
	emit videoItemUpdated_child(videoItem);
}

void DownloadItem::workStarted()
{
	emit downloadStarted_child(videoItem);
}

void DownloadItem::workFinished()
{
	videoItem->unlock(this);
	// emit signal
	emit downloadFinished_child(videoItem);
	// emit the destroy signal (bye bye cruel world!)
	emit downloadDestroyable();
}

/* VideoDownload Class */

VideoDownload::VideoDownload(QString saveTo, int maxActiveDownloads)
{
	setObjectName("VideoDownload");
	// init vars
	setDownloadDir(saveTo);
	setMaxActiveDownloads(maxActiveDownloads);
	// init downloads list
	downloads = new QList<DownloadItem *>;
}

VideoDownload::~VideoDownload()
{
	cancelAllDownloads();
	
	delete downloads;
}

DownloadItem* VideoDownload::findDownloadItemByVideoItem(VideoItem *videoItem)
{
	if (videoItem == NULL) return NULL;

	for (int n = 0; n < downloads->size(); n++)
		if (downloads->at(n)->getVideoItem() == videoItem)
			return downloads->at(n);

	return NULL;
}

void VideoDownload::stopAllDownloads(bool doCancel)
{
	for (int n = downloads->size() - 1; n >= 0; n--)
	{
		if (doCancel) downloads->at(n)->cancelDownload();
		else downloads->at(n)->pauseDownload();
		// process events
		qApp->processEvents();
	}
}

void VideoDownload::downloadVideo(VideoItem *videoItem)
{
	if (videoItem == NULL || !canStartDownload()) return;
	// add a new download
	downloads->append(new DownloadItem(this, videoItem));
	// get the new item added
	DownloadItem *downloadItem = downloads->last();
	// connect signals of this new child
	connect(downloadItem, SIGNAL(videoItemUpdated_child(VideoItem*)), this, SLOT(videoItemUpdated_child(VideoItem*)));
	connect(downloadItem, SIGNAL(downloadStarted_child(VideoItem*)), this, SLOT(downloadStarted_child(VideoItem*)));
	connect(downloadItem, SIGNAL(downloadFinished_child(VideoItem*)), this, SLOT(downloadFinished_child(VideoItem*)));
	connect(downloadItem, SIGNAL(downloadDestroyable()), this, SLOT(downloadDestroyable()));
	// start to download the video
	downloadItem->startDownload();
}

void VideoDownload::pauseDownload(VideoItem *videoItem)
{
	DownloadItem *downloadItem = findDownloadItemByVideoItem(videoItem);

	if (downloadItem != NULL)
		downloadItem->pauseDownload();
}

void VideoDownload::resumeDownload(VideoItem *videoItem)
{
	if (videoItem == NULL) return;
	// cant resume the download now, so mark it as "next to start"
	if (!canStartDownload())
	{
		videoItem->setAsResuming();
		emit videoItemUpdated(videoItem);
		return;
	}
	// add a new download
	downloads->append(new DownloadItem(this, videoItem));
	// get the new item added
	DownloadItem *downloadItem = downloads->last();
	// connect signals of this new child
	connect(downloadItem, SIGNAL(videoItemUpdated_child(VideoItem*)), this, SLOT(videoItemUpdated_child(VideoItem*)));
	connect(downloadItem, SIGNAL(downloadStarted_child(VideoItem*)), this, SLOT(downloadStarted_child(VideoItem*)));
	connect(downloadItem, SIGNAL(downloadFinished_child(VideoItem*)), this, SLOT(downloadFinished_child(VideoItem*)));
	connect(downloadItem, SIGNAL(downloadDestroyable()), this, SLOT(downloadDestroyable()));
	// start to download the video
	downloadItem->resumeDownload();
}

void VideoDownload::cancelDownload(VideoItem *videoItem)
{
	DownloadItem *downloadItem = findDownloadItemByVideoItem(videoItem);

	if (downloadItem != NULL)
		downloadItem->cancelDownload();
}

void VideoDownload::pauseAllDownloads()
{
	stopAllDownloads();
}

void VideoDownload::cancelAllDownloads()
{
	stopAllDownloads(true);
}

bool VideoDownload::canStartDownload()
{
	return downloads->size() < maxActiveDownloads;
}

bool VideoDownload::isDownloading()
{
	return !downloads->isEmpty();
}

QString VideoDownload::getDownloadDir()
{
	return downloadDir;
}

void VideoDownload::setDownloadDir(QString downloadDir)
{
	this->downloadDir = downloadDir;
}

int VideoDownload::getMaxActiveDownloads()
{
	return maxActiveDownloads;
}

void VideoDownload::setMaxActiveDownloads(int maxActiveDownloads)
{
	maxActiveDownloads = maxActiveDownloads < 1 ? 1 : maxActiveDownloads;

	this->maxActiveDownloads = maxActiveDownloads;
}

void VideoDownload::videoItemUpdated_child(VideoItem *videoItem)
{
	emit videoItemUpdated(videoItem);
}

void VideoDownload::downloadStarted_child(VideoItem *videoItem)
{
	emit downloadStarted(videoItem);
}

void VideoDownload::downloadFinished_child(VideoItem *videoItem)
{
	emit downloadFinished(videoItem);
}

void VideoDownload::downloadDestroyable()
{
	downloadItemToDestroy = static_cast<DownloadItem *>(sender());
	// destroy the current download
	QTimer::singleShot(0, this, SLOT(destroyDownload()));
}

void VideoDownload::destroyDownload()
{
	// disconnect signals
	downloadItemToDestroy->disconnect();
	// delete and destroy the DownloadItem
	downloads->removeAt(downloads->indexOf(downloadItemToDestroy));
	delete downloadItemToDestroy;
}