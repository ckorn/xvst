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
	// signals
	connect(spbViewInfo, SIGNAL(clicked()), this, SLOT(viewInfoClicked()));
}

void BugReportImpl::fillErrorInfo(VideoItem *videoItem)
{
	this->videoItem = videoItem;
	// set visual info
	lblVideoURL->setText(QString(lblVideoURL->text()).arg(videoItem->getURL()));
}

void BugReportImpl::viewInfoClicked()
{
	InfoViewImpl infoView(videoItem, edtName->text(), edtEmail->text(), 
		rchComments->toPlainText(), this);
	infoView.exec();
}
//
