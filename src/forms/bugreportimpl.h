#ifndef BUGREPORTIMPL_H
#define BUGREPORTIMPL_H
//
#include <QDialog>
//
#include "ui_bugreport.h"
//
#include "infoviewimpl.h"
//
#include "../programversion.h"
#include "../videoitem.h"
//
class BugReportImpl : public QDialog, public Ui::BugReport
{
Q_OBJECT
	private:
		VideoItem *videoItem; //<! VideoItem with the error
	public:
		/*! Class constructor */
		BugReportImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
		/*! Fill the error information */
		void fillErrorInfo(VideoItem *videoItem);
	private slots:
		void viewInfoClicked();
};
#endif
