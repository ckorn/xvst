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

#ifndef MAINFORMIMPL_H
#define MAINFORMIMPL_H
//
#include <QtGui>
//
#include "ui_mainform.h"
//
#include "dragdropimpl.h"
#include "completedpopupimpl.h"
#include "addvideoimpl.h"
#include "informationimpl.h"
#include "optionsimpl.h"
#include "checkupdatesimpl.h"
#include "bugreportimpl.h"
//
#include "../tools.h"
#include "../options.h"
#include "../progressbardelegate.h"
#include "../videolistcontroller.h"
#include "../videoinformation.h"
#include "../sessionmanager.h"
#include "../programversion.h"
//
class MainFormImpl : public QMainWindow, public Ui::MainForm
{
Q_OBJECT
	private:
		// main form configuration
		void centerWindow();
		// drag & drop config
		DragDropImpl *dragDropForm;
		// tray icon config
		QMenu *trayIconMenu;
		QMenu *trayIconMenuOptions;
		QSystemTrayIcon *trayIcon;
		QString lastTrayIconStr;
		void createTrayIcon();
		// completed popup window
		CompletedPopupImpl *completedPopup;
		// options
		int lastOptionsPage;
		ProgramOptions *programOptions;
		void updateVisualOptions();
		// main objects (program core)
		VideoListController *videoList;
		// lsvDownloadList functions
		QTreeWidgetItem* getQTreeWidgetItemByVideoItem(VideoItem *videoItem);
		VideoItem *getSelectedVideoItem();
		VideoItem *getVideoItemByQTreeWidgetItem(QTreeWidgetItem* treeItem);
		// visual controls update
		void updateVisualControls();
		// Session manager
		SessionManager *sessionManager;
		// forms lockers
		bool addForm_active;
		bool optionsForm_active;
		bool infoForm_active;
		// paste shortcut
		QShortcut *shortCutPasteURL;	//!< Paste from clipboard Shortcut
		QShortcut *shortCutDeleteVideo;	//!< Delete the selected video from list
		// updates
		void checkUpdates();
		// set stay on top flag
		void setStayOnTopFlag();
		// extra information
		void updateListInformation();
	protected:
		void changeEvent(QEvent *event);
		void closeEvent(QCloseEvent *event);
	public:
		MainFormImpl(QWidget * parent = 0, Qt::WFlags f = 0);
		~MainFormImpl();
	private slots:
		void dragDropClicked();
		void updatesClicked();
		void onlineHelpClicked();
		void informationClicked();
		void restoreAppClicked();
		void openDownloadDirClicked();
		void selectDownloadDirClicked();
		void closeAppClicked();
		void addVideoClicked();
		void deleteVideoClicked();
		void startDownloadVideoClicked();
		void pauseResumeDownloadVideoClicked();
		void cancelDownloadVideoClicked();
		void moreOptionsClicked();
		void clearListClicked();
		void clearCompletedClicked();
		void resetStateClicked();
		void moveItemUpClicked();
		void moveItemDownClicked();
		void playVideoClicked();
		void stayOnTopClicked();
		void minimizeToSystemTrayClicked();
		void viewErrorMessageClicked();
		void downloadAutomaticallyStateChanged(int state);
		void convertVideosStateChanged(int state);
		void edtDownloadDirChanged(const QString &text);
		// tray icon menu signals
		void trayIconAboutToShow();
		void optionSelected(bool checked);
		void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
		// options signals
		void optionsDidSomething();
		// lsvDownloadList signals
		void videoItemDoubleClicked(QTreeWidgetItem *item, int column);
		void videoItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
		void videoListContextMenu(const QPoint & pos);
		// VideoListController signals
		void videoAdded(VideoItem *videoItem);
		void videoDeleted(VideoItem *videoItem);
		void videoUpdated(VideoItem *videoItem);
		void videoError(VideoItem *videoItem);
		void videoMoved(int from, int to);
		// shortcuts signals
		void pasteURLfromClipboardClicked();
		// first program executation
		void checkForUpdates();
};
#endif
