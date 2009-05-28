#ifndef SCHEDULEITEMEDITIMPL_H
#define SCHEDULEITEMEDITIMPL_H

#include <QtGui>
//
#include "ui_scheduleitemedit.h"

class ScheduleItemEditImpl : public QDialog, public Ui::ScheduleItemEditImpl
{
Q_OBJECT
	public:
		ScheduleItemEditImpl(QWidget *parent = 0, Qt::WFlags f = 0);
	private slots:
		void btnOkClicked();
};

#endif // SCHEDULEITEMEDITIMPL_H
