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
		// visual controls update
		void updateVisualControls();
		// Session manager
		SessionManager *sessionManager;
		// forms lockers
		bool addForm_active;
		bool optionsForm_active;
		bool infoForm_active;
		// paste shortcut
		QShortcut *shortCurtPasteURL;	//!< Paste from clipboard Shortcut
	protected:
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
		void cancelDownloadVideoClicked();
		void moreOptionsClicked();
		void clearListClicked();
		void clearCompletedClicked();
		void resetStateClicked();
		void moveItemUpClicked();
		void moveItemDownClicked();
		void playVideoClicked();
		void downloadAutomaticallyStateChanged(int state);
		void convertVideosStateChanged(int state);
		// tray icon menu signals
		void trayIconAboutToShow();
		void optionSelected(bool checked);
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
		void videoMoved(int from, int to);
		// shortcuts signals
		void pasteURLfromClipboardClicked();
};
#endif
