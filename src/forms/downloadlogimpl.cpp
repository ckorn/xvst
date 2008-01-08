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

#include "downloadlogimpl.h"
//
DownloadLogImpl::DownloadLogImpl(QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);

	// add the headers
	QStringList headers;
	headers << tr("Date/Time") << tr("Video Title") << tr("URL");
	lsvLog->setHeaderLabels(headers);
	// resize sections (header)
	QHeaderView *header = lsvLog->header();
	header->resizeSection(0, 150);
	header->resizeSection(1, 250);
}

void DownloadLogImpl::buildLog(QList<LogEntry> logEntries, VideoInformation *videoInformation)
{
	for (int n = 0; n < logEntries.count(); n++)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(lsvLog);
		
		item->setText(0, logEntries.at(n).dateTime.toString());
		item->setText(1, logEntries.at(n).title);
		item->setText(2, logEntries.at(n).URL);
		
		item->setIcon(0, QIcon(videoInformation->getHostImage(logEntries.at(n).URL)));
		
		item->setSizeHint(0, QSize(18,18));
	}
}
//

