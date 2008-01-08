#ifndef DRAGDROPIMPL_H
#define DRAGDROPIMPL_H
//
#include <QtGui>
//
#include "ui_dragdrop.h"
//
#include "customalphablendimpl.h"
//
#include "../options.h"
#include "../videolistcontroller.h"

class DragDropImpl : public QWidget, public Ui::DragDrop
{
Q_OBJECT
	private:
		QWidget *parent;				//!< Parent form
		QMenu *dragDropMenu;			//!< Drag and drop menu
		QMenu *alphaBlendMenu;			//!< Alpha blend menu
		ProgramOptions *programOptions;	//!< Program Options reference
		VideoListController *videoList;	//!< Video list controller reference
		int internalTimer;				//!< Internal timer
		QShortcut *shortCurtPasteURL;	//!< Paste from clipboard Shortcut
		void setInitialPos();
		void createDragDropMenu();
		void setAlphaBlend(float);
		void dragEnterEvent(QDragEnterEvent *event);
		void dropEvent(QDropEvent *event);
		void closeEvent (QCloseEvent *event);
		void contextMenuEvent(QContextMenuEvent * event);
		void addVideo(QString URL);
	protected:
		void timerEvent(QTimerEvent *event);
	public:
		DragDropImpl(ProgramOptions *programOptions, VideoListController *videoList, 
					QWidget * parent = 0, Qt::WFlags f = 0);
		~DragDropImpl();
		void addVideoURL(QString URL);
	private slots:
		void pasteURLfromClipboardClicked();
		void displayMainWindowClicked();
		void alphaBlendValueClicked();
};
#endif
