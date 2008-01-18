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

#ifndef __VIDEOINFORMATION_H__
#define __VIDEOINFORMATION_H__

#include <QtGui>
//
#include "videoitem.h"
#include "http.h"

enum BlockedState
{
	bsNotBlocked,
	bsAdultContent,
	bsBlocked
};

class VideoInformation;

class VideoInformation_plugin : public QObject
{
Q_OBJECT
	protected:
		QString ID;			//<! Plugin id (and unique)
		QString caption;	//<! Plugin caption
		bool adultContent;	//<! Flag for know if have adult content
		/*! Regist the current plugin */
		void registPlugin(VideoInformation *videoInformation);
		/*! Set the plugin identifier (the ID is saved as lowercase) */
		void setID(QString ID);
		/*! Set the plugin caption */
		void setCaption(QString caption);
	public:
		/*! Check if the ID passed by parameters is like the stored plugin ID */
		virtual bool isLikeMyId(QString ID);
		/*! Abstract function of getVideoInformation, this function is implemented in sub-classes */
		virtual VideoDefinition getVideoInformation(const QString URL);
		/*! Get the plugin ID */
		QString getID();
		/*! Get the plugin caption */
		QString getCaption();
		/*! Get if this service has adult content (+18) */
		bool hasAdultContent();
};

