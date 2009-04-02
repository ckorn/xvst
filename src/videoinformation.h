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
#include <QtScript>
//
#include "videoitem.h"
#include "http.h"
#include "httpscriptclass.h"
#include "toolsscriptclass.h"

enum BlockedState
{
	bsNotBlocked,
	bsAdultContent,
	bsBlocked
};

class VideoInformation;

/*! Plugin information class */
class VideoInformationPlugin : public QObject
{
	private:
		QString scriptCode;			//<! Plugin source code
		QString version;			//<! Plugin version
		QString minVersion;			//<! Min xVST version to run
		QString author;				//<! Plugin author
		QString videoServicePath;	//<! Where the plugin is stored
		QString ID;					//<! Plugin ID (used by the engine to determine if this plugin knows how to resolve an url)
		QString caption;			//<! Plugin public caption
		QPixmap *icon;				//<! Plugin public icon (acts as a proxy)
		bool adultContent;			//<! Flag for know if this webservice has adult contents
		bool loaded;				//<! Flag for know if this plugin has been loaded
	private:
		/* VideoDefinition struct script definition */
		static QScriptValue create_VideoDefinition(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue toScriptValue_VideoDefinition(QScriptEngine *engine, const VideoDefinition &vd);
		static void fromScriptValue_VideoDefinition(const QScriptValue &obj, VideoDefinition &vd);
	public:
		/*! Class constructor */
		VideoInformationPlugin(VideoInformation *videoInformation, QString videoServicePath);
		/*! Class destructor */
		~VideoInformationPlugin();
		/*! Check if the ID passed by parameters is like the stored plugin ID */
		bool isLikeMyId(QString ID);
		/*! Get the video information from URL (this function executes the JS plugin) */
		VideoDefinition getVideoInformation(const QString URL);
		/*! Get the plugin version */
		QString getVersion() const;
		/*! Get the min xVST version to run */
		QString getMinVersion() const;
		/*! Get the plugin author */
		QString getAuthor() const;
		/*! Get the plugin ID */
		QString getID() const;
		/*! Get the plugin caption */
		QString getCaption() const;
		/*! Get if this service has adult content (+18) */
		bool hasAdultContent() const;
		/*! Get the plugin icon */
		QPixmap *getIcon() const;
		/*! Get if has been loaded */
		bool isLoaded() const;
};

/*! Main video information class */
class VideoInformation : public QThread
{
Q_OBJECT
	private:
		QList<VideoInformationPlugin *> *plugins;	//!< List with all registered plugins (services)
		VideoItem *videoItem;	//!< Current video item
		bool blockAdultContent;	//!< Flag for know if adult content is accepted
		QStringList blockAdultContentList; //!< List of blocked services
		/*! Determine if this index is a valid item index */
		bool validItemIndex(const int index);
		/*! Clear and destroy all the stored plugins */
		void clearPlugins();
		/*! Get the VideoInformation_plugin according to a Host */
		VideoInformationPlugin* getPluginByHost(QUrl URL);
		/*! Thread executation */
		void run();
		/*! Load all plugins from /plugins/.js */
		void loadPlugins(QString pluginsDir);
		/*! Load a plugin and regist it */
		void loadPluginFile(QString scriptFile);
	public:
		/*! Class constructor */
		VideoInformation(QString pluginsDir);
		/*! Class destructor */
		~VideoInformation();
		/* Register a new VideoDownload Plugin */
		void registerPlugin(VideoInformationPlugin *videoInformationPlugin);
		/*! Get a registered VideoDownload Plugin */
		VideoInformationPlugin* getRegisteredPlugin(const int index);
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
		//QString getHostImage(QString URL, bool checkURL = false);
		/*! Get the host icon */
		QPixmap getHostImage(QString URL, bool checkURL = false);
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
