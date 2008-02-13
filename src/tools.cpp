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

int subVersionToInt(QString subVersion)
{
	subVersion = subVersion.toLower();
	
	if (subVersion == "alpha")
		return 1;
	else if (subVersion == "beta")
		return 2;
	else if (subVersion == "final")
		return 3;
	else
		return 4;
}

int compareVersions(QString version1, QString version2)
{
	// are the same version?
	if (version1 == version2)
		return 0;
	else
	{
		// replace comas for dots
		QString s1 = QString(version1).replace(",", ".");
		QString s2 = QString(version2).replace(",", ".");
		// get the version with more "fields" (x.x.x.x)
		int num1 = getTokenCount(s1, ".");
		int num2 = getTokenCount(s1, ".");
		// set the max num
		int max = num1 != num2 ? (num1 > num2 ? num1 : num2) : num1;
		// get subversions
		QString subVersion1, subVersion2;
		// get major version and sub-version (version1)
		if (getTokenCount(s1, " ") > 1)
		{
			subVersion1 = getToken(s1, " ", 1);
			s1 = getToken(s1, " ", 0);
		}
		// get major version and sub-version (version2)
		if (getTokenCount(s2, " ") > 1)
		{
			subVersion2 = getToken(s2, " ", 1);
			s2 = getToken(s2, " ", 0);
		}
		// Check for number versions
		for (int level = 0; level <= max; level++)
		{
			num1 = getToken(s1, ".", level).toInt();
			num2 = getToken(s2, ".", level).toInt();

			if (num1 > num2)
				return -1;
			else if (num1 < num2)
				return 1;
		}
		// Check for subversion (alpha, beta, etc...)
		if (subVersionToInt(subVersion1) < subVersionToInt(subVersion2))
			return 1;
		else if (subVersionToInt(subVersion1) > subVersionToInt(subVersion2))
			return -1;
		else
			return 0;
	}
}

QString cleanURL(QString URL)
{
	// replace hex chars to ascii chars
	while (URL.indexOf("%") != -1)
	{
		int p = URL.indexOf("%");
		QString hexChar = copy(URL, p + 1, p + 3);
		URL.replace("%" + hexChar, QString(hexChar.toInt(NULL, 16)));
	}
	return URL;
	
//	QUrl url = QUrl::fromEncoded(URL.toAscii());
//	return url.toString();
}

bool validURL(QString URL)
{
	// first at all, check if have the "http://"
	if (URL.indexOf("http://") == 0)
	{
		return QUrl(URL).isValid();
	}
	else
		return false;
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

bool openFile(QString filePath)
{
#ifdef Q_OS_WIN32
	return QDesktopServices::openUrl(filePath);
#else
	return QDesktopServices::openUrl("file://" + filePath);
#endif
}

bool openDirectory(QString directoryPath)
{
	return openFile(directoryPath);
}
