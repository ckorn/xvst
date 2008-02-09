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

#include "mainformimpl.h"
//
MainFormImpl::MainFormImpl(QWidget * parent, Qt::WFlags f)
		: QMainWindow(parent, f)
{
	setupUi(this);
	// set caption
	setWindowTitle(QString(windowTitle()).arg(PROGRAM_VERSION));
	// init program options
	lastOptionsPage = 0;
	programOptions = new ProgramOptions(qApp->applicationDirPath());
	programOptions->load();
	// setu-up the main form
	centerWindow();
	// init main objects (program core)
	videoList = new VideoListController(programOptions);
	// set-up the lsvDownloadList widget
	QStringList headers;
	headers << tr(" Video ") << tr(" Size ") << tr(" Progress ") << tr(" Time ") << tr(" Speed ");
	// add the headers
	lsvDownloadList->setHeaderLabels(headers);
	// add the progress bar
	lsvDownloadList->setItemDelegate(new ProgressBarDelegate(this, videoList));
	// change headers sizes
	QFontMetrics fm = fontMetrics();
	QHeaderView *header = lsvDownloadList->header();
	// set sizes
	header->resizeSection(1, fm.width(headers.at(1) + "11.11 MB"));
	header->resizeSection(2, fm.width(headers.at(2) + "  100.00%  "));
	header->resizeSection(3, qMax(fm.width(headers.at(3) + "  "), fm.width(" 24h 59m 59s ")));
	header->resizeSection(4, qMax(fm.width(headers.at(4) + "  "), fm.width("999 KB/s")));
	// configure resize mode
	header->setHighlightSections(false);
	header->setStretchLastSection(false);
	header->setResizeMode(0, QHeaderView::Stretch);
	// set header text aligment
	QTreeWidgetItem * headerItem = lsvDownloadList->headerItem();
	headerItem->setTextAlignment(1, Qt::AlignRight   | Qt::AlignVCenter);
	headerItem->setTextAlignment(2, Qt::AlignHCenter | Qt::AlignVCenter);
	headerItem->setTextAlignment(3, Qt::AlignHCenter | Qt::AlignVCenter);
	headerItem->setTextAlignment(4, Qt::AlignHCenter | Qt::AlignVCenter);
	// configure the TrayIcon
	createTrayIcon();
	// init the completed popup
	completedPopup = new CompletedPopupImpl(this);
	// start the drag & drop window
	dragDropForm = new DragDropImpl(programOptions, videoList, this);
	// connect buttons (header)
	connect(spbDragDrop, SIGNAL(clicked()), this, SLOT(dragDropClicked())); //spb Drag&Drop (clicked)
	connect(spbUpdates, SIGNAL(clicked()), this, SLOT(updatesClicked())); //spb Updates (clicked)
	connect(spbHelp, SIGNAL(clicked()), this, SLOT(onlineHelpClicked())); //spb Online Help (clicked)
	connect(spbInformation, SIGNAL(clicked()), this, SLOT(informationClicked())); //spb Information (clicked)
	// connect buttons directories
	connect(spbSelectDownloadDir, SIGNAL(clicked()), this, SLOT(selectDownloadDirClicked())); //spb Open Download directory (clicked)
	connect(spbOpenDownloadDir, SIGNAL(clicked()), this, SLOT(openDownloadDirClicked())); //spb Open Download directory (clicked)
	// connect actions
	connect(actDragDrop, SIGNAL(triggered()), this, SLOT(dragDropClicked())); // actDragDrop (clicked)
	connect(actUpdates, SIGNAL(triggered()), this, SLOT(updatesClicked())); // actUpdates (clicked)
	connect(actOnlineHelp, SIGNAL(triggered()), this, SLOT(onlineHelpClicked())); // actOnlineHelp (clicked)
	connect(actInformation, SIGNAL(triggered()), this, SLOT(informationClicked())); // actInformation (clicked)
	connect(actRestoreApp, SIGNAL(triggered()), this, SLOT(restoreAppClicked())); // actRestoreApp (clicked)
	connect(actCloseApp, SIGNAL(triggered()), this, SLOT(closeAppClicked())); // actCloseApp (clicked)
	connect(actMoreOptions, SIGNAL(triggered()), this, SLOT(moreOptionsClicked())); // actMoreOptions (clicked)
	connect(actAddVideo, SIGNAL(triggered()), this, SLOT(addVideoClicked())); // actAddVideo (clicked)
	connect(actDeleteVideo, SIGNAL(triggered()), this, SLOT(deleteVideoClicked())); // actMoreOptions (clicked)
	connect(actStartDownload, SIGNAL(triggered()), this, SLOT(startDownloadVideoClicked())); // actStartDownload (clicked)
	connect(actCancelDownload, SIGNAL(triggered()), this, SLOT(cancelDownloadVideoClicked())); // actCancelDownload (clicked)
	connect(actClearList, SIGNAL(triggered()), this, SLOT(clearListClicked())); // actClearList (clicked)
	connect(actClearCompleted, SIGNAL(triggered()), this, SLOT(clearCompletedClicked())); // actClearCompleted (clicked)
	connect(actPlayVideo, SIGNAL(triggered()), this, SLOT(playVideoClicked())); // actPlayVideo (clicked)
	connect(actMoveUP, SIGNAL(triggered()), this, SLOT(moveItemUpClicked())); // actMoveUP (clicked)
	connect(actMoveDOWN, SIGNAL(triggered()), this, SLOT(moveItemDownClicked())); // actMoveDOWN (clicked)
	connect(actResetState, SIGNAL(triggered()), this, SLOT(resetStateClicked())); // actResetState (clicked)
	// edtDownloadDir
	connect(edtDownloadDir, SIGNAL(textChanged(const QString &)), this, SLOT(edtDownloadDirChanged(const QString &)));
	// connect buttons
	connect(btnAddVideo, SIGNAL(clicked()), this, SLOT(addVideoClicked())); //btnAddVideo (clicked)
	connect(btnDeleteVideo, SIGNAL(clicked()), this, SLOT(deleteVideoClicked())); //btnAddVideo (clicked)
	connect(btnStartDownload, SIGNAL(clicked()), this, SLOT(startDownloadVideoClicked())); //btnAddVideo (clicked)
	connect(btnCancelDownload, SIGNAL(clicked()), this, SLOT(cancelDownloadVideoClicked())); //btnAddVideo (clicked)
	connect(btnMoreOptions, SIGNAL(clicked()), this, SLOT(moreOptionsClicked())); //btnMoreOptions (clicked)
	connect(btnClearList, SIGNAL(clicked()), this, SLOT(clearListClicked())); //btnMoreOptions (clicked)
	connect(btnClearCompleted, SIGNAL(clicked()), this, SLOT(clearCompletedClicked())); //btnMoreOptions (clicked)
	// tray icon menu events
	connect(trayIconMenu, SIGNAL(aboutToShow()), this, SLOT(trayIconAboutToShow()));
	connect(actDownloadVideosAutomatically, SIGNAL(toggled(bool)), this, SLOT(optionSelected(bool)));
	connect(actConvertVideos, SIGNAL(toggled(bool)), this, SLOT(optionSelected(bool)));
	connect(actDisplayPopup, SIGNAL(toggled(bool)), this, SLOT(optionSelected(bool)));
	// connect check boxs
	connect(chbDownloadVideosAuto, SIGNAL(stateChanged(int)), this, SLOT(downloadAutomaticallyStateChanged(int)));
	connect(chbConvertVideos, SIGNAL(stateChanged(int)), this, SLOT(convertVideosStateChanged(int)));
	// VideoListController signals
	connect(videoList, SIGNAL(videoAdded(VideoItem *)), this, SLOT(videoAdded(VideoItem *))); //onVideList added
	connect(videoList, SIGNAL(videoDeleted(VideoItem *)), this, SLOT(videoDeleted(VideoItem *))); //onVideList deleted
	connect(videoList, SIGNAL(videoUpdated(VideoItem *)), this, SLOT(videoUpdated(VideoItem *))); //onVideList updated
	connect(videoList, SIGNAL(videoMoved(int, int)), this, SLOT(videoMoved(int, int))); //onVideList item moved
	// lsvDownloadList
	connect(lsvDownloadList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(videoItemDoubleClicked(QTreeWidgetItem *, int)));
	connect(lsvDownloadList, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(videoItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
	connect(lsvDownloadList, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(videoListContextMenu(const QPoint &)));
	// options
	connect(programOptions, SIGNAL(optionsLoadBefore()), this, SLOT(optionsDidSomething()));
	connect(programOptions, SIGNAL(optionsLoadAfter()), this, SLOT(optionsDidSomething()));
	connect(programOptions, SIGNAL(optionsSaveBefore()), this, SLOT(optionsDidSomething()));
	connect(programOptions, SIGNAL(optionsSaveAfter()), this, SLOT(optionsDidSomething()));
	connect(programOptions, SIGNAL(optionsUpdated()), this, SLOT(optionsDidSomething()));
	// update visual options
	updateVisualOptions();
	// session manager
	sessionManager = new SessionManager(programOptions);
	sessionManager->loadSession(videoList);
	// forms lockers
	addForm_active = false;
	optionsForm_active = false;
	infoForm_active = false;
	// shortcut paste
	shortCurtPasteURL = new QShortcut(QKeySequence(tr("Ctrl+V")), this);
	connect(shortCurtPasteURL, SIGNAL(activated()), this, SLOT(pasteURLfromClipboardClicked()));
	// updater timer
	if (programOptions->getCheckForUpdatesOnStartup())
		updaterTimer = this->startTimer(250);

	// videos examples
/*
	videoList->addVideo("http://es.youtube.com/watch?v=0z-hdo3-UEU");
	videoList->addVideo("http://www.metacafe.com/watch/952453/fart_football_funny_tape_ep34/");
	videoList->addVideo("http://video.google.es/videoplay?docid=-6002479527438850286");
	videoList->addVideo("http://video.google.com/videoplay?docid=-2078061509679387625");
	videoList->addVideo("http://www.zappinternet.com/video/tivWjuQyaP/Sabotaje-en-el-metro-de-Madrid");
	videoList->addVideo("http://www.dailymotion.com/es/cluster/music/featured/video/x3vdt4_james-brown-dance-lessons_music");
	videoList->addVideo("http://www.dumpalink.com/videos/No_whoppers-2jg2.html");
	videoList->addVideo("http://www.glumbert.com/media/bushrailroad");
	videoList->addVideo("http://sclipo.com/video/massage-videos-lower-back-wrap-around-massage");
	videoList->addVideo("http://www.lulu.tv/?p=20611");
	videoList->addVideo("http://www.livevideo.com/video/ShatnerVision/D65C961087944AC5AAA4C0BF2893591F/shatner-plans-for-the-future.aspx");
	videoList->addVideo("http://www.yikers.com/video_woman_attacked_by_cheetah.html");
	videoList->addVideo("http://www.123video.nl/playvideos.asp?MovieID=57709");
	videoList->addVideo("http://www.5min.com/Video/7-knots-every-scout-should-know-12334");
	videoList->addVideo("http://www.abum.com/62464/NASA-records-the-sounds-of-Jupiter.html");
	videoList->addVideo("http://www.aniboom.com/Player.aspx?v=94346&CATG=1&SortMethod=3&TimeLine=0");
	videoList->addVideo("http://www.bebo.com/FlashBox.jsp?FlashBoxId=3811155676");
	videoList->addVideo("http://www.break.com/index/monkey-needs-a-drink.html");
	videoList->addVideo("http://www.broadcaster.com/clip/32505");
	videoList->addVideo("http://videos.caught-on-video.com/video/0f0f5255-310d-4f95-87f5-9881018b4697.htm");
	videoList->addVideo("http://clip4e.com/play_po_belyo_na_rabotnoto_mqsto.htm");
	videoList->addVideo("http://www.video.ca/video.php?id=1178558732");
	videoList->addVideo("http://www.liveleak.com/view?i=969_1191475546");
	videoList->addVideo("http://www.tu.tv/videos/ficeb-2007_6");
	videoList->addVideo("http://www.spike.com/video/2938314/collection/25823/minisite/superbowl");
	videoList->addVideo("http://www.myspace.com/index.cfm?fuseaction=vids.individual&VideoID=23409638");
	videoList->addVideo("http://videos.cinemavip.com/informaciondecontenido.php?con=3015");
	videoList->addVideo("http://video.yahoo.com/video/play?vid=1899050&fr=&cache=1");
*/

/*
	// adult sites
	videoList->addVideo("http://yuvutu.com/modules.php?name=Video&op=view&video_id=147706");
	videoList->addVideo("http://badjojo.com/video_play_front.php?Id=6292");
	videoList->addVideo("http://www.desisextube.com/dst/view_video.php?viewkey=d7fbdea1afc51c7c6ad2&page=1&viewtype=&category=mv");
	videoList->addVideo("http://youporn.com/watch/126002");
	videoList->addVideo("http://tuporno.tv/videos/sexo-virtual-morenaza");
	videoList->addVideo("http://pornotube.com/media.php?m=1541072");
	videoList->addVideo("http://www.dalealplay.com/informaciondecontenido.php?con=62750");
	videoList->addVideo("http://www.shufuni.com/milf/FEA3744E-DC96-4FB6-9344-0D6B16C44081.html");
*/
}

MainFormImpl::~MainFormImpl()
{
	// delete shortcuts
	delete shortCurtPasteURL;

	// delete the session manager
	delete sessionManager;

	// delete forms
	delete dragDropForm;
	delete completedPopup;

	// delete program options
	delete programOptions;

	// delete main objects
	delete videoList;
	
	// delete tray icon
	delete trayIconMenu;
	delete trayIcon;
}

void MainFormImpl::centerWindow()
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

void MainFormImpl::closeEvent(QCloseEvent *event)
{
	if (videoList->isWorking())
	{
		if (QMessageBox::question(this, tr("Closing..."),
		                          tr("xVideoServiceThief is working, do you wish Cancel the current work?"),
		                          tr("Yes"), tr("No"), QString(), 0, 1) == 0)
		{
			videoList->cancelDownload();
			videoList->cancelConversion();
			event->accept();
		}
		else
			event->ignore();
	}
	// closing?
	if (event->isAccepted())
		sessionManager->saveSession(videoList);
}

void MainFormImpl::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == updaterTimer)
	{
		// kill this timer, after one executation
		this->killTimer(updaterTimer);
		
		// check if the xUpdater is installed (can install updates?)
		if (!Updates::canUpdate())
		{
			actUpdates->setEnabled(false);
			spbUpdates->setEnabled(false);
			// running the app for 1st time? then display this warning message
			if (programOptions->getFirstTime())
				QMessageBox::information(this, tr("Updates"),
				                               tr("xUpdater application is missing.<br><br>Reinstall xVideoServiceThief if you want update automatically the program."),
				                               tr("Ok"));
			// start the download list
			videoList->start();
			// ok, no more first time
			programOptions->setFirstTime(false);
		}
		else
			checkUpdates();
	}
}

