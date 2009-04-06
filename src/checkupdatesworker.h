#ifndef CHECKUPDATESWORKER_H
#define CHECKUPDATESWORKER_H

#include <QtGui>

#include "forms/updatecenterimpl.h"
#include "forms/windowsvistadetectedimpl.h"

#include "options.h"
#include "updates.h"

static const QString URL_UPDATE_FILE = "http://xviservicethief.sourceforge.net/updates/xVST.update";

class CheckUpdatesWorker : public QObject
{
Q_OBJECT
	private:
		Updates *updates;				//!< Responsable of check and download updates
		ProgramOptions *programOptions;	//!< Program options instance
		QLabel *caption;				//!< Caption which will display the current action
		QProgressBar *progressBar;		//!< ProgressBar used for represent the current progress
		QAbstractButton *cancelButton;	//!< Button for cancel current updates check
		QWidget *parentForm;			//!< Form which called this class
		bool closedByButton;			//!< Flag for know if has been closed by button or automatically
		bool isUser;					//!< Flag for know if this class has been called automatically or not
		/*! Wait while the updater is running */
		void waitThread();
		/*! Return if we are running the program under Windows VISTA */
		bool isWindowsVista();
	public:
		/*! Class constructor */
		CheckUpdatesWorker(ProgramOptions *programOptions, QWidget *parentForm, QLabel *caption,
						   QProgressBar *progressBar, QAbstractButton *cancelButton, bool isUser);
		/*! Class destructor */
		~CheckUpdatesWorker();
		/*! Start check updates */
		void checkUpdates();
	private slots:
		/*! After check for updates */
		void updatesChecked(bool hasUpdates);
		/*! Update proces cancelled */
		void cancelButtonClicked();
		/*! On updates has been cancelled */
		void updatesCancelled();
		/*! On progress update */
		void progressCheckUpdate(int progress);
	signals:
		/*! On finish */
		void finished(bool done, bool closedByButton);
		/*! Before display the update center window */
		void beforeDisplayUpdateCenter();
};

#endif // CHECKUPDATESWORKER_H
