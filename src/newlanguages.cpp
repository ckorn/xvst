#include "newlanguages.h"

NewLanguages::NewLanguages()
{
	installState = isWaiting;
}

NewLanguages::~NewLanguages()
{

}

void NewLanguages::downloadLanguagesList()
{
	installState = isChecking;
	this->start();
}

void NewLanguages::downloadAndInstallSelectedLanguages()
{
	installState = isDownloading;
	this->start();
}

void NewLanguages::run()
{
	switch (installState)
	{
		case isWaiting:
			break;

		case isChecking:
			break;

		case isDownloading:
			break;

		case isInstalling:
			break;
	}
}

/* New language */

NewLanguage::NewLanguage()
{
}