void MainFormImpl::createTrayIcon()
{
	// create the Tray Icon context menu
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(actRestoreApp);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(actDragDrop);
	trayIconMenu->addAction(actAddVideo);
	trayIconMenu->addAction(actOpenDownloadDir);
	trayIconMenu->addSeparator();
	trayIconMenuOptions = trayIconMenu->addMenu(tr("Options"));
	trayIconMenuOptions->addAction(actDownloadVideosAutomatically);
	trayIconMenuOptions->addAction(actConvertVideos);
	trayIconMenuOptions->addAction(actDisplayPopup);
	trayIconMenuOptions->addSeparator();
	trayIconMenuOptions->addAction(actMoreOptions);
	trayIconMenu->addAction(actUpdates);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(actOnlineHelp);
	trayIconMenu->addAction(actInformation);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(actCloseApp);

	// set defaults
	trayIconMenu->setDefaultAction(actRestoreApp);

	// create the Tray Icon object
	lastTrayIconStr = ":/icons/images/film_go.png";
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
	trayIcon->setIcon(QIcon(lastTrayIconStr));
	trayIcon->setToolTip(this->windowTitle());
	trayIcon->show();
}

// buttons and actions events
void MainFormImpl::dragDropClicked()
{
	dragDropForm->show();
	hide();
}

