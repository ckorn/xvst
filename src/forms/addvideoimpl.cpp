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

#include "addvideoimpl.h"
//
AddVideoImpl::AddVideoImpl(VideoInformation *videoInformation, QWidget * parent, Qt::WFlags f)
		: QDialog(parent, f)
{
	setupUi(this);
	this->videoInformation = videoInformation;
	// connect ok button
	connect(btnOk, SIGNAL(clicked()), this, SLOT(btnOkClicked())); //btn Ok (clicked)
	connect(edtURL, SIGNAL(textChanged(const QString &)), this, SLOT(edtURLChanged(const QString &))); //edtURL changed
	connect(spbPasteURL, SIGNAL(clicked()), this, SLOT(spbPasteURLClicked()));
}

void AddVideoImpl::btnOkClicked()
{
	done(QDialog::Accepted);
}

void AddVideoImpl::edtURLChanged(const QString &text)
{
	bool ok = videoInformation->isValidHost(text) && validURL(text);
	QString blockMsg = "";
	
	if (ok)
	{
		BlockedState bs = bsNotBlocked;
		ok = !videoInformation->isBlockedHost(text, bs);
		
		if (bs == bsBlocked) 
			blockMsg = tr(" - Blocked site");
		else if (bs == bsAdultContent) 
			blockMsg = tr(" - Adult content is not allowed");
	}
	
	btnOk->setEnabled(ok);
	
	// set color
	QColor valid(qApp->palette().brush(QPalette::Base).color());
	QColor invalid(255, 170, 127);

	QPalette p = edtURL->palette();
	p.setColor(QPalette::Base, ok ? valid : invalid);
	edtURL->setPalette(p);
	
	// set host info
	lblVideoService->setText(videoInformation->getHostCaption(text) + blockMsg);
	imgService->setPixmap(QPixmap(videoInformation->getHostImage(text, true)));
}

void AddVideoImpl::spbPasteURLClicked()
{
	edtURL->setText(QApplication::clipboard()->text());
}
//
