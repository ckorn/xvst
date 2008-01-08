#ifndef __VIDEOITEM_H__
#define __VIDEOITEM_H__

#include <QtGui>
//
#include "tools.h"

enum VideoState
{
	vsNULL,
	vsGettingURL, vsGettedURL,
	vsDownloading, vsDownloaded,
	vsConverting, vsConverted,
	vsCompleted,
	vsCanceled,
	vsBlocked,
	vsError,
	vsDeleted
};

struct VideoDefinition
{
	QString URL;	//!< real video URL
	QString title;	//!< video title
	
};

class VideoItem : public QObject
{
Q_OBJECT
	private:
		static int internalID;		//!< internal id counter
		int ID;						//!< internal assigned id for this instance
		VideoDefinition videoInfo;	//!< internal video information
		QString URL;				//!< initial video url (i.e.: http://youtube.com/watch?v=AzXR58sd2jY)
		QString videoFile;			//!< downloaded file (*.flv)
		QString videoFileSavedTo;	//!< where to find the final video
		float progress;				//!< current action progress (downloading/converting)
		int videoSize;				//!< video file size in bytes
		int downloadSpeed;			//!< current downoad speed
		int timeRemaining;			//!< current time remaining
		VideoState videoState;		//!< current video state
		QObject *locker;			//!< pointer to the locker
		/*! Init internal data */
		void initData();
		/*! Assign a unique ID for this instance */
		void assignID();
	public:
		/*! Class constructor */
		VideoItem();
		/*! Class constructor */
		VideoItem(const QString URL);
		/*! Assign video info */
		void assign(VideoItem *videoItem);
		/*! Lock item */
		bool lock(QObject *locker);
		/*! Unlock item (only who locked the item can ulnock it) */
		bool unlock(QObject *locker);
		/*! Get if is currently locked */
		bool isLocked();
		/*! Get the current state */
		VideoState getVideoState();
		/*! Get the current state as String */
		QString getVideoStateAsString();
		/*! Get if is NULL yet */
		bool isNULL();
		/*! Get if is Ready to be downloaded */
		bool isReady();
		/*! Get if is Getting the video URL */
		bool isGettingURL();
		/*! Get if has the video URL */
		bool isGettedURL();
		/*! Get if is Downloading */
		bool isDownloading();
		/*! Get if has been Downloaded */
		bool isDownloaded();
		/*! Get if is Converting */
		bool isConverted();
		/*! Get if has been Converted */
		bool isConverting();
		/*! Get if this item has been finished */
		bool isCompleted();
		/*! Get if is Blocked */
		bool isBlocked();
		/*! Get if has Errors */
		bool hasErrors();
		/*! Get if this item is Bussy */
		bool isBussy();
		/*! Get if is removable */
		bool isRemovable();
		/*! Get if this item is canceled */
		bool isCanceled();
		/*! Get if is market as "deleted" */
		bool isDeleted();
		/*! Get the internal ID */
		int getID();
		/*! Get the display label */
		QString getDisplayLabel();
		/*! Get the display size */
		QString getDisplaySize();
		/*! Get the display progress */
		QString getDisplayProgress();
		/*! Get the display time remaining */
		QString getDisplayTimeRemaining();
		/*! Get the display download speed */
		QString getDisplayDownloadSpeed();
		/*! Get video info */
		VideoDefinition getVideoInformation();
		/*! Get the Video URL */
		QString getURL();
		/*! Get the video file */
		QString getVideoFile();
		/*! Get the video file saved to */
		QString getVideoFileSavedTo();
		/*! Get the current progress value */
		float getProgress();
		/*! Get the video file size */
		int getVideoSize();
		/*! Get download speed */
		int getDownloadSpeed();
		/*! Get time remaingin */
		int getTimeRemaining();
		/*! Set the video information */
		void setVideoInformation(VideoDefinition videoInformation, QObject *who = NULL);
		/*! Set video file */
		void setVideoFile(QString videoFile, QObject *who = NULL);
		/*! Set video file saved to */
		void setVideoFileSavedTo(QString videoFileSavedTo, QObject *who = NULL);
		/*! Set the current progress value */
		void setProgress(float progress, QObject *who = NULL);
		/*! Set the video file size */
		void setVideoSize(int videoSize, QObject *who = NULL);
		/*! Set the current download speed */
		void setDownloadSpeed(int downloadSpeed, QObject *who = NULL);
		/*! Set the time remaining */
		void setTimeRemaining(int timeRemaining, QObject *who = NULL);
		/*! Set state */
		void setState(VideoState videoState, QObject *who = NULL);
		/*! Set as NULL */
		void setAsNULL(QObject *who = NULL);
		/*! Set as ready */
		void setAsReady(QObject *who = NULL);
		/*! Set as getting url */
		void setAsGettingURL(QObject *who = NULL);
		/*! Set as getted url */
		void setAsGettedURL(QObject *who = NULL);
		/*! Set as downloading */
		void setAsDownloading(QObject *who = NULL);
		/*! Set as downloaded */
		void setAsDownloaded(QObject *who = NULL);
		/*! Set as converting */
		void setAsConverting(QObject *who = NULL);
		/*! Set as converted */
		void setAsConverted(QObject *who = NULL);
		/*! Set as completed */
		void setAsCompleted(QObject *who = NULL);
		/*! Set as canceled */
		void setAsCanceled(QObject *who = NULL);
		/*! Set as blocked */
		void setAsBlocked(QObject *who = NULL);
		/*! Set as error */
		void setAsError(QObject *who = NULL);
		/*! Set as deleted */
		void setAsDeleted(QObject *who = NULL);
		/*! Init a VideoDefinition structure */
		static void initVideoDefinition(VideoDefinition &videoDef);
};

#endif // __VIDEOITEM_H__
