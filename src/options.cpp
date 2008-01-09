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

#include "options.h"

ProgramOptions::ProgramOptions(QString optionsPath)
{
	appDir.setPath(optionsPath);
	optionsFile = QString(optionsPath + "/config.conf");
	canSendUpdateSignal = true;
}

ProgramOptions::~ProgramOptions()
{
	save();
}

void ProgramOptions::load()
{
	emit optionsLoadBefore();

	// set the default values
	setDefault();
	// load config
	QSettings settings(optionsFile, QSettings::IniFormat);
	downloadAutomatically = settings.value("configuration/downloadAutomatically", downloadAutomatically).toBool();
	downloadDir = settings.value("configuration/downloadDir", downloadDir).toString();
	convertVideos = settings.value("configuration/convertVideos", convertVideos).toBool();
	deleteVideosOnConvert = settings.value("configuration/deleteVideosOnConvert", deleteVideosOnConvert).toBool();
	ffmpegLibLocation = settings.value("configuration/ffmpegLibLocation", ffmpegLibLocation).toString();

	conversionConf.outputFormat = static_cast<OutputFormat>(settings.value("configuration/outputFormat", conversionConf.outputFormat).toInt());
	conversionConf.videoResolution = static_cast<VideoResolution>(settings.value("configuration/videoResolution", conversionConf.videoResolution).toInt());
	conversionConf.audioSampleRatio = static_cast<AudioSampleRatio>(settings.value("configuration/audioSampleRatio", conversionConf.audioSampleRatio).toInt());
	conversionConf.videoFrameRate = static_cast<VideoFrameRate>(settings.value("configuration/videoFrameRate", conversionConf.videoFrameRate).toInt());
	conversionConf.outputQuality = static_cast<OutputQuality>(settings.value("configuration/outputQuality", conversionConf.outputQuality).toInt());

	saveRestoreSessions = settings.value("configuration/saveRestoreSessions", saveRestoreSessions).toBool();
	saveLogDownloadedVideos = settings.value("configuration/saveLogDownloadedVideos", saveLogDownloadedVideos).toBool();
	dontRememberDownloadedVideos = settings.value("configuration/dontRememberDownloadedVideos", dontRememberDownloadedVideos).toBool();

	blockAdultContent = settings.value("configuration/blockAdultContent", blockAdultContent).toBool();
	blockedWebsitesList = settings.value("configuration/blockedWebsitesList", blockAdultContent).toString();

	dragDropLeft = settings.value("configuration/dragDropLeft", dragDropTop).toInt();
	dragDropTop = settings.value("configuration/dragDropTop", dragDropTop).toInt();
	dragDropAlphaBlendValue = settings.value("configuration/dragDropAlphaBlendValue", dragDropAlphaBlendValue).toInt();
	dragDropAlphaBlend = settings.value("configuration/dragDropAlphaBlend", dragDropAlphaBlend).toBool();

	useProxy = settings.value("configuration/useProxy", useProxy).toBool();
	proxyAdress = settings.value("configuration/proxyAdress", proxyAdress).toString();
	proxyPassword = settings.value("configuration/proxyPassword", proxyPassword).toString();
	proxyUserName = settings.value("configuration/proxyUserName", proxyUserName).toString();
	proxyPort = settings.value("configuration/proxyPort", proxyPort).toInt();
	proxyType = settings.value("configuration/proxyType", proxyType).toInt();

	languageFile = settings.value("configuration/languageFile", languageFile).toString();

	emit optionsLoadAfter();
}

