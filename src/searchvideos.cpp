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
#include "options.h"
#include "languages.h"

SearchVideos::SearchVideos()
{
	searchResults = new SearchResults();
	imageCatcher = new SearchResultsPreviewCatcher();
	// inits
	internalKeyWords = "";
	internalPage = 0;
	// connect signals
	connect(imageCatcher, SIGNAL(startedDownloadPreview(SearchResultItem*)), this, SLOT(privateStartedDownloadPreview(SearchResultItem*)));
	connect(imageCatcher, SIGNAL(finishedDownloadPreview(SearchResultItem*,bool)), this, SLOT(privateFinishedDownloadPreview(SearchResultItem*,bool)));
}

SearchVideos::~SearchVideos()
{
	delete searchResults;
	delete imageCatcher;
}

void SearchVideos::searchVideos(QString keyWords, int page, QString pluginId)
{
	if (!this->isRunning())
	{
		internalPluginId = pluginId;
		internalKeyWords = keyWords;
		internalPage = page;
		// start thread
		start();
	}
}

QString SearchVideos::getSearchSummary()
{
	return searchResults->getSummary();
}

void SearchVideos::removePreviews()
{
	QStringList previews;
	QDir previewsDir(QDir::tempPath());
	// get all previews
	previews = previewsDir.entryList(QDir::Files, QDir::Name);
	previews = previews.filter(".xvst.preview");
	// remove all previews from temporary dir
	foreach (QString preview, previews)
		if (preview.endsWith(".xvst.preview"))
			QFile::remove(QDir::tempPath() + "/" + preview);
}

void SearchVideos::privateStartedDownloadPreview(SearchResultItem *searchItem)
{
	emit startedDownloadPreview(searchItem);
}

void SearchVideos::privateFinishedDownloadPreview(SearchResultItem *searchItem, bool error)
{
	emit finishedDownloadPreview(searchItem, error);
}

void SearchVideos::run()
{
	// search started
	emit searchStarted();
	// stop getting results
	imageCatcher->stop();
	// get results
	VideoInformationPlugin *plugin = VideoInformation::instance()->getRegisteredPlugin(internalPluginId);
	SearchResults results = plugin->searchVideos(internalKeyWords, internalPage);
	searchResults->removeAllSearchResults();
	searchResults->addSearchResults(results);
	searchResults->setSummary(results.getSummary());
	// search finished
	emit searchFinished();
	// show results
	for (int n = 0; n < searchResults->getSearchResultCount(); n++)
	{
		// emit add result
		emit searchResultAdded(searchResults->getSearchResult(n));
		// add to previews download previews
		imageCatcher->addPreview(searchResults->getSearchResult(n));
	}
	// download previews
	imageCatcher->downloadPreviews();
}

// SearchResultsPreviewCatcher class

SearchResultsPreviewCatcher::SearchResultsPreviewCatcher()
{
	previews = new QList<SearchResultItem*>();
	http = new Http();
	// connect signals
	connect(http, SIGNAL(downloadFinished(const QFileInfo)), this, SLOT(downloadFinished(const QFileInfo)));
	connect(http, SIGNAL(downloadError(int)), this, SLOT(downloadError(int)));
}

SearchResultsPreviewCatcher::~SearchResultsPreviewCatcher()
{
	stop();
	delete previews;
}

void SearchResultsPreviewCatcher::addPreview(SearchResultItem *searchItem)
{
	if (QFile(searchItem->getPreviewFileName(true)).size() == 0)
		previews->append(searchItem);
}

void SearchResultsPreviewCatcher::downloadPreviews()
{
	// start to download previews
	downloadNextPreview();
}

void SearchResultsPreviewCatcher::stop()
{
	previews->clear();
}

void SearchResultsPreviewCatcher::downloadFinished(const QFileInfo)
{
	if (previews->isEmpty()) return;
	// emit download finished
	emit finishedDownloadPreview(previews->takeFirst(), false);
	// download next preview
	QTimer::singleShot(50, this, SLOT(downloadNextPreview()));
}

void SearchResultsPreviewCatcher::downloadError(int error)
{
	if (previews->isEmpty()) return;
	// emit download finished with errors
	emit finishedDownloadPreview(previews->takeFirst(), true);
	// download next preview
	QTimer::singleShot(50, this, SLOT(downloadNextPreview()));
}

void SearchResultsPreviewCatcher::downloadNextPreview()
{
	// has previews to download?
	if (!previews->isEmpty())
	{
		emit startedDownloadPreview(previews->first());
		// check if this preview already exists
//		if (QFile::exists(previews->first()->getPreviewFileName(true)))
//			//downloadFinished(QFileInfo(previews->first()->getPreviewFileName(true)));
//			QTimer::singleShot(50, this, SLOT(previewIgnored()));
//		else // download first preview
		{
			int httpResult = http->download(QUrl(previews->first()->getImageUrl()), QDir::tempPath(), previews->first()->getPreviewFileName(), false);
			// error?
			if (httpResult != 0)
				downloadError(httpResult);
		}
	}
}

// SearchResults class

SearchResults::SearchResults()
{
	results = new QList<SearchResultItem *>();
	// get current user language id
	QString langFile = ProgramOptions::instance()->getLanguageFile(true);
	Language *lang = LanguageManager::getLanguageInfo(langFile);
	userLanguage = lang != NULL ? lang->getLangLocale() : "en";
}

SearchResults::SearchResults(const SearchResults &other) : QObject()
{
	results = new QList<SearchResultItem *>();
	copyFrom(other, true);
	summary = other.summary;
	userLanguage = other.userLanguage;
}

SearchResults::~SearchResults()
{
	removeAllSearchResults();
	delete results;
}

SearchResults &SearchResults::operator=(const SearchResults &other)
{
	copyFrom(other, true);
	summary = other.summary;
	userLanguage = other.userLanguage;
	// return my self updated
	return *this;
}

void SearchResults::copyFrom(const SearchResults &other, bool clear)
{
	// remove old items
	if (clear) removeAllSearchResults();
	// copy all items from "other"
	for (int n = 0; n < other.results->count(); n++)
	{
		SearchResultItem *item = other.results->at(n);
		addSearchResult(item->getVideoUrl(), item->getImageUrl(), item->getTitle(),
						item->getDescription(), item->getDuration(), item->getRating());
	}
}

void SearchResults::removeAllSearchResults()
{
	while (!results->isEmpty())
		delete results->takeFirst();
}

void SearchResults::addSearchResult(QString videoUrl, QString imageUrl, QString title,
									QString description, int duration, double rating)
{
	results->append(new SearchResultItem(videoUrl, imageUrl, title, description, duration, rating));
}

void SearchResults::addSearchResults(const SearchResults &searchResults)
{
	copyFrom(searchResults, false);
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

void SearchResults::setSummary(QString value)
{
	summary = value;
}

QString SearchResults::getSummary()
{
	return summary;
}

QString SearchResults::getUserLanguage()
{
	return userLanguage;
}

// SearchResultItem class

SearchResultItem::SearchResultItem(QString videoUrl, QString imageUrl, QString title,
									QString description, int duration, double rating)
{
	this->videoUrl = videoUrl.trimmed();
	this->imageUrl = imageUrl.trimmed();
	this->title = title.trimmed();
	this->description = description.trimmed();
	this->duration = duration;
	this->rating = rating;
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

double SearchResultItem::getRating()
{
	return rating;
}

QString SearchResultItem::getPreviewFileName(bool withFullPath)
{
	if (withFullPath)
		return QDir::tempPath() + "/" + videoId + ".xvst.preview";
	else
		return "/" + videoId + ".xvst.preview";
}
