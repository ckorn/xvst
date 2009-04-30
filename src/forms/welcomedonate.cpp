#include "welcomedonate.h"

WelcomeDonate::WelcomeDonate(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
	//
#ifndef Q_OS_WIN32
	resize(555, 355);
#endif
	//
	connect(btnDonate, SIGNAL(clicked()), this, SLOT(donateClicked()));
}

bool WelcomeDonate::getDisplayAgain()
{
	return chbDontDisplay->isChecked();
}

void WelcomeDonate::donateClicked()
{
	QDesktopServices::openUrl(QUrl("http://xviservicethief.sourceforge.net/index.php?action=make_donation"));
}
