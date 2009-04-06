/*
*
* This file is part of xVideoServiceThief,
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 - 2008 Xesc & Technology
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with xVideoServiceThief. If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: Xesc <xeskuu.xvst@gmail.com>
* Program URL   : http://xviservicethief.sourceforge.net/
*
*/

#include "informationimpl.h"
//
InformationImpl::InformationImpl(ProgramOptions *programOptions, QWidget * parent, Qt::WFlags f)
		: QDialog(parent, f)
{
	setupUi(this);
	// signals
	connect(btnCredits, SIGNAL(clicked()), this, SLOT(btnCreditsClicked()));
	// set the program version
	lblProgramVersion->setText(QString("<b>%1</b>").arg(PROGRAM_VERSION));
	// set language info
	Language *language = LanguageManager::getLanguageInfo(programOptions->getLanguageFile(true));
	if (language != NULL)
	{
		lblLanguage->setText(QString("<b>%1</b>").arg(language->getId()));
		lblURL->setText(QString("<a href=\"%1\">%2</href>").arg(language->getContact()).arg(language->getContact()));
	}
	// load the service list
	rchServices->setSource(QUrl("qrc:/service_list/service_list.html"));
	// set the support project link
	imgSupport->setText("<a href=\"http://xviservicethief.sourceforge.net/index.php?action=make_donation\"><img src=\":/buttons/images/support_button.png\" /></a>");
}

void InformationImpl::btnCreditsClicked()
{
	CreditsImpl creditsForm;
	creditsForm.exec();
}
//
