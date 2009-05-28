#include "httpscriptclass.h"

Q_DECLARE_METATYPE(Http)
Q_DECLARE_METATYPE(Http*)
Q_DECLARE_METATYPE(HttpScriptClass*)

/* Http script prototype */

HttpScriptPrototype::HttpScriptPrototype(QObject *parent) : QObject(parent)
{
}

HttpScriptPrototype::~HttpScriptPrototype()
{
}

Http *HttpScriptPrototype::thisHttp() const
{
	return qscriptvalue_cast<Http*>(thisObject().data());
}

QString HttpScriptPrototype::downloadWebpage(const QString URL, bool isUtf8)
{
	return thisHttp()->downloadWebpage(QUrl(URL), isUtf8);
}

QString HttpScriptPrototype::downloadWebpageEx(const QString URL, bool url_tolerantMode, bool isUtf8)
{
	return thisHttp()->downloadWebpage(QUrl(URL, url_tolerantMode ? QUrl::TolerantMode : QUrl::StrictMode), isUtf8);
}

QString HttpScriptPrototype::downloadWebpagePost(const QString URL, QString parameters, bool isUtf8)
{
	return thisHttp()->downloadWebpagePost(QUrl(URL), parameters, isUtf8);
}

QString HttpScriptPrototype::downloadWebpagePostEx(const QString URL, QString parameters, bool url_tolerantMode, bool isUtf8)
{
	return thisHttp()->downloadWebpagePost(QUrl(URL, url_tolerantMode ? QUrl::TolerantMode : QUrl::StrictMode), parameters, isUtf8);
}

QString HttpScriptPrototype::downloadWebpageHeaders(const QString URL, QString separator)
{
	QHttpResponseHeader headers = thisHttp()->head(QUrl(URL));
	return headers.toString().replace("\n", separator);
}

void HttpScriptPrototype::addCookie(QString cookie)
{
	thisHttp()->addCookie(cookie);
}

void HttpScriptPrototype::addCookies(QString cookies, QString separator)
{
	thisHttp()->addCookies(cookies, separator);
}

QString HttpScriptPrototype::getCookies(QString separator)
{
	return thisHttp()->getCookies(separator);
}

void HttpScriptPrototype::clearCookies()
{
	thisHttp()->clearCookies();
}

void HttpScriptPrototype::addHeaderParameter(QString key, QString value)
{
	thisHttp()->addHeaderParameter(key, value);
}

void HttpScriptPrototype::clearHeaderParameters()
{
	thisHttp()->clearHeaderParameters();
}

void HttpScriptPrototype::setMaxRetries(int value)
{
	thisHttp()->setMaxRetries(value);
}

void HttpScriptPrototype::setTimeOut(int value)
{
	thisHttp()->setTimeOut(value);
}

void HttpScriptPrototype::setMaxAutoJumps(int value)
{
	thisHttp()->setMaxAutoJumps(value);
}

int HttpScriptPrototype::getLastError()
{
	return thisHttp()->getLastError();
}

int HttpScriptPrototype::getLastStopReason()
{
	return thisHttp()->getLastStopReason();
}

/* Http script class */

HttpScriptClass::HttpScriptClass(QScriptEngine *engine)
	: QObject(engine), QScriptClass(engine)
{
	qScriptRegisterMetaType<Http>(engine, toScriptValue, fromScriptValue);

	proto = engine->newQObject(new HttpScriptPrototype(this),
							   QScriptEngine::QtOwnership,
							   QScriptEngine::SkipMethodsInEnumeration
							   | QScriptEngine::ExcludeSuperClassMethods
							   | QScriptEngine::ExcludeSuperClassProperties);
	QScriptValue global = engine->globalObject();
	proto.setPrototype(global.property("Object").property("prototype"));

	ctor = engine->newFunction(construct);
	ctor.setData(qScriptValueFromValue(engine, this));
}

HttpScriptClass::~HttpScriptClass()
{
}

QScriptValue HttpScriptClass::construct(QScriptContext *ctx, QScriptEngine *)
{
	HttpScriptClass *cls = qscriptvalue_cast<HttpScriptClass*>(ctx->callee().data());
	if (!cls) return QScriptValue();
	return cls->newInstance();
}

QScriptValue HttpScriptClass::toScriptValue(QScriptEngine *eng, const Http &http)
{
	QScriptValue ctor = eng->globalObject().property("Http");
	HttpScriptClass *cls = qscriptvalue_cast<HttpScriptClass*>(ctor.data());
	if (!cls) return eng->newVariant(qVariantFromValue(http));
	return cls->newInstance(http);
}

void HttpScriptClass::fromScriptValue(const QScriptValue &obj, Http &http)
{
	http = qscriptvalue_cast<Http>(obj.data());
}

QScriptValue HttpScriptClass::constructor()
{
	return ctor;
}

QString HttpScriptClass::name() const
{
	return QLatin1String("Http");
}

QScriptValue HttpScriptClass::prototype() const
{
	return proto;
}

QScriptValue::PropertyFlags HttpScriptClass::propertyFlags(const QScriptValue&, const QScriptString&, uint)
{
	return QScriptValue::Undeletable;
}

QScriptValue HttpScriptClass::newInstance()
{
	return newInstance(Http());
}

QScriptValue HttpScriptClass::newInstance(const Http &http)
{
	QScriptValue data = engine()->newVariant(qVariantFromValue(http));
	return engine()->newObject(this, data);
}
