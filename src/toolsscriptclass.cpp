#include "toolsscriptclass.h"

#include "tools.h"

ToolsScriptClass::ToolsScriptClass(QScriptEngine *engine)
{
	// regist getUrlHost(url) function
	QScriptValue _getUrlHost = engine->newFunction(func_getUrlHost);
	engine->globalObject().setProperty("getUrlHost", _getUrlHost);

	// regist getUrlParam(url,param) function
	QScriptValue _getUrlParam = engine->newFunction(func_getUrlParam);
	engine->globalObject().setProperty("getUrlParam", _getUrlParam);

	// regist getUrlParam(url,param) function
	QScriptValue _cleanUrl = engine->newFunction(func_cleanUrl);
	engine->globalObject().setProperty("cleanUrl", _cleanUrl);

	// regist copyBetween(str,from,to) function
	QScriptValue _copyBetween = engine->newFunction(func_copyBetween);
	engine->globalObject().setProperty("copyBetween", _copyBetween);

	// regist getToken(str,separator,token) function
	QScriptValue _getToken = engine->newFunction(func_getToken);
	engine->globalObject().setProperty("getToken", _getToken);

	// regist getTokenCount(str,separator) function
	QScriptValue _getTokenCount = engine->newFunction(func_getTokenCount);
	engine->globalObject().setProperty("getTokenCount", _getTokenCount);

	// regist strRemove(str,pos,len) function
	QScriptValue _strRemove = engine->newFunction(func_strRemove);
	engine->globalObject().setProperty("strRemove", _strRemove);

	// regist strCopy(str,pos,len) function
	QScriptValue _strCopy = engine->newFunction(func_strCopy);
	engine->globalObject().setProperty("strCopy", _strCopy);

	// regist strReplace(str,before,after,[caseSensitive]) function
	QScriptValue _strReplace = engine->newFunction(func_strReplace);
	engine->globalObject().setProperty("strReplace", _strReplace);

	// regist strFormat(str,...) function
	QScriptValue _strFormat = engine->newFunction(func_strFormat);
	engine->globalObject().setProperty("strFormat", _strFormat);
}

ToolsScriptClass::~ToolsScriptClass()
{
}

QScriptValue ToolsScriptClass::func_getUrlHost(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 1)
	{
		// get params
		QString url = context->argument(0).toString();
		// return the asked item from url
		return engine->newVariant(QVariant(QUrl(url).host()));
	}
	else // invalid arguments count
		return QScriptValue();
}

QScriptValue ToolsScriptClass::func_getUrlParam(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 2)
	{
		// get params
		QString url = context->argument(0).toString();
		QString item = context->argument(1).toString();
		// return the asked item from url
		return engine->newVariant(QVariant(QUrl(url).queryItemValue(item)));
	}
	else // invalid arguments count
		return QScriptValue();
}

QScriptValue ToolsScriptClass::func_cleanUrl(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 1)
	{
		// get params
		QString url = context->argument(0).toString();
		// return the asked item from url
		return engine->newVariant(QVariant(cleanURL(url)));
	}
	else // invalid arguments count
		return QScriptValue();
}

QScriptValue ToolsScriptClass::func_copyBetween(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 3)
	{
		// get params
		QString str = context->argument(0).toString();
		QString from = context->argument(1).toString();
		QString to = context->argument(2).toString();
		// return the asked item from url
		return engine->newVariant(QVariant(copyBetween(str, from, to)));
	}
	else // invalid arguments count
		return QScriptValue();
}

QScriptValue ToolsScriptClass::func_getToken(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 3)
	{
		// get params
		QString str = context->argument(0).toString();
		QString separator = context->argument(1).toString();
		int token = context->argument(2).toInteger();
		// return the asked item from url
		return engine->newVariant(QVariant(getToken(str, separator, token)));
	}
	else // invalid arguments count
		return QScriptValue();
}

QScriptValue ToolsScriptClass::func_getTokenCount(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 3)
	{
		// get params
		QString str = context->argument(0).toString();
		QString separator = context->argument(1).toString();
		// return the asked item from url
		return engine->newVariant(QVariant(getTokenCount(str, separator)));
	}
	else // invalid arguments count
		return QScriptValue();
}

QScriptValue ToolsScriptClass::func_strRemove(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 3)
	{
		// get params
		QString str = context->argument(0).toString();
		int pos = context->argument(1).toInteger();
		int len = context->argument(2).toInteger();
		// return the asked item from url
		return engine->newVariant(QVariant(str.remove(pos, len)));
	}
	else // invalid arguments count
		return QScriptValue();
}

QScriptValue ToolsScriptClass::func_strCopy(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 3)
	{
		// get params
		QString str = context->argument(0).toString();
		int pos = context->argument(1).toInteger();
		int len = context->argument(2).toInteger();
		// return the asked item from url
		return engine->newVariant(QVariant(copy(str, pos, len)));
	}
	else // invalid arguments count
		return QScriptValue();
}

QScriptValue ToolsScriptClass::func_strReplace(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() >= 3)
	{
		// get params
		QString str = context->argument(0).toString();
		QString before = context->argument(1).toString();
		QString after = context->argument(2).toString();
		// is casesensitive by default
		Qt::CaseSensitivity sensitive = Qt::CaseSensitive;
		if (context->argumentCount() == 4)
			sensitive = context->argument(3).toBool() ? Qt::CaseSensitive : Qt::CaseInsensitive;
		// return the asked item from url
		return engine->newVariant(QVariant(QString(str).replace(before, after, sensitive)));
	}
	else // invalid arguments count
		return QScriptValue();
}

QScriptValue ToolsScriptClass::func_strFormat(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() > 0)
	{
		QString str = context->argument(0).toString();
		// replace occurrences
		for (int n = 1; n < context->argumentCount(); n++)
			str.replace("%" + QString("%1").arg(n) , context->argument(n).toString());
		// return final string
		return engine->newVariant(QVariant(str));
	}
	else // invalid arguments count
		return QScriptValue();
}
