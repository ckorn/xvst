/*
*
* This file is part of xVideoServiceThief, 
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 Xesc & Technology
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Xesc <xeskuu@gmail.com>
* Program URL   : http://xviservicethief.sourceforge.net/
*
*/

#include "videoinformation.h"

VideoInformation::VideoInformation()
{
	plugins = new QList<VideoInformation_plugin *>;
	// init data
	videoItem = NULL;
	blockAdultContent = false;
	// add plugins
	new VideoInformation_Youtube(this);
	new VideoInformation_Metacafe(this);
	new VideoInformation_Google(this);
	new VideoInformation_ZappInternet(this);
	new VideoInformation_Dailymotion(this);
	new VideoInformation_Dumpalink(this);
	new VideoInformation_Glumbert(this);
	new VideoInformation_Sclipo(this);
}

VideoInformation::~VideoInformation()
{
	if (isRunning())
		quit();

	clearPlugins();
	delete plugins;
}

bool VideoInformation::validItemIndex(const int index)
{
	return (index >= 0 && index < getPluginsCount());
}

void VideoInformation::clearPlugins()
{
	while (!plugins->isEmpty())
		delete plugins->takeFirst();
}

VideoInformation_plugin* VideoInformation::getPluginByHost(QUrl URL)
{
	QString host = URL.host().isEmpty() ? URL.toString() : URL.host();
	for (int n = 0; n < getPluginsCount(); n++)
		if (plugins->at(n)->isLikeMyId(host))
			return plugins->at(n);
	// not found
	return NULL;
}

void VideoInformation::registerPlugin(VideoInformation_plugin *videoDownload_plugin)
{
	plugins->push_back(videoDownload_plugin);
}

void VideoInformation::run()
{
	videoItem->lock (this);

	VideoInformation_plugin *service = getPluginByHost(QUrl(videoItem->getURL()));

	if (service != NULL)
	{
		if (isBlockedHost(videoItem->getURL()))
			videoItem->setAsBlocked(this);
		else
		{
			videoItem->setAsGettingURL(this);
			emit informationStarted(videoItem);

			videoItem->setVideoInformation(service->getVideoInformation(videoItem->getURL()), this);
			videoItem->setVideoFile(videoItem->getVideoInformation().title + ".flv", this);

			videoItem->setAsGettedURL(this);
		}
	}
	else
		videoItem->setAsError(this);

	videoItem->unlock(this);

	emit informationFinished(videoItem);
}

VideoInformation_plugin* VideoInformation::getRegisteredPlugin(const int index)
{
	if (validItemIndex(index))
		return plugins->at(index);
	else
		return NULL;
}

QStringList VideoInformation::getPluginsList(bool asCaptions)
{
	QStringList results;

	for (int n = 0; n < getPluginsCount(); n++)
		results << (asCaptions ? plugins->at(n)->getCaption() : plugins->at(n)->getID());

	return results;
}

QStringList VideoInformation::getPluginsCompleteList(const QString separator)
{
	QStringList results;

	for (int n = 0; n < getPluginsCount(); n++)
		results << plugins->at(n)->getID() + separator + plugins->at(n)->getCaption();

	return results;
}

int VideoInformation::getPluginsCount()
{
	return plugins->count();
}

void VideoInformation::getVideoInformation(VideoItem *videoItem)
{
	if (videoItem == NULL || isGettingInfo()) return;
	// assign the current video item
	this->videoItem = videoItem;
	this->start();
}

bool VideoInformation::getBlockAdultContent()
{
	return blockAdultContent;
}

void VideoInformation::setBlockAdultContent(bool blockAdultContent)
{
	this->blockAdultContent = blockAdultContent;
}

QString VideoInformation::getBlockAdultContentList()
{
	QString result = "";
	for (int n = 0; n < blockAdultContentList.count(); n++)
		result += "|" + blockAdultContentList.at(n);
		
	return result;
}

void VideoInformation::setBlockAdultContentList(QString hosts)
{
	blockAdultContentList = hosts.split("|", QString::SkipEmptyParts);
}

bool VideoInformation::isGettingInfo()
{
	return isRunning();
}

bool VideoInformation::canGetInformation()
{
	return !isRunning();
}

