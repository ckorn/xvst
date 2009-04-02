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
	// hide the conversion options
	gpbVideoConversion->setVisible(false);
	resize(width(), 50);
	// prepare conversion options
	fillConversionOptions();
	// connect ok button
	connect(btnOk, SIGNAL(clicked()), this, SLOT(btnOkClicked())); //btn Ok (clicked)
	connect(edtURL, SIGNAL(textChanged(const QString &)), this, SLOT(edtURLChanged(const QString &))); //edtURL changed
	connect(spbPasteURL, SIGNAL(clicked()), this, SLOT(spbPasteURLClicked()));
	connect(chbOverrideConversion, SIGNAL(clicked()), this, SLOT(chbOverrideConversionClicked()));
}

void AddVideoImpl::fillConversionOptions()
{
	// string list of items
	QStringList itemsToAdd;
	// set items to outputFormat
	itemsToAdd	<< tr("AVI Format (*.avi)") << tr("WMV Format ( *.wmv)")
	<< tr("MPEG1 Format ( *.mpg)") << tr("MPEG2 Format ( *.mpg)")
	<< tr("MP4 Format (*.mp4)") << tr("Apple iPod (*.mp4)") << tr("Sony PSP (*.mp4)")
	<< tr("3GP Format (*.3gp)") << tr("MP3 Format (*.mp3)");
	cmbOutputFormat->addItems(itemsToAdd);

	// set items to video resolution
	itemsToAdd.clear();
	itemsToAdd	<< tr("Original") << "96 x 72" << "128 x 96" << "160 x 120"
	<< "176 x 120" << "176 x 144" << "192 x 144" << "240 x 180"
	<< "320 x 200" << "320 x 240" << "352 x 240" << "352 x 288"
	<< "480 x 272" << "480 x 360" << "480 x 480" << "624 x 352"
	<< "640 x 480" << "720 x 480" << "720 x 576";
	cmbVideoResolution->addItems(itemsToAdd);

	// set items to audio sampling
	itemsToAdd.clear();
	itemsToAdd	<< tr("Original") << "22050" << "44100";
	cmbAudioSampling->addItems(itemsToAdd);

	// set items to video frame rate
	itemsToAdd.clear();
	itemsToAdd	<< tr("Original") << "15" << "24" << "25" << "29.97" << "30";
	cmbVideoFrameRate->addItems(itemsToAdd);

	// set items to output quality
	itemsToAdd.clear();
	itemsToAdd	<< tr("Lower quality (Video bitrate: 384kbps; Audio bitrate: 64kbps)")
	<< tr("Low quality (Video bitrate: 512kbps; Audio bitrate: 80kbps)")
	<< tr("Normal quality (Video bitrate: 640kbps; Audio bitrate: 96kbps)")
	<< tr("Medium quality (Video bitrate: 800kbps; Audio bitrate: 96kbps)")
	<< tr("Good quality (Video bitrate: 1000kbps; Audio bitrate: 128kbps)")
	<< tr("Superb quality (Video bitrate: 1200kbps; Audio bitrate: 128kbps)")
	<< tr("The Same quality as the original Video");
	cmbOutputQuality->addItems(itemsToAdd);
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

void AddVideoImpl::chbOverrideConversionClicked()
{
	gpbVideoConversion->setVisible(!gpbVideoConversion->isVisible());
	// resize form
	if (!gpbVideoConversion->isVisible()) resize(width(), 50);
}

//
