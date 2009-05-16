#include "loadingimpl.h"

LoadingImpl::LoadingImpl(QWidget *parent) :
	QWidget(parent)
{
    setupUi(this);
	QWidget::setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint);
	lblMessage->setText("Loading plugins... please wait...");
}

void LoadingImpl::finished()
{
	lblMessage->setText("Plugins loaded");
	QTimer::singleShot(400, this, SLOT(closeLoading()));
}

void LoadingImpl::closeLoading()
{
	close();
}
