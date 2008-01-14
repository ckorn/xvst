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
				update->setDependency(copyBetween(fileBlock, "dependency=\"", "\""));
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
	parseBlock(copyBetween(updateFile, "#" + CURRENT_OS + "{", "}"));
	// parse common updates (OS independent)
	parseBlock(copyBetween(updateFile, "#COMMON{", "}"));
}

bool Updates::hasUpdates()
{
	if (cancelled) return false;

	for (int n = updateList->count() - 1; n >= 0; n--)
	{
		bool deleteUpdate = false;
		Update *update = updateList->at(n);
		// what type of file is?
		if (update->getInstallTo().toLower() == "this")
			deleteUpdate = compareVersions(PROGRAM_VERSION, update->getVersion()) != 1;
		else
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
	return updateList->count() > 0;
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
			// is checked?
			if (update->getChecked())
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
			else
				// empty block id
				updateScript << QString(":install_file_%1").arg(n)
							 << (n < getUpdatesCount() - 1 ? QString("goto install_file_%1").arg(n + 1) : "goto finish_update");
			// finish update block
			updateScript << ":finish_update"
						 << QString("del \"%1\"").arg(QDir::tempPath() + XUPDATER_FILE);
		}
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
			break;
		}

		case usDownloading:
		{
			currentItem = 0;
			int lastItem = -1;
			// download updates
			while (!cancelled && updateState == usDownloading)
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
			if (!cancelled && updateState == usInstalling) 
				run();
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
	QProcess::execute(appPath + XUPDATER_PATH, QStringList() << QDir::toNativeSeparators(QDir::tempPath() + XUPDATER_FILE));
}

void Updates::cancel()
{
	cancelled = true;
	http->cancel();
	this->quit();
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

bool Updates::canUpdate()
{
	return QFile::exists(QCoreApplication::applicationDirPath() + XUPDATER_PATH);
}

void Updates::downloadEvent(int pos, int max)
{
	if (updateState == usChecking)
		emit progressCheckUpdate(static_cast<int>(calculePercent(pos, max)));
	else
		emit downloadingUpdate(currentItem, static_cast<int>(calculePercent(pos, max)), http->getDownloadSpeed());
}

void Updates::downloadFinished(const QFileInfo destFile)
{
	if (updateState == usDownloading)
	{
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

void Update::setDependency(QString value)
{
	dependency = value;
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

QString Update::getDependency()
{
	return dependency;
}

bool Update::getChecked()
{
	return checked;
}
