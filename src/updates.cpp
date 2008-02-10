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

#include "updates.h"

// Updates class

Updates::Updates(QString appPath)
{
	updateState = usWaiting;
	cancelled = false;
	this->appPath = appPath;
	currentDownloaded = 0;
	totalDownloaded = 0;
	totalToDownload = 0;
	// init update classes
	updateList = new QList<Update *>;
	http = new Http;
	// signals
	connect(http, SIGNAL(downloadEvent(int, int)), this, SLOT(downloadEvent(int, int)));
	connect(http, SIGNAL(downloadFinished(const QFileInfo)), this, SLOT(downloadFinished(const QFileInfo)));
}

Updates::~Updates()
{
	clear();

	delete http;
	delete updateList;
}

void Updates::parseBlock(QString block)
{
	if (!block.isEmpty())
	{
		int n = 0;
		QString fileBlock = "";
		// get file blocks
		do
		{
			// get current update file
			fileBlock = copyBetween(block, QString("#FILE:%1").arg(n), QString("#END:%1").arg(n));
			// have something to analayze?
			if (!fileBlock.isEmpty())
			{
				updateList->append(new Update);
				Update *update = updateList->last();
				// assign info
				update->setCaption(copyBetween(fileBlock, "caption=\"", "\""));
				update->setVersion(copyBetween(fileBlock, "version=\"", "\""));
				update->setSize(copyBetween(fileBlock, "size=\"", "\"").toInt());
				update->setInstallTo(copyBetween(fileBlock, "installTo=\"", "\""));
				update->setUrl(copyBetween(fileBlock, "url=\"", "\""));
				update->setPacked(copyBetween(fileBlock, "packed=\"", "\"").toLower() == "true");
				update->setChecked(true);
			}
			// next update file
			n++;
			// continue?
		}
		while (!fileBlock.isEmpty());
	}
}

void Updates::pareseUpdateFile(QString updateFile)
{
	if (cancelled) return;
	// parse current OS updates
	parseBlock(copyBetween(updateFile, "#" + CURRENT_OS + "_" + COMPILATION_MODE + "{", "}"));
	// parse common updates (OS independent)
	parseBlock(copyBetween(updateFile, "#COMMON{", "}"));
}

bool Updates::hasUpdates()
{
	if (cancelled) return false;

	for (int n = getUpdatesCount() - 1; n >= 0; n--)
	{
		bool deleteUpdate = false;
		Update *update = updateList->at(n);
		// what type of file is?
		if (update->getInstallTo().toLower() == "this")
			deleteUpdate = compareVersions(PROGRAM_VERSION, update->getVersion()) != 1;
		else
		{
			if (QFile::exists(appPath + update->getInstallTo()))
			{
				QFileInfo fileInf(update->getInstallTo());
				// language files
				if (fileInf.completeSuffix() == "language")
				{
					Language *language = LanguageManager::getLanguageInfo(appPath + update->getInstallTo());
					// have info?
					if (language != NULL)
						deleteUpdate = compareVersions(language->getVersion(), update->getVersion()) != 1;
				}
				else // "unknonw file"
					deleteUpdate = true;
			}
			else // we don't have this "item" installed...
				deleteUpdate = true;
		}
		// can delete this update?
		if (deleteUpdate)
		{
			delete updateList->at(n);
			updateList->removeAt(n);
		}
	}
	return getUpdatesCount() > 0;
}

void Updates::clear()
{
	while (!updateList->isEmpty())
		updateList->removeFirst();
}

