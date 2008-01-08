#include "completedpopupimpl.h"
//
CompletedPopupImpl::CompletedPopupImpl( QWidget * parent, Qt::WFlags f)
		: QWidget(parent, f)
{
	setupUi(this);
	// set window flags
	QWidget::setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint);
	// init the timer
	displayTimer = new QTimer(this);
	connect(displayTimer, SIGNAL(timeout()), this, SLOT(displayTimerOnTimer()));
	// connect buttons
	connect(spbClose, SIGNAL(clicked()), this, SLOT(closeClicked()));
	connect(spbPlayVideo, SIGNAL(clicked()), this, SLOT(playVideoClicked()));
	// init state
	displayState = dsInactive;
}

CompletedPopupImpl::~CompletedPopupImpl()
{
	delete displayTimer;
}

void CompletedPopupImpl::displayTimerOnTimer()
{
	if (!isVisible()) return;

	QDesktopWidget *desktop = QApplication::desktop();
	QRect availableGeometry  = desktop->availableGeometry(this);

	// display popup animation
	if (displayState == dsShowing)
		if (pos().x() > availableGeometry.width() - size().width())// - 15)
			move(pos().x() - 1, pos().y());
		else
		{
			displayState = dsWaiting;
			displayTimer->start(4000);
		}
	// hide popup animation
	else if (displayState == dsHiding)
		if (pos().x() < availableGeometry.width())
			move(pos().x() + 1, pos().y());
		else
		{
			displayState = dsWaiting;
			displayTimer->stop();
			hide();
		}
	else if (displayState == dsWaiting)
	{
		displayState = dsHiding;
		displayTimer->start(1);
	}
}

void CompletedPopupImpl::displayPopup(const QIcon &serviceIcon, const QString videoTitle, const QString videoFile)
{
	// set window info
	imgVideoService->setPixmap(serviceIcon.pixmap(QSize(16,16)));
	lblVideoTitle->setText(videoTitle);
	this->videoFile = videoFile;

	QDesktopWidget *desktop = QApplication::desktop();
	QRect availableGeometry  = desktop->availableGeometry(this);
	move(desktop->width(), availableGeometry.height() - size().height());
	this->show();
	displayState = dsShowing;
	displayTimer->start(1);
}

void CompletedPopupImpl::closeClicked()
{
	displayState = dsHiding;
	displayTimer->start(1);
}

void CompletedPopupImpl::playVideoClicked()
{
	if (QFile::exists(videoFile))
		QDesktopServices::openUrl(videoFile);
}
//
