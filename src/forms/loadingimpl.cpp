#include "loadingimpl.h"

static LoadingImpl* loadingImplInstance = NULL;

LoadingImpl::LoadingImpl(QWidget *parent) :
	QWidget(parent)
{
    setupUi(this);
	QWidget::setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint);
	lblMessage->setText("<font color=white>" + tr("Loading plugins... please wait...") + "</font>");

	QDesktopWidget *desktop = QApplication::desktop();

	int screenWidth, width;
	int screenHeight, height;
	int x, y;
	QSize windowSize;

	screenWidth = desktop->width();
	screenHeight = desktop->height();

	windowSize = size();
	width = windowSize.width();
	height = windowSize.height();

	x = (screenWidth - width) / 2;
	y = (screenHeight - height) / 2;
	y -= 50;

	move(x, y);
}

LoadingImpl* LoadingImpl::instance()
{
	if (loadingImplInstance == NULL) loadingImplInstance = new LoadingImpl;
	return loadingImplInstance;
}

void LoadingImpl::finished()
{
	lblMessage->setText("<font color=white>" + tr("Plugins loaded") + "</font>");
	QTimer::singleShot(400, this, SLOT(closeLoading()));
}

void LoadingImpl::closeLoading()
{
	close();
	// destroy instance
	delete loadingImplInstance;
	loadingImplInstance = NULL;
}
