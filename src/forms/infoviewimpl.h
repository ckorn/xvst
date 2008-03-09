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


#ifndef INFOVIEWIMPL_H
#define INFOVIEWIMPL_H
//
#include <QtGui>
//
#include "ui_infoview.h"
//
#include "../programversion.h"
#include "../videoitem.h"
//
class InfoViewImpl : public QDialog, public Ui::InfoView
{
Q_OBJECT
	public:
		InfoViewImpl(VideoItem *videoItem, QString userName, QString email,
			QString comments, QWidget * parent = 0, Qt::WFlags f = 0 );
};
#endif
