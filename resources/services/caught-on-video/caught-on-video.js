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
	this.website = "http://www.caught-on-video.com/";
	this.ID = ".caught-on-video.com";
	this.caption = "Caught On Video";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_GET_XML = "http://%1/handlers/SFAsysPlayer_GetData.ashx?playerinfo={%20id:'null',%20mode:'stdvideo_from_id',%20note:'true',%20pno:'-1',%20cfg:'4482',%20playlistid:'-1',%20video:'%2',%20album:'null',%20photo:'null',%20overlay:'null',%20conurl:'',%20condomain:'%3',%20watchedlist:''%20}";
	// video information
	var result = new VideoDefinition();
	// get video id
	var video = copyBetween(url, "video/", ".htm");
	// get pre-url
	var xmlUrl = strReplace(URL_GET_XML, "%20", " ");
	xmlUrl = strFormat(xmlUrl, getUrlHost(url), video, url);
	xmlUrl = strReplace(xmlUrl, " ", "%20");
	// download xml
	var http = new Http();
	var xml = http.downloadWebpageEx(xmlUrl, false);
	// get video information
	result.title = strReplace(copyBetween(xml, "<media:title>", "</media:title>"), ". Caught on Video.", "");
	result.URL = copyBetween(xml, "<sf:mediapath>", "</sf:mediapath>");
	// clear and get the final url
	result.URL = cleanUrl(result.URL);
	// return the video information
	return result;
}
