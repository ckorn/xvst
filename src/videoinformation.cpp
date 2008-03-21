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

#include "videoinformation.h"

VideoInformation::VideoInformation()
{
	setObjectName("VideoInformation");
	// plugins
	plugins = new QList<VideoInformation_plugin *>;
	// init data
	videoItem = NULL;
	blockAdultContent = false;
	// add plugins
	new VideoInformation_Youtube(this);
	new VideoInformation_Metacafe(this);
	new VideoInformation_Google(this);
	new VideoInformation_Yahoo(this);
	new VideoInformation_ZappInternet(this);
	new VideoInformation_Dailymotion(this);
	new VideoInformation_Dumpalink(this);
	new VideoInformation_Glumbert(this);
	new VideoInformation_Sclipo(this);
	new VideoInformation_LuluTV(this);
	new VideoInformation_LiveVideo(this);
	new VideoInformation_Yikers(this);
	new VideoInformation_123video(this);
	new VideoInformation_5min(this);
	new VideoInformation_aBum(this);
	new VideoInformation_Aniboom(this);
	new VideoInformation_Bebo(this);
	new VideoInformation_Break(this);
	new VideoInformation_Broadcaster(this);
	new VideoInformation_CaughtOnVideo(this);
	new VideoInformation_Clip4e(this);
	new VideoInformation_VideoCa(this);
	new VideoInformation_LiveLeak(this);
	new VideoInformation_TuTv(this);
	new VideoInformation_Spike(this);
	new VideoInformation_MySpaceTV(this);
	new VideoInformation_CinemaVIP(this);
	new VideoInformation_GameSpot(this);
	new VideoInformation_Stage6(this);
	new VideoInformation_Zuuble(this);
	new VideoInformation_ZippyVideos(this);
	new VideoInformation_Zedge(this);
	new VideoInformation_Blip(this);
	new VideoInformation_Ceknito(this);
	new VideoInformation_ZanyVideos(this);
	new VideoInformation_Zaable(this);
	new VideoInformation_YouTubeIslam(this);
	//new VideoInformation_IndiaVideoBomb(this);
	new VideoInformation_BoingboingTv(this);
	new VideoInformation_Gametrailers(this);
	new VideoInformation_Tudou(this);
	// adult sites
	new VideoInformation_Yuvutu(this);
	new VideoInformation_Badjojo(this);
	new VideoInformation_DesiSexTube(this);
	new VideoInformation_YouPorn(this);
	new VideoInformation_TuPorno(this);
	new VideoInformation_PornoTube(this);
	new VideoInformation_DaleAlPlay(this);
	new VideoInformation_Shufuni(this);
	new VideoInformation_XTube(this);
	new VideoInformation_YourFileHost(this);
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

			VideoDefinition info = service->getVideoInformation(videoItem->getURL());
			// canceled?
			if (videoItem == NULL) return;

			if (info.needLogin)
				videoItem->setAsNeedLogin(this);
			else
			{
				videoItem->setVideoInformation(info, this);
				videoItem->setVideoFile(videoItem->getVideoInformation().title + info.extension, this);

				videoItem->setAsGettedURL(this);
			}
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

void VideoInformation::cancel()
{
	videoItem = NULL;
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

QString VideoInformation::getHostImage(QString URL, bool checkURL)
{
	const QString path = ":/services/images/services/%1.png";

	bool valid = !checkURL ? true : validURL(URL);

	if (valid)//QUrl(URL).isValid())
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
	if (validURL(URL)) //QUrl(URL).isValid())
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
	const QString URL_YOUTBE  = "http://youtube.com/watch?v=%1";
	const QString URL_GET_FLV = "http://%1/get_video?video_id=%2&t=%3";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// default URL
	QString youTubeURL = URL;
	// check if is an embeded video, and get the "real url" of youtube
	if (URL.indexOf(".youtube.com/v/") != -1)
	{
		QString embededID = URL;
		embededID = embededID.remove(0, embededID.lastIndexOf("/v/") + 3);
		if (embededID.indexOf("&") != -1) embededID = copy(embededID, 0, embededID.indexOf("&"));
		youTubeURL = QString(URL_YOUTBE).arg(embededID);
	}
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(youTubeURL));
	// get the video ID and the video HASH
	QString vidID = QUrl(youTubeURL).queryItemValue("v");
	QString vidHash = copyBetween(html, "\"t\": \"", "\"");
	// get the video title
	result.title = copyBetween(html, "<title>YouTube - ", "</title>").trimmed();
	// build the video url
	result.URL = URL_GET_FLV.arg(QUrl(youTubeURL).host()).arg(vidID).arg(vidHash);
	// check if this video need a login
	result.needLogin = result.title == "Broadcast Yourself.";
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
	result.title = result.title.remove("- Metacafe").trimmed();
	// get the flv url
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

// Plugin for Yahoo Videos

VideoInformation_Yahoo::VideoInformation_Yahoo(VideoInformation *videoInformation)
{
	setID("video.yahoo.com");
	setCaption("Yahoo! Video");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Yahoo::getVideoInformation(const QString URL)
{
	const QString GET_SEQUENCE  = "http://cosmos.bcst.yahoo.com/ver/234/process/getSequence.php?&node_id=%1&tech=flv&eventID=0&chid=0&s=&output=%2&bw=101&ptr=playerAPI&callback=y_up.playlist._receive";
	const QString GET_PLAY_LIST = "http://playlist.yahoo.com/makeplaylist.dll?%1&pt=xml&pt=xml";
	const QString GET_FLV_URL   = "http://%1%2?%3";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video info
	result.title = copyBetween(html, "<title>", "- Yahoo!").trimmed();
	// get the xml url
	QString videoId = copyBetween(html, "videoid=", "\">").trimmed();
	videoId = cleanURL(videoId);
	// get sequence ID
	QString sequenceID = copyBetween(html, "c id='", "'");
	QString f = copyBetween(html, "&f=", "&");
	// download sequence
	html = http.downloadWebpage(QUrl(QString(GET_SEQUENCE).arg(sequenceID).arg(f)));
	// get the play list parameters
	QString playListParams = copyBetween(html, "makeplaylist.dll?", "\"}");
	// download the play list
	QString xml = http.downloadWebpage(QUrl(QString(GET_PLAY_LIST).arg(playListParams)));
	// get the flv info
	QString server = copyBetween(xml, "SERVER=\"", "\"");
	QString path = copyBetween(xml, "PATH=\"", "\"");
	QString queryString = copyBetween(xml, "QUERYSTRING=\"", "\"");
	// clear and get the final flv url
	result.URL = cleanURL(QString(GET_FLV_URL).arg(server).arg(path).arg(queryString));
	// convert html to string
	result.URL = result.URL.replace("&amp;", "&");
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
	result.title = copyBetween(html, "<title>", "- ZappInternet.com</title>").trimmed();
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
	const QString URL_GET_URL = "http://www.dailymotion.com%1";
	
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "class=\"nav with_uptitle\">", "<");
	// get flv url
	result.URL = cleanURL(copyBetween(html, ".addVariable(\"video\", \"", "\");"));
	result.URL = getToken(result.URL, "||", 0);
	result.URL = getToken(result.URL, "@@", 0);
	result.URL = QString(URL_GET_URL).arg(result.URL);
	// clear and get the final flv url
	result.URL = cleanURL(result.URL);
	// check if need login
	result.needLogin = html.indexOf("addVariable(\"start\"") != -1;
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
	result.title = copyBetween(html, "<title>", "- video - Sclipo</title>").trimmed();
	// get the video url
	result.URL = copyBetween(html, "<link rel=\"videothumbnail\" href=\"", "\"");
	result.URL = copy(result.URL, result.URL.lastIndexOf("/") + 1, result.URL.length());
	result.URL = copy(result.URL, 0, 20);
	// clear and get the final url
	result.URL = cleanURL(QString(URL_GET_FLV).arg(result.URL)); // get only the first 20 chars (coz is the video ID)
	// return the video information
	return result;
}

// Plugin for Lulu TV Videos

VideoInformation_LuluTV::VideoInformation_LuluTV(VideoInformation *videoInformation)
{
	setID("lulu.tv");
	setCaption("Lulu TV");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_LuluTV::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video info
	result.title = copyBetween(html, "<title>luluTV   &raquo;", "</title>").trimmed();
	// get the video url
	result.URL = copyBetween(html, "swf?file=", "&");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for LiveVideo Videos

VideoInformation_LiveVideo::VideoInformation_LiveVideo(VideoInformation *videoInformation)
{
	setID("livevideo.com");
	setCaption("LiveVideo");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_LiveVideo::getVideoInformation(const QString URL)
{
	const QString URL_GET_XML = "http://www.livevideo.com/media/getflashvideo.ashx?cid=%1&path=%2&mid=%3&t=/image/%4&et=%5&f=flash8&?";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video info
	result.title = copyBetween(html, "<title>", "</title>").trimmed();
	// chechk title
	if (result.title.indexOf(" - LiveVideo.com") != -1) // basic video
		result.title = copy(result.title, 0, result.title.indexOf("- LiveVideo.com")).trimmed();
	else // uservideo
		result.title = copy(result.title, 0, result.title.indexOf("- Channel")).trimmed();
	// get video ID (from the URL)
	QString videoID = getToken(URL, "/", getTokenCount(URL, "/") - 2);
	// ge T parameter
	QString T = copyBetween(html, "rel=\"videothumbnail\"", "/>");
	T = copyBetween(T, "livevideo/image/", "\"");
	// get path parameter
	QString path = getToken(T, "/", 0) + "/" + getToken(T, "/", 1);
	// get mid parameter
	QString mid = getToken(T, "/", 2);
	mid = copy(mid, 0, mid.indexOf("_"));
	// get et parameter
	QString et = copyBetween(html, "et%3d", "'");
	// download the xml file
	QString xml = http.downloadWebpage(QString(URL_GET_XML).arg(videoID).arg(path).arg(mid).arg(T).arg(et));
	// get the FLV url
	result.URL = copyBetween(xml, "video_id=", "&");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Yikers Videos

VideoInformation_Yikers::VideoInformation_Yikers(VideoInformation *videoInformation)
{
	setID("yikers.com");
	setCaption("Yikers");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Yikers::getVideoInformation(const QString URL)
{
	const QString URL_GET_XML = "http://www.yikers.com/%1&f=flash8&";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<title>", "- Yikers.com</title>").trimmed();
	// get the XML url
	QString xmlURL = copyBetween(html, "video=", "\"");
	// clear XML url
	xmlURL = cleanURL(QString(URL_GET_XML).arg(xmlURL));
	// download the xml file
	QString xml = http.downloadWebpage(xmlURL);
	// get the FLV url
	result.URL = copyBetween(xml, "video_id=", "&");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for 123video Videos

VideoInformation_123video::VideoInformation_123video(VideoInformation *videoInformation)
{
	setID("123video.nl");
	setCaption("123video");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_123video::getVideoInformation(const QString URL)
{
	const QString URL_GET_XML = "http://www.123video.nl/initialize_player_v3.asp";
	const QString XML_PARAMS  = "<movie><id>%1</id></movie>";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL), false);
	// get video title
	result.title = copyBetween(html, "<title>", "- 123video</title>").trimmed();
	// get the video ID
	QString videoID = copyBetween(html, "mediaSrc=", "&");
	// get the video xml
	QString xml = http.downloadWebpagePost(QUrl(URL_GET_XML), QString(XML_PARAMS).arg(videoID));
	// get the host IP
	QString hostIP = copyBetween(xml, "MediaIP=\"", "\"");
	// get the FLV url
	result.URL = QString("http://%1/%2/%3.flv").arg(hostIP).arg(copy(videoID, 0, 2)).arg(videoID);
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for 5min Videos

VideoInformation_5min::VideoInformation_5min(VideoInformation *videoInformation)
{
	setID("5min.com");
	setCaption("5min");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_5min::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<title>", "</title>").trimmed();
	result.title = copy(result.title, 0, result.title.indexOf("- Video")).trimmed();
	result.title = result.title.remove("5min - ").trimmed();
	// get the flv url
	result.URL = copyBetween(html, "vidURL', '", "'");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for aBum Videos

VideoInformation_aBum::VideoInformation_aBum(VideoInformation *videoInformation)
{
	setID("abum.com");
	setCaption("aBum");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_aBum::getVideoInformation(const QString URL)
{
	const QString URL_GET_FLV = "http://media.abum.com/video/%1.flv";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<title>", "</title>").trimmed();
	result.title = copy(result.title, 0, result.title.indexOf("- Funny Videos")).trimmed();
	// get the video ID
	QString videoID = copyBetween(URL, "www.abum.com/", "/");
	// clear and get the final url
	result.URL = cleanURL(QString(URL_GET_FLV).arg(videoID));
	// return the video information
	return result;
}

// Plugin for Aniboom Videos

VideoInformation_Aniboom::VideoInformation_Aniboom(VideoInformation *videoInformation)
{
	setID("aniboom.com");
	setCaption("Aniboom");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Aniboom::getVideoInformation(const QString URL)
{
	const QString URL_GET_FLV = "http://video.aniboom.com/http/%1.flv";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<title", "</title>").trimmed();
	result.title = copyBetween(result.title, "aniBoom -", "at Aniboom").trimmed();
	// get the video ID
	QUrl UrlInf(URL);
	QString videoID = UrlInf.queryItemValue("v");
	// clear and get the final url
	result.URL = cleanURL(QString(URL_GET_FLV).arg(videoID));
	// return the video information
	return result;
}

// Plugin for Bebo Videos

VideoInformation_Bebo::VideoInformation_Bebo(VideoInformation *videoInformation)
{
	setID("bebo.com");
	setCaption("Bebo");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Bebo::getVideoInformation(const QString URL)
{
	const QString FLV_FILE = "%1_high.flv?showAd=false";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<title>", "</title>").trimmed();
	// get the video pre-URL
	QString preURL = copyBetween(html, "file=", "&").trimmed();
	// clear and get the final url
	result.URL = cleanURL(QString(FLV_FILE).arg(preURL));
	// return the video information
	return result;
}

// Plugin for Break Videos

VideoInformation_Break::VideoInformation_Break(VideoInformation *videoInformation)
{
	setID("break.com");
	setCaption("Break");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Break::getVideoInformation(const QString URL)
{
	const QString URL_GET_FLV = "http://media1.break.com/dnet/media/%1/%2.flv";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<title>", "</title>").trimmed();
	// get variable values
	QString sGlobalContentFilePath = copyBetween(html, "sGlobalContentFilePath='", "'");
	QString sGlobalFileName = copyBetween(html, "sGlobalFileName='", "'");
	// clear and get the final url
	result.URL = cleanURL(QString(URL_GET_FLV).arg(sGlobalContentFilePath).arg(sGlobalFileName));
	// return the video information
	return result;
}

// Plugin for Broadcaster Videos

VideoInformation_Broadcaster::VideoInformation_Broadcaster(VideoInformation *videoInformation)
{
	setID("broadcaster.com");
	setCaption("Broadcaster");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Broadcaster::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "&page_title=", "\"").trimmed();
	// get variable values
	QString videoURL = copyBetween(html, "\"&clip_loc=\"", ";");
	QString videoFile = copyBetween(videoURL, "(", ")");
	videoFile = copyBetween(videoFile, "\"", "\"");
	videoURL = copyBetween(videoURL, "\"", "\"");
	// clear and get the final url
	result.URL = cleanURL(videoURL + videoFile);
	// return the video information
	return result;
}

// Plugin for CaughtOnVideo Videos

VideoInformation_CaughtOnVideo::VideoInformation_CaughtOnVideo(VideoInformation *videoInformation)
{
	setID("videos.caught-on-video.com");
	setCaption("CaughtOnVideo");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_CaughtOnVideo::getVideoInformation(const QString URL)
{
	const QString URL_GEL_XML = "http://videos.caught-on-video.com/playlistbuilder/buildflash.ashx?%1";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<title>Video - ", "</title>").trimmed();
	result.title = result.title.remove("Caught on Video");
	// get Flash Vars
	QString flashVars = copyBetween(html, "player.swf\" FlashVars=\"", "\"");
	// get the xml file
	QString xml = http.downloadWebpage(QUrl(QString(URL_GEL_XML).arg(flashVars)));
	if (!xml.isEmpty())
	{
		// get the flv url
		result.URL = copyBetween(xml, "<file>", "</file>");
		result.URL = copyBetween(result.URL, "CDATA[", "]");
		// clear and get the final url
		result.URL = cleanURL(result.URL);
	}
	// return the video information
	return result;
}

// Plugin for CaughtOnVideo Videos

VideoInformation_Clip4e::VideoInformation_Clip4e(VideoInformation *videoInformation)
{
	setID("clip4e.com");
	setCaption("Clip4e");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Clip4e::getVideoInformation(const QString URL)
{
	const QString URL_GEL_XML = "http://clip4e.com/getflv.php?id=%1";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// check if we are into "adult confirmation"
	if (html.indexOf("adult_confirm.php") != -1)
	{
		// get the ID and confirm ;)
		QString id = copyBetween(html, "adult_confirm.php?act=confirm&id=", "'");
		html = http.downloadWebpage(QUrl(QString("http://clip4e.com/adult_confirm.php?act=confirm&id=%1").arg(id)));
	}
	// get video title
	result.title = copyBetween(html, "<TITLE>", "</TITLE>").trimmed();
	// get the video ID
	QString videoID = copyBetween(html, "\"cl_id\", \"", "\"");
	// get the xml
	QString xml = http.downloadWebpage(QUrl(QString(URL_GEL_XML).arg(videoID)));
	// get flv url
	result.URL = copyBetween(xml, "<url>", "</url>");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for VideoCa Videos

VideoInformation_VideoCa::VideoInformation_VideoCa(VideoInformation *videoInformation)
{
	setID("video.ca");
	setCaption("VIDEO.CA");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_VideoCa::getVideoInformation(const QString URL)
{
	const QString URL_GET_FLV = "http://www.video.ca/media/video/%1.flv";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<title>VIDEO.CA -", "</title>").trimmed();
	// get the video id
	QUrl UrlInf(URL);
	QString videoID = UrlInf.queryItemValue("id");
	// clear and get the final url
	result.URL = cleanURL(QString(URL_GET_FLV).arg(videoID));
	// return the video information
	return result;
}

// Plugin for VideoLeak Videos

VideoInformation_LiveLeak::VideoInformation_LiveLeak(VideoInformation *videoInformation)
{
	setID("liveleak.com");
	setCaption("LiveLeak");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_LiveLeak::getVideoInformation(const QString URL)
{
	const QString URL_GET_XML = "http://www.liveleak.com/play_flash_xml.php?token=%1&autoplay=true";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpagePost(QUrl(URL), "adult_confirmed=yes");
	// get video title
	result.title = copyBetween(html, "<title>LiveLeak.com -", "</title>").trimmed();
	// get the video parameters
	QString token = QUrl(URL).queryItemValue("i");
	// download the video info
	QString xml = http.downloadWebpage(QUrl(QString(URL_GET_XML).arg(token)));
	// get the flv url
	result.URL = copyBetween(xml, "<flv8><![CDATA[", "]");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for VideoCa Videos

VideoInformation_TuTv::VideoInformation_TuTv(VideoInformation *videoInformation)
{
	setID("tu.tv");
	setCaption("Tu.tv");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_TuTv::getVideoInformation(const QString URL)
{
	const QString URL_CONFIRM = "http://www.tu.tv%1";
	const QString URL_GET_XML = "http://tu.tv/visualizacionExterna2.php?web=&codVideo=%1";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL), false);
	// +18 without login?
	if (html.indexOf(">Si<") != -1)
	{
		QString confirm_url = copyBetween(html, "<h5 class=\"txt_alerta\"><a href=\"", "\">Si");
		html = http.downloadWebpage(QUrl(QString(URL_CONFIRM).arg(confirm_url)), false);
	}
	// +18 and need login?
	else if (html.indexOf("<title>Tu.tv - Advertencia de contenido inapropiado</title>") != -1)
	{
		// ohms, need login
		result.needLogin = true;
		// return the video information
		return result;
	}
	// Get the video title
	result.title = copyBetween(html, "<title>Tu.tv - Videos -", "</title>").trimmed();
	// get the video code
	QString videoCode = copyBetween(html, "?codVideo=", "\"");
	// get the video info
	QString xml = http.downloadWebpage(QUrl(QString(URL_GET_XML).arg(videoCode)));
	// get the flv url
	result.URL = copyBetween(xml, "urlVideo0=", "&");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Spike Videos

VideoInformation_Spike::VideoInformation_Spike(VideoInformation *videoInformation)
{
	setID("spike.com");
	setCaption("Spike");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Spike::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<title>", "-").trimmed();
	// get the video code
	result.URL = copyBetween(html, "*url* : *http://", "*");
	// replace the "$" for "&"
	result.URL = result.URL.replace("$", "&");
	// clear and get the final url
	result.URL = cleanURL("http://" + result.URL);
	// return the video information
	return result;
}

// Plugin for MySpaceTV Videos

VideoInformation_MySpaceTV::VideoInformation_MySpaceTV(VideoInformation *videoInformation)
{
	setID("myspace.com");
	setCaption("MySpaceTV");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_MySpaceTV::getVideoInformation(const QString URL)
{
	const QString URL_GET_XML = "http://mediaservices.myspace.com/services/rss.ashx?type=video&videoID=%1";
	
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<h1>", "</h1>").trimmed();
	// get the video code
	QString VideoID = QUrl(URL.toLower()).queryItemValue("videoid");
	// get the xml file
	QString xml = http.downloadWebpage(QUrl(QString(URL_GET_XML).arg(VideoID)));
	// get the video flv
	result.URL = copyBetween(xml, "content url=\"", "\"").trimmed();
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for CinemaVIP Videos

VideoInformation_CinemaVIP::VideoInformation_CinemaVIP(VideoInformation *videoInformation)
{
	setID("videos.cinemavip.com");
	setCaption("CinemaVIP");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_CinemaVIP::getVideoInformation(const QString URL)
{
	const QString URL_GET_XML = "http://films.cinemavip.com/contenidos/cinemavip/%1";
	
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<title>", "-").trimmed();
	// get the flv pre-path
	QString videPath = copyBetween(html, "file=", "&");
	// get the video flv
	result.URL = QString(URL_GET_XML).arg(videPath);
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for GameSpot Videos

VideoInformation_GameSpot::VideoInformation_GameSpot(VideoInformation *videoInformation)
{
	setID("gamespot.com");
	setCaption("GameSpot");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_GameSpot::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<title>GameSpot Video:", "</title>").trimmed();
	// get the xml file
	QString xmlUrl = cleanURL(copyBetween(html, "'paramsURI', '", "'"));
	QString xml = http.downloadWebpage(QUrl(xmlUrl));
	// get the flv url
	result.URL = copyBetween(xml, "<URI>", "</URI>");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Stage6 Videos

VideoInformation_Stage6::VideoInformation_Stage6(VideoInformation *videoInformation)
{
	setID("stage6.com");
	setCaption("Stage6");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Stage6::getVideoInformation(const QString URL)
{
	const QString GET_URL_FLV = "http://%1";
	
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	result.extension = ".divx";
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<title>", "</title>").trimmed();
	result.title = htmlToStr(result.title);
	result.title = copyBetween(result.title, "Stage6 ·", " - Video and Download");
	// get the flv url
	result.URL = copyBetween(html, "value=&quot;http://", "&quot;");
	// clear and get the final url
	result.URL = cleanURL(QString(GET_URL_FLV).arg(result.URL));
	// return the video information
	return result;
}

// Plugin for Zuuble Videos

VideoInformation_Zuuble::VideoInformation_Zuuble(VideoInformation *videoInformation)
{
	setID("zuuble.com");
	setCaption("Zuuble");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Zuuble::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<title>", "|").trimmed();
	// get the flv url
	result.URL = copyBetween(html, "&flvFileName=", "&");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for ZippyVideos Videos

VideoInformation_ZippyVideos::VideoInformation_ZippyVideos(VideoInformation *videoInformation)
{
	setID("zippyvideos.com");
	setCaption("ZippyVideos");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_ZippyVideos::getVideoInformation(const QString URL)
{
	const QString GET_URL_FLV = "%1/%2/%3/%4";
	
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<title>", "-").trimmed();
	// check if is a windows media video or a FLV
	if (html.indexOf("application/x-mplayer2") != -1) // is a WMV file
	{
		// get the wmv url
		result.URL = copyBetween(html, "<embed type=\"application/x-mplayer2\"", "</embed>");
		result.URL = copyBetween(result.URL, "src=\"", "\"");
		// set the video extension
		result.extension = ".wmv";
	}
	else
	{
		// build the flv url
		QString storageId = copyBetween(html, "storageId=", "&");
		QString userName = copyBetween(html, "userName=", "&");
		QString videoId = copyBetween(html, "&videoId=", "&");
		QString srvURL = copyBetween(html, "srvURL=", "'");
		// get the final flv url
		result.URL = QString(GET_URL_FLV).arg(srvURL).arg(storageId).arg(userName).arg(videoId);
	}
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Zedge Videos

VideoInformation_Zedge::VideoInformation_Zedge(VideoInformation *videoInformation)
{
	setID("zedge.net");
	setCaption("Zedge");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Zedge::getVideoInformation(const QString URL)
{
	const QString GET_URL_XML = "http://www.zedge.net/zedge-api/api-video-player.php";
	const QString XML_PARAMS = "zapi_session_id=%1&video_id=%2&get_video=";
	
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get the video id
	QString zapId = copyBetween(html, "sessionid=", "&");
	QString vidID = copyBetween(html, "vid=", "&");
	// get xml file
	QString xml = http.downloadWebpagePost(QUrl(GET_URL_XML), QString(XML_PARAMS).arg(zapId).arg(vidID));
	// get the video title
	result.title = copyBetween(xml, "vtitle=", "&").trimmed();
	// get the flv url
	result.URL = copyBetween(xml, "vflv=", "&");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Blip Videos

VideoInformation_Blip::VideoInformation_Blip(VideoInformation *videoInformation)
{
	setID("blip.tv");
	setCaption("Blip");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Blip::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<title>", "</title>").trimmed();
	// get the flv url
	result.URL = copyBetween(html, "setPrimaryMediaUrl(\"", "?");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Ceknito Videos

VideoInformation_Ceknito::VideoInformation_Ceknito(VideoInformation *videoInformation)
{
	setID("ceknito.sk");
	setCaption("Ceknito");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Ceknito::getVideoInformation(const QString URL)
{
	const QString GET_URL_FLV = "http://ceknito.sk/d/vf/%1.flv";
	
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<title>", "-").trimmed();
	// get the flv url
	QString fid = copyBetween(html, "fid\", \"", "\"");
	// clear and get the final url
	result.URL = cleanURL(QString(GET_URL_FLV).arg(fid));
	// return the video information
	return result;
}

// Plugin for ZanyVideos Videos

VideoInformation_ZanyVideos::VideoInformation_ZanyVideos(VideoInformation *videoInformation)
{
	setID("zanyvideos.com");
	setCaption("ZanyVideos");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_ZanyVideos::getVideoInformation(const QString URL)
{
	const QString GET_URL_FLV = "http://www.zanyvideos.com%1";
	
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<title>", "</title>").trimmed();
	// get the flv url
	QString path = copyBetween(html, "\"file\",\"", "\"");
	// clear and get the final url
	result.URL = cleanURL(QString(GET_URL_FLV).arg(path));
	// return the video information
	return result;
}

// Plugin for Zaable Videos

VideoInformation_Zaable::VideoInformation_Zaable(VideoInformation *videoInformation)
{
	setID("zaable.com");
	setCaption("Zaable");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Zaable::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<title>", "-").trimmed();
	// get the flv url
	result.URL = copyBetween(html, "flv_filename=", "&");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for YouTubeIslam Videos

VideoInformation_YouTubeIslam::VideoInformation_YouTubeIslam(VideoInformation *videoInformation)
{
	setID("youtubeislam.com");
	setCaption("YouTubeIslam");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_YouTubeIslam::getVideoInformation(const QString URL)
{
	const QString GET_URL_FLV = "http://www.youtubeislam.com/flvideo/%1";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// download webpage
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// Get the video title
	result.title = copyBetween(html, "<li id=\"main\"", "<h3>").trimmed();
	result.title = copyBetween(result.title, "<h2>", "</h2>");
	// get the flv url
	QString id = copyBetween(html, "video=", "&");
	// clear and get the final url
	result.URL = cleanURL(QString(GET_URL_FLV).arg(id));
	// return the video information
	return result;
}

// Plugin for YourFileHost Videos

VideoInformation_YourFileHost::VideoInformation_YourFileHost(VideoInformation *videoInformation)
{
	setID("yourfilehost.com");
	setCaption("YourFileHost");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_YourFileHost::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the html
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "Description:", "<br>").trimmed();
	// get flv url
	result.URL = copyBetween(html, "videoembed_id=", "&");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for IndiaVideo Bomb videos

VideoInformation_IndiaVideoBomb::VideoInformation_IndiaVideoBomb(VideoInformation *videoInformation)
{
	setID("indavideo.hu");
	setCaption("IndiaVideo Bomb");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_IndiaVideoBomb::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the html
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<title>", "</title>").trimmed();
	// get the video ID
	QString vID = copyBetween(html, "vID=", "\"").trimmed();
	// get the video info
	//QString flvData = http.downloadWebpagePost(QUrl(URL), vID);
	// get the flv url
	//qDebug() << flvData;
	// clear and get the final url
	//result.URL = cleanURL(result.URL);
	// return the video information
	return result;

	//http://www.indavideo.hu/video/Cipotisztitas
}

// Plugin for Boingboing TV Videos

VideoInformation_BoingboingTv::VideoInformation_BoingboingTv(VideoInformation *videoInformation)
{
	setID("tv.boingboing.net");
	setCaption("Boingboing TV");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_BoingboingTv::getVideoInformation(const QString URL)
{
	const QString FLV_URL = "http://video.boingboing.net%1";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the html
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<title>", "</title>").trimmed();
	result.title = result.title.remove("- Boing Boing TV").trimmed();
	// get flv url
	result.URL = copyBetween(html, "<embed class=", "</embed>");
	result.URL = copyBetween(result.URL, "src='", "'");
	result.URL = result.URL.remove(0, result.URL.indexOf("/video/"));
	// clear and get the final url
	result.URL = cleanURL(QString(FLV_URL).arg(result.URL));
	// return the video information
	return result;
}

// Plugin for Gametrailers Videos

VideoInformation_Gametrailers::VideoInformation_Gametrailers(VideoInformation *videoInformation)
{
	setID("gametrailers.com");
	setCaption("GameTrailers");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Gametrailers::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	result.extension = ".wmv";
	// get the html
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<title>Gametrailers.com -", "</title>").trimmed();
	// get flv url
	result.URL = copyBetween(html, ".mov", ">Windows Media");
	result.URL = copyBetween(result.URL, "href=\"", "\""); 
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Tudou Videos

VideoInformation_Tudou::VideoInformation_Tudou(VideoInformation *videoInformation)
{
	setID("tudou.com");
	setCaption("Tudou");
	adultContent = false;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Tudou::getVideoInformation(const QString URL)
{
	const QString GET_PLAY_LIST = "http://www.tudou.com/player/v.php?id=%1";
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// create a http instance
	Http http;
	QString id;
	// get the ID (play list method)
	if (URL.indexOf("/playlist/") != -1)
		id = QUrl(URL).queryItemValue("iid");
	// get the ID (simple watch)
	else 
	{
		// get the html
		QString html = http.downloadWebpage(QUrl(URL));
		// get the id
		id = copyBetween(html, "var iid=", ";");
	}
	// get the video info
	QString playList = http.downloadWebpage(QString(GET_PLAY_LIST).arg(id));
	// get video title
	result.title = copyBetween(playList, "q='", "'");	
	// get the flv url
	result.URL = copyBetween(playList, "<f w='10'>", "</f>");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Adult websites

// Plugin for Yuvutu Videos

VideoInformation_Yuvutu::VideoInformation_Yuvutu(VideoInformation *videoInformation)
{
	setID("yuvutu.com");
	setCaption("yuvutu");
	adultContent = true;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Yuvutu::getVideoInformation(const QString URL)
{
	const QString URL_GET_HTML = "http://yuvutu.com/modules.php?name=Video&op=view&video_id=%1&name=Video&proceed=yes";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the video id
	QUrl UrlInf(URL);
	QString videoID = UrlInf.queryItemValue("video_id");
	// get the html
	Http http;
	QString html = http.downloadWebpage(QUrl(QString(URL_GET_HTML).arg(videoID)));
	// get video title
	result.title = copyBetween(html, "class=\"videoTitle\"", "<td");
	result.title = copyBetween(result.title, "</span>", "</td>").trimmed();
	// get the flv url
	result.URL = copyBetween(html, "longTailPlayer", "/>");
	result.URL = copyBetween(result.URL, "flashvars=\"", "\"");
	result.URL = copyBetween(result.URL, "file=", "&");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Badjojo Videos

VideoInformation_Badjojo::VideoInformation_Badjojo(VideoInformation *videoInformation)
{
	setID("badjojo.com");
	setCaption("BadJojo");
	adultContent = true;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Badjojo::getVideoInformation(const QString URL)
{
	const QString URL_GET_XML = "http://www.badjojo.com/videoConfigXmlIndraCode.php?vId=%1";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the video id
	QUrl UrlInf(URL);
	QString videoID = UrlInf.queryItemValue("Id");
	// get the xml
	Http http;
	QString xml = http.downloadWebpage(QUrl(QString(URL_GET_XML).arg(videoID)));
	// get the title
	result.title = copyBetween(xml, "Name=\"Header\" Value=\"", "\"").trimmed();
	// get flv url
	result.URL = copyBetween(xml, "Name=\"FLVPath\" Value=\"", "\"/>");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for DesiSexTube Videos

VideoInformation_DesiSexTube::VideoInformation_DesiSexTube(VideoInformation *videoInformation)
{
	setID("desisextube.com");
	setCaption("DesiSexTube");
	adultContent = true;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_DesiSexTube::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the html
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get the title
	result.title = copyBetween(html, "<DIV class=moduleFrameTitle>", "</DIV>");
	result.title = copyBetween(result.title, "parent>", "</A>").trimmed();
	// get the flv url
	result.URL = copyBetween(html, "flvplayer.swf?file=", "&");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for YouPorn Videos

VideoInformation_YouPorn::VideoInformation_YouPorn(VideoInformation *videoInformation)
{
	setID("youporn.com");
	setCaption("YouPorn");
	adultContent = true;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_YouPorn::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the html
	Http http;
	QString html = http.downloadWebpage(QUrl(URL + "/?user_choice=Enter"));
	// get video title
	result.title = copyBetween(html, "<title>", "</title>");
	result.title = result.title.remove(0, result.title.indexOf("-") + 1).trimmed();
	// get the flv url
	result.URL = copyBetween(html, "http://download.youporn.com/download/", ".flv");
	result.URL = QString("http://download.youporn.com/download/%1.flv").arg(result.URL);
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for TuPorno Videos

VideoInformation_TuPorno::VideoInformation_TuPorno(VideoInformation *videoInformation)
{
	setID("tuporno.tv");
	setCaption("TuPorno TV");
	adultContent = true;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_TuPorno::getVideoInformation(const QString URL)
{
	//const QString URL_GET_FLV = "http://vc1n.tuporno.tv/videoscodi/%1/%2/%3.flv";
	const QString URL_GET_FLV = "http://130.117.82.221/videoscodi/%1/%2/%3.flv";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the html
	Http http;
	QString html = http.downloadWebpage(QUrl(URL), false);
	// get video title
	result.title = copyBetween(html, "<title>tuporno.tv - Videos -", "</title>").trimmed();
	// get the video name
	QString videoName = copy(URL, URL.lastIndexOf("/") + 1, URL.length());
	result.URL = QString(URL_GET_FLV).arg(videoName.at(0)).arg(videoName.at(1)).arg(videoName);
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for PornoTube Videos

VideoInformation_PornoTube::VideoInformation_PornoTube(VideoInformation *videoInformation)
{
	setID("pornotube.com");
	setCaption("PornoTube");
	adultContent = true;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_PornoTube::getVideoInformation(const QString URL)
{
	const QString AGE_VALIDATION = "bMonth=01&bDay=1&bYear=1970&verifyAge=true&submit=CONTINUE &raquo;";
	const QString GET_FLV_INFO_URL = "http://www.pornotube.com/player/player.php?%1";
	const QString GET_FLV_URL = "http://%1.pornotube.com/%2/%3.flv";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// emulate the "validation process"
	Http http;
	QString html = http.downloadWebpagePost(QUrl(URL), AGE_VALIDATION);
	// get video title
	result.title = copyBetween(html, "<span class=\"blue\">", "<").trimmed();
	// get the video ID
	QString videoID = copyBetween(html, "player/v.swf?v=", "\"");
	// get the video Information
	html = http.downloadWebpage(QUrl(QString(GET_FLV_INFO_URL).arg(videoID)));
	// get the video ID
	QString flvId = copyBetween(html, "mediaId=", "&");
	// get the User ID
	QString userId = copyBetween(html, "userId=", "&");
	// get domain
	QString domain = copyBetween(html, "mediaDomain=", "&");
	// build url
	result.URL = QString(GET_FLV_URL).arg(domain).arg(userId).arg(flvId);
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for DaleAlPlay Videos

VideoInformation_DaleAlPlay::VideoInformation_DaleAlPlay(VideoInformation *videoInformation)
{
	setID("dalealplay.com");
	setCaption("DaleAlPlay");
	adultContent = true;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_DaleAlPlay::getVideoInformation(const QString URL)
{
	const QString URL_GET_FLV = "http://videos.dalealplay.com/contenidos/%1";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the html
	Http http;
	QString html = http.downloadWebpage(QUrl(URL), false);
	// get video title
	result.title = copyBetween(html, "<title>", "- www.dalealplay.com</title>").trimmed();
	// get the video path
	QString videoPath = copyBetween(html, "file=", "&");
	// build url
	result.URL = QString(URL_GET_FLV).arg(videoPath);
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for Shufuni Videos

VideoInformation_Shufuni::VideoInformation_Shufuni(VideoInformation *videoInformation)
{
	setID("shufuni.com");
	setCaption("Shufuni");
	adultContent = true;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_Shufuni::getVideoInformation(const QString URL)
{
	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the html
	Http http;
	QString html = http.downloadWebpage(QUrl(URL), false);
	// get video title
	result.title = copyBetween(html, "<title>", "- Shufuni.com").trimmed();
	// get video url
	result.URL = copyBetween(html, "file\", \"", "\"");
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}

// Plugin for XTube Videos

VideoInformation_XTube::VideoInformation_XTube(VideoInformation *videoInformation)
{
	setID("xtube.com");
	setCaption("XTube");
	adultContent = true;
	registPlugin(videoInformation);
}

VideoDefinition VideoInformation_XTube::getVideoInformation(const QString URL)
{
	QString URL_GET_XML = "http://video.xtube.com/find_video.php";

	// init result
	VideoDefinition result;
	VideoItem::initVideoDefinition(result);
	// get the html
	Http http;
	QString html = http.downloadWebpage(QUrl(URL));
	// get video title
	result.title = copyBetween(html, "<!-- Title Bar -->", "</div>").trimmed();
	result.title = copyBetween(result.title, "<h5>", "</h5>").trimmed();
	// get video xml
	QString flv_url = copyBetween(html, "FlashObject(\"", ".xtube.com/");
	QString xml_params = copyBetween(html, ".swf?", "\"");
	// get the xml
	QString xml = http.downloadWebpagePost(QUrl(URL_GET_XML), xml_params);
	QString flv_path = copyBetween(xml, "&filename=", "&");
	// build the final flv url
	result.URL = QString("%1.xtube.com%2").arg(flv_url).arg(flv_path);
	// clear and get the final url
	result.URL = cleanURL(result.URL);
	// return the video information
	return result;
}
