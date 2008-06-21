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

#include "downloadlogimpl.h"
//
class LogTreeWidgetItem : public QTreeWidgetItem
{
	public:
		// constructors 
		LogTreeWidgetItem(QTreeWidget *tree) : QTreeWidgetItem(tree) {}
		LogTreeWidgetItem(QTreeWidget *parent, const QStringList &strings) : QTreeWidgetItem (parent,strings) {}
		// custom comparation method
		bool operator< ( const QTreeWidgetItem & other ) const
		{
			QDateTime myDate = data(0, Qt::UserRole).toDateTime();
			QDateTime otherDate = other.data(0, Qt::UserRole).toDateTime();
			// compare
			return myDate < otherDate;
		}
};
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
		QTreeWidgetItem *item = new LogTreeWidgetItem(lsvLog);//QTreeWidgetItem(lsvLog);
		
		item->setText(0, logEntries.at(n).dateTime.toString());
		item->setText(1, logEntries.at(n).title);
		item->setText(2, logEntries.at(n).URL);
		
		item->setIcon(0, QIcon(videoInformation->getHostImage(logEntries.at(n).URL)));
		
		item->setSizeHint(0, QSize(18,18));

		item->setData(0, Qt::UserRole, logEntries.at(n).dateTime);
	}
	// sort list
	lsvLog->sortItems(0, Qt::AscendingOrder);
}
//

