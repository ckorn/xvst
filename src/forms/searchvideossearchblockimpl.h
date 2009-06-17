#ifndef SEARCHVIDEOSSEARCHBLOCKIMPL_H
#define SEARCHVIDEOSSEARCHBLOCKIMPL_H

#include "ui_searchvideossearchblock.h"

class VideoInformationPlugin;

class SearchVideosSearchBlockImpl : public QWidget, private Ui::SearchVideosSearchBlockImpl
{
Q_OBJECT
	private:
		VideoInformationPlugin *plugin;
	public:
		SearchVideosSearchBlockImpl(QWidget *parent, VideoInformationPlugin *plugin, QString summary);
};

#endif // SEARCHVIDEOSSEARCHBLOCKIMPL_H