void ProgramOptions::save()
{
	emit optionsSaveBefore();

	QSettings settings(optionsFile, QSettings::IniFormat);

	settings.beginGroup("configuration");
	settings.setValue("downloadAutomatically", downloadAutomatically);
	settings.setValue("downloadDir", downloadDir);
	settings.setValue("convertVideos", convertVideos);
	settings.setValue("deleteVideosOnConvert", deleteVideosOnConvert);
	settings.setValue("ffmpegLibLocation", ffmpegLibLocation);

	settings.setValue("outputFormat", conversionConf.outputFormat);
	settings.setValue("videoResolution", conversionConf.videoResolution);
	settings.setValue("audioSampleRatio", conversionConf.audioSampleRatio);
	settings.setValue("videoFrameRate", conversionConf.videoFrameRate);
	settings.setValue("outputQuality", conversionConf.outputQuality);

	settings.setValue("saveRestoreSessions", saveRestoreSessions);
	settings.setValue("saveLogDownloadedVideos", saveLogDownloadedVideos);
	settings.setValue("dontRememberDownloadedVideos", dontRememberDownloadedVideos);

	settings.setValue("blockAdultContent", blockAdultContent);
	settings.setValue("blockedWebsitesList", blockedWebsitesList);

	settings.setValue("useProxy", useProxy);
	settings.setValue("proxyAdress", proxyAdress);
	settings.setValue("proxyPassword", proxyPassword);
	settings.setValue("proxyUserName", proxyUserName);
	settings.setValue("proxyPort", proxyPort);
	settings.setValue("proxyType", proxyType);

	settings.setValue("dragDropLeft", dragDropLeft);
	settings.setValue("dragDropTop", dragDropTop);
	settings.setValue("dragDropAlphaBlendValue", dragDropAlphaBlendValue);
	settings.setValue("dragDropAlphaBlend", dragDropAlphaBlend);

	settings.setValue("languageFile", languageFile);

	settings.endGroup();

	emit optionsSaveAfter();
}

void ProgramOptions::setDefault()
{
	downloadAutomatically = true;
	downloadDir = QString(appDir.absolutePath() + DEFAULT_DOWNLOADS);
	convertVideos = true;
	deleteVideosOnConvert = true;
#ifdef Q_OS_WIN32
	ffmpegLibLocation = QString(appDir.absolutePath() + DEFAULT_FFMPEGLIB + "/ffmpeg.exe");
#else
	ffmpegLibLocation = QString(DEFAULT_FFMPEGLIB + "/ffmpeg");
#endif
	displayPopup = true;

	conversionConf.outputFormat = ofAVI;
	conversionConf.videoResolution = vrOriginal;
	conversionConf.audioSampleRatio = asrOriginal;
	conversionConf.videoFrameRate = vfrOriginal;
	conversionConf.outputQuality = oqSame_quality;

	blockAdultContent = false;
	blockedWebsitesList = "";

	saveRestoreSessions = true;
	saveLogDownloadedVideos = true;
	dontRememberDownloadedVideos = true;

	useProxy = false;
	proxyAdress = "";
	proxyPassword = "";
	proxyUserName = "";
	proxyPort = 0;
	proxyType = QNetworkProxy::NoProxy;
	
	dragDropLeft = -1;
	dragDropTop = -1;
	dragDropAlphaBlendValue = 100;
	dragDropAlphaBlend = false;
	
	languageFile = "english_uk.language";
}

void ProgramOptions::setCanSendUpdateSignal(bool canSendUpdateSignal)
{
	this->canSendUpdateSignal = canSendUpdateSignal;
}

QString ProgramOptions::getApplicationPath()
{
	return appDir.absolutePath();
}

void ProgramOptions::sendUpdateSignal()
{
	if (canSendUpdateSignal)
		emit optionsUpdated();
}

QString ProgramOptions::getDownloadDir()
{
	return QDir::toNativeSeparators(downloadDir);
}

void ProgramOptions::setDownloadDir(QString value)
{
	downloadDir = value;

	sendUpdateSignal();
}

bool ProgramOptions::getDownloadAutomatically()
{
	return downloadAutomatically;
}

void ProgramOptions::setDownloadAutomatically(bool value)
{
	downloadAutomatically = value;

	sendUpdateSignal();
}

bool ProgramOptions::getConvertVideos()
{
	return convertVideos;
}

void ProgramOptions::setConvertVideos(bool value)
{
	convertVideos = value;

	sendUpdateSignal();
}

bool ProgramOptions::getDeleteVideosOnConvert()
{
	return deleteVideosOnConvert;
}

