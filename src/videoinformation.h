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

/*! Plugin for Yahoo Videos */
class VideoInformation_Yahoo : public VideoInformation_plugin
{
	public:
		/*! Yahoo plugin class constructor */
		VideoInformation_Yahoo(VideoInformation *videoInformation);
		/*! Get the full Video Information of Yahoo Videos */
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

/*! Plugin for LiveLeak Videos */
class VideoInformation_LiveLeak : public VideoInformation_plugin
{
	public:
		/*! LiveLeak plugin class constructor */
		VideoInformation_LiveLeak(VideoInformation *videoInformation);
		/*! Get the full Video Information of LiveLeak Videos */
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

/*! Plugin for Tu.tv Videos */
class VideoInformation_TuTv : public VideoInformation_plugin
{
	public:
		/*! Tu.tv plugin class constructor */
		VideoInformation_TuTv(VideoInformation *videoInformation);
		/*! Get the full Video Information of Tu.tv Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Spike Videos */
class VideoInformation_Spike : public VideoInformation_plugin
{
	public:
		/*! Spike plugin class constructor */
		VideoInformation_Spike(VideoInformation *videoInformation);
		/*! Get the full Video Information of Spike Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for MySpaceTV Videos */
class VideoInformation_MySpaceTV : public VideoInformation_plugin
{
	public:
		/*! MySpaceTV plugin class constructor */
		VideoInformation_MySpaceTV(VideoInformation *videoInformation);
		/*! Get the full Video Information of MySpaceTV Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for CinemaVIP Videos */
class VideoInformation_CinemaVIP : public VideoInformation_plugin
{
	public:
		/*! CinemaVIP plugin class constructor */
		VideoInformation_CinemaVIP(VideoInformation *videoInformation);
		/*! Get the full Video Information of CinemaVIP Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for GameSpot Videos */
class VideoInformation_GameSpot : public VideoInformation_plugin
{
	public:
		/*! GameSpot plugin class constructor */
		VideoInformation_GameSpot(VideoInformation *videoInformation);
		/*! Get the full Video Information of GameSpot Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Stage6 Videos */
class VideoInformation_Stage6 : public VideoInformation_plugin
{
	public:
		/*! Stage6 plugin class constructor */
		VideoInformation_Stage6(VideoInformation *videoInformation);
		/*! Get the full Video Information of Stage6 Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Zuuble Videos */
class VideoInformation_Zuuble : public VideoInformation_plugin
{
	public:
		/*! Zuuble plugin class constructor */
		VideoInformation_Zuuble(VideoInformation *videoInformation);
		/*! Get the full Video Information of Zuuble Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for ZippyVideos Videos */
class VideoInformation_ZippyVideos : public VideoInformation_plugin
{
	public:
		/*! ZippyVideos plugin class constructor */
		VideoInformation_ZippyVideos(VideoInformation *videoInformation);
		/*! Get the full Video Information of ZippyVideos Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Zedge Videos */
class VideoInformation_Zedge : public VideoInformation_plugin
{
	public:
		/*! Zedge plugin class constructor */
		VideoInformation_Zedge(VideoInformation *videoInformation);
		/*! Get the full Video Information of Zedge Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Blip Videos */
class VideoInformation_Blip : public VideoInformation_plugin
{
	public:
		/*! Blip plugin class constructor */
		VideoInformation_Blip(VideoInformation *videoInformation);
		/*! Get the full Video Information of Blip Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Ceknito Videos */
class VideoInformation_Ceknito : public VideoInformation_plugin
{
	public:
		/*! Ceknito plugin class constructor */
		VideoInformation_Ceknito(VideoInformation *videoInformation);
		/*! Get the full Video Information of Ceknito Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for ZanyVideos Videos */
class VideoInformation_ZanyVideos : public VideoInformation_plugin
{
	public:
		/*! ZanyVideos plugin class constructor */
		VideoInformation_ZanyVideos(VideoInformation *videoInformation);
		/*! Get the full Video Information of ZanyVideos Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Zaable Videos */
class VideoInformation_Zaable : public VideoInformation_plugin
{
	public:
		/*! Zaable plugin class constructor */
		VideoInformation_Zaable(VideoInformation *videoInformation);
		/*! Get the full Video Information of Zaable Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for YouTubeIslam Videos */
class VideoInformation_YouTubeIslam : public VideoInformation_plugin
{
	public:
		/*! YouTubeIslam plugin class constructor */
		VideoInformation_YouTubeIslam(VideoInformation *videoInformation);
		/*! Get the full Video Information of YouTubeIslam Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for YourFileHost Videos */
class VideoInformation_YourFileHost : public VideoInformation_plugin
{
	public:
		/*! YourFileHost plugin class constructor */
		VideoInformation_YourFileHost(VideoInformation *videoInformation);
		/*! Get the full Video Information of YourFileHost Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for IndiaVideo Bomb Videos */
class VideoInformation_IndiaVideoBomb : public VideoInformation_plugin
{
	public:
		/*! IndiaVideo Bomb plugin class constructor */
		VideoInformation_IndiaVideoBomb(VideoInformation *videoInformation);
		/*! Get the full Video Information of IndiaVideo Bomb Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Boingboing TV Videos */
class VideoInformation_BoingboingTv : public VideoInformation_plugin
{
	public:
		/*! Boingboing TV plugin class constructor */
		VideoInformation_BoingboingTv(VideoInformation *videoInformation);
		/*! Get the full Video Information of Boingboing TV Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Gametrailers Videos */
class VideoInformation_Gametrailers : public VideoInformation_plugin
{
	public:
		/*! Gametrailers TV plugin class constructor */
		VideoInformation_Gametrailers(VideoInformation *videoInformation);
		/*! Get the full Video Information of Gametrailers TV Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

// Adult websites

/*! Plugin for Yuvutu Videos */
class VideoInformation_Yuvutu : public VideoInformation_plugin
{
	public:
		/*! Yuvutu plugin class constructor */
		VideoInformation_Yuvutu(VideoInformation *videoInformation);
		/*! Get the full Video Information of Yuvutu Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Badjojo Videos */
class VideoInformation_Badjojo : public VideoInformation_plugin
{
	public:
		/*! Badjojo plugin class constructor */
		VideoInformation_Badjojo(VideoInformation *videoInformation);
		/*! Get the full Video Information of Badjojo Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for DesiSexTube Videos */
class VideoInformation_DesiSexTube : public VideoInformation_plugin
{
	public:
		/*! DesiSexTube plugin class constructor */
		VideoInformation_DesiSexTube(VideoInformation *videoInformation);
		/*! Get the full Video Information of DesiSexTube Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for YouPorn Videos */
class VideoInformation_YouPorn : public VideoInformation_plugin
{
	public:
		/*! YouPorn plugin class constructor */
		VideoInformation_YouPorn(VideoInformation *videoInformation);
		/*! Get the full Video Information of YouPorn Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for TuPorno Videos */
class VideoInformation_TuPorno : public VideoInformation_plugin
{
	public:
		/*! TuPorno plugin class constructor */
		VideoInformation_TuPorno(VideoInformation *videoInformation);
		/*! Get the full Video Information of TuPorno Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for PornoTube Videos */
class VideoInformation_PornoTube : public VideoInformation_plugin
{
	public:
		/*! PornoTube plugin class constructor */
		VideoInformation_PornoTube(VideoInformation *videoInformation);
		/*! Get the full Video Information of PornoTube Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for DaleAlPlay Videos */
class VideoInformation_DaleAlPlay : public VideoInformation_plugin
{
	public:
		/*! DaleAlPlay plugin class constructor */
		VideoInformation_DaleAlPlay(VideoInformation *videoInformation);
		/*! Get the full Video Information of DaleAlPlay Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for Shufuni Videos */
class VideoInformation_Shufuni : public VideoInformation_plugin
{
	public:
		/*! Shufuni plugin class constructor */
		VideoInformation_Shufuni(VideoInformation *videoInformation);
		/*! Get the full Video Information of Shufuni Videos */
		VideoDefinition getVideoInformation(const QString URL);
};

/*! Plugin for XTube Videos */
class VideoInformation_XTube : public VideoInformation_plugin
{
	public:
		/*! XTube plugin class constructor */
		VideoInformation_XTube(VideoInformation *videoInformation);
		/*! Get the full Video Information of XTube Videos */
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
		/*! Cancel the current work */
		void cancel();
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
		QString getHostImage(QString URL, bool checkURL = false);
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
