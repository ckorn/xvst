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

#include "videolistcontroller.h"

// VideoListController class

VideoListController::VideoListController(ProgramOptions *programOptions)
{
	this->programOptions = programOptions;

	videoList = new QList<VideoItem *>;

	videoInformation = new VideoInformation;
	videoDownload = new VideoDownload(programOptions->getDownloadDir());
	videoConverter = new VideoConverter(programOptions->getFfmpegLibLocation(),
	                                    programOptions->getDownloadDir(),
	                                    programOptions->getConversionConf(),
	                                    programOptions->getDeleteVideosOnConvert());

	// video information
	connect(videoInformation, SIGNAL(informationStarted(VideoItem *)), this, SLOT(actionStarted(VideoItem *)));
	connect(videoInformation, SIGNAL(informationFinished(VideoItem *)), this, SLOT(actionFinished(VideoItem *)));
	// video download
	connect(videoDownload, SIGNAL(videoItemUpdated(VideoItem *)), this, SLOT(videoItemUpdated(VideoItem *)));
	connect(videoDownload, SIGNAL(downloadStarted(VideoItem *)), this, SLOT(actionStarted(VideoItem *)));
	connect(videoDownload, SIGNAL(downloadFinished(VideoItem *)), this, SLOT(actionFinished(VideoItem *)));
	// video converter
	connect(videoConverter, SIGNAL(videoItemUpdated(VideoItem *)), this, SLOT(videoItemUpdated(VideoItem *)));
	connect(videoConverter, SIGNAL(conversionStarted(VideoItem *)), this, SLOT(actionStarted(VideoItem *)));
	connect(videoConverter, SIGNAL(conversionFinished(VideoItem *)), this, SLOT(actionFinished(VideoItem *)));
}

VideoListController::~VideoListController()
{
	this->killTimer(internalTimer);

	clear();
	delete videoList;

	delete videoConverter;
	delete videoInformation;
	delete videoDownload;
}

bool VideoListController::validItemIndex(const int index)
{
	return (index >= 0 && index < getVideoItemCount());
}

VideoItem* VideoListController::getFirstByState(VideoState videoState)
{
	for (int n = 0; n < getVideoItemCount(); n++)
		if (getVideoItem(n)->getVideoState() == videoState)
			return getVideoItem(n);
	// no items are ready to be downloaded
	return NULL;
}

void VideoListController::swapVideoItems(VideoItem *from, VideoItem *to)
{
	if (from != NULL && to != NULL)
	{
		int i = getVideoItemIndexByVideoItem(from);
		int j = getVideoItemIndexByVideoItem(to);
		
		if (validItemIndex(i) && validItemIndex(j))
		{
			videoList->swap(i, j);
			emit videoMoved(i, j);
		}
	}
}

void VideoListController::timerEvent(QTimerEvent *event)
{
	// get first null item to, to start get info
	if (videoInformation->canGetInformation())
		startGetInformation(getFirstNULL());

	// get the first ready item, to start the download
	if (programOptions->getDownloadAutomatically() && videoDownload->canStartDownload())
		startDownload(getFirstReady());

	// get the first downloaded item, to start the conversion
	if (programOptions->getConvertVideos() && videoConverter->canStartConversion())
		startConversion(getFirstDownloaded());
}

void VideoListController::start()
{
	internalTimer = this->startTimer(100);
}

VideoItem* VideoListController::addVideo(const QString URL)
{
	videoList->append(new VideoItem(URL));
	emit videoAdded(videoList->last());
	return videoList->last();
}

VideoItem* VideoListController::addVideo(VideoItem *videoItem)
{
	videoList->append(new VideoItem);
	videoList->last()->assign(videoItem);
	emit videoAdded(videoList->last());
	return videoList->last();
}

void VideoListController::deleteVideo(const int index)
{
	if (validItemIndex(index))
	{
		VideoItem *videoItem = getVideoItem(index);
		videoItem->setAsDeleted();
		emit videoDeleted(videoItem);
		delete videoList->at(index);
		videoList->removeAt(index);
	}
}

void VideoListController::deleteVideo(VideoItem *videoItem)
{
	deleteVideo(videoList->indexOf(videoItem));
}

void VideoListController::clear()
{
	if (videoInformation->isGettingInfo())
		videoInformation->cancel();
	
	while (!videoList->isEmpty())
		deleteVideo(videoList->at(0));
}

VideoItem* VideoListController::getFirstNULL()
{
	return getFirstByState(vsNULL);
}

VideoItem* VideoListController::getFirstReady()
{
	return getFirstByState(vsGettedURL);
}

VideoItem* VideoListController::getFirstDownloaded()
{
	return getFirstByState(vsDownloaded);
}

VideoItem* VideoListController::getFirstConverted()
{
	return getFirstByState(vsConverted);
}

bool VideoListController::canBeDeleted(const int index)
{
	return !isBussy(index);
}

bool VideoListController::canBeDeleted(VideoItem *videoItem)
{
	return canBeDeleted(videoList->indexOf(videoItem));
}

bool VideoListController::isBussy(const int index)
{
	if (validItemIndex(index))
		return getVideoItem(index)->isBussy();
	else
		return false;
}

bool VideoListController::isBussy(VideoItem *videoItem)
{
	return isBussy(videoList->indexOf(videoItem));
}

void VideoListController::deleteCompletedItems()
{
	for (int n = videoList->count() - 1; n >= 0; n--)
		if (videoList->at(n)->isCompleted())
			deleteVideo(videoList->at(n));
}

