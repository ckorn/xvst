/*
*
* This file is part of xVideoServiceThief, 
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 - 2010 Xesc & Technology
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

#include "tools.h"
#include "http.h"
#include "videoitem.h"
#include "searchvideos.h"
#include "searchvideosscriptclass.h"
#include "httpscriptclass.h"
#include "toolsscriptclass.h"
#include "programversion.h"
#include "options.h"

Q_DECLARE_METATYPE(VideoDefinition)
Q_DECLARE_METATYPE(SearchResults)
Q_DECLARE_METATYPE(SearchResults*)

QScriptValue myObjectToScriptValue(QScriptEngine *engine, SearchResults* const &in)
{
	return engine->newQObject(in);
}

void myObjectFromScriptValue(const QScriptValue &object, SearchResults* &out)
{
	out = qobject_cast<SearchResults*>(object.toQObject());
}

static VideoInformation *lastVideoInformationInstance; //!< Used as semi-singleton (remember the last instance created)

VideoInformation::VideoInformation(QString pluginsDir)
{
	setObjectName("VideoInformation");
	// plugins
	plugins = new QList<VideoInformationPlugin *>;
	if (!pluginsDir.isEmpty()) loadPlugins(pluginsDir);
	// init data
	videoItem = NULL;
	blockAdultContent = false;
	// update lastThis reference
	lastVideoInformationInstance = this;
	// create plugins catcher
	faviconsCatcher = new VideoInformationPluginIconsCatcher(this);
	faviconsCatcher->downloadFavicons();
}

VideoInformation::~VideoInformation()
{
	// abort any current plugin execution
	abortExecution();
	// wait until thread end
	if (isRunning()) quit();
	// abort plugins image catcher
	delete faviconsCatcher;
	// remove loaded plugins
	clearPlugins();
	// destroy plugins container
	delete plugins;
}

bool VideoInformation::validItemIndex(const int index)
{
	return (index >= 0 && index < pluginsCount());
}

void VideoInformation::clearPlugins()
{
	while (!plugins->isEmpty())
		delete plugins->takeFirst();
}

VideoInformationPlugin *VideoInformation::getPluginByHost(QUrl URL)
{
	QString host = URL.host().isEmpty() ? URL.toString() : URL.host();
	for (int n = 0; n < pluginsCount(); n++)
		if (plugins->at(n)->isLikeMyId(host))
			return plugins->at(n);
	// not found
	return NULL;
}

void VideoInformation::registerPlugin(VideoInformationPlugin *videoInformationPlugin)
{
	plugins->push_back(videoInformationPlugin);
}

void VideoInformation::unregisterPlugin(VideoInformationPlugin *videoInformationPlugin, bool destroy)
{
	if (plugins->contains(videoInformationPlugin))
		plugins->removeAt(plugins->indexOf(videoInformationPlugin));
	// destroy this plugin?
	if (destroy) delete videoInformationPlugin;
}

void VideoInformation::run()
{
	videoItem->lock(this);

	VideoInformationPlugin *service = getPluginByHost(QUrl(videoItem->getURL()));

	if (service != NULL)
	{
		if (isBlockedHost(videoItem->getURL()))
			videoItem->setAsBlocked(this);
		else
		{
			videoItem->setAsGettingURL(this);

			// if this item was market as "need update the url" then change the status to "updating url..."
			bool urlWasUpdated = videoItem->needUpdateUrl();
			if (urlWasUpdated) videoItem->setAsUpdatingURL();

			emit informationStarted(videoItem);

			VideoDefinition info = service->getVideoInformation(videoItem->getURL());

			// canceled?
			if (videoItem == NULL) return;

			if (info.needLogin)
			{
				videoItem->setAsNeedLogin(this);
				videoItem->removeUpdatingURLStatus();
			}
			else // ok, assign information and prepare the item to be downloaded
			{
				videoItem->setVideoInformation(info, this);
				if (!urlWasUpdated) videoItem->setVideoFile(cleanFileName(info.title + info.extension), this);
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
	else // not found
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

VideoInformationPlugin* VideoInformation::getRegisteredPlugin(const QString pluginId)
{
	for (int n = 0; n < plugins->count(); n++)
	{
		VideoInformationPlugin *plugin = plugins->at(n);
		// compare names
		if (plugin->getID() == pluginId)
			return plugin;
	}
	// not found
	return NULL;
}

QStringList VideoInformation::getPluginsList(bool asCaptions)
{
	QStringList results;

	for (int n = 0; n < pluginsCount(); n++)
		results << (asCaptions ? plugins->at(n)->getCaption() : plugins->at(n)->getID());

	return results;
}

QStringList VideoInformation::getPluginsCompleteList(const QString separator)
{
	QStringList results;

	for (int n = 0; n < pluginsCount(); n++)
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

QList<VideoInformationPlugin*> VideoInformation::getAllSearchPlugins() const
{
	QList<VideoInformationPlugin*> result;

	for(int n = 0; n < plugins->count(); n++)
		if (plugins->at(n)->isSearchEngineAvailable())
			result.append(plugins->at(n));

	return result;
}

QList<VideoInformationPlugin*> VideoInformation::getAllSearchStandardPlugins() const
{
	QList<VideoInformationPlugin*> result;

	for(int n = 0; n < plugins->count(); n++)
		if (plugins->at(n)->isSearchEngineAvailable() && !plugins->at(n)->hasAdultContent())
			result.append(plugins->at(n));

	return result;
}

QList<VideoInformationPlugin*> VideoInformation::getAllSearchAdultPlugins() const
{
	QList<VideoInformationPlugin*> result;

	for(int n = 0; n < plugins->count(); n++)
		if (plugins->at(n)->isSearchEngineAvailable() && plugins->at(n)->hasAdultContent())
			result.append(plugins->at(n));

	return result;
}

void VideoInformation::getVideoInformation(VideoItem *videoItem)
{
	if (videoItem == NULL || isGettingInfo()) return;
	// assign the current video item
	this->videoItem = videoItem;
	this->start();
}

int VideoInformation::pluginsCount() const
{
	return plugins->size();
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
	else if (isRtmpURL(URL))
		return tr("Adobe Flash streaming");
	else
		return tr("Invalid URL");
}

bool VideoInformation::isValidHost(QString URL)
{
	return getPluginByHost(QUrl(URL)) != NULL && QUrl(URL).isValid() && !isRtmpURL(URL)? true : false;
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

VideoInformation* VideoInformation::instance()
{
	return lastVideoInformationInstance;
}

// VideoInformationPluginIconsCatcher class

VideoInformationPluginIconsCatcher::VideoInformationPluginIconsCatcher(VideoInformation *videoInformation)
{
	this->videoInformation = videoInformation;
	plugins = new QList<VideoInformationPlugin *>;
	http = new Http();
	// connect signals
	connect(http, SIGNAL(downloadFinished(const QFileInfo)), this, SLOT(downloadFinished(const QFileInfo)));
	connect(http, SIGNAL(downloadError(int)), this, SLOT(downloadError(int)));
}

VideoInformationPluginIconsCatcher::~VideoInformationPluginIconsCatcher()
{
	delete http;
	delete plugins;
}

void VideoInformationPluginIconsCatcher::downloadFavicons()
{
	plugins->clear();
	// fill plugins
	for (int n = 0; n < videoInformation->pluginsCount(); n++)
		if (videoInformation->getRegisteredPlugin(n)->getIcon()->isNull() && videoInformation->getRegisteredPlugin(n)->useOnlineFavicon())
			plugins->append(videoInformation->getRegisteredPlugin(n));
	// start to download favicons
	QTimer::singleShot(50, this, SLOT(downloadNextFavicon()));
}

void VideoInformationPluginIconsCatcher::downloadNextFavicon()
{
	// has plguins to download his favicon?
	if (!plugins->isEmpty())
	{
		QFileInfo fileInfo(plugins->first()->getScriptFile(false));
		QString cahcePath = ProgramOptions::instance()->getOptionsPath() + PLUGINS_IMAGE_CACHE_DIR;
		// download
		int httpError = http->download(plugins->first()->getFaviconUrl(), cahcePath, fileInfo.baseName(), false);
		// has errors?
		if (httpError != 0)	downloadError(httpError);
	}
}

void VideoInformationPluginIconsCatcher::downloadFinished(const QFileInfo)
{
	if (plugins->isEmpty()) return;
	// take out the first favicon (due it has been processed)
	plugins->takeFirst()->reloadIcon();
	// download next favicon
	QTimer::singleShot(50, this, SLOT(downloadNextFavicon()));
}

void VideoInformationPluginIconsCatcher::downloadError(int)
{
	if (plugins->isEmpty()) return;
	// remove first favicon (due it has been processed)
	plugins->removeFirst();
	// download next favicon
	QTimer::singleShot(50, this, SLOT(downloadNextFavicon()));
}

// VideoInformationPlugin class

VideoInformationPlugin::VideoInformationPlugin(VideoInformation *videoInformation, QString videoServicePath)
{
	setObjectName("VideoInformationPlugin");
	// set the plugin owner
	this->owner = videoInformation;
	// inits
	loaded = false;
	hasSearchEngine = false;
	icon = new QPixmap();
	onlineFaviconUrl = "";
	pluginFilePath = videoServicePath;
	// configure debug options
#ifndef QT_NO_SCRIPTTOOLS
	debug = false;
#endif
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
				musicSite = engine->globalObject().property("musicSite").toBool();
				// validate if all main information is assigned
				loaded = !version.isEmpty() && !minVersion.isEmpty() && !ID.isEmpty() && !caption.isEmpty();
				// if this plugin has been loaded, then try to load the service icon
				if (loaded && compareVersions(minVersion, PROGRAM_VERSION_SHORT) >= 0)
				{
					// get if this plugin has a search engine
					hasSearchEngine = engine->evaluate("searchVideos").isFunction();
					// get the plugin icon
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
						else if (hexData.isString()) // check if is an string (favicon url)
						{
							onlineFaviconUrl = hexData.toString();
							// try to load icon plugin
							reloadIcon();
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
	obj.setProperty("headers", QScriptValue(engine, vd.headers));
	obj.setProperty("userAgent", QScriptValue(engine, vd.userAgent));
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
	vd.headers = obj.property("headers").toString();
	vd.userAgent = obj.property("userAgent").toString();
}

QScriptValue VideoInformationPlugin::func_isPluginInstalled(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 1)
	{
		// get params
		QString id = context->argument(0).toString();
		// get this plugin is installed
		VideoInformation *vidInf = VideoInformation::instance();
		// VideoInformation instance exists?
		if (vidInf == NULL) return QScriptValue();
		// get if this plugin has been installed
		bool installed = vidInf->getRegisteredPlugin(id + ".js", true) != NULL;
		// return the asked item from url
		return engine->newVariant(QVariant(installed));
	}
	else // invalid arguments count
		return QScriptValue();
}

QScriptValue VideoInformationPlugin::func_executePlugin(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 2)
	{
		// get params
		QString id = context->argument(0).toString();
		QString url = context->argument(1).toString();
		// create a temporal VideoItem
		VideoInformation *vidInf = VideoInformation::instance();
		VideoInformationPlugin *plugin = vidInf->getRegisteredPlugin(id + ".js", true);
		// is it registered?
		if (plugin != NULL)
			return engine->toScriptValue(plugin->getVideoInformation(url));
		else // plugin is not installed
			return QScriptValue();
	}
	else // invalid arguments count
		return QScriptValue();
}

QScriptValue VideoInformationPlugin::func_programVersion(QScriptContext *, QScriptEngine *engine)
{
	return engine->newVariant(QVariant(PROGRAM_VERSION));
}

QScriptValue VideoInformationPlugin::func_programVersionShort(QScriptContext *, QScriptEngine *engine)
{
	return engine->newVariant(QVariant(PROGRAM_VERSION_SHORT));
}

QScriptValue VideoInformationPlugin::func_programVersionNumber(QScriptContext *, QScriptEngine *engine)
{
	return engine->newVariant(QVariant(PROGRAM_VERSION_NUMBER));
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

	// regist isPluginInstalled(id) function
	QScriptValue _isPluginInstalled = engine->newFunction(func_isPluginInstalled);
	engine->globalObject().setProperty("isPluginInstalled", _isPluginInstalled);

	// regist executePlugin(id,url) function
	QScriptValue _executePlugin = engine->newFunction(func_executePlugin);
	engine->globalObject().setProperty("executePlugin", _executePlugin);

	// regist programVersion() function
	QScriptValue _programVersion = engine->newFunction(func_programVersion);
	engine->globalObject().setProperty("programVersion", _programVersion);

	// regist programVersionShort() function
	QScriptValue _programVersionShort = engine->newFunction(func_programVersionShort);
	engine->globalObject().setProperty("programVersionShort", _programVersionShort);

	// regist programVersionNumber() function
	QScriptValue _programVersionNumber = engine->newFunction(func_programVersionNumber);
	engine->globalObject().setProperty("programVersionNumber", _programVersionNumber);

	// create and regist the script tools class
	ToolsScriptClass *toolsClass = new ToolsScriptClass(engine);

	// create and regist the Http class
	HttpScriptClass *httpClass = new HttpScriptClass(engine);

	// configure debugger
#ifdef xVST_DEBUG_PLUGINS_ON
	QScriptEngineDebugger debugger;
	debugger.attachTo(engine);
#endif

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
			// start debug from begining (only if this->debug == true)
#ifdef xVST_DEBUG_PLUGINS_ON
			if (debug) debugger.action(QScriptEngineDebugger::InterruptAction)->activate(QAction::Trigger);
#endif
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

SearchResults VideoInformationPlugin::searchVideos(const QString keyWords, const int page)
{
	SearchResults result;

	// If this plugin has not been loaded before, then return an empty "information"
	if (!isLoaded() || !isSearchEngineAvailable()) return result;

	// plugin script engine
	engine = new QScriptEngine();

	// create and regist the script tools class
	ToolsScriptClass *toolsClass = new ToolsScriptClass(engine);

	// create and regist the Http class
	HttpScriptClass *httpClass = new HttpScriptClass(engine);

	// create and regist the SearchResults class
	SearchResultsScriptClass *searchResultsClass = new SearchResultsScriptClass(engine);

	// configure debugger
#ifdef xVST_DEBUG_PLUGINS_ON
	QScriptEngineDebugger debugger;
	debugger.attachTo(engine);
#endif

	// evaluate plugin code
	engine->evaluate(scriptCode);
	// execute regist code if no errors found
	if (!engine->hasUncaughtException())
	{
		QScriptValue func_searchVideos = engine->evaluate("searchVideos");
		// check if searchVideos function has been loaded
		if (func_searchVideos.isFunction())
		{
			QScriptValueList args;
			args << QScriptValue(engine, keyWords) << QScriptValue(engine, page);
			// start debug from begining (only if this->debug == true)
#ifdef xVST_DEBUG_PLUGINS_ON
			if (debug) debugger.action(QScriptEngineDebugger::InterruptAction)->activate(QAction::Trigger);
#endif
			// execute plugin
			QScriptValue searchResults = func_searchVideos.call(QScriptValue(), args);
			QVariant v = searchResults.data().toVariant();
			result = v.value<SearchResults>();
		}
	}
	else // error found
		qWarning() << "Plugin error : " << engine->uncaughtException().toString();
	// destroy auxiliar classes
	delete toolsClass;
	delete httpClass;
	delete searchResultsClass;
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
		return pluginFilePath;
	else
	{
		QFileInfo fileInf(pluginFilePath);
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

bool VideoInformationPlugin::useOnlineFavicon() const
{
	return !onlineFaviconUrl.isEmpty();
}

QString VideoInformationPlugin::getFaviconUrl() const
{
	return onlineFaviconUrl;
}

void VideoInformationPlugin::reloadIcon()
{
	if (icon->isNull() && useOnlineFavicon())
	{
		QFileInfo fileInfo(pluginFilePath);
		QString cahcePath = ProgramOptions::instance()->getOptionsPath() + PLUGINS_IMAGE_CACHE_DIR;
		// load icon
		icon->load(cahcePath + fileInfo.baseName());
	}
}

bool VideoInformationPlugin::isLoaded() const
{
	return loaded;
}

bool VideoInformationPlugin::isSearchEngineAvailable() const
{
	return hasSearchEngine;
}

#ifdef xVST_DEBUG_PLUGINS_ON
void VideoInformationPlugin::setDebug(bool value)
{
	debug = value;
}
#endif
