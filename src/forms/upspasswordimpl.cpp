#include "upspasswordimpl.h"

UPSPasswordImpl::UPSPasswordImpl(QWidget *parent, Qt::WFlags f)
	: QDialog(parent, f)
{
    setupUi(this);
	// resize form if is needed
#ifndef Q_OS_WIN32
	resize(472, 162);
#endif
}