void Updates::buildInstalScript()
{
	QFile updateScriptFile(QDir::tempPath() + XUPDATER_FILE);
	if (updateScriptFile.open(QFile::WriteOnly | QFile::Truncate))
	{
		//QTextStream updateScript(&updateScriptFile);
		QStringList updateScript;
		// write the info vars
		updateScript << "#MAIN_APP=" + QCoreApplication::applicationFilePath()
					 << "#PARAMETERS="
					 << "#RESTART=true";
		// add updates
		for (int n = 0; n < getUpdatesCount(); n++)
		{
			Update *update = updateList->at(n);
			// check if is "this" reference
			if (update->getInstallTo().toLower() == "this")
			{
				QFileInfo appExe(QCoreApplication::applicationFilePath());
				update->setInstallTo("/" + appExe.fileName());
			}
			// is checked?
			if (update->getChecked())
				if (!update->getPacked())
					// block id
					updateScript << QString(":install_file_%1").arg(n)
					// copy the downloaded file
								 << QString("if copy \"%1\" \"%2\"")
								 		.arg(QDir::tempPath() + QString(XUPDATER_DWN_FILE).arg(n))	// downloaded file
								 		.arg(appPath + update->getInstallTo())						// destination file
					// if the downloaded file has been copied, then delete it and jump to the next file
								 << QString("del \"%1\"").arg(QDir::tempPath() + QString(XUPDATER_DWN_FILE).arg(n))
					// if isn't the last file, go to next update file
								 << (n < getUpdatesCount() - 1 ? QString("goto install_file_%1").arg(n + 1) : "goto finish_update")
					// else condition (file has not been copied)
								 << "else"
								 << QString("goto install_file_%1").arg(n)
								 << "end";
				else // is a packed update
				{
					// added for convenience
					updateScript << QString(":install_file_%1").arg(n)
								 << QString("goto install_file_%1_0").arg(n);
					// extract files from the packed update
					Unpacker *unpacker = new Unpacker;
					unpacker->extractPackage(QString(QDir::tempPath() + QString(XUPDATER_DWN_FILE).arg(n)).toStdString(),
											 QDir::tempPath().toStdString() + "/",
											 false);
					// add files into the script
					int i;
					for (i = 0; i < unpacker->getExtractedFilesCount(); i++)
					{
						// block id
						updateScript << QString(":install_file_%1_%2").arg(n).arg(i)
						// copy the downloaded file
									 << QString("if copy \"%1\" \"%2\"")
									 		.arg(QString::fromStdString(unpacker->getExtractedFileName(i)))	// unpacked file
									 		.arg(QFileInfo(appPath + update->getInstallTo()).absolutePath() + 
									 					   "/" + QString::fromStdString(unpacker->getExtractedFileName(i, true))) // destination file
						// if the downloaded file has been copied, then delete it and jump to the next file
									 << QString("del \"%1\"").arg(QString::fromStdString(unpacker->getExtractedFileName(i)))
						// next update
									 << QString("goto install_file_%1_%2").arg(n).arg(i + 1)
						// else condition (file has not been copied)
									 << "else"
									 << QString("goto install_file_%1_%2").arg(n).arg(i)
									 << "end";
					}
					// delete the downloaded pack
					updateScript << QString(":install_file_%1_%2").arg(n).arg(i)
								 << QString("del \"%1\"").arg(QDir::tempPath() + QString(XUPDATER_DWN_FILE).arg(n))
								 << (n < getUpdatesCount() - 1 ? QString("goto install_file_%1").arg(n + 1) : "goto finish_update");
					
					delete unpacker;
				}
			else
				// empty block id
				updateScript << QString(":install_file_%1").arg(n)
							 << (n < getUpdatesCount() - 1 ? QString("goto install_file_%1").arg(n + 1) : "goto finish_update");
		}
		// finish update block
		updateScript << ":finish_update"
					 << QString("del \"%1\"").arg(QDir::tempPath() + XUPDATER_FILE);
		// replace dirs separators
#ifdef Q_OS_WIN32
		updateScript.replaceInStrings("/", "\\");
#else
		updateScript.replaceInStrings("\\", "/");
#endif
		// save to disc
		QTextStream updateScriptOut(&updateScriptFile);
		foreach (QString line, updateScript)
			updateScriptOut << line << "\n";
	}
}

void Updates::getTotalSizeToDownload()
{
	currentDownloaded = 0;
	totalDownloaded = 0;
	totalToDownload = 0;
	// calcule
	for (int n = 0; n < getUpdatesCount(); n++)
	{
		Update *update = updateList->at(n);
		// is checked?
		if (update->getChecked())
			totalToDownload += update->getSize();
	}
}

