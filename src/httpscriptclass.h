#ifndef HTTPSCRIPTCLASS_H
#define HTTPSCRIPTCLASS_H

#include <QtCore>
#include <QtScript>

#include "http.h"

class HttpScriptPrototype : public QObject, public QScriptable
{
Q_OBJECT
	private:
		Http *thisHttp() const;
		bool url_tolerantMode;
	public:
		HttpScriptPrototype(QObject *parent = 0);
		~HttpScriptPrototype();
	public slots:
		QString downloadWebpage(const QString URL, bool isUtf8 = true);
		QString downloadWebpageEx(const QString URL, bool url_tolerantMode, bool isUtf8 = true);
		QString downloadWebpagePost(const QString URL, QString parameters, bool isUtf8 = true);
		QString downloadWebpagePostEx(const QString URL, QString parameters, bool url_tolerantMode, bool isUtf8 = true);
		void addCookie(QString cookie);
		void addCookies(QString cookies, QString separator);
		QString getCookies(QString separator);
		void clearCookies();
		void addHeaderParameter(QString key, QString value);
		void clearHeaderParameters();
		void setMaxRetries(int value);
		void setTimeOut(int value);
		void setMaxAutoJumps(int value);
		int getLastError();
		int getLastStopReason();
};

class HttpScriptClass : public QObject, public QScriptClass
{
	private:
		QScriptValue proto;
		QScriptValue ctor;
		static QScriptValue construct(QScriptContext *ctx, QScriptEngine *eng);
		static QScriptValue toScriptValue(QScriptEngine *eng, const Http &http);
		static void fromScriptValue(const QScriptValue &obj, Http &http);
	public:
		HttpScriptClass(QScriptEngine *engine);
		~HttpScriptClass();
		QScriptValue constructor();
		QString name() const;
		QScriptValue prototype() const;
		QScriptValue::PropertyFlags propertyFlags(const QScriptValue &object, const QScriptString &name, uint id);
		QScriptValue newInstance();
		QScriptValue newInstance(const Http &http);
};

#endif // HTTPSCRIPTCLASS_H
