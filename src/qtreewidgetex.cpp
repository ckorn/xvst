#include "qtreewidgetex.h"

QTreeWidgetEx::QTreeWidgetEx(QWidget *parent) : QTreeWidget(parent) {}

void QTreeWidgetEx::setVideoList(VideoListController *videoList)
{
	this->videoList = videoList;
}

void QTreeWidgetEx::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/plain"))
		event->acceptProposedAction();
}

void QTreeWidgetEx::dropEvent(QDropEvent *event)
{
	// get the url
	QString url = event->mimeData()->text();
	if (getTokenCount(url, "\n") > 0) url = getToken(url, "\n", 0);
	// emit drop event with url
	if (videoList != NULL) videoList->addVideo(url);
	// ok
	event->acceptProposedAction();
}
