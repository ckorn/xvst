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

#ifndef __VIDEOLISTCONTROLLER_H__
#define __VIDEOLISTCONTROLLER_H__

#include <QtGui>
#include <QNetworkProxy>
//
#include "options.h"
#include "videoitem.h"
#include "videoinformation.h"
#include "videoconvert.h"
#include "videodownload.h"

class VideoListController : public QObject
{
Q_OBJECT
	private:
		QList<VideoItem *> *videoList;		//!< list with all stored videos
		VideoInformation *videoInformation;	//!< video information class
		VideoDownload *videoDownload;		//!< video download class
		VideoConverter *videoConverter;		//!< video converter class
		ProgramOptions *programOptions; 	//!< pointer to the program options
		int internalTimer;					//!< internal timer
		/*! Determine if this index is a valid item index */
		bool validItemIndex(const int index);
		/*! Get the first item found using the videoState */
		VideoItem* getFirstByState(VideoState videoState);
		/*! Swap two items */
		void swapVideoItems(VideoItem *from, VideoItem *to);
	protected:
		/*! Internal timer event */
		void timerEvent(QTimerEvent *event);
	public:
		/*! Class constructor */
		VideoListController(ProgramOptions *programOptions);
		/*! Class destructor */
		~VideoListController();
		/*! Add a new TDownloadVideoItem */
		VideoItem* addVideo(const QString URL);
		/*! Add a Stored TDownloadVideoItem */
		VideoItem* addVideo(VideoItem *videoItem);
		/*! Delete by Index an existent TDownloadVideoItem */
		void deleteVideo(const int index);
		/*! Delete an existent TDownloadVideoItem */
		void deleteVideo(VideoItem *videoItem);
		/*! Clear TDownloadVideoItem list */
		void clear();
		/*! Get the first null item to get info */
		VideoItem* getFirstNULL();
		/*! Get the first ready to be downloaded */
		VideoItem* getFirstReady();
		/*! Get the first downloaded to be converted */
		VideoItem* getFirstDownloaded();
		/*! Get the first item converted */
		VideoItem* getFirstConverted();
		/*! Get if this item can be deleted or not (by Position) */
		bool canBeDeleted(const int index);
		/*! Get if this item can be deleted or not (by Item) */
		bool canBeDeleted(VideoItem *videoItem);
		/*! Get if this item is bussy (by Position) */
		bool isBussy(const int index);
		/*! Get if this item is bussy (by Item) */
		bool isBussy(VideoItem *videoItem);
		/*! Delete completed items */
		void deleteCompletedItems();
		/*! Get the completed items count */
		int getCompletedItemsCount();
		/*! Get a VideoItem by ibdex */
		VideoItem* getVideoItem(const int index);
		/*! Get a VideoItem by ID */
		VideoItem* getVideoItemByID(const int ID);
		/*! Get a VideoItem by QVAriant (variant must have the item ID)*/
		VideoItem* getVideoItemFromQVAriant(const QVariant variant);
		/*! Get a VideoItem Index by VideoItem */
		int getVideoItemIndexByVideoItem(VideoItem* videoItem);
		/*! Get videos count */
		int getVideoItemCount(bool ignoreDeleted = false);
		/*! Get if some process is working */
		bool isWorking();
		/*! Get if is downloading */
		bool isDownloading();
		/*! Get if is converting */
		bool isConverting();
		/*! Get if is can start a new download */
		bool canStartDownload();
		/*! Start to get video information */
		void startGetInformation(VideoItem *videoItem);
		/*! Start a new download */
		void startDownload(VideoItem *videoItem);
		/*! Cancel download */
		void cancelDownload();
		/*! Start a new conversion */
		void startConversion(VideoItem *videoItem);
		/*! Cancel conversion */
		void cancelConversion();
		/*! Update options */
		void updateOptions();
		/*! Get the internal video information */
		VideoInformation* getVideoInformation();
		/*! Move an item up (into prior list) */
		void moveUP(VideoItem *videoItem);
		/*! Move an item down (into prior list) */
		void moveDOWN(VideoItem *videoItem);
	private slots:
		/*! Video item has been updated */
		void videoItemUpdated(VideoItem *videoItem);
		/*! Video srtart a new process */
		void actionStarted(VideoItem *videoItem);
		/*! Video has been finished the process */
		void actionFinished(VideoItem *videoItem);
	signals:
		/*! When a video is added, this signal is emited */
		void videoAdded(VideoItem *videoItem);
		/*! When a video is deleted, this signal is emited */
		void videoDeleted(VideoItem *videoItem);
		/*! When a videoItem is updated, this signal is emited */
		void videoUpdated(VideoItem *videoItem);
		/*! When a videoItem has been moved */
		void videoMoved(int from, int to);
};

#endif // __VIDEOLISTCONTROLLER_H__
