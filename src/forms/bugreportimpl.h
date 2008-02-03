#ifndef BUGREPORTIMPL_H
#define BUGREPORTIMPL_H
//
#include <QDialog>
//
#include "ui_bugreport.h"
//
#include "infoviewimpl.h"
//
#include "../tools.h"
#include "../trackerreport.h"
#include "../programversion.h"
#include "../videoitem.h"
#include "../videoinformation.h"
#include "../options.h"
//
class BugReportImpl : public QDialog, public Ui::BugReport
{
Q_OBJECT
	private:
		ProgramOptions *programOptions;		//<! Program options reference
		VideoItem *videoItem; 				//<! VideoItem with the error
		TrackerReport *trackerReport;		//<! Tracker report class
		VideoInformation *videoInformation;	//<! Video service info
	public:
		/*! Class constructor */
		BugReportImpl(ProgramOptions *programOptions, QWidget * parent = 0, Qt::WFlags f = 0);
		/*! Class destructor */
		~BugReportImpl();
		/*! Fill the error information */
		void fillErrorInfo(VideoItem *videoItem, VideoInformation *videoInformation);
	private slots:
		void viewInfoClicked();
		void sendReportClicked();
		void cancelClicked();
		void trackerReportSent(QString result);
};
#endif
