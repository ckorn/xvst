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

#include "sessionmanager.h"
//
SessionManager::SessionManager(ProgramOptions *programOptions)
{
	this->programOptions = programOptions;
}

void SessionManager::saveSession(VideoListController *videoListController)
{
	if (videoListController == NULL || !programOptions->getSaveRestoreSessions()) return;

	if (QFile::exists(programOptions->getApplicationPath() + SESSION_FILE))
		QFile::remove(programOptions->getApplicationPath() + SESSION_FILE);

	QSettings settings(programOptions->getApplicationPath() + SESSION_FILE, QSettings::IniFormat);

	for (int n = 0; n < videoListController->getVideoItemCount(); n++)
	{
		VideoItem *videoItem = videoListController->getVideoItem(n);
		bool canAdd = !videoItem->isDeleted();

		if (canAdd && programOptions->getDontRememberDownloadedVideos() && videoItem->isCompleted())
			canAdd = false;

		if (canAdd || !videoItem->isCompleted())
		{
			settings.beginGroup(QString("SESSION_VIDEO_%1").arg(n));

			settings.setValue("URL", videoItem->getURL());
			settings.setValue("videoFile", videoItem->getVideoFile());
			if (videoItem->isGettingURL())
				settings.setValue("videoState", vsNULL);
			else if (videoItem->isDownloading())
				settings.setValue("videoState", vsGettedURL);
			else if (videoItem->isConverting())
				settings.setValue("videoState", vsDownloaded);
			else
				settings.setValue("videoState", videoItem->getVideoState());
			settings.setValue("videoSize", videoItem->getVideoSize());
			settings.setValue("videoProgress", videoItem->getProgress());
			settings.setValue("VIDEO_URL", videoItem->getVideoInformation().URL);
			settings.setValue("VIDEO_title", videoItem->getVideoInformation().title);
			settings.endGroup();
		}
	}
}

void SessionManager::loadSession(VideoListController *videoListController)
{
	if (videoListController == NULL ||
	    !programOptions->getSaveRestoreSessions() ||
	    !QFile::exists(programOptions->getApplicationPath() + SESSION_FILE)) return;

	QSettings settings(programOptions->getApplicationPath() + SESSION_FILE, QSettings::IniFormat);

	QStringList videos = settings.childGroups();

	for (int n = 0; n < videos.count(); n++)
	{
		VideoItem *videoItem = new VideoItem(settings.value(videos.at(n) + "/URL", "").toString());
		videoItem->setVideoFile(settings.value(videos.at(n) + "/videoFile", "").toString());
		videoItem->setState(static_cast<VideoState>(settings.value(videos.at(n) + "/videoState", 0).toInt()));
		videoItem->setVideoSize(settings.value(videos.at(n) + "/videoSize", 0).toInt());
		videoItem->setProgress(settings.value(videos.at(n) + "/videoProgress", 0).toDouble());
		VideoDefinition videoInformation;
		videoInformation.URL = settings.value(videos.at(n) + "/VIDEO_URL", "").toString();
		videoInformation.title = settings.value(videos.at(n) + "/VIDEO_title", "").toString();
		videoItem->setVideoInformation(videoInformation);
		if (videoItem->hasErrors()) videoItem->setAsReported();
		if (!videoItem->isReady() && !videoItem->isNULL() && !QFile::exists(videoItem->getVideoFile())) continue;
		videoListController->addVideo(videoItem);
	}
}

void SessionManager::addToLog(VideoItem *videoItem)
{
	if (videoItem == NULL || !programOptions->getSaveLogDownloadedVideos()) return;

	QSettings settings(programOptions->getApplicationPath() + LOG_FILE, QSettings::IniFormat);

	int lastID = settings.value("information/lastID", 0).toInt() + 1;

	settings.beginGroup("information");
	settings.setValue("lastID", lastID);
	settings.endGroup();

	settings.beginGroup(QString("historic%1").arg(lastID));
	settings.setValue("date", QDateTime::currentDateTime());
	settings.setValue("title", videoItem->getVideoInformation().title);
	settings.setValue("URL", videoItem->getURL());
	settings.endGroup();
}

void SessionManager::clearLog()
{
	if (QFile::exists(programOptions->getApplicationPath() + LOG_FILE))
		QFile::remove(programOptions->getApplicationPath() + LOG_FILE);
}

QList<LogEntry> SessionManager::getLogEntries()
{
	QList<LogEntry> results;

	QSettings settings(programOptions->getApplicationPath() + LOG_FILE, QSettings::IniFormat);

	QStringList historics = settings.childGroups();

	for (int n = 0; n < historics.count(); n++)
		if (historics.at(n).toLower() != "information")
		{
			LogEntry item;
			// set info
			item.dateTime = settings.value(historics.at(n) + "/date", item.dateTime).toDateTime();
			item.title = settings.value(historics.at(n) + "/title", item.title).toString();
			item.URL = settings.value(historics.at(n) + "/URL", item.URL).toString();
			// add it
			results << item;
		}

	return results;
}
//
