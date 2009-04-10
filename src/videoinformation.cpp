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

Q_DECLARE_METATYPE(VideoDefinition)

VideoInformation::VideoInformation(QString pluginsDir)
{
	setObjectName("VideoInformation");
	// plugins
	plugins = new QList<VideoInformationPlugin *>;
	loadPlugins(pluginsDir);
	// init data
	videoItem = NULL;
	blockAdultContent = false;
	// update lastThis reference
	lastVideoInformationInstance = this;
}

VideoInformation::~VideoInformation()
{
	// abort any current plugin execution
	abortExecution();
	// wait until thread end
//	while (isRunning()) { /* do nothing, just wait... */ };
	if (isRunning())
		quit();
	// remove loaded plugins
	clearPlugins();
	// destroy plugins container
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

VideoInformationPlugin *VideoInformation::getPluginByHost(QUrl URL)
{
	QString host = URL.host().isEmpty() ? URL.toString() : URL.host();
	for (int n = 0; n < getPluginsCount(); n++)
		if (plugins->at(n)->isLikeMyId(host))
			return plugins->at(n);
	// not found
	return NULL;
}

void VideoInformation::registerPlugin(VideoInformationPlugin *videoInformationPlugin)
{
	plugins->push_back(videoInformationPlugin);
}

void VideoInformation::run()
{
	videoItem->lock (this);

	VideoInformationPlugin *service = getPluginByHost(QUrl(videoItem->getURL()));

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
				videoItem->setVideoFile(cleanFileName(videoItem->getVideoInformation().title + info.extension), this);

				videoItem->setAsGettedURL(this);
			}
		}
	}
	else
		videoItem->setAsError(this);

	videoItem->unlock(this);
	emit informationFinished(videoItem);
}

void VideoInformation::loadPlugins(QString pluginsDir)
{
	QStringList scriptFiles;
	QDir scriptsDir(pluginsDir);

	scriptFiles = scriptsDir.entryList(QDir::Files, QDir::Name);
	scriptFiles = scriptFiles.filter(".js");

	foreach (QString scriptFile, scriptFiles)
		if (scriptFile.endsWith(".js"))
			loadPluginFile(QDir::toNativeSeparators(pluginsDir + "/" + scriptFile));
}

void VideoInformation::loadPluginFile(QString scriptFile)
{
	// try to regist this
	VideoInformationPlugin *plugin = new VideoInformationPlugin(this, scriptFile);
	// if this plugin has not been loaded, we must destroy it manually
	if (!plugin->isLoaded()) delete plugin;
}

VideoInformationPlugin *VideoInformation::getRegisteredPlugin(const int index)
{
	if (validItemIndex(index))
		return plugins->at(index);
	else
		return NULL;
}