void ProgramOptions::setDeleteVideosOnConvert(bool value)
{
	deleteVideosOnConvert = value;

	sendUpdateSignal();
}

QString ProgramOptions::getFfmpegLibLocation()
{
	return QDir::toNativeSeparators(ffmpegLibLocation);
}

void ProgramOptions::setFfmpegLibLocation(QString value)
{
	ffmpegLibLocation = value.trimmed();

	sendUpdateSignal();
}

VideoConversionConfig ProgramOptions::getConversionConf()
{
	return conversionConf;
}

void ProgramOptions::setConversionConf(VideoConversionConfig value)
{
	conversionConf = value;

	sendUpdateSignal();
}

bool ProgramOptions::getDisplayPopup()
{
	return displayPopup;
}

void ProgramOptions::setDisplayPopup(bool value)
{
	displayPopup = value;

	sendUpdateSignal();
}

bool ProgramOptions::getSaveRestoreSessions()
{
	return saveRestoreSessions;
}

void ProgramOptions::setSaveRestoreSessions(bool value)
{
	saveRestoreSessions = value;
}

bool ProgramOptions::getDontRememberDownloadedVideos()
{
	return dontRememberDownloadedVideos;
}

void ProgramOptions::setDontRememberDownloadedVideos(bool value)
{
	dontRememberDownloadedVideos = value;
}

bool ProgramOptions::getSaveLogDownloadedVideos()
{
	return saveLogDownloadedVideos;
}

void ProgramOptions::setSaveLogDownloadedVideos(bool value)
{
	saveLogDownloadedVideos = value;
}

bool ProgramOptions::getBlockAdultContent()
{
	return blockAdultContent;
}

void ProgramOptions::setBlockAdultContent(bool value)
{
	blockAdultContent = value;
}

QString ProgramOptions::getBlockedWebsitesList()
{
	return blockedWebsitesList;
}

void ProgramOptions::setBlockedWebsitesList(QString value)
{
	blockedWebsitesList = value;
}

bool ProgramOptions::getUseProxy()
{
	return useProxy;
}

void ProgramOptions::setUseProxy(bool value)
{
	useProxy = value;
}

QString ProgramOptions::getProxyUserName()
{
	return proxyUserName;
}

void ProgramOptions::setProxyUserName(QString value)
{
	proxyUserName = value;
}

QString ProgramOptions::getProxyPassword()
{
	return proxyPassword;
}

void ProgramOptions::setProxyPassword(QString value)
{
	proxyPassword = value;
}

QString ProgramOptions::getProxyAdress()
{
	return proxyAdress;
}

void ProgramOptions::setProxyAdress(QString value)
{
	proxyAdress = value;
}

int ProgramOptions::getProxyPort()
{
	return proxyPort;
}

void ProgramOptions::setProxyPort(int value)
{
	proxyPort = value;
}

int ProgramOptions::getProxyType()
{
	return proxyType;
}

void ProgramOptions::setProxyType(int value)
{
	proxyType = value;
}

int ProgramOptions::getDragDropLeft()
{
	return dragDropLeft;
}

void ProgramOptions::setDragDropLeft(int value)
{
	dragDropLeft = value;
}

int ProgramOptions::getDragDropTop()
{
	return dragDropTop;
}

void ProgramOptions::setDragDropTop(int value)
{
	dragDropTop = value;
}

bool ProgramOptions::getDragDropAlphaBlend()
{
	return dragDropAlphaBlend;
}

void ProgramOptions::setDragDropAlphaBlend(bool value)
{
	dragDropAlphaBlend = value;
}

int ProgramOptions::getDragDropAlphaBlendValue()
{
	return dragDropAlphaBlendValue;
}

void ProgramOptions::setDragDropAlphaBlendValue(int value)
{
	dragDropAlphaBlendValue = value;
}

QString ProgramOptions::getLanguageFile(bool fullPath)
{
	return fullPath ? QDir::toNativeSeparators(appDir.absolutePath() + "/languages/" + languageFile) : languageFile;
}

void ProgramOptions::setLanguageFile(QString value)
{
	languageFile = value;
}
