#ifndef INFOVIEWIMPL_H
#define INFOVIEWIMPL_H
//
#include <QtGui>
//
#include "ui_infoview.h"
//
#include "../programversion.h"
#include "../videoitem.h"
//
class InfoViewImpl : public QDialog, public Ui::InfoView
{
Q_OBJECT
	public:
		InfoViewImpl(VideoItem *videoItem, QString userName, QString email,
			QString comments, QWidget * parent = 0, Qt::WFlags f = 0 );
};
#endif
