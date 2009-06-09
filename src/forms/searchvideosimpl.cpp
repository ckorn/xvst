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

#include "searchvideosimpl.h"

#include "searchvideositemimpl.h"

#include "../searchvideos.h"
#include "../tools.h"

SearchVideosImpl::SearchVideosImpl(QWidget *parent, Qt::WFlags f)
	: QDialog(parent, f)
{
	setupUi(this);
	// set it as beta
	setWindowTitle(windowTitle() + " - Beta!");
	//
	searchVideos = new SearchVideos();
	//
	layoutSearchItems->setAlignment(Qt::AlignTop);
	//
	connect(btnSearch, SIGNAL(clicked()), this, SLOT(btnSearchClicked()));
	connect(btnClose, SIGNAL(clicked()), this, SLOT(btnCloseClicked()));
	connect(searchVideos, SIGNAL(searchStarted()), this, SLOT(searchStarted()));
	connect(searchVideos, SIGNAL(searchFinished()), this, SLOT(searchFinished()));
	connect(searchVideos, SIGNAL(searchResultAdded(SearchResultItem*)), this, SLOT(searchResultAdded(SearchResultItem*)));
	connect(searchVideos, SIGNAL(startedDownloadPreview(SearchResultItem*)), this, SLOT(startedDownloadPreview(SearchResultItem*)));
	connect(searchVideos, SIGNAL(finishedDownloadPreview(SearchResultItem*,bool)), this, SLOT(finishedDownloadPreview(SearchResultItem*,bool)));
}

SearchVideosImpl::~SearchVideosImpl()
{
	delete searchVideos;
}

void SearchVideosImpl::btnSearchClicked()
{
	scrollAreaSearchItems->hide();
	searchVideos->searchVideos(edtKeyWord->text(), spinBoxPage->value());
	scrollAreaSearchItems->show();
}

void SearchVideosImpl::searchStarted()
{
}

void SearchVideosImpl::searchFinished()
{
}

void SearchVideosImpl::searchResultAdded(SearchResultItem *searchResultItem)
{
	layoutSearchItems->addWidget(new SearchVideosItemImpl(this, searchResultItem));
}

void SearchVideosImpl::btnCloseClicked()
{
	delete this;
}

void SearchVideosImpl::startedDownloadPreview(SearchResultItem *searchItem)
{
	qDebug() << "TODO startedDownloadPreview(SearchResultItem *searchItem)";
}

void SearchVideosImpl::finishedDownloadPreview(SearchResultItem *searchItem, bool error)
{
	SearchVideosItemImpl *impl = getSearchVideosItemImplBySearchItem(searchItem);
	if (impl != NULL)
		impl->reloadPreview();
}

SearchVideosItemImpl* SearchVideosImpl::getSearchVideosItemImplBySearchItem(SearchResultItem *item)
{
	if (item != NULL)
		return this->findChild<SearchVideosItemImpl*>(item->getVideoId());
	else
		return NULL;
}
