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

#include "optionsimpl.h"
//
OptionsImpl::OptionsImpl(ProgramOptions *programOptions, SessionManager *sessionManager,
                         VideoInformation *videoInformation, int lastOptionsPage,
                         QWidget * parent, Qt::WFlags f)
		: QDialog(parent, f)
{
	setupUi(this);
	this->programOptions = programOptions;
	this->sessionManager = sessionManager;
	this->videoInformation = videoInformation;
	this->lastPageViewed = lastOptionsPage;
	languageManager = new LanguageManager;
	//signals
	connect(btnOk, SIGNAL(clicked()), this, SLOT(btnOkClicked())); //btn Ok (clicked)
	connect(spbSelectDownloadDir, SIGNAL(pressed()), this, SLOT(spbSelectDownloadDirPressed()));
	connect(spbSelectFFmpegLib, SIGNAL(pressed()), this, SLOT(spbSelectFFmpegLibPressed()));
	connect(edtFFmpegLib, SIGNAL(textChanged(const QString &)), this, SLOT(ffmpegTextChanged(const QString &)));
	connect(btnViewLog, SIGNAL(clicked()), this, SLOT(btnViewLogClicked()));
	connect(btnClearLog, SIGNAL(clicked()), this, SLOT(btnClearLogClicked()));
	connect(chbSaveRestore, SIGNAL(clicked(bool)), this, SLOT(chbSaveRestoreStateClicked(bool)));
	connect(spbRemove, SIGNAL(pressed()), this, SLOT(spbRemovePressed()));
	connect(spbRemoveAll, SIGNAL(pressed()), this, SLOT(spbRemoveAllPressed()));
	connect(spbAddAll, SIGNAL(pressed()), this, SLOT(spbAddAllPressed()));
	connect(spbAdd, SIGNAL(pressed()), this, SLOT(spbAddPressed()));
	connect(lsvServices1, SIGNAL(itemSelectionChanged()), this, SLOT(servicesItemSelectionChanged()));
	connect(lsvServices2, SIGNAL(itemSelectionChanged()), this, SLOT(servicesItemSelectionChanged()));
	connect(lsvLanguages, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(langCurrentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
	connect(btnUseThis, SIGNAL(clicked()), this, SLOT(btnUseThisClicked()));
	connect(lsvLanguages, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(langItemDoubleClicked(QTreeWidgetItem *, int)));
	// create menu
	createMenu();
	// add info
	fillInitialData();
	// add languages
	fillLanguages();
	// set values
	setInitialOptionsValues();
}

OptionsImpl::~OptionsImpl()
{
	delete languageManager;
}

int OptionsImpl::getLastPageViewed()
{
	return lastPageViewed;
}

void OptionsImpl::createMenu()
{
	// hide the menu header
	trvMenu->header()->hide();
	// connect slots
	connect(trvMenu, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(menuCurrentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));

	// add the menu items
	QTreeWidgetItem *newItem;

	newItem = new QTreeWidgetItem(trvMenu);

	// basic
	newItem->setText(0, tr("Basic"));
	newItem->setIcon(0, QIcon(":/options/images/tools.png"));
	newItem->setSelected(true);

	// language
	newItem = new QTreeWidgetItem(trvMenu);
	newItem->setText(0, tr("Language"));
	newItem->setIcon(0, QIcon(":/options/images/language.png"));

	// session
	newItem = new QTreeWidgetItem(trvMenu);
	newItem->setText(0, tr("Session"));
	newItem->setIcon(0, QIcon(":/options/images/session.png"));

	// updates
	newItem = new QTreeWidgetItem(trvMenu);
	newItem->setText(0, tr("Updates"));
	newItem->setIcon(0, QIcon(":/options/images/updates.png"));

	// ups!
	newItem = new QTreeWidgetItem(trvMenu);
	newItem->setText(0, tr("Ups!"));
	newItem->setIcon(0, QIcon(":/options/images/ups.png"));

	// proxy
	newItem = new QTreeWidgetItem(trvMenu);
	newItem->setText(0, tr("Proxy"));
	newItem->setIcon(0, QIcon(":/options/images/proxy.png"));

	if (lastPageViewed != -1)
	{
		pgOptions->setCurrentIndex(lastPageViewed);
		trvMenu->setCurrentItem(trvMenu->topLevelItem(lastPageViewed));
	}
	chbRememberView->setChecked(lastPageViewed != -1);
}

void OptionsImpl::fillInitialData()
{
	// string list of items
	QStringList itemsToAdd;
	// set items to outputFormat
	itemsToAdd	<< tr("AVI Format (*.avi)") << tr("WMV Format ( *.wmv)")
	<< tr("MPEG1 Format ( *.mpg)") << tr("MPEG2 Format ( *.mpg)")
	<< tr("Apple iPod (*.mp4)") << tr("3GP Format (*.3gp)")
	<< tr("MP3 Format (*.mp3)");
	cmbOutputFormat->addItems(itemsToAdd);

	// set items to video resolution
	itemsToAdd.clear();
	itemsToAdd	<< tr("Original") << "96 x 72" << "128 x 96" << "160 x 120"
	<< "176 x 120" << "176 x 144" << "192 x 144" << "240 x 180"
	<< "320 x 200" << "320 x 240" << "352 x 240" << "352 x 288"
	<< "480 x 272" << "480 x 360" << "480 x 480" << "624 x 352"
	<< "640 x 480" << "720 x 480" << "720 x 576";
	cmbVideoResolution->addItems(itemsToAdd);

	// set items to audio sampling
	itemsToAdd.clear();
	itemsToAdd	<< tr("Original") << "22050" << "44100";
	cmbAudioSampling->addItems(itemsToAdd);

	// set items to video frame rate
	itemsToAdd.clear();
	itemsToAdd	<< tr("Original") << "15" << "24" << "25" << "29.97" << "30";
	cmbVideoFrameRate->addItems(itemsToAdd);

	// set items to output quality
	itemsToAdd.clear();
	itemsToAdd	<< tr("Lower quality (Video bitrate: 384kbps; Audio bitrate: 64kbps)")
	<< tr("Low quality (Video bitrate: 512kbps; Audio bitrate: 80kbps)")
	<< tr("Normal quality (Video bitrate: 640kbps; Audio bitrate: 96kbps)")
	<< tr("Medium quality (Video bitrate: 800kbps; Audio bitrate: 96kbps)")
	<< tr("Good quality (Video bitrate: 1000kbps; Audio bitrate: 128kbps)")
	<< tr("Superb quality (Video bitrate: 1200kbps; Audio bitrate: 128kbps)")
	<< tr("The Same quality as the original Video");
	cmbOutputQuality->addItems(itemsToAdd);

	// set items to update every
	itemsToAdd.clear();
	itemsToAdd	<< tr("Day") << tr("2 Days") << tr("3 Days") << tr("4 Days")
	<< tr("5 Days") << tr("6 Days") << tr("Week");
	cmbUpdateEvery->addItems(itemsToAdd);

	// set proxy type items
	itemsToAdd.clear();
	itemsToAdd	<< tr("Http Proxy") << tr("Socks5 Proxy");
	cmbProxyType->addItems(itemsToAdd);

	// set plugins list
	lsvServices1->header()->hide();
	lsvServices2->header()->hide();
	QStringList plugins = videoInformation->getPluginsCompleteList();

	for (int n = 0; n < plugins.count(); n++)
	{
		QStringList info = plugins.at(n).split("|");

		QTreeWidgetItem *item = new QTreeWidgetItem(lsvServices1);
		item->setData(0, Qt::UserRole, info.at(0)); // store the host id
		item->setText(0, info.at(1)); // caption
		item->setIcon(0, QIcon(videoInformation->getHostImage(info.at(0))));
		item->setSizeHint(0, QSize(18,18));
	}
	servicesItemSelectionChanged();
}

void OptionsImpl::fillLanguages()
{
	tmpLangFile = programOptions->getLanguageFile(false);
	languageManager->loadLangFiles(programOptions->getApplicationPath() + "/languages");
	lsvLanguages->header()->hide();

	for (int n = 0; n < languageManager->getLanguagesCount(); n++)
	{
		QTreeWidgetItem *newItem = new QTreeWidgetItem(lsvLanguages);
		newItem->setText(0, languageManager->getLanguage(n)->getId());
		newItem->setSizeHint(0, QSize(18,18));
		QFileInfo langFile(languageManager->getLanguage(n)->getFile());
		if (langFile.fileName() == tmpLangFile)
		{
			newItem->setIcon(0, QIcon(":/buttons/images/script_go.png"));
			// set as bold
			QFont oriFont = newItem->font(0);
			oriFont.setBold(true);
			newItem->setFont(0, oriFont);
			// select it
			lsvLanguages->setCurrentItem(newItem);
		}
		else
			newItem->setIcon(0, QIcon(":/buttons/images/script.png"));
	}
}

void OptionsImpl::setInitialOptionsValues()
{
	edtDownloadsDir->setText(programOptions->getDownloadDir());
	chbDisplayPopup->setChecked(programOptions->getDisplayPopup());

	edtFFmpegLib->setText(programOptions->getFfmpegLibLocation());

	gpbVideoConversion->setChecked(programOptions->getConvertVideos());
	cmbOutputFormat->setCurrentIndex(programOptions->getConversionConf().outputFormat);
	cmbVideoResolution->setCurrentIndex(programOptions->getConversionConf().videoResolution);
	cmbAudioSampling->setCurrentIndex(programOptions->getConversionConf().audioSampleRatio);
	cmbVideoFrameRate->setCurrentIndex(programOptions->getConversionConf().videoFrameRate);
	cmbOutputQuality->setCurrentIndex(programOptions->getConversionConf().outputQuality);
	chbDeleteOriginalVideo->setChecked(programOptions->getDeleteVideosOnConvert());

	chbSaveRestore->setChecked(programOptions->getSaveRestoreSessions());
	chbDontrestoreDownloads->setChecked(programOptions->getDontRememberDownloadedVideos());
	chbSaveLog->setChecked(programOptions->getSaveLogDownloadedVideos());

	chbDisableAdultSupport->setChecked(programOptions->getBlockAdultContent());

	// get the blocked items
	QStringList hosts = programOptions->getBlockedWebsitesList().split("|", QString::SkipEmptyParts);
	for (int i = 0; i < lsvServices1->topLevelItemCount(); ++i)
		if (hosts.indexOf(lsvServices1->topLevelItem(i)->data(0, Qt::UserRole).toString(), 0) != -1)
			lsvServices1->topLevelItem(i)->setSelected(true);
	spbAddPressed();

	gpbProxy->setChecked(programOptions->getUseProxy());
	edtUserName->setText(programOptions->getProxyUserName());
	edtPassword->setText(programOptions->getProxyPassword());
	edtAdress->setText(programOptions->getProxyAdress());
	spinBoxPort->setValue(programOptions->getProxyPort());

	if (programOptions->getProxyType() != QNetworkProxy::HttpProxy &&
	    programOptions->getProxyType() != QNetworkProxy::Socks5Proxy)
		cmbProxyType->setCurrentIndex(0);
	else
		cmbProxyType->setCurrentIndex(programOptions->getProxyType());

}

void OptionsImpl::setOptionsValues()
{
	programOptions->setCanSendUpdateSignal(false);

	programOptions->setDownloadDir(edtDownloadsDir->text());
	programOptions->setDisplayPopup(chbDisplayPopup->isChecked());

	programOptions->setFfmpegLibLocation(edtFFmpegLib->text());

	programOptions->setConvertVideos(gpbVideoConversion->isChecked());
	VideoConversionConfig convConf;
	convConf.outputFormat = static_cast<OutputFormat>(cmbOutputFormat->currentIndex());
	convConf.videoResolution = static_cast<VideoResolution>(cmbVideoResolution->currentIndex());
	convConf.audioSampleRatio = static_cast<AudioSampleRatio>(cmbAudioSampling->currentIndex());
	convConf.videoFrameRate = static_cast<VideoFrameRate>(cmbVideoFrameRate->currentIndex());
	convConf.outputQuality = static_cast<OutputQuality>(cmbOutputQuality->currentIndex());
	programOptions->setConversionConf(convConf);
	programOptions->setDeleteVideosOnConvert(chbDeleteOriginalVideo->isChecked());

	programOptions->setSaveRestoreSessions(chbSaveRestore->isChecked());
	programOptions->setDontRememberDownloadedVideos(chbDontrestoreDownloads->isChecked());
	programOptions->setSaveLogDownloadedVideos(chbSaveLog->isChecked());

	QString hosts = "";
	for (int i = 0; i < lsvServices2->topLevelItemCount(); ++i)
		hosts += "|" + lsvServices2->topLevelItem(i)->data(0, Qt::UserRole).toString();

	programOptions->setBlockAdultContent(chbDisableAdultSupport->isChecked());
	programOptions->setBlockedWebsitesList(hosts);

	programOptions->setUseProxy(gpbProxy->isChecked());
	programOptions->setProxyUserName(edtUserName->text());
	programOptions->setProxyPassword(edtPassword->text());
	programOptions->setProxyAdress(edtAdress->text());
	programOptions->setProxyPort(spinBoxPort->value());
	programOptions->setProxyType(cmbProxyType->currentIndex());

	programOptions->setCanSendUpdateSignal(true);

	programOptions->setLanguageFile(tmpLangFile);

	lastPageViewed = chbRememberView->isChecked() ? pgOptions->currentIndex() : -1;
}

void OptionsImpl::dragEnterEvent(QDragEnterEvent *event)
{
	qDebug() << "dragEnterEvent";
}

void OptionsImpl::dropEvent(QDropEvent *event)
{
	qDebug() << "dropEvent";
}

void OptionsImpl::chbSaveRestoreStateClicked(bool checked)
{
	chbDontrestoreDownloads->setEnabled(checked);
}

void OptionsImpl::menuCurrentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
	pgOptions->setCurrentIndex(trvMenu->indexOfTopLevelItem(current));
	// we have a current ietm?
	if (current != NULL)
	{
		QFont oriFont = current->font(0);
		oriFont.setBold(true);
		current->setFont(0, oriFont);
	}
	// we have a prevous item?
	if (previous != NULL)
	{
		QFont oriFont = previous->font(0);
		oriFont.setBold(false);
		previous->setFont(0, oriFont);
	}
}

void OptionsImpl::langCurrentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
	if (current != NULL)
	{
		Language *language =languageManager->getLanguage(lsvLanguages->indexOfTopLevelItem(current));

		lblLanguage->setText(language->getId());
		lblVersion->setText(language->getVersion());
		lblAuthor->setText(language->getAuthor());
		lblContact->setText(QString("<a href=\"%1\">%2</a>").arg(language->getContact()).arg(language->getContact()));
		rchDescription->setHtml(language->getDescription());

		btnUseThis->setEnabled(language->getFile() != tmpLangFile);
	}
}

