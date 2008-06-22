#ifndef WINDOWSVISTADETECTEDIMPL_H
#define WINDOWSVISTADETECTEDIMPL_H
//
#include <QDialog>
//
#include "ui_winvistaupdatesmsg.h"
//
#include "../options.h"
//
class WindowsVistaDetectedImpl : public QDialog, public Ui::WindowsVistaDetected
{
Q_OBJECT
	public:
		WindowsVistaDetectedImpl(QWidget * parent = 0, Qt::WFlags f = 0 );
	private slots:
};
#endif





