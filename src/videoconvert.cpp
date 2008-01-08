#include "videoconvert.h"

VideoConverter::VideoConverter(QString ffmpegApp, QString workingDir,
                               VideoConversionConfig convConf, bool deleteOriginalVideo)
{
	// set vars
	setFFmpegApp(ffmpegApp);
	setWorkingDir(workingDir);
	setConversionConfig(convConf);
	setDeleteOriginalVideo(deleteOriginalVideo);
	// create a new qprocess
	ffmpegProcess = new QProcess();
	// connect signals
	connect(ffmpegProcess, SIGNAL(started()), this, SLOT(started()));
	connect(ffmpegProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));
	connect(ffmpegProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
	connect(ffmpegProcess, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
	// cofnigure process
	ffmpegProcess->setWorkingDirectory(workingDir);
}

VideoConverter::~VideoConverter()
{
	delete ffmpegProcess;
}

QStringList VideoConverter::getCommandLine()
{
	QStringList parameters;
	QString extension;
	QFileInfo videoFile;

	parameters << "-i" << videoItem->getVideoFile();

	switch (convConf.outputFormat)
	{
		case ofAVI:
			parameters << "-f" << "avi" << "-vcodec" << "mpeg4";
			extension = ".avi";
			break;
		case ofWMV:
			extension = ".wmv";
			break;
		case ofMPEG1:
			parameters << "-vcodec" << "mpeg1video";
			extension = ".mpg";
			break;
		case ofMPEG2:
			parameters << "-vcodec" << "mpeg2video";
			extension = ".mpeg";
			break;
		case ofAppleiPod:
			extension = "";
			break;
		case of3GP:
			extension = ".3gp";
			break;
		case ofMP3:
			extension = ".mp3";
			break;
	}

	switch (convConf.videoResolution)
	{
		case vrOriginal:
			break;
		case vr96x72:
			parameters << "-s" << "96x72";
			break;
		case vr128x96:
			parameters << "-s" << "128x96";
			break;
		case vr160x120:
			parameters << "-s" << "160x120";
			break;
		case vr176x120:
			parameters << "-s" << "176x120";
			break;
		case vr176x144:
			parameters << "-s" << "176x144";
			break;
		case vr192x144:
			parameters << "-s" << "192x144";
			break;
		case vr240x180:
			parameters << "-s" << "240x180";
			break;
		case vr320x200:
			parameters << "-s" << "320x200";
			break;
		case vr320x240:
			parameters << "-s" << "320x240";
			break;
		case vr352x240:
			parameters << "-s" << "352x240";
			break;
		case vr352x288:
			parameters << "-s" << "352x288";
			break;
		case vr480x272:
			parameters << "-s" << "480x272";
			break;
		case vr480x360:
			parameters << "-s" << "480x360";
			break;
		case vr480x480:
			parameters << "-s" << "480x480";
			break;
		case vr624x352:
			parameters << "-s" << "624x352";
			break;
		case vr640x480:
			parameters << "-s" << "640x480";
			break;
		case vr720x480:
			parameters << "-s" << "720x480";
			break;
		case vr720x576:
			parameters << "-s" << "720x576";
			break;
	}

	if (convConf.videoFrameRate == vfr15)
		parameters << "-r" << "15";
	else if (convConf.videoFrameRate == vfr24)
		parameters << "-r" << "24";
	else if (convConf.videoFrameRate == vfr25)
		parameters << "-r" << "25";
	else if (convConf.videoFrameRate == vfr29_97)
		parameters << "-r" << "29.97";
	else if (convConf.videoFrameRate == vfr30)
		parameters << "-r" << "30";

	switch (convConf.audioSampleRatio)
	{
		case asrOriginal:
			break;
		case asr22050:
			parameters << "-ar" << "22050";
			break;
		case asr44100:
			parameters << "-ar" << "44100";
			break;
	}

	switch (convConf.outputQuality)
	{
		case oqLower_quality:
			parameters << "-b" << "384k" << "-ab" << "64k";
			break;
		case oqLow_quality:
			parameters << "-b" << "512k" << "-ab" << "80k";
			break;
		case oqNormal_quality:
			parameters << "-b" << "640k" << "-ab" << "96k";
			break;
		case oqMedium_quality:
			parameters << "-b" << "800k" << "-ab" << "96k";
			break;
		case oqGood_quality:
			parameters << "-b" << "1000k" << "-ab" << "128k";
			break;
		case oqSuperb_quality:
			parameters << "-b" << "1200k" << "-ab" << "128k";
			break;
		case oqSame_quality:
			break;
	}

	videoItem->setVideoFileSavedTo(uniqueFileName(changeFileExt(videoItem->getVideoFile(), extension)).absoluteFilePath(), this);

	parameters << "-y" << videoItem->getVideoFileSavedTo();

	return parameters;
}

void VideoConverter::setVideoDuration(QString strVideoDuration)
{
	if (strVideoDuration == "N/A") return;
	// extrat h-m-s
	QStringList hms = strVideoDuration.split(":");
	// get hours
	videoLength = hms.at(0).toFloat() * 3600;
	// get minutes
	videoLength = videoLength + hms.at(1).toFloat() * 60;
	// get seconds
	videoLength = videoLength + hms.at(2).toFloat();
}

void VideoConverter::getCurrentTimeConversion(QString strFrame)
{
	// get the current progress
	float time = strFrame.toFloat();

	videoItem->setProgress(calculePercent(time, videoLength), this);

	emit videoItemUpdated(videoItem);
}

void VideoConverter::parseOutput(QString output)
{
	if (videoLength == 0)
	{
		if (output.indexOf("Duration: ") > -1)
		{
			setVideoDuration(copyBetween(output, "Duration: ", ", start"));
			// we have the durtion? if yes, then capture the first step
			if (videoLength != 0)
				getCurrentTimeConversion(copyBetween(output, "time=", " bitrate"));
		}
	}
	else // get the current frame
		getCurrentTimeConversion(copyBetween(output, "time=", " bitrate"));
}

void VideoConverter::startConversionVideo(VideoItem *videoItem)
{
	if (videoItem == NULL || !canStartConversion() || !ffmpegInstalled()) return;
	// assign data
	this->videoItem = videoItem;
	videoItem->lock (this);
	videoItem->setProgress(0, this);
	// init internal data
	videoLength = 0;
	// exec ffmpeg
	ffmpegProcess->start(ffmpegApp, getCommandLine());
}

void VideoConverter::cancelConversion()
{
	if (isConverting())
	{
		ffmpegProcess->kill();
		ffmpegProcess->waitForFinished();
	}
}

bool VideoConverter::canStartConversion()
{
	return !isConverting();
}

bool VideoConverter::isConverting()
{
	return ffmpegProcess->state() == QProcess::Running;
}

bool VideoConverter::ffmpegInstalled()
{
	return QFile::exists(ffmpegApp);
}

QString VideoConverter::getFFmpegApp()
{
	return ffmpegApp;
}

QString VideoConverter::getWorkingDir()
{
	return workingDir;
}

VideoConversionConfig VideoConverter::getConversionConfig()
{
	return convConf;
}

bool VideoConverter::getDeleteOriginalVideo()
{
	return deleteOriginalVideo;
}

void VideoConverter::setFFmpegApp(QString ffmpegApp)
{
	this->ffmpegApp = ffmpegApp;
}

void VideoConverter::setWorkingDir(QString workingDir)
{
	this->workingDir = workingDir;
}

void VideoConverter::setConversionConfig(VideoConversionConfig convConf)
{
	this->convConf = convConf;
}

void VideoConverter::setDeleteOriginalVideo(bool deleteOriginalVideo)
{
	this->deleteOriginalVideo = deleteOriginalVideo;
}

void VideoConverter::started()
{
	videoItem->setAsConverting(this);
	emit conversionStarted(videoItem);
}

void VideoConverter::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
	if (QFile::exists(videoItem->getVideoFileSavedTo()))
	{
		if (deleteOriginalVideo)
			QFile::remove(videoItem->getVideoFile());
	}
	else
		videoItem->setVideoFileSavedTo(videoItem->getVideoFile(), this);

	videoItem->setAsConverted(this);
	videoItem->unlock(this);

	emit conversionFinished(videoItem);
}

void VideoConverter::readyReadStandardOutput()
{
	parseOutput(QString::fromLocal8Bit(ffmpegProcess->readAllStandardOutput()));
}

void VideoConverter::readyReadStandardError()
{
	parseOutput(QString::fromLocal8Bit(ffmpegProcess->readAllStandardError()));
}