void MainFormImpl::updatesClicked()
{
	if (videoList->isWorking())
	{
		QMessageBox::information(this, tr("Updates"),
		                               tr("Another process is currently working, please stop it or wait until the end of process."),
		                               tr("Ok"));
		return;
	}

	spbUpdates->setEnabled(false);
	actUpdates->setEnabled(false);
	
	if (!isVisible()) restoreAppClicked();
	
	CheckUpdatesImpl checkUpdatesForm(programOptions, true, this);
	checkUpdatesForm.exec();
	
	spbUpdates->setEnabled(true);
	actUpdates->setEnabled(true);
}

void MainFormImpl::onlineHelpClicked()
{
	QDesktopServices::openUrl(QUrl("http://xviservicethief.sourceforge.net/index.php?action=onlinehelp"));
}

void MainFormImpl::informationClicked()
{
	if (infoForm_active) return;

	infoForm_active = true;
	spbInformation->setEnabled(false);
	actInformation->setEnabled(false);

	InformationImpl informationForm(programOptions, this);
	informationForm.exec();

	spbInformation->setEnabled(true);
	actInformation->setEnabled(true);
	infoForm_active = false;
}

void MainFormImpl::restoreAppClicked()
{
	dragDropForm->hide();
	show();
}

void MainFormImpl::selectDownloadDirClicked()
{
	QString s = QFileDialog::getExistingDirectory(this, tr("Select the download directory:"),
	            edtDownloadDir->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// if is emtpy, cancel the proces
	if (s.isEmpty()) return;
	edtDownloadDir->setText(QDir::toNativeSeparators(s));
}

void MainFormImpl::openDownloadDirClicked()
{
	openDirectory(edtDownloadDir->text());
}

void MainFormImpl::closeAppClicked()
{
	close();
}

void MainFormImpl::addVideoClicked()
{
	if (addForm_active) return;

	addForm_active = true;
	actAddVideo->setEnabled(false);
	btnAddVideo->setEnabled(false);

	if (!isVisible()) restoreAppClicked();

	AddVideoImpl addVideoForm(videoList->getVideoInformation(), this);
	if (addVideoForm.exec() == QDialog::Accepted)
		videoList->addVideo(addVideoForm.edtURL->text());

	actAddVideo->setEnabled(true);
	btnAddVideo->setEnabled(true);
	addForm_active = false;
}

void MainFormImpl::deleteVideoClicked()
{
	videoList->deleteVideo(getSelectedVideoItem());
}

void MainFormImpl::startDownloadVideoClicked()
{
	VideoItem *videoItem;

	if (lsvDownloadList->currentItem() != NULL)
		if (!getSelectedVideoItem()->isDownloaded())
			videoItem = getSelectedVideoItem();
		else
			videoItem = videoList->getFirstReady();
	else
		videoItem = videoList->getFirstReady();
	// we have a video to download?
	if (videoItem != NULL)
	{
		videoList->startDownload(videoItem);
		updateVisualControls();
	}
}

void MainFormImpl::cancelDownloadVideoClicked()
{
	videoList->cancelDownload();
	btnCancelDownload->setEnabled(false);
}

void MainFormImpl::moreOptionsClicked()
{
	if (optionsForm_active) return;

	optionsForm_active = true;
	actMoreOptions->setEnabled(false);
	btnMoreOptions->setEnabled(false);
	
	if (!isVisible()) restoreAppClicked();

	OptionsImpl optionsForm(programOptions, sessionManager, videoList, lastOptionsPage, this);
	if (optionsForm.exec() == QDialog::Accepted)
	{
		updateVisualOptions();
		lastOptionsPage = optionsForm.getLastPageViewed();
	}

	actMoreOptions->setEnabled(true);
	btnMoreOptions->setEnabled(true);
	optionsForm_active = false;
}

void MainFormImpl::clearListClicked()
{
	videoList->clear();

	updateVisualControls();
}

void MainFormImpl::clearCompletedClicked()
{
	videoList->deleteCompletedItems();
}

void MainFormImpl::resetStateClicked()
{
	getSelectedVideoItem()->setAsNULL();
}

void MainFormImpl::moveItemUpClicked()
{
	videoList->moveUP(getSelectedVideoItem());
}

void MainFormImpl::moveItemDownClicked()
{
	videoList->moveDOWN(getSelectedVideoItem());
}

void MainFormImpl::playVideoClicked()
{
	VideoItem *videoItem = getSelectedVideoItem();

	if (videoItem->isCompleted())
		if (QFile::exists(videoItem->getVideoFileSavedTo()))
			openDirectory(getSelectedVideoItem()->getVideoFileSavedTo());
}

void MainFormImpl::downloadAutomaticallyStateChanged(int state)
{
	programOptions->setCanSendUpdateSignal(false);
	programOptions->setDownloadAutomatically(state == 2 ? true : false);
	programOptions->setCanSendUpdateSignal(true);
}

void MainFormImpl::edtDownloadDirChanged(const QString &text)
{
	programOptions->setDownloadDir(text);
}

void MainFormImpl::convertVideosStateChanged(int state)
{
	programOptions->setCanSendUpdateSignal(false);
	programOptions->setConvertVideos(state == 2 ? true : false);
	programOptions->setCanSendUpdateSignal(true);
}

// tray icon menu events
void MainFormImpl::trayIconAboutToShow()
{
	updateVisualOptions();
}

// tray icon menu events
void MainFormImpl::optionSelected(bool checked)
{
	if (sender() == actDownloadVideosAutomatically)
		programOptions->setDownloadAutomatically(checked);
	else if (sender() == actConvertVideos)
		programOptions->setConvertVideos(checked);
	else if (sender() == actDisplayPopup)
		programOptions->setDisplayPopup(checked);
}

// VideoList events
void MainFormImpl::videoAdded(VideoItem *videoItem)
{
	QTreeWidgetItem *item = new QTreeWidgetItem(lsvDownloadList);

	item->setData(0, Qt::UserRole, videoItem->getID());
	item->setData(2, Qt::UserRole, videoItem->getID());

	item->setTextAlignment(1, Qt::AlignRight   | Qt::AlignVCenter);
	item->setTextAlignment(2, Qt::AlignHCenter | Qt::AlignVCenter);
	item->setTextAlignment(3, Qt::AlignHCenter | Qt::AlignVCenter);
	item->setTextAlignment(4, Qt::AlignHCenter | Qt::AlignVCenter);

	item->setIcon(0, QIcon(videoList->getVideoInformation()->getHostImage(videoItem->getURL())));

	item->setSizeHint(0, QSize(18,18));

	videoUpdated(videoItem);
}

void MainFormImpl::videoDeleted(VideoItem *videoItem)
{
	delete getQTreeWidgetItemByVideoItem(videoItem);
	lsvDownloadList->setCurrentItem(NULL);
	updateVisualControls();
}

void MainFormImpl::videoUpdated(VideoItem *videoItem)
{
	QTreeWidgetItem *item = getQTreeWidgetItemByVideoItem(videoItem);

	item->setText(0, videoItem->getDisplayLabel());
	item->setText(1, videoItem->getDisplaySize());
	item->setText(2, videoItem->getDisplayProgress());
	item->setText(3, videoItem->getDisplayTimeRemaining());
	item->setText(4, videoItem->getDisplayDownloadSpeed());

	item->setToolTip(0, videoItem->getDisplayLabel());
	if (videoItem->isDownloading() || videoItem->isConverting())
		item->setToolTip(2, videoItem->getVideoStateAsString() + " (" + videoItem->getDisplayProgress() + ")");
	else
		item->setToolTip(2, "");

	// display completed popup
	if (videoItem->isCompleted())
	{
		// add to log
		sessionManager->addToLog(videoItem);
		// display popup
		if (programOptions->getDisplayPopup())
			completedPopup->displayPopup(item->icon(0),
			                             videoItem->getVideoInformation().title,
			                             videoItem->getVideoFileSavedTo());
	}

	// error form
	if (videoItem->hasErrors() && !videoItem->isReported() && programOptions->getDisplayBugReport())
	{
		// update tray icon
		QString trayIconStr = ":/icons/images/film_error.png";
		trayIcon->setIcon(QIcon(trayIconStr));
		lastTrayIconStr = trayIconStr;
		// display error report form
		BugReportImpl errorReport(programOptions, this);
		errorReport.fillErrorInfo(videoItem, videoList->getVideoInformation());
		errorReport.exec();
		// mark it as reported
		videoItem->setAsReported();
	}
	
	updateVisualControls();
}

void MainFormImpl::videoMoved(int from, int to)
{
	QTreeWidgetItem *item;
	QTreeWidgetItem *itemFrom = lsvDownloadList->topLevelItem(from)->clone();
	QTreeWidgetItem *itemTo = lsvDownloadList->topLevelItem(to)->clone();

	// FROM ITEM
	item = lsvDownloadList->topLevelItem(from);
	item->setData(0, Qt::UserRole, itemTo->data(0, Qt::UserRole));
	item->setData(2, Qt::UserRole, itemTo->data(2, Qt::UserRole));
	item->setIcon(0, itemTo->icon(0));
	item->setText(0, itemTo->text(0));
	item->setText(1, itemTo->text(1));
	item->setText(2, itemTo->text(2));
	item->setText(3, itemTo->text(3));
	item->setText(4, itemTo->text(4));
	item->setToolTip(0, itemTo->toolTip(0));
	item->setToolTip(2, itemTo->toolTip(2));

	// TO ITEM
	item = lsvDownloadList->topLevelItem(to);
	item->setData(0, Qt::UserRole, itemFrom->data(0, Qt::UserRole));
	item->setData(2, Qt::UserRole, itemFrom->data(2, Qt::UserRole));
	item->setIcon(0, itemFrom->icon(0));
	item->setText(0, itemFrom->text(0));
	item->setText(1, itemFrom->text(1));
	item->setText(2, itemFrom->text(2));
	item->setText(3, itemFrom->text(3));
	item->setText(4, itemFrom->text(4));
	item->setToolTip(0, itemFrom->toolTip(0));
	item->setToolTip(2, itemFrom->toolTip(2));

	// select the "to item"
	lsvDownloadList->setCurrentItem(lsvDownloadList->topLevelItem(to));

	// destroy temporal items
	delete itemFrom;
	delete itemTo;
}

// shortcut signals
void MainFormImpl::pasteURLfromClipboardClicked()
{
	dragDropForm->addVideoURL(QApplication::clipboard()->text());
}

// lsvDownloadList functions
QTreeWidgetItem* MainFormImpl::getQTreeWidgetItemByVideoItem(VideoItem *videoItem)
{
	for (int i = 0; i < lsvDownloadList->topLevelItemCount(); ++i)
	{
		QVariant variant = lsvDownloadList->topLevelItem(i)->data(0, Qt::UserRole);
		if (videoList->getVideoItemFromQVAriant(variant) == videoItem)
			return lsvDownloadList->topLevelItem(i);
	}
	return NULL;
}

VideoItem* MainFormImpl::getSelectedVideoItem()
{
	if (lsvDownloadList->currentItem() != NULL)
		return videoList->getVideoItemFromQVAriant(lsvDownloadList->currentItem()->data(0, Qt::UserRole));
	else
		return NULL;
}

// options controls
void MainFormImpl::updateVisualOptions()
{
	// update internal options
	videoList->updateOptions();

	// actions
	actDownloadVideosAutomatically->setChecked(programOptions->getDownloadAutomatically());
	actConvertVideos->setEnabled(QFile::exists(programOptions->getFfmpegLibLocation()));
	actConvertVideos->setChecked(actConvertVideos->isEnabled() ? programOptions->getConvertVideos() : false);
	actDisplayPopup->setChecked(programOptions->getDisplayPopup());

	// controls
	chbDownloadVideosAuto->setChecked(programOptions->getDownloadAutomatically());
	chbConvertVideos->setEnabled(QFile::exists(programOptions->getFfmpegLibLocation()));
	chbConvertVideos->setChecked(chbConvertVideos->isEnabled() ? programOptions->getConvertVideos() : false);
	edtDownloadDir->setText(programOptions->getDownloadDir());
}

void MainFormImpl::checkUpdates()
{
	bool noUpdates = true;
	QDate nextUpdate = programOptions->getLastUpdate().addDays(programOptions->getCheckForUpdatesEvery());
	
	if (nextUpdate <= QDate::currentDate())
	{
		spbUpdates->setEnabled(false);
		actUpdates->setEnabled(false);
		
		CheckUpdatesImpl checkUpdatesForm(programOptions, false, this);
		noUpdates = checkUpdatesForm.exec() != QDialog::Accepted;
		
		spbUpdates->setEnabled(true);
		actUpdates->setEnabled(true);
	}
	// if no updates are ready then, start the main loop of video List
	if (noUpdates)
		videoList->start();
}

// visual controls
void MainFormImpl::updateVisualControls()
{
	VideoItem *videoItem = getSelectedVideoItem();
	
	if (videoItem == NULL)
	{
		btnDeleteVideo->setEnabled(false);
		btnStartDownload->setEnabled(false);
		btnCancelDownload->setEnabled(!videoList->canStartDownload());

		actPlayVideo->setEnabled(false);
		actResetState->setEnabled(false);
	}
	else // item is clicked
	{
		btnDeleteVideo->setEnabled(videoItem->isRemovable());
		btnStartDownload->setEnabled(videoList->canStartDownload() && videoItem->isReady());
		btnCancelDownload->setEnabled(!videoList->canStartDownload());

		actPlayVideo->setEnabled(videoItem->isCompleted());
		actResetState->setEnabled(videoItem->isCanceled() ||
		                          videoItem->isBlocked() ||
		                          videoItem->hasErrors());
	}
	btnClearList->setEnabled(!videoList->isWorking() && videoList->getVideoItemCount(true) > 0);
	btnClearCompleted->setEnabled(videoList->getCompletedItemsCount() > 0);

	// update actions
	actDeleteVideo->setEnabled(btnDeleteVideo->isEnabled());
	actStartDownload->setEnabled(btnStartDownload->isEnabled());
	actCancelDownload->setEnabled(btnCancelDownload->isEnabled());
	actClearList->setEnabled(btnClearList->isEnabled());
	actClearCompleted->setEnabled(btnClearCompleted->isEnabled());
	
	// update tray icon
	QString trayIconStr = videoList->isWorking() ? ":/icons/images/film_save.png" : ":/icons/images/film_go.png";
	if (lastTrayIconStr != trayIconStr)
		trayIcon->setIcon(QIcon(trayIconStr));
	lastTrayIconStr = trayIconStr;
}

// lsvDownloadList signals
void MainFormImpl::videoItemDoubleClicked(QTreeWidgetItem *item, int column)
{
	playVideoClicked();
}

void MainFormImpl::videoItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	if (current != NULL)
		updateVisualControls();
}

