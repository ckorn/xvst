#include "tools.h"

QString copy(QString str, int start, int end)
{
	end = end > str.length() ? str.length() : end;
	
	if (end > start)
	{
		str.remove(end, str.length() - end);
		str.remove(0, start);
		return str;
	}
	else
		return "";
}

QString getToken(QString str, QString separator, int token)
{
	QStringList results = str.split(separator, QString::KeepEmptyParts);

	if (token >= 0 && token < results.count())
		return results.at(token);
	else
		return "";
}

int getTokenCount(QString str, QString separator)
{
	QStringList results = str.split(separator, QString::KeepEmptyParts);

	return results.count();
}

QString copyBetween(QString str, QString from, QString to)
{
	int start = str.indexOf(from, 0) + from.length();
	int end = str.indexOf(to, start + 1);

	return copy(str, start, end);
}

QString fileSizeToString(const int bytes)
{
	float fbytes = bytes;
	QString result;

	if (fbytes <= 1024)
		result = floatToStr(fbytes) + " bytes"; //QString("").sprintf("%.2f bytes", fbytes);
	else if (fbytes / 1024 <= 1024)
		result = floatToStr(fbytes / 1024) + " KB"; //QString("").sprintf("%.2f KB", fbytes / 1024);
	else if (fbytes / 1024 / 1024 <= 1024)
		result = floatToStr(fbytes / 1024 / 1024) + " MB"; //QString("").sprintf("%.2f MB", fbytes / 1024 / 1024);
	else if (fbytes / 1024 / 1024 / 1024 <= 1024)
		result = floatToStr(fbytes / 1024 / 1024 / 1024) + " GB"; //QString("").sprintf("%.2f GB", fbytes / 1024 / 1024 / 1024);

	return result;
}

QFileInfo uniqueFileName(const QString fileName)
{
	int n = 1;
	QString tmp = fileName;

	while (QFileInfo(tmp).exists())
	{
		tmp = QString("%1/%2(%3).%4")
		      .arg(QFileInfo(fileName).absolutePath())
		      .arg(QFileInfo(fileName).baseName())
		      .arg(n)
		      .arg(QFileInfo(fileName).completeSuffix());
		// next
		n++;
	}
	return QFileInfo(tmp);
}

QString changeFileExt(const QString file, const QString ext)
{
	return QFileInfo(file).absolutePath() + "/" + QFileInfo(file).baseName() + ext;
}

QString cleanFileName(const QString fileName, QString replaceFor)
{
	QString invalids = "\\/:*?\"<>|\n\a\f\r\t\v";
	QString result = fileName;

	for (int n = 0; n < fileName.length(); n++)
	{
		if (invalids.indexOf(fileName.at(n), 0) != -1)
			result.replace(fileName.at(n), replaceFor);
	}
	return result;
}

QString secondsToHMS(const int seconds, const bool compact, const bool allowNegative)
{
	int sec = seconds;

	if (!allowNegative && seconds < 0)
		sec = 0;

	if (!compact)
		return QString("%1h %2m %3s")
		       .arg(sec/3600)	//h
		       .arg(sec/60%60)	//m
		       .arg(sec%60);	//s
	else
	{
		QString result("");
		// h
		if (sec/3600 != 0)
			result = QString("%1h").arg(sec/3600);
		// m
		if (sec/60%60 != 0)
			result += QString("%1m").arg(sec/60%60);
		// s
		if (sec%60 != 0)
			result += QString("%1s").arg(sec%60);
		// return result
		return result;
	}
}

float calculePercent(const float value, const float total)
{
	if (total == 0)
		return 0;
	else
		return value/total*100;
}

QString floatToStr(const float value, const int precision)
{
	QLocale locale;
	return locale.toString(value, 'f', precision);
}

QString cleanURL(QString URL)
{
	QUrl url = QUrl::fromEncoded(URL.toAscii());
	return url.toString();
}

QString getPathAndQuery(QUrl URL)
{
	QString result = URL.toEncoded();
	return result.remove("http://" + URL.host()); 
}

QString htmlCharToHtmlCode(QString html)
{
	bool ok;
	int index = QString(copyBetween(html, "#", ";")).toInt(&ok);

	if (ok)
		switch (index)
		{
			case 34:
				return getToken(HTML_Char_Conversion_Table[0], "=", 1);
			case 38:
				return getToken(HTML_Char_Conversion_Table[1], "=", 1);
			case 60:
				return getToken(HTML_Char_Conversion_Table[2], "=", 1);
			case 62:
				return getToken(HTML_Char_Conversion_Table[3], "=", 1);
			default:
				return getToken(HTML_Char_Conversion_Table[99 - (255 - index)], "=", 1);
		}
	else // error
		return "";
}

QString htmlCodeToHtmlChar(QString html)
{
	for (int n = 0; n < 100; n++)
		if (getToken(HTML_Char_Conversion_Table[n], "=", 1) == html)
			return getToken(HTML_Char_Conversion_Table[n], "=", 0);
	// not found
	return "";
}

QString charToHtml(QChar asciiChar, bool htmlCodes)
{
	if (asciiChar.isNull()) return "";

	if	(asciiChar == ' ' || asciiChar == '!' || asciiChar == '=' ||
	    (asciiChar >= '#' && asciiChar <= '%') ||
	    (asciiChar >= '\'' && asciiChar <= ';') ||
	    (asciiChar >= '?' && asciiChar <= '~'))
		return asciiChar;
	else // is a "rare" char (nonstandard)
	{
		QString result = QString("&#%1;").arg(int(asciiChar.unicode()));
		// use html codes?
		if (htmlCodes)
		{
			// get the html char code
			QString htmlChar = htmlCharToHtmlCode(result);
			// we have results?
			if (!htmlChar.isEmpty())
				return htmlChar;
			else
				return result;
		}
		else
			return result;
	}
}

QString htmlToStr(QString html)
{
	QString result = "";
	// parse html text
	while (html.indexOf("&") > -1)
	{
		// copy text & delete it from HTML
		int n = html.indexOf("&");
		result += copy(html, 0, n);
		html.remove(0, n);
		// get the HTML char & delete it from HTML
		n = html.indexOf(";");
		if (n == -1) return result + html;
		QString htmlChar = copy(html, 0, n + 1);
		html.remove(0, n + 1);
		// convert
		result += htmlToChar(htmlChar);
	}
	// all has been parsed?
	if (!html.isEmpty())
		result += html;
	// did something?
	if (result.isEmpty())
		result = html;
	// final string
	return result;
}

QChar htmlToChar(QString html)
{
	// try to convert the html code to html decimal
	QString charString = htmlCodeToHtmlChar(html);
	// if we do not have results then get the original HTML passed by parameters
	if (charString.isEmpty())
		charString = html;
	// start the conversion
	charString = copyBetween(charString, "#", ";");
	// convert
	bool ok;
	int charValue = charString.toInt(&ok);
	// is a valid value
	if (ok)
		return QChar(charValue);
	else
		return ' ';
}

QString strToHtml(QString str, bool htmlCodes)
{
	str = str.fromUtf8(str.toAscii());
	QString result = "";

	for (int n = 0; n < str.length(); n++)
		result += charToHtml(str[n], htmlCodes);

	return result;
}
