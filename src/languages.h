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

#ifndef __LANGUAGES_H__
#define __LANGUAGES_H__
//
#include <QtGui>
//
/*! Language file description */
class Language : public QObject
{
Q_OBJECT
	private:
		QString file;			//!< Language file
		QString langFile;		//!< QM file
		QString id;				//!< Language id (english, català, español...
		QString version;		//!< Current language file version
		QString author;			//!< Language file author
		QString contact;		//!< Language file author contact
		QString description;	//!< Language file description
	public:
		QString getFile();
		void setFile(QString value);
		QString getLangFile();
		void setLangFile(QString value);
		QString getId();
		void setId(QString value);
		QString getVersion();
		void setVersion(QString value);
		QString getAuthor();
		void setAuthor(QString value);
		QString getContact();
		void setContact(QString value);
		QString getDescription();
		void setDescription(QString value);
};

/*! Language files manager */
class LanguageManager : public QObject
{
Q_OBJECT
	private:
		QList<Language *> *languagesList;	//!< List with all detected languages
		/*! Clear the language list */
		void clear();
		/*! Get if is a valid index */
		bool isValidIndex(const int index);
		/*! Load an individual language file */
		void loadLangFile(QString langFile);
	public:
		/*! Class constructor */
		LanguageManager();
		/*! Class destructor */
		~LanguageManager();
		/*! Get a language item */
		Language* getLanguage(const int index);
		/*! Get the languages items count */
		int getLanguagesCount();
		/*! Detect all installed languages */
		void loadLangFiles(QString langDir);
		/*! Install a language */
		static QString get_qm_languageFile(QString langFile);
		/*! Return a Langage item info */
		static Language* getLanguageInfo(QString langFile);
};
#endif // __LANGUAGES_H__