/*! Plugin for YouTube Videos */
class VideoInformation_Youtube : public VideoInformation_plugin
{
	public:
		/*! Youtube plugin class constructor */
		VideoInformation_Youtube(VideoInformation *videoInformation);
		/*! Get the full Video Information of Youtube Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Metacafe Videos */
class VideoInformation_Metacafe : public VideoInformation_plugin
{
	public:
		/*! Metacafe plugin class constructor */
		VideoInformation_Metacafe(VideoInformation *videoInformation);
		/*! Get the full Video Information of Metacafe Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Google Videos */
class VideoInformation_Google : public VideoInformation_plugin
{
	public:
		/*! Google plugin class constructor */
		VideoInformation_Google(VideoInformation *videoInformation);
		/*! Get the full Video Information of Google Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for ZappInternet Videos */
class VideoInformation_ZappInternet : public VideoInformation_plugin
{
	public:
		/*! ZappInternet plugin class constructor */
		VideoInformation_ZappInternet(VideoInformation *videoInformation);
		/*! Get the full Video Information of ZappInternet Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Dailymotion Videos */
class VideoInformation_Dailymotion : public VideoInformation_plugin
{
	public:
		/*! Dailymotion plugin class constructor */
		VideoInformation_Dailymotion(VideoInformation *videoInformation);
		/*! Get the full Video Information of Dailymotion Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Dumpalink Videos */
class VideoInformation_Dumpalink : public VideoInformation_plugin
{
	public:
		/*! Dumpalink plugin class constructor */
		VideoInformation_Dumpalink(VideoInformation *videoInformation);
		/*! Get the full Video Information of Dumpalink Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Glumbert Videos */
class VideoInformation_Glumbert : public VideoInformation_plugin
{
	public:
		/*! Glumbert plugin class constructor */
		VideoInformation_Glumbert(VideoInformation *videoInformation);
		/*! Get the full Video Information of Glumbert Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Sclipo Videos */
class VideoInformation_Sclipo : public VideoInformation_plugin
{
	public:
		/*! Sclipo plugin class constructor */
		VideoInformation_Sclipo(VideoInformation *videoInformation);
		/*! Get the full Video Information of Sclipo Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Lulu TV Videos */
class VideoInformation_LuluTV : public VideoInformation_plugin
{
	public:
		/*! Lulu TV plugin class constructor */
		VideoInformation_LuluTV(VideoInformation *videoInformation);
		/*! Get the full Video Information of Lulu TV Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for LiveVideo Videos */
class VideoInformation_LiveVideo : public VideoInformation_plugin
{
	public:
		/*! LiveVideo plugin class constructor */
		VideoInformation_LiveVideo(VideoInformation *videoInformation);
		/*! Get the full Video Information of LiveVideo Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Yikers Videos */
class VideoInformation_Yikers : public VideoInformation_plugin
{
	public:
		/*! Yikers plugin class constructor */
		VideoInformation_Yikers(VideoInformation *videoInformation);
		/*! Get the full Video Information of Yikers Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for 123video Videos */
class VideoInformation_123video : public VideoInformation_plugin
{
	public:
		/*! 123video plugin class constructor */
		VideoInformation_123video(VideoInformation *videoInformation);
		/*! Get the full Video Information of 123video Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for 5min Videos */
class VideoInformation_5min : public VideoInformation_plugin
{
	public:
		/*! 5min plugin class constructor */
		VideoInformation_5min(VideoInformation *videoInformation);
		/*! Get the full Video Information of 5min Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for aBum Videos */
class VideoInformation_aBum : public VideoInformation_plugin
{
	public:
		/*! aBum plugin class constructor */
		VideoInformation_aBum(VideoInformation *videoInformation);
		/*! Get the full Video Information of aBum Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Aniboom Videos */
class VideoInformation_Aniboom : public VideoInformation_plugin
{
	public:
		/*! Aniboom plugin class constructor */
		VideoInformation_Aniboom(VideoInformation *videoInformation);
		/*! Get the full Video Information of Aniboom Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Bebo Videos */
class VideoInformation_Bebo : public VideoInformation_plugin
{
	public:
		/*! Bebo plugin class constructor */
		VideoInformation_Bebo(VideoInformation *videoInformation);
		/*! Get the full Video Information of Bebo Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Break Videos */
class VideoInformation_Break : public VideoInformation_plugin
{
	public:
		/*! Break plugin class constructor */
		VideoInformation_Break(VideoInformation *videoInformation);
		/*! Get the full Video Information of Break Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Broadcaster Videos */
class VideoInformation_Broadcaster : public VideoInformation_plugin
{
	public:
		/*! Broadcaster plugin class constructor */
		VideoInformation_Broadcaster(VideoInformation *videoInformation);
		/*! Get the full Video Information of Broadcaster Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for CaughtOnVideo Videos */
class VideoInformation_CaughtOnVideo : public VideoInformation_plugin
{
	public:
		/*! CaughtOnVideo plugin class constructor */
		VideoInformation_CaughtOnVideo(VideoInformation *videoInformation);
		/*! Get the full Video Information of CaughtOnVideo Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Clip4e Videos */
class VideoInformation_Clip4e : public VideoInformation_plugin
{
	public:
		/*! Clip4e plugin class constructor */
		VideoInformation_Clip4e(VideoInformation *videoInformation);
		/*! Get the full Video Information of Clip4e Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for VideoCa Videos */
class VideoInformation_VideoCa : public VideoInformation_plugin
{
	public:
		/*! VideoCa plugin class constructor */
		VideoInformation_VideoCa(VideoInformation *videoInformation);
		/*! Get the full Video Information of VideoCa Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Main video information class */
class VideoInformation : public QThread
{
Q_OBJECT
	private:
		QList<VideoInformation_plugin *> *plugins;	//!< List with all registered plugins (services)
		VideoItem *videoItem;	//!< Current video item
		bool blockAdultContent;	//!< Flag for know if adult content is accepted
		QStringList blockAdultContentList; //!< List of blocked services
		/*! Determine if this index is a valid item index */
		bool validItemIndex(const int index);
		/*! Clear and destroy all the stored plugins */
		void clearPlugins();
		/*! Get the VideoInformation_plugin according to a Host */
		VideoInformation_plugin* getPluginByHost(QUrl URL);
		/*! Thread executation */
		void run();
	public:
		/*! Class constructor */
		VideoInformation();
		/*! Class destructor */
		~VideoInformation();
		/* Register a new VideoDownload Plugin */
		void registerPlugin(VideoInformation_plugin *videoDownload_plugin);
		/*! Get a registered VideoDownload Plugin */
		VideoInformation_plugin* getRegisteredPlugin(const int index);
		/*! Get registered plugins count */
		int getPluginsCount();
		/*! Get the list of all registered plugins */
		QStringList getPluginsList(bool asCaptions = true);
		/*! Get the list of all registered plugins (host + caption) */
		QStringList getPluginsCompleteList(const QString separator = "|");
		/*! Get the video URL */
		void getVideoInformation(VideoItem *videoItem);
		/*! Get block adult content flag */
		bool getBlockAdultContent();
		/*! Set the block adult content flag */
		void setBlockAdultContent(bool blockAdultContent);
		/*! Get the block adult content list */
		QString getBlockAdultContentList();
		/*! Set the block adult content list */
		void setBlockAdultContentList(QString hosts);
		/*! Get if is getting the video information */
		bool isGettingInfo();
		/*! Get if can start to get information */
		bool canGetInformation();
		/*! Get the host icon */
		QString getHostImage(QString URL);
		/*! Get the caption of this host */
		QString getHostCaption(QString URL);
		/*! Get if is a valid host */
		bool isValidHost(QString URL);
		/*! Get if is a blocked host (and what block is) */
		bool isBlockedHost(QString URL, BlockedState &result);
		/*! Get if is a blocked host */
		bool isBlockedHost(QString URL);
	signals:
		/*! A Video item download started */
		void informationStarted(VideoItem *videoItem);
		/*! A Video item download finished */
		void informationFinished(VideoItem *videoItem);
};

#endif // __VIDEOINFORMATION_H__
