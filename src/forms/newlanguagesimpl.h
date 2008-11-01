/*
*
* This file is part of xVideoServiceThief,
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 - 2008 Xesc & Technology
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with xVideoServiceThief. If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: Xesc <xeskuu.xvst@gmail.com>
* Program URL   : http://xviservicethief.sourceforge.net/
*
*/

#ifndef NEWLANGUAGESIMPL_H
#define NEWLANGUAGESIMPL_H

#include <QtGui>
//
#include "ui_newlanguages.h"
//
#include "../newlanguages.h"

class NewLanguagesImpl : public QDialog, public Ui::NewLanguages
{
Q_OBJECT
	private:
		NewLanguages *newLanguages;	//! Responsable of check, download and install new languages
	public:
		NewLanguagesImpl(QWidget * parent = 0, Qt::WFlags f = 0 );
		~NewLanguagesImpl();
};

#endif // NEWLANGUAGESIMPL_H
