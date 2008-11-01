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

#include "newlanguagesimpl.h"

NewLanguagesImpl::NewLanguagesImpl(QWidget * parent, Qt::WFlags f)
{
	setupUi(this);
	// create list columns
	QStringList headers;
	headers << tr(" Language ") << tr(" Version ") << tr(" Author ") << tr(" Progress ");
	// add the headers
	lsvLanguages->setHeaderLabels(headers);
	// configure resize mode
	QHeaderView *header = lsvLanguages->header();
	header->setHighlightSections(false);
	header->setStretchLastSection(false);
	header->setResizeMode(0, QHeaderView::Stretch);
	// set header text aligment
	QTreeWidgetItem * headerItem = lsvLanguages->headerItem();
	headerItem->setTextAlignment(1, Qt::AlignHCenter | Qt::AlignVCenter);
	headerItem->setTextAlignment(2, Qt::AlignHCenter | Qt::AlignVCenter);
	headerItem->setTextAlignment(3, Qt::AlignHCenter | Qt::AlignVCenter);
	// create new languages controller
	newLanguages = new NewLanguages();
}

NewLanguagesImpl::~NewLanguagesImpl()
{
    delete newLanguages;
}
