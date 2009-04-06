#include "checkupdatesworker.h"

// windows vista detection
#ifdef Q_WS_WIN
	#include <windows.h>
#endif

CheckUpdatesWorker::CheckUpdatesWorker(ProgramOptions *programOptions, QWidget *parentForm, QLabel *caption,
									   QProgressBar *progressBar, QAbstractButton *cancelButton, bool isUser)
	: QObject()
{
	this->programOptions = programOptions;
	this->parentForm = parentForm;
	this->caption = caption;
	this->progressBar = progressBar;
	this->cancelButton = cancelButton;
	this->isUser = isUser;
	// init updater
	updates = new Updates(programOptions->getApplicationPath());
	// signals
	connect(updates, SIGNAL(progressCheckUpdate(int)), this, SLOT(progressCheckUpdate(int)));
	connect(updates, SIGNAL(updatesChecked(bool)), this, SLOT(updatesChecked(bool)));
	connect(updates, SIGNAL(updatesCancelled()), this, SLOT(updatesCancelled()));
	//
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
}

CheckUpdatesWorker::~CheckUpdatesWorker()
{
	delete updates;
}

void CheckUpdatesWorker::checkUpdates()
{
	programOptions->setLastUpdate(QDate::currentDate());
	programOptions->save();

	updates->checkForUpdates(URL_UPDATE_FILE);
}

void CheckUpdatesWorker::waitThread()
{
	while (updates->isRunning())
		qApp->processEvents();
}

bool CheckUpdatesWorker::isWindowsVista()
{
#ifdef Q_WS_WIN
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	// return if we are running the windows vista OS
	return((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion == 0));
#else
	return false;
#endif
}

void CheckUpdatesWorker::updatesChecked(bool hasUpdates)
{
	// if has updates, then display the update center
	if (hasUpdates)
	{
		//this->setVisible(false);
		// result checker
		int result = QDialog::Accepted;
		// check if we are running windwos vista
		if (isWindowsVista() && !programOptions->getVistaUpdatesMessage())
		{
			WindowsVistaDetectedImpl windowsVistaDetectedForm(parentForm);
			result = windowsVistaDetectedForm.exec();
			// update the option "don't display this message"
			programOptions->setVistaUpdatesMessage(windowsVistaDetectedForm.chbDontDisplay->isChecked());
		}
		// open update center (this condition works only on windows vista)
		if (result == QDialog::Accepted)
		{
			emit beforeDisplayUpdateCenter();
			// dispaly
			UpdateCenterImpl updateCenterForm(updates, programOptions->getInstallAutomaticallyUpdates(), NULL);
			result = updateCenterForm.exec();
		}
		// wait until "thread end"
		waitThread();
		// updates downloaded??? yes? then install them
		if (result == QDialog::Accepted)
		{
			updates->installUpdates();
			QApplication::closeAllWindows();
		}
		else // update center has been cancelled
		{
			closedByButton = true;
			//done(QDialog::Rejected);
			emit finished(false, closedByButton);
		}
	}
	else
	{
		waitThread();

		if (isUser)
			QMessageBox::information(NULL,
									 tr("Updates"),
									 tr("You are using the most recent version of this program."),
									 tr("Ok"));
		closedByButton = true;

		//done(QDialog::Rejected);
		emit finished(false, closedByButton);
	}
}

void CheckUpdatesWorker::cancelButtonClicked()
{
	cancelButton->setEnabled(false);
	updates->cancel();
}

void CheckUpdatesWorker::updatesCancelled()
{
	caption->setText(tr("Cancelling... please wait..."));

	waitThread();

	closedByButton = true;
	//done(QDialog::Rejected);
	emit finished(false, closedByButton);
}

void CheckUpdatesWorker::progressCheckUpdate(int progress)
{
	caption->setText(tr("Checking for updates..."));
	progressBar->setMaximum(100);
	progressBar->setValue(progress);

	// imposible cancel it
	if (progress == 100)
		cancelButton->setEnabled(false);
}