void OptionsImpl::btnUseThisClicked()
{
	Language *language =languageManager->getLanguage(lsvLanguages->indexOfTopLevelItem(lsvLanguages->currentItem()));
	tmpLangFile = language->getFile();
	btnUseThis->setEnabled(false);

	for (int n = 0; n < languageManager->getLanguagesCount(); n++)
	{
		Language *language = languageManager->getLanguage(n);
		QTreeWidgetItem *item = lsvLanguages->topLevelItem(n);

		if (language->getFile() == tmpLangFile)
			item->setIcon(0, QIcon(":/buttons/images/script_go.png"));
		else
			item->setIcon(0, QIcon(":/buttons/images/script.png"));
		// set as bold?
		QFont oriFont = item->font(0);
		oriFont.setBold(language->getFile() == tmpLangFile);
		item->setFont(0, oriFont);
	}
	// disply update message
	QMessageBox::information(this, tr("Language Setup"),
	                         tr("In order to apply the new selected language, the program must be restarted."),
	                         tr("Ok"), QString(), 0, 1);
}

void OptionsImpl::langItemDoubleClicked(QTreeWidgetItem *item, int column)
{
	if (btnUseThis->isEnabled())
		btnUseThisClicked();
}

void OptionsImpl::btnOkClicked()
{
	setOptionsValues();
	done(QDialog::Accepted);
}

