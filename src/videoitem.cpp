#include "videoitem.h"

int VideoItem::internalID = 0;

VideoItem::VideoItem()
{
	locker = NULL;
	assignID();
	// init data
	initData();
}

VideoItem::VideoItem(const QString URL)
{
	locker = NULL;
	assignID();
	// init data
	initData();
	// set initial data
	this->URL = URL;
}

void VideoItem::assign(VideoItem *videoItem)
{
	URL = videoItem->getURL();
	videoFile = videoItem->getVideoFile();
	videoFileSavedTo = videoItem->getVideoFileSavedTo();
	progress = videoItem->getProgress();
	videoSize = videoItem->getVideoSize();
	downloadSpeed = videoItem->getDownloadSpeed();
	timeRemaining = videoItem->getTimeRemaining();
	videoState = videoItem->getVideoState();
	videoInfo.URL = videoItem->getVideoInformation().URL;
	videoInfo.title = videoItem->getVideoInformation().title;
}

void VideoItem::initData()
{
	URL = "";
	videoFile = "";
	videoFileSavedTo = "";
	progress = 0;
	videoSize = 0;
	downloadSpeed = 0;
	timeRemaining = 0;
	videoState = vsNULL;
	videoInfo.URL = "";
	videoInfo.title = "";
}

void VideoItem::assignID()
{
	internalID += 1;
	ID = internalID;
}

bool VideoItem::lock (QObject *locker)
{
	if (!isLocked())
	{
		this->locker = locker;
		return true;
	}
	else
		return false;
}

bool VideoItem::unlock(QObject *locker)
{
	if (isLocked())
		if (this->locker == locker)
		{
			this->locker = NULL;
			return true;
		}
		else
			return false;
	else
		return false;
}

bool VideoItem::isLocked()
{
	return locker != NULL;
}

VideoState VideoItem::getVideoState()
{
	return videoState;
}

QString VideoItem::getVideoStateAsString()
{
	switch (videoState)
	{
		case vsNULL:
			return tr("-");
		case vsGettingURL:
			return tr("Getting info...");
		case vsGettedURL:
			return tr("Ready");
		case vsDownloading:
			return tr("Downloading...");
		case vsDownloaded:
			return tr("Downloaded");
		case vsConverting:
			return tr("Converting...");
		case vsConverted:
			return tr("Converted");
		case vsCompleted:
			return tr("Completed");
		case vsCanceled:
			return tr("Canceled");
		case vsBlocked:
			return tr("Blocked");
		case vsError:
			return tr("Error");
		case vsDeleted:
			return tr("Deleted");
	}
	// default value
	return "-";
}

bool VideoItem::isNULL()
{
	return videoState == vsNULL;
}

bool VideoItem::isReady()
{
	return videoState == vsGettedURL;
}

bool VideoItem::isGettingURL()
{
	return videoState == vsGettingURL;
}

bool VideoItem::isGettedURL()
{
	return videoState == vsGettedURL;
}

bool VideoItem::isDownloading()
{
	return videoState == vsDownloading;
}

bool VideoItem::isDownloaded()
{
	return videoState == vsDownloaded;
}

bool VideoItem::isConverted()
{
	return videoState == vsConverted;
}

bool VideoItem::isConverting()
{
	return videoState == vsConverting;
}

bool VideoItem::isCompleted()
{
	return videoState == vsCompleted;
}

bool VideoItem::isBlocked()
{
	return videoState == vsBlocked;
}

bool VideoItem::hasErrors()
{
	return videoState == vsError;
}

bool VideoItem::isBussy()
{
	return isGettingURL() || isDownloading() || isConverting();
}

bool VideoItem::isRemovable()
{
	return !isBussy();
}

bool VideoItem::isCanceled()
{
	return videoState == vsCanceled;
}

bool VideoItem::isDeleted()
{
	return videoState == vsDeleted;
}

int VideoItem::getID()
{
	return ID;
}

QString VideoItem::getDisplayLabel()
{
	if (videoInfo.title.isEmpty())
		return URL;
	else
		return videoInfo.title;
}

