#ifndef __VIDEODOWNLOAD_H__
#define __VIDEODOWNLOAD_H__

#include <QtGui>
//
#include "http.h"
#include "videoitem.h"

class VideoDownload : public QObject
{
Q_OBJECT
	private:
		Http *http;				//!< download class
		VideoItem *videoItem;	//!< current video item
		QString downloadDir;	//!< where to download videos
		/*! when work started */
		void workStarted();
		/*! when work finished */
		void workFinished();
	public:
		/*! Class csontructor */
		VideoDownload(QString downloadDir);
		/*! class destructor */
		~VideoDownload();
		/*! start to download video */
		void downloadVideo(VideoItem *videoItem);
		/*! cancel download */
		void cancelDownload();
		/*! Get if possible start a new download */
		bool canStartDownload();
		/*! Get if is downloading some video */
		bool isDownloading();
		/*! Get where to save downloads */
		QString getDownloadDir();
		/*! Set where to save downloads */
		void setDownloadDir(QString downloadDir);
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
		void videoItemUpdated(VideoItem *videoItem);
		/*! A Video item download started */
		void downloadStarted(VideoItem *videoItem);
		/*! A Video item download finished */
		void downloadFinished(VideoItem *videoItem);
};

#endif // __VIDEODOWNLOAD_H__
