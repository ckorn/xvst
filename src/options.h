#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <QtGui>
#include <QNetworkProxy>
//
#include "tools.h"
#include "videoconvert.h"

static const QString DEFAULT_DOWNLOADS = "/downloads";	//!< Default sub-directory for downloads
static const QString DEFAULT_FFMPEGLIB = "/bin";		//!< Default sub-directory for the ffmpeg lib

/*! Program options class: Save and Load the main program options */
class ProgramOptions : public QObject
{
Q_OBJECT
	private:
		QString ffmpegLibLocation;		//!< ffmpeg lib (app) path (path + name)
		bool deleteVideosOnConvert;		//!< flag for know if the original video must be deleted after its conversion
		bool convertVideos;				//!< flag for know if the downloaded video should be converted
		QString downloadDir;			//!< downloads directory
		bool downloadAutomatically;		//!< flag for know if can download videos automatically 
		VideoConversionConfig conversionConf;	//!< Video conversion configuration
		bool displayPopup;				//!< flag for know if can display popups on finish
		bool saveRestoreSessions;		//!< flag for know if can save and restore sessions
		bool saveLogDownloadedVideos;	//!< flag for know if can make a log with all downloaded files
		bool dontRememberDownloadedVideos;	//!< flag for know if can remember "completed" videos
		bool blockAdultContent;			//!< flag for know if adult content is allowed or not
		QString blockedWebsitesList;	//!< fist of services which are blocked (not allowed)	int proxyPort;
		bool useProxy;					//!< flag for know if Proxy is enabled or not
		QString proxyAdress;			//!< Proxy Adress
		QString proxyPassword;			//!< Proxy Password
		QString proxyUserName;			//!< Proxy UserName
		int proxyPort;					//!< Proxy Port
		int proxyType;					//!< Proxy Type
		int dragDropLeft;				//!< Drag & Drop left position
		int dragDropTop;				//!< Drag & Drop top position
		int dragDropAlphaBlendValue;	//!< Drag & Drop enable alpha blend
		bool dragDropAlphaBlend;		//!< Drag & Drop alpha blend value
		QString languageFile;			//!< Language file

		QDir appDir;			//!< initial program path
		QString optionsFile;	//!< options file name (path + name)
		
		bool canSendUpdateSignal;	//!< flag for know if is the update signal can be sended
		/*! Send the update signal (only if is possible) */
		void sendUpdateSignal();
	public:
		/*! Class constructor */
		ProgramOptions(QString optionsPath);
		/*! Class destructor */
		~ProgramOptions();
		/*! Load the program options */
		void load();
		/*! Save the program options */
		void save();
		/*! Set the default program options */
		void setDefault();
		/*! Set if the update signal can be launched */
		void setCanSendUpdateSignal(bool canSendUpdateSignal);
		/*! Get the application path */
		QString getApplicationPath();
		// set / get methods
		void setFfmpegLibLocation(QString value);
		QString getFfmpegLibLocation();
		void setDeleteVideosOnConvert(bool value);
		bool getDeleteVideosOnConvert();
		void setConvertVideos(bool value);
		bool getConvertVideos();
		void setDownloadDir(QString value);
		QString getDownloadDir();
		void setDownloadAutomatically(bool value);
		bool getDownloadAutomatically();
		void setConversionConf(VideoConversionConfig value);
		VideoConversionConfig getConversionConf();
		void setDisplayPopup(bool value);
		bool getDisplayPopup();
		void setSaveRestoreSessions(bool value);
		bool getSaveRestoreSessions();
		void setSaveLogDownloadedVideos(bool value);
		bool getSaveLogDownloadedVideos();
		void setDontRememberDownloadedVideos(bool value);
		bool getDontRememberDownloadedVideos();
		void setBlockAdultContent(bool value);
		bool getBlockAdultContent();
		void setBlockedWebsitesList(QString value);
		QString getBlockedWebsitesList();
		void setProxyPort(int value);
		int getProxyPort();
		void setProxyAdress(QString value);
		QString getProxyAdress();
		void setProxyPassword(QString value);
		QString getProxyPassword();
		void setProxyUserName(QString value);
		QString getProxyUserName();
		void setUseProxy(bool value);
		bool getUseProxy();
		void setProxyType(int value);
		int getProxyType();
		void setDragDropLeft(int value);
		int getDragDropLeft();
		void setDragDropAlphaBlendValue(int value);
		int getDragDropAlphaBlendValue();
		void setDragDropAlphaBlend(bool value);
		bool getDragDropAlphaBlend();
		void setDragDropTop(int value);
		int getDragDropTop();
		void setLanguageFile(QString value);
		QString getLanguageFile(bool fullPath);
	signals:
		/*! Options will load the configuration */
		void optionsLoadBefore();
		/*! Options loaded the configuration */
		void optionsLoadAfter();
		/*! Options will save the configuration */
		void optionsSaveBefore();
		/*! Options saved the configuration */
		void optionsSaveAfter();
		/*! An option has been modified */
		void optionsUpdated();
};

#endif // __OPTIONS_H__
