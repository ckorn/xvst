#ifndef NEWLANGUAGES_H
#define NEWLANGUAGES_H

#include <QtGui>
//
#include "pakunpak/pakunpak.h"
#include "http.h"
#include "languages.h"
#include "options.h"
#include "updates.h"

static const QString URL_LANGUAGES_FILE = "http://xviservicethief.sourceforge.net/updates/xVST.languages";

/*! Get new languages and install them */
class NewLanguagesController : public QThread
{
Q_OBJECT
	private:
		ProgramOptions *programOptions;
		LanguageManager *languageManager;
		QList<Update *> *newLanguages;
		QString newLanguagesFile;
		Http *http;
		bool installing;
		/*! Thread executation */
		void run();
		/*! Fill installed languages list */
		void fillInstalledLanguages();
		/*! Update the list of languages ready to install */
		void updateLanguagesToInstall();
		/*! Parse a block */
		void parseBlock(QString block);
		/*! Clear all stored new languages */
		void clearAllNewLanguages();
		/*! Update new languages */
		void updateNewLanguages();
	public:
		/*! Class construction */
		NewLanguagesController(ProgramOptions *programOptions);
		/* Class destruction */
		~NewLanguagesController();
		/*! Initialize class */
		void initialize();
		/*! Uninstall a selected language */
		void uninstallLanguage(int index);
		/*! Install a selected language */
		void installLanguage(int index);
		/*! Get if is installing a new language */
		bool isInstalling() const;
	private slots:
		/*! On finish a download */
		void downloadFinished(const QFileInfo destFile);
		/*! Private Download progress */
		void privateDownloadProgress(int pos, int max);
	signals:
		/*! On clear installed languages */
		void installedLanguagesClear();
		/*! On clear to install languages */
		void toInstallLanguagesClear();
		/*! Add a installed language */
		void installedLanaguageAdded(Language *language);
		/*! Removed an installed language */
		void installedLanguageRemoved(Language *language, bool removed);
		/*! After check updates */
		void afterCheckNewLanguages();
		/*! Add a installable language */
		void toInstallLanguageAdded(Update *language);
		/*! Before update the new languages list */
		void beforeUpdateNewLanguages();
		/*! Before start a install */
		void beforeInstallNewLanguage();
		/*! After install a language */
		void afterInstallNewLanguage();
		/*! Download progress */
		void downloadProgress(int pos, int max);
};

#endif // NEWLANGUAGES_H
