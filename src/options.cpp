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
	
	if (!QFile::exists(optionsFile)) return;
	
	// load config
	QSettings settings(optionsFile, QSettings::IniFormat);
	downloadAutomatically = settings.value("configuration/downloadAutomatically", downloadAutomatically).toBool();
	downloadDir = settings.value("configuration/downloadDir", downloadDir).toString();
	convertVideos = settings.value("configuration/convertVideos", convertVideos).toBool();
	deleteVideosOnConvert = settings.value("configuration/deleteVideosOnConvert", deleteVideosOnConvert).toBool();
	ffmpegLibLocation = settings.value("configuration/ffmpegLibLocation", ffmpegLibLocation).toString();
	displayPopup = settings.value("configuration/displayPopup", displayPopup).toBool();

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

	maxActiveDownloads = settings.value("configuration/maxActiveDownloads", maxActiveDownloads).toInt();

	useProxy = settings.value("configuration/useProxy", useProxy).toBool();
	proxyAdress = settings.value("configuration/proxyAdress", proxyAdress).toString();
	proxyPassword = settings.value("configuration/proxyPassword", proxyPassword).toString();
	proxyUserName = settings.value("configuration/proxyUserName", proxyUserName).toString();
	proxyPort = settings.value("configuration/proxyPort", proxyPort).toInt();
	proxyType = settings.value("configuration/proxyType", proxyType).toInt();

	languageFile = settings.value("configuration/languageFile", languageFile).toString();

	installAutomaticallyUpdates = settings.value("configuration/installAutomaticallyUpdates", installAutomaticallyUpdates).toBool();
	lastUpdate = settings.value("configuration/lastUpdate", lastUpdate).toDate();
	checkForUpdatesOnStartup = settings.value("configuration/checkForUpdatesOnStartup", checkForUpdatesOnStartup).toBool();
	checkForUpdatesEvery = settings.value("configuration/checkForUpdatesEvery", checkForUpdatesEvery).toInt();

	displayBugReport = settings.value("configuration/displayBugReport", displayBugReport).toBool();

	firstTime = settings.value("configuration/firstTime", firstTime).toBool();

	stayOnTop = settings.value("configuration/stayOnTop", stayOnTop).toBool();
	minimizeToSystemTray = settings.value("configuration/minimizeToSystemTray", minimizeToSystemTray).toBool();

	mainWindowHeight = settings.value("configuration/mainWindowHeight", mainWindowHeight).toInt();
	mainWindowWidth = settings.value("configuration/mainWindowWidth", mainWindowWidth).toInt();
	mainWinowMaximized = settings.value("configuration/mainWinowMaximized", mainWinowMaximized).toBool();

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
	settings.setValue("displayPopup", displayPopup);

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

	settings.setValue("maxActiveDownloads", maxActiveDownloads);

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

	settings.setValue("installAutomaticallyUpdates", installAutomaticallyUpdates);
	settings.setValue("lastUpdate", lastUpdate);
	settings.setValue("checkForUpdatesOnStartup", checkForUpdatesOnStartup);
	settings.setValue("checkForUpdatesEvery", checkForUpdatesEvery);
	
	settings.setValue("displayBugReport", displayBugReport);

	settings.setValue("firstTime", firstTime);

	settings.setValue("stayOnTop", stayOnTop);
	settings.setValue("minimizeToSystemTray", minimizeToSystemTray);

	settings.setValue("mainWindowHeight", mainWindowHeight);
	settings.setValue("mainWindowWidth", mainWindowWidth);
	settings.setValue("mainWinowMaximized", mainWinowMaximized);

	settings.endGroup();

	emit optionsSaveAfter();
}

void ProgramOptions::setDefault()
{
	downloadAutomatically = true;
#ifdef Q_WS_MAC
	downloadDir = QString(QDir::homePath() + DEFAULT_DOWNLOADS);
#else
#ifdef Q_OS_LINUX
	QString _homeDirectory  = getenv("HOME");
	downloadDir = QString(_homeDirectory + DEFAULT_DOWNLOADS);
	// modification made by "AzalSup"
#else
	downloadDir = QString(appDir.absolutePath() + DEFAULT_DOWNLOADS);
#endif
#endif
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

	maxActiveDownloads = 4;

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
	
	installAutomaticallyUpdates = true;
	lastUpdate = QDate(2007, 01, 01);
	checkForUpdatesOnStartup = true;
	checkForUpdatesEvery = 1;
	
	displayBugReport = true;
	
	firstTime = true;

	stayOnTop = false;
	minimizeToSystemTray = false;

	mainWindowHeight = 0;
	mainWindowWidth = 0;
	mainWinowMaximized = false;
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

void ProgramOptions::setMaxActiveDownloads(int value)
{
	maxActiveDownloads = value;
}

int ProgramOptions::getMaxActiveDownloads()
{
	return maxActiveDownloads;
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

int ProgramOptions::getCheckForUpdatesEvery()
{
	return checkForUpdatesEvery;
}

void ProgramOptions::setCheckForUpdatesEvery(int value)
{
	checkForUpdatesEvery = value;
}

bool ProgramOptions::getCheckForUpdatesOnStartup()
{
	return checkForUpdatesOnStartup;
}

void ProgramOptions::setCheckForUpdatesOnStartup(bool value)
{
	checkForUpdatesOnStartup = value;
}

QDate ProgramOptions::getLastUpdate()
{
	return lastUpdate;
}

void ProgramOptions::setLastUpdate(QDate value)
{
	lastUpdate = value;
}

bool ProgramOptions::getInstallAutomaticallyUpdates()
{
	return installAutomaticallyUpdates;
}

void ProgramOptions::setInstallAutomaticallyUpdates(bool value)
{
	installAutomaticallyUpdates = value;
}

bool ProgramOptions::getDisplayBugReport()
{
	return displayBugReport;
}

void ProgramOptions::setDisplayBugReport(bool value)
{
	displayBugReport = value;
}

bool ProgramOptions::getFirstTime()
{
	return firstTime;
}

void ProgramOptions::setFirstTime(bool value)
{
	firstTime = value;
}

bool ProgramOptions::getStayOnTop()
{
	return stayOnTop;
}

void ProgramOptions::setStayOnTop(bool value)
{
	stayOnTop = value;
}

void ProgramOptions::setMinimizeToSystemTray(bool value)
{
	minimizeToSystemTray = value;
}

bool ProgramOptions::getMinimizeToSystemTray()
{
	return minimizeToSystemTray;
}

void ProgramOptions::setMainWindowHeight(int value)
{
	mainWindowHeight = value;
}

int ProgramOptions::getMainWindowHeight()
{
	return mainWindowHeight;
}

void ProgramOptions::setMainWindowWidth(int value)
{
	mainWindowWidth = value;
}

int ProgramOptions::getMainWindowWidth()
{
	return mainWindowWidth;
}

void ProgramOptions::setMainWinowMaximized(bool value)
{
	mainWinowMaximized = value;
}

bool ProgramOptions::getMainWinowMaximized()
{
	return mainWinowMaximized;
}
