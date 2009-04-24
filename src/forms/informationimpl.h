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

#ifndef INFORMATIONIMPL_H
#define INFORMATIONIMPL_H
//
#include "ui_information.h"
//
#include "creditsimpl.h"
//
#include "../programversion.h"
#include "../options.h"
#include "../languages.h"
#include "../videoinformation.h"
//
class InformationImpl : public QDialog, public Ui::Information
{
Q_OBJECT
	private:
		void buildVideoServicesList(VideoInformation *videoInformation);
	public:
		InformationImpl(ProgramOptions *programOptions, VideoInformation *videoInformation,
						QWidget * parent = 0, Qt::WFlags f = 0);
	private slots:
		void btnCreditsClicked();
};
#endif
