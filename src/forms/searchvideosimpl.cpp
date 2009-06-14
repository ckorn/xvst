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

#include "../videoinformation.h"
#include "../searchvideos.h"
#include "../tools.h"

SearchVideosImpl::SearchVideosImpl(QWidget *parent, Qt::WFlags f)
	: QDialog(parent, f)
{
	setupUi(this);
	centerWindow();
	// set it as beta
	setWindowTitle(windowTitle() + " - Beta!");
	//
	searchVideos = new SearchVideos();
	loadingMovie = new QMovie(":/search/images/loader.gif");
	loadingMovie->start();
	//
	layoutSearchItems->setAlignment(Qt::AlignTop);
	//
	updateButons(true);
	fillSearchServices();
	//
	connect(this, SIGNAL(finished(int)), this, SLOT(finished(int)));
	connect(edtKeyWord, SIGNAL(textChanged(QString)), this, SLOT(edtKeyWordChanged(QString)));
	connect(btnSearch, SIGNAL(clicked()), this, SLOT(btnSearchClicked()));
	connect(btnPrevSearch, SIGNAL(clicked()), this, SLOT(btnPrevSearchClicked()));
	connect(btnNextSearch, SIGNAL(clicked()), this, SLOT(btnNextSearchClicked()));
	connect(searchVideos, SIGNAL(searchStarted()), this, SLOT(searchStarted()));
	connect(searchVideos, SIGNAL(searchFinished()), this, SLOT(searchFinished()));
	connect(searchVideos, SIGNAL(searchResultAdded(SearchResultItem*)), this, SLOT(searchResultAdded(SearchResultItem*)));
	connect(searchVideos, SIGNAL(startedDownloadPreview(SearchResultItem*)), this, SLOT(startedDownloadPreview(SearchResultItem*)));
	connect(searchVideos, SIGNAL(finishedDownloadPreview(SearchResultItem*,bool)), this, SLOT(finishedDownloadPreview(SearchResultItem*,bool)));
}

SearchVideosImpl::~SearchVideosImpl()
{
	delete loadingMovie;
	delete searchVideos;
}

void SearchVideosImpl::finished(int)
{
	this->deleteLater();
}

void SearchVideosImpl::edtKeyWordChanged(QString text)
{
	spinBoxPage->setValue(1);
	updateButons(text.isEmpty());
}

void SearchVideosImpl::btnSearchClicked()
{
	clearResults();
	searchVideos->searchVideos(edtKeyWord->text(), spinBoxPage->value(),
							   cmbSearchIn->itemData(cmbSearchIn->currentIndex(), Qt::UserRole).toString());
	updateButons(true);
}

void SearchVideosImpl::btnPrevSearchClicked()
{
	if (spinBoxPage->value() > 1)
	{
		spinBoxPage->setValue(spinBoxPage->value() - 1);
		btnSearch->click();
	}
}

void SearchVideosImpl::btnNextSearchClicked()
{
	spinBoxPage->setValue(spinBoxPage->value() + 1);
	btnSearch->click();
}

void SearchVideosImpl::searchStarted()
{
	btnSearch->setEnabled(false);
	// create loading info
	QLabel *loadingText, *loadingImage;
	layoutSearchItems->addStretch();
	loadingText = new QLabel(tr("Searching \"<b>%1</b>\" (page %2)... please wait...").arg(edtKeyWord->text()).arg(spinBoxPage->value()), this);
	loadingText->setAlignment(Qt::AlignHCenter);
	layoutSearchItems->addWidget(loadingText);
	loadingImage = new QLabel(this);
	loadingImage->setMovie(loadingMovie);
	loadingImage->setAlignment(Qt::AlignHCenter);
	layoutSearchItems->addWidget(loadingImage);
	layoutSearchItems->addStretch();
}

void SearchVideosImpl::searchFinished()
{
	// remove the loading "status"...
	clearResults();
	// enable buttons again
	updateButons(false);
	// set the search summary
	lblResultsCount->setText(searchVideos->getSearchSummary());

	// add a default strech (top alignment)
	//qDebug() << "addStretch();";
	//layoutSearchItems->addStretch();
}

void SearchVideosImpl::searchResultAdded(SearchResultItem *searchResultItem)
{
	// add a new widget before the strech item (top alignment)
	//layoutSearchItems->insertWidget(layoutSearchItems->count() - 1, new SearchVideosItemImpl(this, searchResultItem, loadingMovie));
	layoutSearchItems->addWidget(new SearchVideosItemImpl(this, searchResultItem, loadingMovie));
}

void SearchVideosImpl::startedDownloadPreview(SearchResultItem *searchItem)
{
/*
	SearchVideosItemImpl *impl = getSearchVideosItemImplBySearchItem(searchItem);
	if (impl != NULL)
		impl->setAsLoading();
*/
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

void SearchVideosImpl::clearResults()
{
	while (!layoutSearchItems->isEmpty())
		delete layoutSearchItems->takeAt(0)->widget();
}

void SearchVideosImpl::updateButons(bool searching)
{
	btnPrevSearch->setEnabled(!searching && spinBoxPage->value() > 1);
	btnNextSearch->setEnabled(!searching);
	btnSearch->setEnabled(!searching);
}

void SearchVideosImpl::fillSearchServices()
{
	QList<VideoInformationPlugin*> searchEngines = VideoInformation::instance()->getAllSearchPlugins();
	// has more than one item?
	if (searchEngines.count() > 1)
	{
		cmbSearchIn->addItem(tr("All video services"), QVariant("?"));
		cmbSearchIn->addItem(tr("Custom search"), QVariant("*"));
		cmbSearchIn->insertSeparator(2);
	}
	// add them to combobox
	for (int n = 0; n < searchEngines.count(); n++)
		cmbSearchIn->addItem(*(searchEngines.at(n)->getIcon()),
							 searchEngines.at(n)->getCaption(),
							 QVariant(searchEngines.at(n)->getID()));
}

void SearchVideosImpl::centerWindow()
{
	QDesktopWidget *desktop = QApplication::desktop();

	int screenWidth, width;
	int screenHeight, height;
	int x, y;
	QSize windowSize;

	screenWidth = desktop->width();
	screenHeight = desktop->height();

	windowSize = size();
	width = windowSize.width();
	height = windowSize.height();

	x = (screenWidth - width) / 2;
	y = (screenHeight - height) / 2;
	y -= 50;

	move(x, y);
}
