#include "videodownload.h"

VideoDownload::VideoDownload(QString saveTo)
{
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
	if (videoItem == NULL || !canStartDownload()) return;
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
}
