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

#include "checkupdatesimpl.h"
// windows vista detection
#ifdef Q_WS_WIN
	#include <windows.h>
#endif
//
CheckUpdatesImpl::CheckUpdatesImpl(ProgramOptions *programOptions, bool isUser, QWidget * parent, Qt::WFlags f)
	: QDialog(parent, f)
{
	setupUi(this);
	this->programOptions = programOptions;
	this->isUser = isUser;
	closedByButton = false;
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

void CheckUpdatesImpl::closeEvent(QCloseEvent *event)
{
	if (!closedByButton)
	{
		event->ignore();
		btnCancelClicked();
	}
}

void CheckUpdatesImpl::checkUpdates()
{
	programOptions->setLastUpdate(QDate::currentDate());
	programOptions->save();
	
	updates->checkForUpdates(URL_UPDATE_FILE);
}

void CheckUpdatesImpl::waitThread()
{
	while (updates->isRunning())
		qApp->processEvents();
}

bool CheckUpdatesImpl::isWindowsVista()
{
#ifdef Q_WS_WIN
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	// return if we are running the windows vista OS
	return((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion == 0));
#else
	return false;
#endif
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
		// result checker
		int result = QDialog::Accepted;
		// check if we are running windwos vista
		if (isWindowsVista() && !programOptions->getVistaUpdatesMessage())
		{
			WindowsVistaDetectedImpl windowsVistaDetectedForm(this);
			result = windowsVistaDetectedForm.exec();
			// update the option "don't display this message"
			programOptions->setVistaUpdatesMessage(windowsVistaDetectedForm.chbDontDisplay->isChecked());
		}
		// open update center (this condition works only on windows vista)
		if (result == QDialog::Accepted) 
		{		
			// dispaly
			UpdateCenterImpl updateCenterForm(updates, programOptions->getInstallAutomaticallyUpdates(), this);
			result = updateCenterForm.exec();
		}
		// wait until "thread end"
		waitThread();
		// updates downloaded??? yes? then install them
		if (result == QDialog::Accepted)
		{
			updates->installUpdates();
			QApplication::closeAllWindows();
		}
		else // update center has been cancelled
		{
			closedByButton = true;
			done(QDialog::Rejected);
		}
	}
	else
	{
		waitThread();
		
		if (isUser)
			QMessageBox::information(this, tr("Updates"),
			                               tr("You are using the most recent version of this program."),
			                               tr("Ok"));
		closedByButton = true;
		done(QDialog::Rejected);
	}
}

void CheckUpdatesImpl::updatesCancelled()
{
	lblUpdating->setText(tr("Cancelling... please wait..."));
	
	waitThread();

	closedByButton = true;
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
