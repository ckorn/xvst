#ifndef NEWLANGUAGES_H
#define NEWLANGUAGES_H

#include <QtGui>
//
#include "http.h"

enum InstallState
{
	isWaiting,
	isChecking,
	isDownloading,
	isInstalling
};

/*! Language item */
class NewLanguage : QObject
{
Q_OBJECT
	public:
		/* Class constructor */
		NewLanguage();
};

/*! Get new languages and install them */
class NewLanguages : QThread
{
Q_OBJECT
	private:
		InstallState installState;	//! Current installation state
		/*! Thread executation */
		void run();
	public:
		/*! Class construction */
		NewLanguages();
		/* Class destruction */
		~NewLanguages();
		/*! download languages list */
		void downloadLanguagesList();
		/*! download and install selected languages */
		void downloadAndInstallSelectedLanguages();
};

#endif // NEWLANGUAGES_H