void Updates::run()
{
	switch (updateState)
	{
		case usWaiting:
			break;

		case usChecking:
		{
			// analyze the update file
			pareseUpdateFile(http->downloadWebpage(QUrl(updateURL), false));

			// cancelled?
			if (!cancelled)
			{
				// wait 650 miliseconds
				msleep(650);
				// check versions
				emit updatesChecked(hasUpdates());
			}
			else // yes, cancelled
				emit updatesCancelled();
			break;
		}

		case usDownloading:
		{
			currentItem = getFirstUpdateToDownload();
			int lastItem = -1;
			getTotalSizeToDownload();
			// download updates
			while (!cancelled && updateState == usDownloading && currentItem != -1)
			{
				if (lastItem != currentItem)
				{
					lastItem = currentItem;
					// disc file
					QString fileName = QDir::tempPath() + QString(XUPDATER_DWN_FILE).arg(currentItem);
					// if this file already exsits, delete it
					if (QFile::exists(fileName))
						QFile::remove(fileName);
					// start to download the current update
					http->download(QUrl(updateList->at(currentItem)->getUrl()),
					               QDir::tempPath(), QString(XUPDATER_DWN_FILE).arg(currentItem));
				}
				// wait 100 miliseconds (prevent 100% cpu)
				msleep(100);
			}
			// install updates
			if (!cancelled && updateState == usInstalling && currentItem != -1)
				run();
			else // process cancelled?
				if (cancelled) // yes
				{
					// delete all temporal files
					for (int n = 0; n < getUpdatesCount(); n++)
						if (QFile::exists(QDir::tempPath() + QString(XUPDATER_DWN_FILE).arg(currentItem)))
							QFile::remove(QDir::tempPath() + QString(XUPDATER_DWN_FILE).arg(currentItem));
					
					emit updatesCancelled();
				}
			break;
		}

		case usInstalling:
			// make the install file
			buildInstalScript();
			// send the ok signal
			emit readyToInstallUpdates();
			break;
	}
}

void Updates::checkForUpdates(QString url)
{
	updateState = usChecking;
	updateURL = url;
	this->start();
}

void Updates::downloadUpdates()
{
	updateState = usDownloading;
	this->start();
}

void Updates::installUpdates()
{
	QProcess::startDetached(appPath + XUPDATER_PATH, QStringList() << QDir::toNativeSeparators(QDir::tempPath() + XUPDATER_FILE));
}

void Updates::cancel()
{
	updateState = usWaiting;
	cancelled = true;
	http->cancel();
}

Update* Updates::getUpdateItem(const int index)
{
	if (index >= 0 && index < getUpdatesCount())
		return updateList->at(index);
	else
		return NULL;
}

int Updates::getUpdatesCount()
{
	return updateList->count();
}

int Updates::getCurrentDownloaded()
{
	return currentDownloaded;
}

int Updates::getTotalToDownload()
{
	return totalToDownload;
}

int Updates::getFirstUpdateToDownload()
{
	for (int n = 0; n < getUpdatesCount(); n++)
		if (updateList->at(n)->getChecked())
			return n;
	
	return -1;
}

bool Updates::canUpdate()
{
	return QFile::exists(QCoreApplication::applicationDirPath() + XUPDATER_PATH);
}

void Updates::downloadEvent(int pos, int max)
{
	if (updateState == usChecking)
		emit progressCheckUpdate(static_cast<int>(calculePercent(pos, max)));
	else
	{
		currentDownloaded = totalDownloaded + pos;
		
		emit downloadingUpdate(currentItem, 
							   static_cast<int>(calculePercent(pos, max)), 
							   static_cast<int>(calculePercent(currentDownloaded, totalToDownload)));
	}
}

void Updates::downloadFinished(const QFileInfo destFile)
{
	if (updateState == usDownloading)
	{
		// update total downloaded
		totalDownloaded += updateList->at(currentItem)->getSize();
		// emit the signle end singal
		emit downloadFinished(currentItem);
		// next
		currentItem++;
		// get next file to download
		while (currentItem < getUpdatesCount())
		{
			if (updateList->at(currentItem)->getChecked())
				return;
			//next
			currentItem++;
		}
		// if we are here, then send the "END" signal
		emit downloadsFinished();
		// set as installing state
		updateState = usInstalling;
	}
}

// Update class

void Update::setCaption(QString value)
{
	caption = value;
}

void Update::setVersion(QString value)
{
	version = value;
}

void Update::setSize(int value)
{
	size = value;
}

void Update::setInstallTo(QString value)
{
	installTo = value;
}

void Update::setUrl(QString value)
{
	url = value;
}

void Update::setPacked(bool value)
{
	packed = value;
}

void Update::setChecked(bool value)
{
	checked = value;
}

QString Update::getCaption()
{
	return caption;
}

QString Update::getVersion()
{
	return version;
}

int Update::getSize()
{
	return size;
}

QString Update::getInstallTo()
{
	return installTo;
}

QString Update::getUrl()
{
	return url;
}

bool Update::getPacked()
{
	return packed;
}

bool Update::getChecked()
{
	return checked;
}