QString VideoInformation::getHostImage(QString URL)
{
	const QString path = ":/services/images/services/%1.png";
	
	if (QUrl(URL).isValid())
	{
		VideoInformation_plugin *plugin = getPluginByHost(QUrl(URL));
		
		QString result = plugin != NULL ? 
						 QString(path).arg(plugin->getID()):
						 QString(path).arg("invalid");
		
		if (!QFile::exists(result))
			result = QString(path).arg("no_icon");
		
		return result;		
	}
	else
		return QString(path).arg("link_error");
}

QString VideoInformation::getHostCaption(QString URL)
{
	if (QUrl(URL).isValid())
	{
		VideoInformation_plugin *plugin = getPluginByHost(QUrl(URL));
		return plugin != NULL ? plugin->getCaption() : tr("Unsupported video service");
	}
	else
		return tr("Invalid URL");
}

bool VideoInformation::isValidHost(QString URL)
{
	return getPluginByHost(QUrl(URL)) != NULL && QUrl(URL).isValid() ? true : false;
}

bool VideoInformation::isBlockedHost(QString URL, BlockedState &result)
{
	VideoInformation_plugin *service = getPluginByHost(QUrl(URL));
	result = bsNotBlocked;
	
	if (service != NULL)
	{
		if (service->hasAdultContent() && blockAdultContent)
			result = bsAdultContent;
		else
			if (blockAdultContentList.indexOf(service->getID(), 0) != -1)
				result = bsBlocked;
	}

	return result != bsNotBlocked;
}

bool VideoInformation::isBlockedHost(QString URL)
{
	BlockedState bs;
	
	return isBlockedHost(URL, bs);
}

// VideoInformation_plugin class

void VideoInformation_plugin::registPlugin(VideoInformation *videoInformation)
{
	if (videoInformation != NULL)
		videoInformation->registerPlugin(this);
}

void VideoInformation_plugin::setID(QString ID)
{
	this->ID = ID.toLower();
}

void VideoInformation_plugin::setCaption(QString caption)
{
	this->caption = caption;
}

bool VideoInformation_plugin::isLikeMyId(QString ID)
{
	return ID.indexOf(this->ID) > -1;
}

QString VideoInformation_plugin::getID()
{
	return ID;
}

QString VideoInformation_plugin::getCaption()
{
	return caption;
}

bool VideoInformation_plugin::hasAdultContent()
{
	return adultContent;
}

VideoDefinition VideoInformation_plugin::getVideoInformation(const QString URL)
{
	// abstract function, implement this in sub-classes
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// empty result
	return result;
}

// Plugin for YouTube Videos

