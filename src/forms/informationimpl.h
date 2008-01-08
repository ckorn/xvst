#ifndef INFORMATIONIMPL_H
#define INFORMATIONIMPL_H
//
#include "ui_information.h"
//
#include "../programversion.h"
#include "../options.h"
#include "../languages.h"
//
class InformationImpl : public QDialog, public Ui::Information
{
Q_OBJECT
	public:
		InformationImpl(ProgramOptions *programOptions, QWidget * parent = 0, Qt::WFlags f = 0);
};
#endif
