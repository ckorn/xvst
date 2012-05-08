/*
*
* This file is part of xVideoServiceThief,
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 - 2012 Xesc & Technology
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

#ifndef SEARCHVIDEOSSEARCHBLOCKIMPL_H
#define SEARCHVIDEOSSEARCHBLOCKIMPL_H

#include "ui_searchvideossearchblock.h"

class VideoInformationPlugin;

class SearchVideosSearchBlockImpl : public QWidget, private Ui::SearchVideosSearchBlockImpl
{
Q_OBJECT
	private:
		VideoInformationPlugin *plugin;
	public:
		SearchVideosSearchBlockImpl(QWidget *parent, VideoInformationPlugin *plugin, QString summary);
};

#endif // SEARCHVIDEOSSEARCHBLOCKIMPL_H
