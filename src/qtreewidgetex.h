#ifndef QTREEWIDGETEX_H
#define QTREEWIDGETEX_H

#include <QTreeWidget>
//
#include "tools.h"
#include "videolistcontroller.h"

class QTreeWidgetEx : public QTreeWidget
{
	private:
		VideoListController *videoList;
		void dragEnterEvent(QDragEnterEvent *event);
		void dropEvent(QDropEvent *event);
	public:
		QTreeWidgetEx(QWidget *parent = 0);
		void setVideoList(VideoListController *videoList);
};

#endif // QTREEWIDGETEX_H
