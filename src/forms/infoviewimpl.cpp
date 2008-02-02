#include "infoviewimpl.h"
//
InfoViewImpl::InfoViewImpl(VideoItem *videoItem, QString userName, QString email,
	QString comments, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);
	// fill text
	QStringList info;
	info 	<< "<p><b>Video Information:</b>\n"
			<< "Video URL: " + videoItem->getURL() + "\n"
			<< "xVST Version: " + PROGRAM_VERSION + " (" + CURRENT_OS + ")" + "\n"
			<< "FLV URL: " + videoItem->getVideoInformation().URL + "\n"
			<< "FLV Title: " + videoItem->getVideoInformation().title + "</p>"
			<< "<p><b>Sender Information:</b>\n"
			<< "User name: " + userName + "\n"
			<< "User eMail: " + email + "\n"
			<< "User Comments: " + comments + "</p>";
			
	foreach (QString line, info)
		rchInformation->setHtml(rchInformation->toHtml() + line);
}
//

