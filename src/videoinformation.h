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