void OptionsImpl::spbSelectDownloadDirPressed()
{
	QString s = QFileDialog::getExistingDirectory(this, tr("Select the download directory:"),
	            edtDownloadsDir->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
	// if is emtpy, cancel the proces
	if (s.isEmpty()) return;
	edtDownloadsDir->setText(QDir::toNativeSeparators(s));
}

void OptionsImpl::spbSelectFFmpegLibPressed()
{
	QString s = QFileDialog::getOpenFileName(this, tr("Select the ffmpeg lib:"),
	            edtFFmpegLib->text(),
#ifdef Q_OS_WIN32
	            "ffmpeg lib (ffmpeg.exe)");
#else
	            "ffmpeg lib (ffmpeg)");
#endif
	// if is emtpy, cancel the proces
	if (s.isEmpty()) return;
	edtFFmpegLib->setText(QDir::toNativeSeparators(s));
}

void OptionsImpl::ffmpegTextChanged(const QString &text)
{
	QColor exists(qApp->palette().brush(QPalette::Base).color());
	QColor notExist(255, 170, 127);

	QPalette p = edtFFmpegLib->palette();
	p.setColor(QPalette::Base, QFile::exists(text.trimmed()) ? exists : notExist);
	edtFFmpegLib->setPalette(p);
}

void OptionsImpl::btnViewLogClicked()
{
	DownloadLogImpl downloadLog;
	downloadLog.buildLog(sessionManager->getLogEntries(), videoInformation);
	downloadLog.exec();
}

