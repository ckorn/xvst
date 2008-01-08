#ifndef __PROGRESSBARDELEGATE_H__
#define __PROGRESSBARDELEGATE_H__

#include <QtGui>
//
#include "videolistcontroller.h"

class ProgressBarDelegate : public QItemDelegate 
{
Q_OBJECT
	private:
		VideoListController *videoList;
	public:
		ProgressBarDelegate(QObject *parent = 0, VideoListController *videoList = NULL);
		void paint(QPainter * painter, const QStyleOptionViewItem & option, 
			const QModelIndex & index ) const;
}; 

#endif // __PROGRESSBARDELEGATE_H__
