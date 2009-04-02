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

	// regist copyBetween(str,from,to) function
	QScriptValue _copyBetween = engine->newFunction(func_copyBetween);
	engine->globalObject().setProperty("copyBetween", _copyBetween);

	// regist strRemove(str,pos,len) function
	QScriptValue _strRemove = engine->newFunction(func_strRemove);
	engine->globalObject().setProperty("strRemove", _strRemove);

	// regist strCopy(str,pos,len) function
	QScriptValue _strCopy = engine->newFunction(func_strCopy);
	engine->globalObject().setProperty("strCopy", _strCopy);

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
		// return the asket item from url
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
		// return the asket item from url
		return engine->newVariant(QVariant(QUrl(url).queryItemValue(item)));
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
		// return the asket item from url
		return engine->newVariant(QVariant(copyBetween(str, from, to)));
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
		// return the asket item from url
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
		// return the asket item from url
		return engine->newVariant(QVariant(copy(str, pos, len)));
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