#include "newlanguages.h"

NewLanguagesController::NewLanguagesController(ProgramOptions *programOptions)
{
	this->programOptions = programOptions;
	newLanguages = new QList<Update *>;
	languageManager = new LanguageManager;
	currentUpdate = NULL;
	// http class
	http = new Http();
	connect(http, SIGNAL(downloadFinished(const QFileInfo)), this, SLOT(downloadFinished(const QFileInfo)));
	connect(http, SIGNAL(downloadEvent(int,int)), this, SLOT(privateDownloadProgress(int,int)));
}

NewLanguagesController::~NewLanguagesController()
{
	clearAllNewLanguages();
	//
	delete http;
	delete languageManager;
	delete newLanguages;
}

void NewLanguagesController::run()
{
	updateLanguagesToInstall();
}

void NewLanguagesController::fillInstalledLanguages()
{
	emit installedLanguagesClear();
	//
	languageManager->loadLangFiles(programOptions->getLanguagesPath());
	for (int n = 0; n < languageManager->getLanguagesCount(); n++)
		emit installedLanaguageAdded(languageManager->getLanguage(n));
}

void NewLanguagesController::updateLanguagesToInstall()
{
	newLanguagesFile = http->downloadWebpage(QUrl(URL_LANGUAGES_FILE), false);
	updateNewLanguages();
	// finished
	emit afterCheckNewLanguages();
}

void NewLanguagesController::parseBlock(QString block)
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
				Update *update = new Update;
				// assign info
				update->setCaption(copyBetween(fileBlock, "caption=\"", "\""));
				update->setVersion(copyBetween(fileBlock, "version=\"", "\""));
				update->setSize(copyBetween(fileBlock, "size=\"", "\"").toInt());
				update->setInstallTo(copyBetween(fileBlock, "installTo=\"", "\""));
				update->setUrl(copyBetween(fileBlock, "url=\"", "\""));
				update->setPacked(copyBetween(fileBlock, "packed=\"", "\"").toLower() == "true");
				update->setObligatory(copyBetween(fileBlock, "obligatory=\"", "\"").toLower() == "true");
				update->setChecked(true);
				// if this language is not installed, add it into the list
				if (!languageManager->isLanguageInstalled(update->getCaption()))
				{
					newLanguages->append(update);
					emit toInstallLanguageAdded(update);
				}
				else // destroy it
					delete update;
			}
			// next update file
			n++;
			// continue?
		}
		while (!fileBlock.isEmpty());
	}
}

void NewLanguagesController::clearAllNewLanguages()
{
	while (!newLanguages->isEmpty())
	{
		delete newLanguages->at(0);
		newLanguages->removeFirst();
	}
	//
	emit toInstallLanguagesClear();
}

void NewLanguagesController::updateNewLanguages()
{
	emit beforeUpdateNewLanguages();
	// remove prev. new languages
	clearAllNewLanguages();
	// fill with new languages
	parseBlock(copyBetween(newLanguagesFile, "#COMMON{", "}"));
}

void NewLanguagesController::initialize()
{
	// load local lanuages
	fillInstalledLanguages();
	// load remote languages
	start();
}

void NewLanguagesController::uninstallLanguage(int index)
{
	Language *toRemove = languageManager->getLanguage(index);
	// if we found this language
	if (toRemove != NULL)
	{
		// delete language files
		bool fileRemoved = QFile::remove(programOptions->getLanguagesPath() + "/" + toRemove->getFile());
		QFile::remove(programOptions->getLanguagesPath() + "/" + toRemove->getLangFile());
		// send signal after uninstall language
		emit installedLanguageRemoved(toRemove, fileRemoved);// && qmRemoved);
		// reload all languages again
		fillInstalledLanguages();
		// reload new languages
		updateNewLanguages();
	}
}

void NewLanguagesController::installLanguage(int index)
{
	emit beforeInstallNewLanguage();

	currentUpdate = newLanguages->at(index);
	http->download(QUrl(currentUpdate->getUrl()), QDir::tempPath(), QString("xVST.language"), false);
}

bool NewLanguagesController::isInstalling() const
{
	return currentUpdate != NULL;
}

void NewLanguagesController::downloadFinished(const QFileInfo destFile)
{
	// create languages directory
	QDir languagesDir;
	languagesDir.mkpath(programOptions->getLanguagesPath());
	// unpack the downloaded file
	Unpacker *unpacker = new Unpacker;
	// install it directly to languages folder
	unpacker->extractPackage(destFile.filePath().toStdString(),
							 QString(programOptions->getLanguagesPath() + "/").toStdString(),
							 true);
	// check if we installed this language
	bool error = false;
	// check if all files has been extracted
	for (int n = 0; n < unpacker->getExtractedFilesCount(); n++)
		if (!error)	// check if the extracted file exists
			error = !QFile::exists(QString(programOptions->getLanguagesPath() + "/" +
										   QString().fromStdString(unpacker->getExtractedFileName(n, true))));
	// destroy unpacker
	delete unpacker;
	// remove temporal fil
	QFile::remove(destFile.filePath());
	// finish event
	emit afterInstallNewLanguage(currentUpdate, error);
	// reset flags
	currentUpdate = NULL;
	// reload all info
	fillInstalledLanguages();
	updateNewLanguages();
}

void NewLanguagesController::downloadError(int error)
{
	// finish event
	emit afterInstallNewLanguage(currentUpdate, true);
	// reset flags
	currentUpdate = NULL;
	// reload all info
	fillInstalledLanguages();
	updateNewLanguages();
}

void NewLanguagesController::privateDownloadProgress(int pos, int max)
{
	emit downloadProgress(pos, max);
}