VideoInformation_Youtube::VideoInformation_Youtube(VideoInformation *videoInformation)
{
	setID("youtube.com");
	setCaption("Youtube");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Youtube::getVideoInformation(const QString URL)
{
	const QString URL_GET_FLV = "http://%1/get_video?video_id=%2&t=%3";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get the video ID and the video HASH
	QString vidID = QUrl(URL).queryItemValue("v");
	QString vidHash = copyBetween(html, ",t:'", "'");
	// get the video title
	result.title = copyBetween(html, "<title>YouTube - ", "</title>").trimmed();
	// build the video url
	result.URL = URL_GET_FLV.arg(QUrl(URL).host()).arg(vidID).arg(vidHash);
	// return the video information
	return result;
}

// Plugin for Metacafe Videos

VideoInformation_Metacafe::VideoInformation_Metacafe(VideoInformation *videoInformation)
{
	setID("metacafe.com");
	setCaption("Metacafe");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Metacafe::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get the video title
	result.title = copyBetween(html, "<title>", "</title>").trimmed();
	result.URL = copyBetween(html, "mediaURL=", "&").trimmed();
	// clean the video URL
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Google Videos

VideoInformation_Google::VideoInformation_Google(VideoInformation *videoInformation)
{
	setID("video.google");
	setCaption("Google Video");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Google::getVideoInformation(const QString URL)
{
	const QString URL_GET_SHEET = "http://video.google.com/videohosted?docid=%1";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the video ID
	QString vidID = QUrl(URL).queryItemValue("docid");
	// get the video-sheet
	Http http;
	QString html = http.downloadWebpage(QUrl(URL_GET_SHEET.arg(vidID)));
	// use YouTube to get this video url???
	if (html.indexOf("<title>YouTube") > -1)
	{
		VideoInformation_Youtube youtube(NULL);
		// get the youtube url
		QString youtubeURL = copyBetween(html, "<input name=\"video_link\"", ">");
		youtubeURL = copyBetween(youtubeURL, "value=\"", "\"");
		// get the video URL using YouTube
		result = youtube.getVideoInformation(youtubeURL);
	}
	else // google video
	{
		result.title = copyBetween(html, "<title>", "</title>");
		// delete the google video tag
		if (result.title.indexOf("- Google Video") > -1)
			result.title = copy(result.title, 0, result.title.indexOf("- Google Video"));
		// clear title
		result.title = result.title.trimmed();
		// get the video URL
		result.URL = copyBetween(html, "videoUrl", "messagesUrl");
		// get the separator token
		int sep = result.URL.indexOf("http");
		result.URL = result.URL.remove(0, sep);
		result.URL = copy(result.URL, 0, result.URL.length() - sep);
		// clean the video URL
		result.URL = cleanURL(result.URL);
	}
	// return the video information
	return result;
}

// Plugin for ZappInternet Videos

VideoInformation_ZappInternet::VideoInformation_ZappInternet(VideoInformation *videoInformation)
{
	setID("zappinternet.com");
	setCaption("ZappInternet");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_ZappInternet::getVideoInformation(const QString URL)
{
	const QString URL_GET_URL = "http://213.190.0.235/videos/%1/%2.flv";
	
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video info
	result.title = copyBetween(html, "<title>", " - ZappInternet.com</title>").trimmed();
	QString videoID = copyBetween(html, "var idvideo = \"", "\"");
	// clear and get the final flv url
	result.URL = cleanURL(QString(URL_GET_URL).arg(videoID[0]).arg(videoID));
	// return the video information
	return result;
}

// Plugin for Dailymotion Videos

VideoInformation_Dailymotion::VideoInformation_Dailymotion(VideoInformation *videoInformation)
{
	setID("dailymotion.com");
	setCaption("Dailymotion");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Dailymotion::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video info
	result.title = copyBetween(html, "class=\"nav with_uptitle\">", "<");
	result.URL = copyBetween(html, "\"url\", \"", "\");");
	// clear and get the final flv url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Dumpalink Videos

VideoInformation_Dumpalink::VideoInformation_Dumpalink(VideoInformation *videoInformation)
{
	setID("dumpalink.com");
	setCaption("Dumpalink");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Dumpalink::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video info
	result.title = copyBetween(html, "<title>", " - Dumpalink.com</title>").trimmed();
	// get the video info url
	QString infoURL = copyBetween(html, "autoStart=1&video=", "\"");
	infoURL = cleanURL(infoURL) + "&f=flash8&?";
	// get the video info
	html = http.downloadWebpage(infoURL);
	result.URL = copyBetween(html, "video_id=", "&url");
	// clear and get the final flv url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Glumbert Videos

VideoInformation_Glumbert::VideoInformation_Glumbert(VideoInformation *videoInformation)
{
	setID("glumbert.com");
	setCaption("Glumbert");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Glumbert::getVideoInformation(const QString URL)
{
	const QString URL_GET_FLV = "http://cdn.glumbert.com/video/flash/%1.flv";
	
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video info
	result.title = copyBetween(html, "var NAME = \"", "\"").trimmed();
	result.URL = copyBetween(html, "var URLNAME = \"", "\"");
	// clear and get the final url
	result.URL = cleanURL(QString(URL_GET_FLV).arg(result.URL));
	// return the video information
	return result;
}

// Plugin for Sclipo Videos

VideoInformation_Sclipo::VideoInformation_Sclipo(VideoInformation *videoInformation)
{
	setID("sclipo.com");
	setCaption("Sclipo");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Sclipo::getVideoInformation(const QString URL)
{
	const QString URL_GET_FLV = "http://www.sclipo.com/videos/%1.flv";
	
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video info
	result.title = copyBetween(html, "<title>Sclipo: ", "</title>").trimmed();
	// get the video url
	result.URL = copyBetween(html, "<link rel=\"videothumbnail\" href=\"", "\"");
	result.URL = copy(result.URL, result.URL.lastIndexOf("/") + 1, result.URL.length());
	result.URL = copy(result.URL, 0, 20);
	// clear and get the final url
	result.URL = cleanURL(QString(URL_GET_FLV).arg(result.URL)); // get only the first 20 chars (coz is the video ID)
	// return the video information
	return result;
}
