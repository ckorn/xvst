/*
*
* This file is part of xVideoServiceThief,
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 - 2010 Xesc & Technology
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

#ifndef ADDVIDEOIMPL_H
#define ADDVIDEOIMPL_H

#include "ui_addvideo.h"

class ProgramOptions;
class VideoInformation;

struct OverridedVideoConversionConfig;

class AddVideoImpl : public QDialog, public Ui::AddVideo
{
Q_OBJECT
	private:
		QSize originalSize;
		VideoInformation *videoInformation;
		ProgramOptions *programOptions;
		bool isCustomDownloadFlag;
		QString customDownloadTitle;
		void fillConversionOptions();
	public:
		AddVideoImpl(ProgramOptions *programOptions, VideoInformation *videoInformation,
					 QWidget * parent = 0, Qt::WFlags f = 0 );
		OverridedVideoConversionConfig getOverridedConversionConfig();
		bool isCustomDownload();
		QString getCustomDownloadTitle();
	private slots:
		void btnOkClicked();
		void edtURLChanged(const QString &text);
		void spbPasteURLClicked();
		void chbOverrideConversionClicked();
		void linkActivated(const QString &link);
};
#endif
