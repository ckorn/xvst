#ifndef WELCOMEDONATE_H
#define WELCOMEDONATE_H

#include <QtGui>

#include "ui_welcomedonate.h"

class WelcomeDonate : public QDialog, private Ui::WelcomeDonate
{
Q_OBJECT
	public:
		WelcomeDonate(QWidget *parent = 0);
		bool getDisplayAgain();
	private slots:
		void donateClicked();
};

#endif // WELCOMEDONATE_H
