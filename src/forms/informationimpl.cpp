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
InformationImpl::InformationImpl(ProgramOptions *programOptions, VideoInformation *videoInformation,
								 QWidget * parent, Qt::WFlags f)
		: QDialog(parent, f)
{
	setupUi(this);
#ifdef Q_WS_MAC
	resize(518, 550);
	setMinimumSize(518, 550);
#endif
	//
	creditsForm = new CreditsImpl(this, Qt::Sheet);
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
	buildVideoServicesList(videoInformation);
	// set the support project link
	imgPaypal->setText("<a href=\"http://xviservicethief.sourceforge.net/index.php?action=make_donation\"><img src=\":/buttons/images/support_button.png\" /></a>");
}

InformationImpl::~InformationImpl()
{
	delete creditsForm;
}

QString getPluginsAsHtml(QString title, QList<VideoInformationPlugin*> plugins)
{
	QString html;

	if (plugins.count() > 0)
	{
		html += title;
		html += "<ul>";
		for (int n = 0; n < plugins.count(); n++)
			html += QString("<li><a href='%1' target='_blank'>%2</a> <i><font color=#909090>(%3)</font></i></li>").
					arg(plugins.at(n)->getWebsite()).
					arg(plugins.at(n)->getCaption()).
					arg(plugins.at(n)->getVersion());
		html += "</ul>";
	}

	return html;
}

void InformationImpl::buildVideoServicesList(VideoInformation *videoInformation)
{
	QList<VideoInformationPlugin*> standardPlugins = videoInformation->getAllStandardPlugins();
	QList<VideoInformationPlugin*> adultPlugins = videoInformation->getAllAdultPlugins();
	QList<VideoInformationPlugin*> musicPlugins = videoInformation->getAllMusicPlugins();
	// generate html
	QString html;
	html += tr("<p>Total websites supported (plugins installed): <strong>%1</strong></p>").arg(videoInformation->pluginsCount());
	html += getPluginsAsHtml(tr(""), standardPlugins);
	html += getPluginsAsHtml(tr("Adult web sites:"), adultPlugins);
	html += getPluginsAsHtml(tr("Music web sites:"), musicPlugins);
	// display html
	rchServices->setHtml(html);
}

void InformationImpl::btnCreditsClicked()
{
	creditsForm->show();
}
//
