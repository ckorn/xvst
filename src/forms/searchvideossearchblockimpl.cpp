#include "searchvideossearchblockimpl.h"

#include "../videoinformation.h"

SearchVideosSearchBlockImpl::SearchVideosSearchBlockImpl(QWidget *parent, VideoInformationPlugin *plugin, QString summary)
	: QWidget(parent)
{
    setupUi(this);
	//
	this->plugin = plugin;
	//
	imgService->setPixmap(*(plugin->getIcon()));
	lblTitle->setText(QString("<b>%1</b>").arg(plugin->getCaption()));
	lblSummary->setText(summary);
}