void OptionsImpl::btnClearLogClicked()
{
	if (QMessageBox::question(this, tr("Clear Log"),
	                          tr("Are you sure to Clear the Historic File?"),
	                          tr("Yes"), tr("No"), QString(), 0, 1) == 0)
		sessionManager->clearLog();
}

void OptionsImpl::spbRemovePressed()
{
	for (int i = lsvServices2->topLevelItemCount() - 1; i >= 0; --i)
		if (lsvServices2->topLevelItem(i)->isSelected())
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(lsvServices1);
			item->setData(0, Qt::UserRole, lsvServices2->topLevelItem(i)->data(0, Qt::UserRole));
			item->setText(0, lsvServices2->topLevelItem(i)->text(0));
			item->setIcon(0, lsvServices2->topLevelItem(i)->icon(0));
			item->setSizeHint(0, QSize(18,18));

			delete lsvServices2->topLevelItem(i);
		}
	servicesItemSelectionChanged();
}

void OptionsImpl::spbRemoveAllPressed()
{
	for (int i = 0; i < lsvServices2->topLevelItemCount(); ++i)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(lsvServices1);
		item->setData(0, Qt::UserRole, lsvServices2->topLevelItem(i)->data(0, Qt::UserRole));
		item->setText(0, lsvServices2->topLevelItem(i)->text(0));
		item->setIcon(0, lsvServices2->topLevelItem(i)->icon(0));
		item->setSizeHint(0, QSize(18,18));
	}
	lsvServices2->clear();
	lsvServices1->sortItems(0, Qt::AscendingOrder);
	servicesItemSelectionChanged();
}

