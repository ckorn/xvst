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

#include "completedpopupimpl.h"
//
CompletedPopupImpl::CompletedPopupImpl( QWidget * parent, Qt::WFlags f)
		: QWidget(parent, f)
{
	setupUi(this);
	// set window flags
	QWidget::setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint);
	// init the timer
	displayTimer = new QTimer(this);
	connect(displayTimer, SIGNAL(timeout()), this, SLOT(displayTimerOnTimer()));
	// connect buttons
	connect(spbClose, SIGNAL(clicked()), this, SLOT(closeClicked()));
	connect(spbPlayVideo, SIGNAL(clicked()), this, SLOT(playVideoClicked()));
	// init state
	displayState = dsInactive;
}

CompletedPopupImpl::~CompletedPopupImpl()
{
	delete displayTimer;
}

void CompletedPopupImpl::displayTimerOnTimer()
{
	if (!isVisible()) return;

	QDesktopWidget *desktop = QApplication::desktop();
	QRect availableGeometry  = desktop->availableGeometry(this);

	// display popup animation
	if (displayState == dsShowing)
		if (pos().x() > availableGeometry.width() - size().width())// - 15)
			move(pos().x() - 1, pos().y());
		else
		{
			displayState = dsWaiting;
			displayTimer->start(4000);
		}
	// hide popup animation
	else if (displayState == dsHiding)
		if (pos().x() < availableGeometry.width())
			move(pos().x() + 1, pos().y());
		else
		{
			displayState = dsWaiting;
			displayTimer->stop();
			hide();
		}
	else if (displayState == dsWaiting)
	{
		displayState = dsHiding;
		displayTimer->start(1);
	}
}

void CompletedPopupImpl::displayPopup(const QIcon &serviceIcon, const QString videoTitle, const QString videoFile)
{
	// set window info
	imgVideoService->setPixmap(serviceIcon.pixmap(QSize(16,16)));
	lblVideoTitle->setText(videoTitle);
	this->videoFile = videoFile;

	QDesktopWidget *desktop = QApplication::desktop();
	QRect availableGeometry  = desktop->availableGeometry(this);
	move(desktop->width(), availableGeometry.height() - size().height());
	this->show();
	displayState = dsShowing;
	displayTimer->start(1);
}

void CompletedPopupImpl::closeClicked()
{
	displayState = dsHiding;
	displayTimer->start(1);
}

void CompletedPopupImpl::playVideoClicked()
{
	if (QFile::exists(videoFile))
		openFile(videoFile);
}
//
