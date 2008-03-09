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

#ifndef __VIDEODOWNLOAD_H__
#define __VIDEODOWNLOAD_H__

#include <QtGui>
//
#include "http.h"
#include "videoitem.h"

class VideoDownload;

/*! Single download */
class DownloadItem : public QObject
{
Q_OBJECT
	private:
		VideoDownload *parent;	// parent class
		Http *http;				//!< download class
		VideoItem *videoItem;	//!< current video item
		/*! when work started */
		void workStarted();
		/*! when work finished */
		void workFinished();
	public:
		/*! Class constructor */
		DownloadItem(VideoDownload *parent, VideoItem *videoItem);
		/*! Class destructor */
		~DownloadItem();
		/*! Start the download */
		void startDownload();
		/*! Cancel the download */
		void cancelDownload();
		/*! Get the video item associated */
		VideoItem* getVideoItem();
	private slots:
		/*! A new download started */
		void downloadStarted();
		/*! A download finished */
		void downloadFinished(const QFileInfo destFile);
		/*! Download has been canceled */
		void downloadCanceled();
		/*! Ops, an error ocurred */
		void downloadError(int error);
		/*! A single download event */
		void downloadEvent(int pos, int max);
	signals:
		/*! Video item has been modified */
		void videoItemUpdated_child(VideoItem *videoItem);
		/*! A Video item download started */
		void downloadStarted_child(VideoItem *videoItem);
		/*! A Video item download finished */
		void downloadFinished_child(VideoItem *videoItem);
		/*! This signal is send when all work finished */
		void downloadDestroyable();
};

/*! Downloads controller */
class VideoDownload : public QObject
{
Q_OBJECT
	private:
		QList<DownloadItem *> *downloads;		//!< active downloads
		QString downloadDir;					//!< where to download videos
		int maxActiveDownloads;					//!< maximum active downloads
		DownloadItem *downloadItemToDestroy;	//!< Download item to destroy
		/*! Find a download item by a video item*/
		DownloadItem* findDownloadItemByVideoItem(VideoItem *videoItem);
	public:
		/*! Class csontructor */
		VideoDownload(QString downloadDir, int maxActiveDownloads);
		/*! class destructor */
		~VideoDownload();
		/*! start to download video */
		void downloadVideo(VideoItem *videoItem);
		/*! cancel download */
		void cancelDownload(VideoItem *videoItem);
		/*! cancel all downloads */
		void cancelAllDownloads();
		/*! Get if possible start a new download */
		bool canStartDownload();
		/*! Get if is downloading some video */
		bool isDownloading();
		/*! Get where to save downloads */
		QString getDownloadDir();
		/*! Set where to save downloads */
		void setDownloadDir(QString downloadDir);
		/*! Get how many downloads can run simultaneous */
		int getMaxActiveDownloads();
		/*! Set how many downloads can run simultaneous */
		void setMaxActiveDownloads(int maxActiveDownloads);
	private slots:
		/*! Video item has been modified */
		void videoItemUpdated_child(VideoItem *videoItem);
		/*! A Video item download started */
		void downloadStarted_child(VideoItem *videoItem);
		/*! A Video item download finished */
		void downloadFinished_child(VideoItem *videoItem);
		/*! The download object can be destroyed (and deleted from list) */
		void downloadDestroyable();
		/*! Destroy the last downloadItem finished */
		void destroyDownload();
	signals:
		/*! Video item has been modified */
		void videoItemUpdated(VideoItem *videoItem);
		/*! A Video item download started */
		void downloadStarted(VideoItem *videoItem);
		/*! A Video item download finished */
		void downloadFinished(VideoItem *videoItem);
};

#endif // __VIDEODOWNLOAD_H__
