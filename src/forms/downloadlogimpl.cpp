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

