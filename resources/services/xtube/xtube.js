/*
*
* This file is part of xVideoServiceThief,
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 - 2009 Xesc & Technology
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

function RegistVideoService()
{
	this.version = "1.0.0";
	this.minVersion = "2.0.0a";
	this.author = "Xesc & Technology 2009";
	this.website = "http://www.xtube.com/";
	this.ID = "xtube.com";
	this.caption = "XTube";
	this.adultContent = true;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_POST_XML = "http://%1.xtube.com/find_video.php";
	const URL_POST_XML_PARAMS = "user_id=%1&clip_id=%2&video_id=%3";
	// video information
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get video title
	result.title = copyBetween(html, "<div class=\"titleBl1\"", "</span>");
	result.title = copyBetween(result.title, "<font color=#000>", "</font>");
	// get subdomain
	var subDomain = copyBetween(url, "http://", ".");
	var swfUrl = copyBetween(html, "swfURL\", \"", "\"");
	var user_id = copyBetween(html, "user_id\", \"", "\"");
	var clip_id = copyBetween(html, "clip_id\", \"", "\"");
	var video_id = copyBetween(html, "video_id\", \"", "\"");
	// get xml
	var xml = http.downloadWebpagePost(strFormat(URL_POST_XML, subDomain),
									   strFormat(URL_POST_XML_PARAMS, user_id, clip_id, video_id));
	// get video url
	result.URL = copyBetween(xml, "&filename=", "&");
	result.URL = swfUrl + result.URL;
	// return the video information
	return result;
}
