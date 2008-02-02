#include "bugreportimpl.h"
//
BugReportImpl::BugReportImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	// hide "send group"
	line_2->hide();
	lblSending->hide();
	prbSending->hide();
	// set program version
	lblxVSTVersion->setText(QString(lblxVSTVersion->text()).arg(PROGRAM_VERSION));
	// configure tracker
	trackerReport = new TrackerReport("205061", "1037803");
	// add category
	trackerReport->addCategory("None", "100");
	// add groups
	trackerReport->addGroup("1.0a", "804496");
	trackerReport->addGroup("1.1a", "804497");
	trackerReport->addGroup("1.2a", "804498");
	trackerReport->addGroup("1.3a", "804499");
	trackerReport->addGroup("1.4a", "804500");
	trackerReport->addGroup("1.5a", "804501");
	// signals
	connect(spbViewInfo, SIGNAL(clicked()), this, SLOT(viewInfoClicked()));
	connect(btnSend, SIGNAL(clicked()), this, SLOT(sendReportClicked()));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
	connect(trackerReport, SIGNAL(trackerReportSent(QString)), this, SLOT(trackerReportSent(QString)));
}

BugReportImpl::~BugReportImpl()
{
	delete trackerReport;
}

void BugReportImpl::fillErrorInfo(VideoItem *videoItem, VideoInformation *videoInformation)
{
	this->videoItem = videoItem;
	this->videoInformation = videoInformation;
	// set visual info
	lblVideoURL->setText(QString(lblVideoURL->text()).arg(videoItem->getURL()));
}

void BugReportImpl::viewInfoClicked()
{
	InfoViewImpl infoView(videoItem, edtName->text(), edtEmail->text(), 
		rchComments->toPlainText(), this);
	infoView.exec();
}

void BugReportImpl::sendReportClicked()
{
	btnSend->setEnabled(false);
	btnCancel->setEnabled(false);
	// display "send group"
	line_2->show();
	lblSending->show();
	prbSending->show();
	// resize form
	// fill text
	QStringList info;
	info 	<< "Video Information:\n"
			<< "Video URL: " + videoItem->getURL() + "\n"
			<< "xVST Version: " + PROGRAM_VERSION + " (" + CURRENT_OS + ")" + "\n"
			<< "FLV URL: " + videoItem->getVideoInformation().URL + "\n"
			<< "FLV Title: " + videoItem->getVideoInformation().title + "\n\n"
			<< "Sender Information:\n"
			<< "User name: " + edtName->text() + "\n"
			<< "User eMail: " + edtEmail->text() + "\n"
			<< "User Comments: " + rchComments->toPlainText();

	QString infoStr;
	foreach (QString line, info)
		infoStr += line;

	QString videoServiceName = videoInformation->getHostCaption(videoItem->getURL());

	trackerReport->sendTrackerReport("None", PROGRAM_VERSION_SHORT, 
									 QString("xVST: %1").arg(videoServiceName), 
									 QString(infoStr).replace("&", "%26"));
}

void BugReportImpl::cancelClicked()
{
	done(QDialog::Rejected);
}

void BugReportImpl::trackerReportSent(QString result)
{
	line_2->hide();
	lblSending->hide();
	prbSending->hide();
	
	QString msg1 = copy(result, 0, result.indexOf("<p>")).trimmed();
	QString msg2 = copyBetween(result, "<p>", "<br>").trimmed();
	QString msg3 = copyBetween(result, "<a href='", "'>").trimmed();

	QMessageBox::information(this, 
							 QString("SourceForge.net Tracker: %1").arg(msg1),
							 QString("%1 <a href=\"%2\">%2</a>").arg(msg2).arg(msg3),
							 tr("Ok"));

	done(QDialog::Accepted);
}
//