QString VideoItem::getDisplaySize()
{
	if (videoSize == 0)
		return "-";
	else
		return fileSizeToString(videoSize);
}

QString VideoItem::getDisplayProgress()
{
	if (isDownloading() || isConverting())
		return floatToStr(progress) + "%";
	else
		return getVideoStateAsString();
}

QString VideoItem::getDisplayTimeRemaining()
{
	if (isDownloading())
		return secondsToHMS(timeRemaining, false);
	else
		return "-";
}

QString VideoItem::getDisplayDownloadSpeed()
{
	if (isDownloading())
		return fileSizeToString(downloadSpeed) + tr("/sec");
	else
		return "-";
}

VideoDefinition VideoItem::getVideoInformation()
{
	return videoInfo;
}

QString VideoItem::getURL()
{
	return URL;
}

QString VideoItem::getVideoFile()
{
	return videoFile;
}

QString VideoItem::getVideoFileSavedTo()
{
	return videoFileSavedTo;
}

float VideoItem::getProgress()
{
	return progress;
}

int VideoItem::getVideoSize()
{
	return videoSize;
}

int VideoItem::getDownloadSpeed()
{
	return downloadSpeed;
}

int VideoItem::getTimeRemaining()
{
	return timeRemaining;
}

void VideoItem::setVideoInformation(VideoDefinition videoInformation, QObject *who)
{
	if (isLocked() && who != locker) return;
	// set value
	this->videoInfo = videoInformation;
	// convert the html title to human title
	this->videoInfo.title = htmlToStr(this->videoInfo.title);
}

void VideoItem::setVideoFile(QString videoFile, QObject *who)
{
	if (isLocked() && who != locker) return;
	// set value
	this->videoFile = videoFile;
}

void VideoItem::setVideoFileSavedTo(QString videoFileSavedTo, QObject *who)
{
	if (isLocked() && who != locker) return;
	// set value
	this->videoFileSavedTo = videoFileSavedTo;
}

void VideoItem::setProgress(float progress, QObject *who)
{
	if (isLocked() && who != locker) return;
	// check value
	if (progress < 0)
		progress = 0;
	else if (progress > 100)
		progress = 100;
	// set value
	this->progress = progress;
}

void VideoItem::setVideoSize(int videoSize, QObject *who)
{
	if (isLocked() && who != locker) return;
	// check value
	if (videoSize < 0)
		videoSize = 0;
	// set value
	this->videoSize = videoSize;
}

void VideoItem::setDownloadSpeed(int downloadSpeed, QObject *who)
{
	if (isLocked() && who != locker) return;
	// check value
	if (downloadSpeed < 0)
		downloadSpeed = 0;
	// set value
	this->downloadSpeed = downloadSpeed;
}

void VideoItem::setTimeRemaining(int timeRemaining, QObject *who)
{
	if (isLocked() && who != locker) return;
	// check value
	if (timeRemaining < 0)
		timeRemaining = 0;
	// set value
	this->timeRemaining = timeRemaining;
}

void VideoItem::setState(VideoState videoState, QObject *who)
{
	if (isLocked() && who != locker) return;
	this->videoState = videoState;
}

void VideoItem::setAsNULL(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsNULL;
}

void VideoItem::setAsReady(QObject *who)
{
	setAsGettedURL(who);
}

void VideoItem::setAsGettingURL(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsGettingURL;
}

void VideoItem::setAsGettedURL(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsGettedURL;
}

void VideoItem::setAsDownloading(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsDownloading;
}

void VideoItem::setAsDownloaded(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsDownloaded;
}

void VideoItem::setAsConverting(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsConverting;
}

void VideoItem::setAsConverted(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsConverted;
}

void VideoItem::setAsCompleted(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsCompleted;
}

void VideoItem::setAsCanceled(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsCanceled;
}

void VideoItem::setAsBlocked(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsBlocked;
}

void VideoItem::setAsError(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsError;
}

void VideoItem::setAsDeleted(QObject *who)
{
	if (isLocked() && who != locker) return;
	videoState = vsDeleted;
}

void VideoItem::initVideoDefinition(VideoDefinition &videoDef)
{
	videoDef.URL = "";
	videoDef.title = "";
}