VideoInformationPlugin* VideoInformation::getRegisteredPlugin(const QString fileName, const bool onlyFileName)
{
	for (int n = 0; n < plugins->count(); n++)
	{
		VideoInformationPlugin *plugin = plugins->at(n);
		// compare names
		if (plugin->getScriptFile(onlyFileName) == fileName)
			return plugin;
	}
	// not found
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

QList<VideoInformationPlugin*> VideoInformation::getAllStandardPlugins() const
{
	QList<VideoInformationPlugin*> result;

	for(int n = 0; n < plugins->count(); n++)
	{
		if (!plugins->at(n)->hasAdultContent() && !plugins->at(n)->isMusicSite())
			result.append(plugins->at(n));
	}

	return result;
}

QList<VideoInformationPlugin*> VideoInformation::getAllAdultPlugins() const
{
	QList<VideoInformationPlugin*> result;

	for(int n = 0; n < plugins->count(); n++)
	{
		if (plugins->at(n)->hasAdultContent())
			result.append(plugins->at(n));
	}

	return result;
}

QList<VideoInformationPlugin*> VideoInformation::getAllMusicPlugins() const
{
	QList<VideoInformationPlugin*> result;

	for(int n = 0; n < plugins->count(); n++)
	{
		if (plugins->at(n)->isMusicSite())
			result.append(plugins->at(n));
	}

	return result;
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

int VideoInformation::pluginsCount()
{
	return plugins->count();
}

bool VideoInformation::hasPlugins()
{
	return pluginsCount() > 0;
}

void VideoInformation::abortExecution()
{
	if (videoItem != NULL && isGettingInfo())
	{
		VideoInformationPlugin *service = getPluginByHost(QUrl(videoItem->getURL()));
		if (service != NULL)
		{
			service->abortExecution();
			videoItem->setAsGettedURL(this);
			videoItem->unlock(this);
		}
	}
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

QPixmap VideoInformation::getHostImage(QString URL, bool checkURL)
{
	const QString path = ":/services/images/services/%1.png";

	bool valid = !checkURL ? true : validURL(URL);
	// if is a valid URL
	if (valid)
	{
		QPixmap resultat;
		// find a plugin which can resolve this url
		VideoInformationPlugin *plugin = getPluginByHost(QUrl(URL));
		// check if we found a plugin which can give us the service icon
		if (plugin != NULL)
		{
			QPixmap *p = plugin->getIcon();
			resultat = *p;
		}
		else // is an invalid service
			resultat = QPixmap(QString(path).arg("invalid"));
		// if this plugin hasn't an image loaded, then set an standard icon
		if (resultat.isNull())
			resultat = QPixmap(QString(path).arg("no_icon"));
		// return the final image
		return resultat;
	}
	else // return link error image
		return QPixmap(QString(path).arg("link_error"));
}

QString VideoInformation::getHostCaption(QString URL)
{
	if (validURL(URL)) //QUrl(URL).isValid())
	{
		VideoInformationPlugin *plugin = getPluginByHost(QUrl(URL));
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
	VideoInformationPlugin *service = getPluginByHost(QUrl(URL));
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

VideoInformation* VideoInformation::getLastVideoInformationInstance()
{
	return lastVideoInformationInstance;
}

// VideoInformationPlugin class

VideoInformationPlugin::VideoInformationPlugin(VideoInformation *videoInformation, QString videoServicePath)
{
	setObjectName("VideoInformationPlugin");
	// inits
	loaded = false;
	icon = new QPixmap();
	this->videoServicePath = videoServicePath;
	// load js code
	QFile scriptFile(videoServicePath);
	if (scriptFile.exists())
	{
		engine = new QScriptEngine();
		// load code
		scriptFile.open(QIODevice::ReadOnly);
		scriptCode = scriptFile.readAll();
		scriptFile.close();
		// execute plugin script
		engine->evaluate(scriptCode);
		// execute regist code if no errors found
		if (!engine->hasUncaughtException())
		{
			// get the regist func
			QScriptValue func_regist = engine->evaluate("RegistVideoService");
			// check if RegistVideoService function has been loaded
			if (func_regist.isFunction())
			{
				func_regist.call();
				// capture register result
				version = engine->globalObject().property("version").toString();
				minVersion = engine->globalObject().property("minVersion").toString();
				author = engine->globalObject().property("author").toString();
				website = engine->globalObject().property("website").toString();
				ID = engine->globalObject().property("ID").toString();
				caption = engine->globalObject().property("caption").toString();
				adultContent = engine->globalObject().property("adultContent").toBool();
				musicSite = engine->globalObject().property("isMusicSite").toBool();
				// validate if all main information is assigned
				loaded = !version.isEmpty() && !minVersion.isEmpty() && !ID.isEmpty() && !caption.isEmpty();
				// if this plugin has been loaded, then try to load the service icon
				if (loaded && compareVersions(minVersion, PROGRAM_VERSION_SHORT) <= 0)
				{
					QScriptValue func_getIcon = engine->evaluate("getVideoServiceIcon");
					// check if getVideoServiceIcon function has been loaded
					if (func_getIcon.isFunction())
					{
						// get the stored hex icon data from plugin
						QScriptValue hexData = func_getIcon.call();
						// check if is an array
						if (hexData.isArray())
						{
							// convert hex js array to QList<int>
							QList<int> intData;
							qScriptValueToSequence(hexData, intData);
							// fill a QByteArray with QList<int> data
							QByteArray byteArray;
							for (int n = 0; n < intData.size(); n++) byteArray.append(intData.at(n));
							// load the serivce icon from the QByteArray
							icon->loadFromData(byteArray);
						}
					}
				}
				else // this plugin, is not loaded
					loaded = false;
			}
		}
		// detach global engine
		delete engine;
		engine = NULL;
	}
	// regist this plugin
	if (videoInformation != NULL && loaded)
		videoInformation->registerPlugin(this);
}

VideoInformationPlugin::~VideoInformationPlugin()
{
	delete icon;
}

bool VideoInformationPlugin::isLikeMyId(QString ID)
{
	return ID.indexOf(this->ID) > -1;
}

QScriptValue VideoInformationPlugin::create_VideoDefinition(QScriptContext *, QScriptEngine *engine)
{
	return engine->toScriptValue(VideoDefinition());
}

QScriptValue VideoInformationPlugin::toScriptValue_VideoDefinition(QScriptEngine *engine, const VideoDefinition &vd)
{
	QScriptValue obj = engine->newObject();
	obj.setProperty("title", QScriptValue(engine, vd.title));
	obj.setProperty("URL", QScriptValue(engine, vd.URL));
	obj.setProperty("extension", QScriptValue(engine, vd.extension));
	obj.setProperty("isAudioFile", QScriptValue(engine, vd.isAudioFile));
	obj.setProperty("needLogin", QScriptValue(engine, vd.needLogin));
	obj.setProperty("cookies", QScriptValue(engine, vd.cookies));
	return obj;
}

void VideoInformationPlugin::fromScriptValue_VideoDefinition(const QScriptValue &obj, VideoDefinition &vd)
{
	vd.title = obj.property("title").toString();
	vd.URL = obj.property("URL").toString();
	vd.extension = obj.property("extension").toString();
	vd.isAudioFile = obj.property("isAudioFile").toBool();
	vd.needLogin = obj.property("needLogin").toBool();
	vd.cookies = obj.property("cookies").toString();
}

VideoDefinition VideoInformationPlugin::getVideoInformation(const QString URL)
{
	VideoDefinition result;

	// If this plugin has not been loaded before, then return an empty "information"
	if (!isLoaded()) return result;

	// plugin script engine
	engine = new QScriptEngine();

	// create and regist VideoDefinition
	qScriptRegisterMetaType(engine, toScriptValue_VideoDefinition, fromScriptValue_VideoDefinition);
	QScriptValue ctor_VidDef = engine->newFunction(create_VideoDefinition);
	engine->globalObject().setProperty("VideoDefinition", ctor_VidDef);

	// create and regist the script tools class
	ToolsScriptClass *toolsClass = new ToolsScriptClass(engine);

	// create and regist the Http class
	HttpScriptClass *httpClass = new HttpScriptClass(engine);
	engine->globalObject().setProperty("Http", httpClass->constructor());

	// evaluate plugin code
	engine->evaluate(scriptCode);
	// execute regist code if no errors found
	if (!engine->hasUncaughtException())
	{
		QScriptValue func_getVideoInfo = engine->evaluate("getVideoInformation");
		// check if getVideoInformation function has been loaded
		if (func_getVideoInfo.isFunction())
		{
			QScriptValueList args;
			args << QScriptValue(engine, URL);
			// execute plugin
			QScriptValue videoDefinition = func_getVideoInfo.call(QScriptValue(), args);
			result = qscriptvalue_cast<VideoDefinition>(videoDefinition);
			// trim some values
			result.title = result.title.trimmed();
			result.URL = result.URL.trimmed();
			result.extension = result.extension.trimmed();
		}
	}
	else // error found
		qWarning() << "Plugin error : " << engine->uncaughtException().toString();
	// destroy auxiliar classes
	delete toolsClass;
	delete httpClass;
	// detach global engine
	delete engine;
	engine = NULL;
	// return the video definition returned
	return result;
}

void VideoInformationPlugin::abortExecution()
{
	if (engine != NULL)
		engine->abortEvaluation();
}

QString VideoInformationPlugin::getScriptFile(const bool onlyName) const
{
	if (!onlyName)
		return videoServicePath;
	else
	{
		QFileInfo fileInf(videoServicePath);
		return fileInf.fileName();
	}
}

QString VideoInformationPlugin::getVersion() const
{
	return version;
}

QString VideoInformationPlugin::getMinVersion() const
{
	return minVersion;
}

QString VideoInformationPlugin::getAuthor() const
{
	return author;
}

QString VideoInformationPlugin::getWebsite() const
{
	return website;
}

QString VideoInformationPlugin::getID() const
{
	return ID;
}

QString VideoInformationPlugin::getCaption() const
{
	return caption;
}

bool VideoInformationPlugin::hasAdultContent() const
{
	return adultContent;
}

bool VideoInformationPlugin::isMusicSite() const
{
	return musicSite;
}

QPixmap *VideoInformationPlugin::getIcon() const
{
	return icon;
}

bool VideoInformationPlugin::isLoaded() const
{
	return loaded;
}
