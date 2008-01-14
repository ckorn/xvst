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

#ifndef OPTIONSIMPL_H
#define OPTIONSIMPL_H
//
#include <QtGui>
//
#include "ui_options.h"
//
#include "downloadlogimpl.h"
#include "checkupdatesimpl.h"
//
#include "../options.h"
#include "../sessionmanager.h"
#include "../videoinformation.h"
#include "../languages.h"
//
class OptionsImpl : public QDialog, public Ui::Options
{
Q_OBJECT
	private:
		ProgramOptions *programOptions;
		SessionManager *sessionManager;
		VideoInformation *videoInformation;
		LanguageManager *languageManager;
		int lastPageViewed;
		QString tmpLangFile;
		void createMenu();
		void fillInitialData();
		void fillLanguages();
		void setInitialOptionsValues();
		void setOptionsValues();
		void dragEnterEvent(QDragEnterEvent *event);
		void dropEvent(QDropEvent *event);
	public:
		OptionsImpl(ProgramOptions *programOptions, SessionManager *sessionManager,
			VideoInformation *videoInformation, int lastOptionsPage, 
			QWidget * parent = 0, Qt::WFlags f = 0);
		~OptionsImpl();
		int getLastPageViewed();
	private slots:
		void menuCurrentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
		void langCurrentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
		void spbSelectDownloadDirPressed();
		void spbSelectFFmpegLibPressed();
		void btnOkClicked();
		void ffmpegTextChanged(const QString &text);
		void btnViewLogClicked();
		void btnClearLogClicked();
		void chbSaveRestoreStateClicked(bool checked = false);
		void spbRemovePressed();
		void spbRemoveAllPressed();
		void spbAddAllPressed();
		void spbAddPressed();
		void servicesItemSelectionChanged();
		void btnUseThisClicked();
		void langItemDoubleClicked(QTreeWidgetItem *item, int column);
		void btnCheckNowClicked();
};
#endif
