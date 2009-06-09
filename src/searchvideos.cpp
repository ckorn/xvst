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

#include "searchvideos.h"

#include "http.h"
#include "videoinformation.h"

SearchVideos::SearchVideos()
{
	searchResults = NULL;
	imageCatcher = new SearchResultsPreviewCatcher();
	// connect signals
	connect(imageCatcher, SIGNAL(startedDownloadPreview(SearchResultItem*)), this, SLOT(privateStartedDownloadPreview(SearchResultItem*)));
	connect(imageCatcher, SIGNAL(finishedDownloadPreview(SearchResultItem*,bool)), this, SLOT(privateFinishedDownloadPreview(SearchResultItem*,bool)));
}

SearchVideos::~SearchVideos()
{
	if (searchResults != NULL) delete searchResults;
	delete imageCatcher;
}

void SearchVideos::searchVideos(QString keyWords, int page)
{
	if (searchResults != NULL) delete searchResults;

	VideoInformationPlugin *plugin = VideoInformation::getLastVideoInformationInstance()->getRegisteredPlugin("video.google.");
	searchResults = new SearchResults(plugin->searchVideos(keyWords, page));

	for (int n = 0; n < searchResults->getSearchResultCount(); n++)
	{
		// emit add result
		emit searchResultAdded(searchResults->getSearchResult(n));
		// add to previews download queue
		imageCatcher->addPreview(searchResults->getSearchResult(n));
	}
}

void SearchVideos::privateStartedDownloadPreview(SearchResultItem *searchItem)
{
	emit startedDownloadPreview(searchItem);
}

void SearchVideos::privateFinishedDownloadPreview(SearchResultItem *searchItem, bool error)
{
	emit finishedDownloadPreview(searchItem, error);
}

// SearchResultsPreviewCatcher class

SearchResultsPreviewCatcher::SearchResultsPreviewCatcher()
{
	queue = new QList<SearchResultItem*>();
	http = new Http();
	// connect signals
	connect(http, SIGNAL(downloadFinished(const QFileInfo)), this, SLOT(downloadFinished(const QFileInfo)));
	connect(http, SIGNAL(downloadError(int)), this, SLOT(downloadError(int)));
}

SearchResultsPreviewCatcher::~SearchResultsPreviewCatcher()
{
	stop();
	delete queue;
}

void SearchResultsPreviewCatcher::addPreview(SearchResultItem *searchItem)
{
	queue->append(searchItem);
	// if is not downloading any image preview, then start it
	if (!http->isDownloading())
		QTimer::singleShot(50, this, SLOT(downloadNextPreview()));
}

void SearchResultsPreviewCatcher::stop()
{
	queue->clear();
}

void SearchResultsPreviewCatcher::downloadFinished(const QFileInfo)
{
	// emit download finished
	emit finishedDownloadPreview(queue->takeFirst(), false);
	// download next preview
	QTimer::singleShot(50, this, SLOT(downloadNextPreview()));
}

void SearchResultsPreviewCatcher::downloadError(int error)
{
	// emit download finished with errors
	emit finishedDownloadPreview(queue->takeFirst(), true);
	// download next preview
	QTimer::singleShot(50, this, SLOT(downloadNextPreview()));
}

void SearchResultsPreviewCatcher::downloadNextPreview()
{
	// has previews to download?
	if (!queue->isEmpty())
		// download first preview
		http->download(QUrl(queue->first()->getImageUrl()), QDir::tempPath(), queue->first()->getPreviewFileName(), false);
}

// SearchResults class

SearchResults::SearchResults()
{
	results = new QList<SearchResultItem *>();
}

SearchResults::SearchResults(const SearchResults &other) : QObject()
{
	results = new QList<SearchResultItem *>();
	copyFrom(other);
}

SearchResults::~SearchResults()
{
	removeAllSearchResults();
	delete results;
}

SearchResults &SearchResults::operator=(const SearchResults &other)
{
	copyFrom(other);
	// return my self updated
	return *this;
}

void SearchResults::copyFrom(const SearchResults &other)
{
	// remove old items
	removeAllSearchResults();
	// copy all items from "other"
	for (int n = 0; n < other.results->count(); n++)
	{
		SearchResultItem *item = other.results->at(n);
		addSearchResult(item->getVideoUrl(), item->getImageUrl(), item->getTitle(),
						item->getDescription(), item->getDuration());
	}
}

void SearchResults::removeAllSearchResults()
{
	while (!results->isEmpty())
		delete results->takeFirst();
}

void SearchResults::addSearchResult(QString videoUrl, QString imageUrl, QString title,
									QString description, int duration)
{
	results->append(new SearchResultItem(videoUrl, imageUrl, title, description, duration));
}

SearchResultItem* SearchResults::getSearchResult(int index)
{
	if (index >= 0 && index < getSearchResultCount())
		return results->at(index);
	else
		return NULL;
}

int SearchResults::getSearchResultCount()
{
	return results->count();
}

// SearchResultItem class

SearchResultItem::SearchResultItem(QString videoUrl, QString imageUrl, QString title,
									QString description, int duration)
{
	this->videoUrl = videoUrl.trimmed();
	this->imageUrl = imageUrl.trimmed();
	this->title = title.trimmed();
	this->description = description.trimmed();
	this->duration = duration;
	// auto-generate the video id (MD5 hash of video URL)
	this->videoId = QCryptographicHash::hash(videoUrl.toAscii(), QCryptographicHash::Md5).toHex();
}

QString SearchResultItem::getVideoId()
{
	return videoId;
}

QString SearchResultItem::getVideoUrl()
{
	return videoUrl;
}

QString SearchResultItem::getImageUrl()
{
	return imageUrl;
}

QString SearchResultItem::getTitle()
{
	return title;
}

QString SearchResultItem::getDescription()
{
	return description;
}

int SearchResultItem::getDuration()
{
	return duration;
}

QString SearchResultItem::getPreviewFileName()
{
	return "/" + videoId + ".xvst.preview";
}
