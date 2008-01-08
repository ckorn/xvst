#include "languages.h"

LanguageManager::LanguageManager()
{
	languagesList = new QList<Language *>;
}

LanguageManager::~LanguageManager()
{
	clear();
	delete languagesList;
}

void LanguageManager::clear()
{
	while (!languagesList->isEmpty())
		languagesList->removeFirst();
}

bool LanguageManager::isValidIndex(const int index)
{
	return index >= 0 && index < getLanguagesCount();
}

void LanguageManager::loadLangFile(QString langFile)
{
	if (QFile::exists(langFile))
	{
		languagesList->append(new Language);
		Language *language = languagesList->last();
		
		QFileInfo file(langFile);
		QSettings langInfo(langFile, QSettings::IniFormat);
		
		language->setFile(file.fileName());
		language->setLangFile(langInfo.value("language/file", "").toString());
		language->setId(langInfo.value("language/id", "").toString());
		language->setVersion(langInfo.value("language/version", "").toString());
		language->setAuthor(langInfo.value("language/author", "").toString());
		language->setContact(langInfo.value("language/contact", "").toString());
		language->setDescription(langInfo.value("language/description", "").toString());
	}
}

Language* LanguageManager::getLanguage(const int index)
{
	if (isValidIndex(index))
		return languagesList->at(index);
	else
		return NULL;
}

int LanguageManager::getLanguagesCount()
{
	return languagesList->count();
}

void LanguageManager::loadLangFiles(QString langDir)
{
	QStringList languageFiles;
	QDir languagesDir(langDir);
	
	languageFiles = languagesDir.entryList(QDir::Files, QDir::Name);
	languageFiles = languageFiles.filter(".language");
	
	foreach (QString langFile, languageFiles)
		loadLangFile(QDir::toNativeSeparators( langDir + "/" + langFile));
}

QString LanguageManager::get_qm_languageFile(QString langFile)
{
	QFileInfo langFileInfo(langFile);
	QSettings langInfo(langFile, QSettings::IniFormat);
	QString qmFile = langInfo.value("language/file", "null").toString();
	
	if (qmFile.toLower() != "null" && QFile::exists(langFileInfo.path() + "/" + qmFile))
		return langFileInfo.path() + "/" + qmFile;
	else
		return "null";
}

Language* LanguageManager::getLanguageInfo(QString langFile)
{
	Language *result = NULL;
	
	if (QFile::exists(langFile))
	{
		result = new Language;
		
		QFileInfo file(langFile);
		QSettings langInfo(langFile, QSettings::IniFormat);
		
		result->setFile(file.fileName());
		result->setLangFile(langInfo.value("language/file", "").toString());
		result->setId(langInfo.value("language/id", "").toString());
		result->setVersion(langInfo.value("language/version", "").toString());
		result->setAuthor(langInfo.value("language/author", "").toString());
		result->setContact(langInfo.value("language/contact", "").toString());
		result->setDescription(langInfo.value("language/description", "").toString());
	}
	
	return result;
}

// Language file description

QString Language::getFile()
{
	return file;
}

void Language::setFile(QString value)
{
	file = value;
}

QString Language::getLangFile()
{
	return langFile;
}

void Language::setLangFile(QString value)
{
	langFile = value;
}

QString Language::getId()
{
	return id;
}

void Language::setId(QString value)
{
	id = value;
}

QString Language::getVersion()
{
	return version;
}

void Language::setVersion(QString value)
{
	version = value;
}

QString Language::getAuthor()
{
	return author;
}

void Language::setAuthor(QString value)
{
	author = value;
}

QString Language::getContact()
{
	return contact;
}

void Language::setContact(QString value)
{
	contact = value;
}

QString Language::getDescription()
{
	return description;
}

void Language::setDescription(QString value)
{
	description = value;
}
