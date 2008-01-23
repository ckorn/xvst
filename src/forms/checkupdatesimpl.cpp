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

#include "checkupdatesimpl.h"
//
CheckUpdatesImpl::CheckUpdatesImpl(ProgramOptions *programOptions, bool isUser, QWidget * parent, Qt::WFlags f)
	: QDialog(parent, f)
{
	setupUi(this);
	this->programOptions = programOptions;
	this->isUser = isUser;
	// init update class
	updates = new Updates(programOptions->getApplicationPath());
	// signals
	connect(updates, SIGNAL(progressCheckUpdate(int)), this, SLOT(progressCheckUpdate(int)));
	connect(updates, SIGNAL(updatesChecked(bool)), this, SLOT(updatesChecked(bool)));
	connect(updates, SIGNAL(updatesCancelled()), this, SLOT(updatesCancelled()));
	//
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(btnCancelClicked()));
	// check for updates
	checkUpdates();
}

CheckUpdatesImpl::~CheckUpdatesImpl()
{
	delete updates;
}

void CheckUpdatesImpl::checkUpdates()
{
	programOptions->setLastUpdate(QDate::currentDate());
	programOptions->save();
	
	updates->checkForUpdates(URL_UPDATE_FILE);
}

void CheckUpdatesImpl::btnCancelClicked()
{
	btnCancel->setEnabled(false);
	updates->cancel();
}

void CheckUpdatesImpl::updatesChecked(bool hasUpdates)
{
	// if has updates, then display the update center
	if (hasUpdates)
	{
		this->setVisible(false);
		// dispaly
		UpdateCenterImpl updateCenterForm(updates, programOptions->getInstallAutomaticallyUpdates(), this);
		// check te "exit" result (!accepted == cancelled)
		if (updateCenterForm.exec() == QDialog::Accepted)
			done(QDialog::Accepted);
	}
	else
	{
		if (isUser)
			QMessageBox::information(this, tr("Updates"),
			                               tr("You are using the most recent version of this program."),
			                               tr("Ok"));
		done(QDialog::Rejected);
	}
}

void CheckUpdatesImpl::updatesCancelled()
{
	qDebug() << "CANCEL";
	
	lblUpdating->setText(tr("Cancelling... please wait..."));
	
	while (updates->isRunning())
		qApp->processEvents();
		
	done(QDialog::Rejected);
}

void CheckUpdatesImpl::progressCheckUpdate(int progress)
{
	lblUpdating->setText(tr("Checking for updates..."));
	pbrUpdate->setMaximum(100);
	pbrUpdate->setValue(progress);
	// imposible cancel it
	if (progress == 100)
		btnCancel->setEnabled(false);
}
//
