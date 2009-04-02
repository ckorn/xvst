#ifndef TOOLSSCRIPTCLASS_H
#define TOOLSSCRIPTCLASS_H

#include <QtCore>
#include <QtScript>

class ToolsScriptClass : public QObject
{
	private:
		static QScriptValue func_getUrlHost(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue func_getUrlParam(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue func_copyBetween(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue func_strRemove(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue func_strCopy(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue func_strFormat(QScriptContext *context, QScriptEngine *engine);
	public:
		ToolsScriptClass(QScriptEngine *engine);
		~ToolsScriptClass();
};

#endif // TOOLSSCRIPTCLASS_H