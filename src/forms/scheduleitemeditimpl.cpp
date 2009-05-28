#include "scheduleitemeditimpl.h"

ScheduleItemEditImpl::ScheduleItemEditImpl(QWidget *parent, Qt::WFlags f)
	: QDialog(parent, f)
{
    setupUi(this);
	// resize window (macosx and linux)
#ifdef Q_WS_MAC
	resize(356, 200);
#endif
	// connect signals
	connect(btnOk, SIGNAL(clicked()), this, SLOT(btnOkClicked()));
}

void ScheduleItemEditImpl::btnOkClicked()
{
	if (timerStart->time() > timerEnd->time())
		QMessageBox::information(this,
								tr("Invalid schedule configuration"),
								tr("The start time cannot exceed the end time."),
								tr("Ok"));
	else if (timerStart->time() == timerEnd->time())
		QMessageBox::information(this,
								tr("Invalid schedule configuration"),
								tr("The start time and end time cannot be equals."),
								tr("Ok"));
	else
		accept();
}
