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
