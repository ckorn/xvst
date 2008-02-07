#ifndef CREDITSIMPL_H
#define CREDITSIMPL_H
//
#include <QDialog>
//
#include "ui_credits.h"
//
class CreditsImpl : public QDialog, public Ui::Credits
{
Q_OBJECT
	public:
		CreditsImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
};
#endif
