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

#ifndef UPDATECENTERIMPL_H
#define UPDATECENTERIMPL_H
//
#include <QDialog>
//
#include "ui_updatecenter.h"
//
#include "../updates.h"
#include "../tools.h"
#include "../programversion.h"

class UpdateCenterImpl : public QDialog, public Ui::UpdateCenter
{
Q_OBJECT
	private:
		Updates *updates;
		bool closedByButton;
		void fillUpdates();
	protected:
		void closeEvent(QCloseEvent *event);
	public:
		UpdateCenterImpl(Updates *updates, bool autoDownloadAndInstall, QWidget * parent = 0, Qt::WFlags f = 0 );
	private slots:
		void itemChanged(QTreeWidgetItem * item, int column);
		void btnCancelClicked();
		void btnUpdateClicked();
		// updater
		void downloadingUpdate(int updateIndex, int pogress, int totalProgress);
		void downloadFinished(int updateIndex);
		void downloadsFinished();
		void readyToInstallUpdates();
};
#endif