void MainFormImpl::videoListContextMenu(const QPoint & pos)
{
	QTreeWidgetItem *item = lsvDownloadList->itemAt(pos);

	if (item != NULL)
	{
		QMenu *videoItemMenu = new QMenu(this);
		videoItemMenu->addAction(actPlayVideo);
		videoItemMenu->addSeparator();
		videoItemMenu->addAction(actAddVideo);
		videoItemMenu->addAction(actDeleteVideo);
		videoItemMenu->addSeparator();
		videoItemMenu->addAction(actStartDownload);
		videoItemMenu->addAction(actCancelDownload);
		videoItemMenu->addSeparator();
		videoItemMenu->addAction(actMoveUP);
		videoItemMenu->addAction(actMoveDOWN);
		videoItemMenu->addSeparator();
		videoItemMenu->addAction(actResetState);
		videoItemMenu->addSeparator();
		videoItemMenu->addAction(actClearList);
		videoItemMenu->addAction(actClearCompleted);
		// set defaut
		videoItemMenu->setDefaultAction(actPlayVideo);
		// update move up/down actions
		actMoveUP->setEnabled(lsvDownloadList->topLevelItem(0) != item);
		actMoveDOWN->setEnabled(lsvDownloadList->topLevelItem(lsvDownloadList->topLevelItemCount() - 1) != item);
		// display popup
		videoItemMenu->exec(QCursor::pos());
		// destrueix el submenu
		delete videoItemMenu;
	}
	else
	{
		lsvDownloadList->setCurrentItem(NULL);
		updateVisualControls();

		QMenu *videoItemMenu = new QMenu(this);
		videoItemMenu->addAction(actAddVideo);
		videoItemMenu->addSeparator();
		videoItemMenu->addAction(actClearList);
		videoItemMenu->addAction(actClearCompleted);
		// display popup
		videoItemMenu->exec(QCursor::pos());
		// destrueix el submenu
		delete videoItemMenu;
	}
}

// options signals
void MainFormImpl::optionsDidSomething()
{
	updateVisualOptions();
}
