#ifndef COMPLETEDPOPUPIMPL_H
#define COMPLETEDPOPUPIMPL_H
//
#include <QtGui>
//
#include "ui_completedpopup.h"
//

enum DisplayState {dsInactive, dsShowing, dsWaiting, dsHiding};

class CompletedPopupImpl : public QWidget, public Ui::CompletedPopup
{
Q_OBJECT
	private:
		QTimer *displayTimer;
		QString videoFile;
		DisplayState displayState;
	public:
		CompletedPopupImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
		~CompletedPopupImpl();
		void displayPopup(const QIcon &, const QString, const QString);
	private slots:
		void displayTimerOnTimer();
		void closeClicked();
		void playVideoClicked();
};
#endif
