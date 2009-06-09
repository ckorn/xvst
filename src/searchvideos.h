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

#ifndef SEARCHVIDEOS_H
#define SEARCHVIDEOS_H

#include <QtCore>

class Http;

class SearchResultItem : public QObject
{
	private:
		QString videoId;		//! Video id
		QString videoUrl;		//! Video url
		QString imageUrl;		//! Video image preview url
		QString title;			//! Video title
		QString description;	//! Video description
		int duration;			//! Video duration
	public:
		/*! Class constructor */
		SearchResultItem(QString videoUrl, QString imageUrl, QString title,
						 QString description, int duration);
		/*! Get the video Id */
		QString getVideoId();
		/*! Get the video url */
		QString getVideoUrl();
		/*! Get the video image preview url */
		QString getImageUrl();
		/*! Get the video title */
		QString getTitle();
		/*! Get the video description */
		QString getDescription();
		/*! Get the video duration */
		int getDuration();
		/*! Get the preview file name */
		QString getPreviewFileName();
};

class SearchResults : public QObject
{
Q_OBJECT
	private:
		QList<SearchResultItem *> *results;	//! List of search results
		/*! Copy elements from another SearchResults */
		void copyFrom(const SearchResults &);
		/*! Remove all search results */
		void removeAllSearchResults();
	public:
		/*! Class constructor */
		SearchResults();
		/*! copy class constructor */
		SearchResults(const SearchResults&);
		/*! Class destructor */
		~SearchResults();
		/*! Override = operator */
		SearchResults &operator=(const SearchResults &);
		/*! Add a new search result */
		void addSearchResult(QString videoUrl, QString imageUrl, QString title, QString description, int duration);
		/*! Get a search result */
		SearchResultItem *getSearchResult(int index);
		/*! Get search results count */
		int getSearchResultCount();
};

class SearchResultsPreviewCatcher : public QObject
{
Q_OBJECT
	private:
		Http *http;							//!< Download class
		QList<SearchResultItem*> *queue;	//!< List of previews to download
	public:
		/*! Class constructor */
		SearchResultsPreviewCatcher();
		/*! Class destructor */
		~SearchResultsPreviewCatcher();
		/*! Add a new  */
		void addPreview(SearchResultItem *searchItem);
		/*! Stop and also clear the downloads queue */
		void stop();
	private slots:
		/*! Preview download finished */
		void downloadFinished(const QFileInfo destFile);
		/*! Preivew download error */
		void downloadError(int error);
		/*! Download the next preview */
		void downloadNextPreview();
	signals:
		/*! Signal send when download preview started */
		void startedDownloadPreview(SearchResultItem *searchItem);
		/*! Signal send when download preview finished */
		void finishedDownloadPreview(SearchResultItem *searchItem, bool error);
};

class SearchVideos : public QObject
{
Q_OBJECT
	private:
		SearchResults *searchResults;				//!< List with search results
		SearchResultsPreviewCatcher *imageCatcher;	//!< Image preview downloader
	public:
		/*! Class constructor */
		SearchVideos();
		/*! Class destructor */
		~SearchVideos();
		/*! Start to search videos */
		void searchVideos(QString keyWords, int page);
	private slots:
		/*! Download preview started */
		void privateStartedDownloadPreview(SearchResultItem *searchItem);
		/*! Download preview finished (w/o errors) */
		void privateFinishedDownloadPreview(SearchResultItem *searchItem, bool error);
	signals:
		/*! New search started */
		void searchStarted();
		/*! The current search finished */
		void searchFinished();
		/*! A new search result has been added */
		void searchResultAdded(SearchResultItem *searchResultItem);
		/*! Signal send when download preview started */
		void startedDownloadPreview(SearchResultItem *searchItem);
		/*! Signal send when download preview finished */
		void finishedDownloadPreview(SearchResultItem *searchItem, bool error);
};

#endif // SEARCHVIDEOS_H
