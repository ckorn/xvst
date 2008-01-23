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

#ifndef __UPDATES_H__
#define __UPDATES_H__
//
#include <QtGui>
//
#include "pakunpak/pakunpak.h"
#include "programversion.h"
#include "languages.h"
#include "http.h"
#include "tools.h"

#ifdef Q_OS_WIN32
static const QString CURRENT_OS = "MSWINDOWS"; //<! Microsoft windows version
#endif
#ifdef Q_OS_LINUX 
static const QString CURRENT_OS = "LINUX"; //<! Gnu/Linux version
#endif
#ifdef Q_WS_MAC
static const QString CURRENT_OS = "MACOSX"; //<! MacOS X version
#endif
#ifdef Q_OS_WIN32
static const QString XUPDATER_PATH = "/bin/xUpdater.exe";	//<! xUpdater app (win32)
#else
static const QString XUPDATER_PATH = "/bin/xUpdater";		//<! xUpdater app (unix)
#endif
static const QString XUPDATER_FILE 			= "/xVST.update";		//!< update script file
static const QString XUPDATER_DWN_FILE 		= "/xVST.update.%1";	//!< downloaded update file

enum UpdateState
{
	usWaiting,
	usChecking,
	usDownloading,
	usInstalling
};

/*! Update item */
class Update : public QObject
{
Q_OBJECT
	private:
		QString caption;	//!< Update caption
		QString version;	//!< Update version
		int size;			//!< Update file size
		QString installTo;	//!< Where to find and install the new update
		QString url;		//!< Where to download this update
		bool packed;		//!< Flag for know if this update is packed
		bool checked;		//!< Selected to update
	public:
		/*! Set the update caption */
		void setCaption(QString value);
		/*! Set the update version */
		void setVersion(QString value);
		/*! Set the update file size */
		void setSize(int value);
		/*! Set where to install this update */
		void setInstallTo(QString value);
		/*! Set the update url */
		void setUrl(QString value);
		/*! Set if is update is packed */
		void setPacked(bool value);
		/*! Set as selected */
		void setChecked(bool value);
		/* Get the update caption */
		QString getCaption();
		/*! Get the update version */
		QString getVersion();
		/*! Get the update file size */
		int getSize();
		/*! Get the install path */
		QString getInstallTo();
		/*! Get the update url */
		QString getUrl();
		/*! Get if this update is packed */
		bool getPacked();
		/*! Get if is selected */
		bool getChecked();
};

/*! Check and download program updates */
class Updates : public QThread
{
Q_OBJECT
	private:
		QString appPath;				//!< Application path
		QList<Update *>* updateList;	//!< List with all aviable updates
		Http *http;						//!< download class
		QString updateURL;				//!< Where to get the update file
		UpdateState updateState;		//!< Current download state
		bool cancelled;					//!< Cancelled
		int currentItem;				//!< Current download item index
		int totalToDownload;			//!< Total size to download (bytes)
		int totalDownloaded;			//!< Total size downloaded (bytes)
		int currentDownloaded;			//!< Current size downloaded (bytes)
		/*! Parse a block */
		void parseBlock(QString block);
		/*! Parse the downloaded update file */
		void pareseUpdateFile(QString updateFile);
		/*! Determine if has updates */
		bool hasUpdates();
		/*! Clear update items */
		void clear();
		/*! Install updates */
		void buildInstalScript();
		/*! Get the total size to download */
		void getTotalSizeToDownload();
		/*! Get the first update file to download */
		int getFirstUpdateToDownload();
		/*! Thread executation */
		void run();
	public:
		/*! Class constructor */
		Updates(QString appPath);
		/*! Class destructor */
		~Updates();
		/*! Check for update */
		void checkForUpdates(QString url);
		/*! Download updates */
		void downloadUpdates();
		/*! Run the xUpdater and install the updates */
		void installUpdates();
		/*! Cancel the current process */
		void cancel();
		/*! Get an update item */
		Update* getUpdateItem(const int index);
		/*! Get updates count */
		int getUpdatesCount();
		/*! Get total downloaded */
		int getCurrentDownloaded();
		/*! Get total to download */
		int getTotalToDownload();
		/*! Check if the xUpdater is installed */
		static bool canUpdate();
	private slots:
		/*! When the http read data */
		void downloadEvent(int pos, int max);
		/*! When the http finish a download */
		void downloadFinished(const QFileInfo destFile);
	signals:
		/*! After check for updates */
		void updatesChecked(bool hasUpdates);
		/*! Update proces cancelled */
		void updatesCancelled();
		/*! On progress update */
		void progressCheckUpdate(int progress);
		/*! Downloading update */
		void downloadingUpdate(int updateIndex, int pogress, int totalProgress);
		/*! Update item Download finished */
		void downloadFinished(int updateIndex);
		/*! Downloads finished */
		void downloadsFinished();
		/*! When its ready to install updates */
		void readyToInstallUpdates();
};

#endif // __UPDATES_H__
