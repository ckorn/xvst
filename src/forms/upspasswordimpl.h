#ifndef UPSPASSWORDIMPL_H
#define UPSPASSWORDIMPL_H

#include "ui_upspassword.h"

class UPSPasswordImpl : public QDialog, public Ui::UPSPasswordImpl
{
	public:
		UPSPasswordImpl(QWidget *parent = 0, Qt::WFlags f = 0 );
};

#endif // UPSPASSWORDIMPL_H
