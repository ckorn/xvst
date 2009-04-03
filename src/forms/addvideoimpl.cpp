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
AddVideoImpl::AddVideoImpl(ProgramOptions *programOptions, VideoInformation *videoInformation,
						   QWidget * parent, Qt::WFlags f)
		: QDialog(parent, f)
{
	setupUi(this);
	this->programOptions = programOptions;
	this->videoInformation = videoInformation;
	// hide the conversion options
	originalSize = size();
	gpbVideoConversion->setVisible(false);
	chbOverrideConversion->setVisible(QFile::exists(programOptions->getFfmpegLibLocation()));
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
	// set conversion data
	cmbOutputFormat->addItems(VideoConverter::getOutputFormatAsStrings());
	cmbVideoResolution->addItems(VideoConverter::getVideoResolutionAsStrings());
	cmbAudioSampling->addItems(VideoConverter::getAudioSampleRatioAsStrings());
	cmbVideoFrameRate->addItems(VideoConverter::getVideoFrameRateAsStrings());
	cmbOutputQuality->addItems(VideoConverter::getOutputQualityAsStrings());

	// set the current video conversion config
	gpbVideoConversion->setChecked(programOptions->getConvertVideos());
	cmbOutputFormat->setCurrentIndex(programOptions->getConversionConf().outputFormat);
	cmbVideoResolution->setCurrentIndex(programOptions->getConversionConf().videoResolution);
	cmbAudioSampling->setCurrentIndex(programOptions->getConversionConf().audioSampleRatio);
	cmbVideoFrameRate->setCurrentIndex(programOptions->getConversionConf().videoFrameRate);
	cmbOutputQuality->setCurrentIndex(programOptions->getConversionConf().outputQuality);
}

OverridedVideoConversionConfig AddVideoImpl::getOverridedConversionConfig()
{
	VideoConversionConfig convConf;
	convConf.outputFormat = static_cast<OutputFormat>(cmbOutputFormat->currentIndex());
	convConf.videoResolution = static_cast<VideoResolution>(cmbVideoResolution->currentIndex());
	convConf.audioSampleRatio = static_cast<AudioSampleRatio>(cmbAudioSampling->currentIndex());
	convConf.videoFrameRate = static_cast<VideoFrameRate>(cmbVideoFrameRate->currentIndex());
	convConf.outputQuality = static_cast<OutputQuality>(cmbOutputQuality->currentIndex());
	// should it be converted?
	OverridedVideoConversionConfig result;
	result.convertVideo = gpbVideoConversion->isChecked();
	result.videoConversionConfig = convConf;
	return result;
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
	if (!gpbVideoConversion->isVisible())
	{
		gpbVideoConversion->setVisible(true);
		// calcule size diference
		int preWidth = (width() > originalSize.width() ? width() - originalSize.width() : originalSize.width() - width())/2;
		int preHeight = (height() > originalSize.height() ? height() - originalSize.height() : originalSize.height() - height())/2;
		// move the form to update the new size
		move(x() - preWidth, y() - preHeight);
	}
	// enable or disable the groupbox
	gpbVideoConversion->setEnabled(chbOverrideConversion->isChecked());
}
//
