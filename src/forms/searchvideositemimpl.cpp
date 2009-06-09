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

#include "searchvideositemimpl.h"

#include "../tools.h"
#include "../searchvideos.h"
#include "../videolistcontroller.h"

SearchVideosItemImpl::SearchVideosItemImpl(QWidget *parent, SearchResultItem *searchItem)
	: QWidget(parent)
{
	setupUi(this);
	setSearchVideosItem(searchItem);
	//
	connect(lblDownloadVideo, SIGNAL(linkActivated(const QString&)), this, SLOT(linkActivated(const QString&)));
}

void SearchVideosItemImpl::setSearchVideosItem(SearchResultItem *searchItem)
{
	if (searchItem == NULL) return;

	this->searchItem = searchItem;
	this->setObjectName(searchItem->getVideoId());

	// auto configure items
	lblTitle->setText(QString(lblTitle->text()).arg(searchItem->getTitle()));
	lblDescription->setText(QString(lblDescription->text()).arg(searchItem->getDescription()));

	lblExtraInformation->setText(QString("(%1)").arg(secondsToHMS_formatted(searchItem->getDuration(), ":", true, false)));
	lblExtraInformation->setVisible(searchItem->getDuration() > 0);

	lblDownloadVideo->setText(QString("<a href=\"%1\"><img src=\":/buttons/images/film_add.png\" /></a>").arg(searchItem->getVideoUrl()));
	lblPlayVideo->setText(QString("<a href=\"%1\"><img src=\":/buttons/images/control_play.png\" /></a>").arg(searchItem->getVideoUrl()));
}

void SearchVideosItemImpl::reloadPreview()
{
	if (searchItem == NULL) return;
	// reload preview if it exists
	if (QFile::exists(QDir::tempPath() + searchItem->getPreviewFileName()))
	{
		QPixmap preview = QPixmap(QDir::tempPath() + searchItem->getPreviewFileName());
		pre

		if (!preview.isNull())
			imgVideo->setPixmap(preview);
	}
}

void SearchVideosItemImpl::linkActivated(const QString &link)
{
	if (!added)
	{
		VideoListController::getLastInstance()->addVideo(link);
		added = true;
	}
	else
		QMessageBox::information(this,
								tr("Already added"),
								tr("You already added this video."),
								tr("Ok"));
}