int VideoListController::getCompletedItemsCount()
{
	int count = 0;

	for (int n = 0; n < videoList->count(); n++)
		if (videoList->at(n)->isCompleted())
			count++;

	return count;
}

VideoItem* VideoListController::getVideoItem(const int index)
{
	if (validItemIndex(index))
		return videoList->at(index);
	else
		return NULL;
}

VideoItem* VideoListController::getVideoItemByID(const int ID)
{
	for (int n = 0; n < getVideoItemCount(); n++)
		if (getVideoItem(n)->getID() == ID)
			return getVideoItem(n);
	// no items found with this ID
	return NULL;
}

VideoItem* VideoListController::getVideoItemFromQVAriant(const QVariant variant)
{
	bool ok;
	int ID = variant.toInt(&ok);
	// ok?
	if (ok)
		return getVideoItemByID(ID);
	else
		return NULL;
}

int VideoListController::getVideoItemIndexByVideoItem(VideoItem* videoItem)
{
	return videoList->indexOf(videoItem);
}

int VideoListController::getVideoItemCount(bool ignoreDeleted)
{
	if (ignoreDeleted)
	{
		int count = 0;
		
		for (int n = 0; n < videoList->count(); n++)
			if (!videoList->at(n)->isDeleted())
				count++;
				
		return count;
	}

	else
		return videoList->count();
}

bool VideoListController::isWorking()
{
	return isDownloading() || isConverting();
}

bool VideoListController::isDownloading()
{
	return videoDownload->isDownloading();
}

bool VideoListController::isConverting()
{
	return videoConverter->isConverting();
}

bool VideoListController::canStartDownload()
{
	return !videoDownload->isDownloading();
}

void VideoListController::startGetInformation(VideoItem *videoItem)
{
	videoInformation->getVideoInformation(videoItem);
}

void VideoListController::startDownload(VideoItem *videoItem)
{
	if (videoItem == NULL) return;
	// check if is possible download this video
	if (!videoInformation->isBlockedHost(videoItem->getURL()))
		videoDownload->downloadVideo(videoItem);
	else
	{
		videoItem->setAsBlocked();
		
		emit videoItemUpdated(videoItem);
	}
}

void VideoListController::cancelDownload()
{
	videoDownload->cancelDownload();
}

void VideoListController::startConversion(VideoItem *videoItem)
{
	videoConverter->startConversionVideo(videoItem);
}

void VideoListController::cancelConversion()
{
	videoConverter->cancelConversion();
}

void VideoListController::updateOptions()
{
	// video information
	videoInformation->setBlockAdultContent(programOptions->getBlockAdultContent());
	videoInformation->setBlockAdultContentList(programOptions->getBlockedWebsitesList());
	
	// video download
	videoDownload->setDownloadDir(programOptions->getDownloadDir());
	
	// converter
	videoConverter->setFFmpegApp(programOptions->getFfmpegLibLocation());
	videoConverter->setWorkingDir(programOptions->getDownloadDir());
	videoConverter->setConversionConfig(programOptions->getConversionConf());
	videoConverter->setDeleteOriginalVideo(programOptions->getDeleteVideosOnConvert());
	
	// update proxy
	if (programOptions->getUseProxy())
	{
		QNetworkProxy networkProxy;
		networkProxy.setUser(programOptions->getProxyUserName());
		networkProxy.setPassword(programOptions->getProxyPassword());
		networkProxy.setHostName(programOptions->getProxyAdress());
		networkProxy.setPort(programOptions->getProxyPort());
		networkProxy.setType(static_cast<QNetworkProxy::ProxyType>(programOptions->getProxyType()));
		// set application proxy
		QNetworkProxy::setApplicationProxy(networkProxy);
	}
	else
	{
		QNetworkProxy networkProxy;
		networkProxy.setType(QNetworkProxy::NoProxy);
		// remove application proxy
		QNetworkProxy::setApplicationProxy(networkProxy);
	}
}

VideoInformation* VideoListController::getVideoInformation()
{
	return videoInformation;
}

void VideoListController::moveUP(VideoItem *videoItem)
{
	int prevItem = videoList->indexOf(videoItem);
	
	if (prevItem > 0)
		swapVideoItems(videoItem, videoList->at(prevItem - 1));
}

void VideoListController::moveDOWN(VideoItem *videoItem)
{
	int nextItem = videoList->indexOf(videoItem);
	
	if (nextItem < getVideoItemCount() - 1)
		swapVideoItems(videoItem, videoList->at(nextItem + 1));
}

void VideoListController::videoItemUpdated(VideoItem *videoItem)
{
	if (videoItem != NULL)
		emit videoUpdated(videoItem);
}

void VideoListController::actionStarted(VideoItem *videoItem)
{
	if (videoItem != NULL)
		emit videoUpdated(videoItem);
}

void VideoListController::actionFinished(VideoItem *videoItem)
{
	if (videoItem != NULL)
	{
		if (!videoItem->isCanceled())
			if	(sender() == videoConverter ||
			    (sender() == videoDownload && !programOptions->getConvertVideos()) ||
			    (sender() == videoDownload && programOptions->getConvertVideos() && !videoConverter->ffmpegInstalled()))
			{
				// set the saved to
				if (sender() == videoDownload)
					videoItem->setVideoFileSavedTo(videoItem->getVideoFile());
				// ok, set as completed
				videoItem->setAsCompleted(videoItem);
			}
		emit videoUpdated(videoItem);
	}
}