void OptionsImpl::spbAddAllPressed()
{
	for (int i = 0; i < lsvServices1->topLevelItemCount(); ++i)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(lsvServices2);
		item->setData(0, Qt::UserRole, lsvServices1->topLevelItem(i)->data(0, Qt::UserRole));
		item->setText(0, lsvServices1->topLevelItem(i)->text(0));
		item->setIcon(0, lsvServices1->topLevelItem(i)->icon(0));
		item->setSizeHint(0, QSize(18,18));
	}
	lsvServices1->clear();
	lsvServices2->sortItems(0, Qt::AscendingOrder);
	servicesItemSelectionChanged();
}

void OptionsImpl::spbAddPressed()
{
	for (int i = lsvServices1->topLevelItemCount() - 1; i >= 0; --i)
		if (lsvServices1->topLevelItem(i)->isSelected())
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(lsvServices2);
			item->setData(0, Qt::UserRole, lsvServices1->topLevelItem(i)->data(0, Qt::UserRole));
			item->setText(0, lsvServices1->topLevelItem(i)->text(0));
			item->setIcon(0, lsvServices1->topLevelItem(i)->icon(0));
			item->setSizeHint(0, QSize(18,18));

			delete lsvServices1->topLevelItem(i);
		}
	servicesItemSelectionChanged();
}

void OptionsImpl::servicesItemSelectionChanged()
{
	// disable buttons
	spbAdd->setEnabled(false);
	spbRemove->setEnabled(false);
	// add button
	for (int i = 0; i < lsvServices1->topLevelItemCount(); ++i)
		if (lsvServices1->topLevelItem(i)->isSelected())
		{
			spbAdd->setEnabled(true);
			i = lsvServices1->topLevelItemCount();
		}
	// remove button
	for (int i = 0; i < lsvServices2->topLevelItemCount(); ++i)
		if (lsvServices2->topLevelItem(i)->isSelected())
		{
			spbRemove->setEnabled(true);
			i = lsvServices2->topLevelItemCount();
		}
	// add/rem all buttons
	spbAddAll->setEnabled(lsvServices1->topLevelItemCount() != 0);
	spbRemoveAll->setEnabled(lsvServices2->topLevelItemCount() != 0);
	// sort items
	lsvServices1->sortItems(0, Qt::AscendingOrder);
	lsvServices2->sortItems(0, Qt::AscendingOrder);
}
//
