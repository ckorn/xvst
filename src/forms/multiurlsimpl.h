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

#ifndef MULTIURLSIMPL_H
#define MULTIURLSIMPL_H

#include <QtGui>

#include "ui_multiurls.h"

class VideoInformation;

class MultiURLsImpl : public QDialog, private Ui::MultiURLs
{
Q_OBJECT
	private:
		VideoInformation *videoInformation;
		QStringList selectedURLs;
		QStringList originalURLs;
	public:
		MultiURLsImpl(VideoInformation *videoInformation, QStringList selectedURLs, QStringList originalURLs,
					  QWidget * parent = 0, Qt::WFlags f = 0);
		QStringList getSelectedURLs();
};

#endif // MULTIURLSIMPL_H
