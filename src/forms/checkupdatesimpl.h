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

#ifndef CHECKUPDATESIMPL_H
#define CHECKUPDATESIMPL_H
//
#include <QDialog>
//
#include "ui_checkupdates.h"
//
#include "windowsvistadetectedimpl.h"
#include "updatecenterimpl.h"
//
#include "../options.h"
#include "../updates.h"
//

static const QString URL_UPDATE_FILE = "http://xviservicethief.sourceforge.net/updates/xVST.update";

class CheckUpdatesImpl : public QDialog, public Ui::CheckUpdates
{
Q_OBJECT
	private:
		ProgramOptions *programOptions;
		Updates *updates;
		bool closedByButton;
		bool isUser;
		void checkUpdates();
		void waitThread();
		bool isWindowsVista();
	protected:
		void closeEvent(QCloseEvent *event);
	public:
		CheckUpdatesImpl(ProgramOptions *programOptions, bool isUser, QWidget * parent = 0, Qt::WFlags f = 0 );
		~CheckUpdatesImpl();
	private slots:
		void btnCancelClicked();
		void updatesChecked(bool hasUpdates);
		void updatesCancelled();
		void progressCheckUpdate(int progress);		
};
#endif
