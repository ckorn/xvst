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
	this.version = "1.0.2";
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
	// video information
	var result = new VideoDefinition();
	// download xml
	var http = new Http();
	var html = http.downloadWebpageEx(url, false);
	// check if is an embeded youtube video
	if (strIndexOf(html, "http://www.youtube.com/") != -1)
		return getVideoFromYoutube(html);
	else if (strIndexOf(html, "http://www.veoh.com/") != -1)
		return getVideoFromVeoh(html);
	else if (strIndexOf(html, "http://www.dailymotion.com/") != -1)
		return getVideoFromDailymotion(html);
	else
		return new VideoDefinition();
}

function getVideoFromYoutube(html)
{
	// check if youtube plugin is installed
	if (isPluginInstalled("youtube") == true)
	{
		var youtubeUrl = copyBetween(html, 'SWFObject("', '"');
		youtubeUrl = strReplace(youtubeUrl, "&amp;", "&");
		return executePlugin("youtube", youtubeUrl);
	}
	else // youtube plugin not installed
		return new VideoDefinition();
}

function getVideoFromVeoh(html)
{
	// check if youtube plugin is installed
	if (isPluginInstalled("veoh") == true)
	{
		var veohUrl = copyBetween(html, 'SWFObject("', '"');
		veohUrl = strReplace(veohUrl, "&amp;", "&");
		return executePlugin("veoh", veohUrl);
	}
	else // youtube plugin not installed
		return new VideoDefinition();
}

function getVideoFromDailymotion(html)
{
	const VIDEO_URL = "http://www.dailymotion.com/video/";
	// check if youtube plugin is installed
	if (isPluginInstalled("dailymotion") == true)
	{
		var videoId = copyBetween(html, 'SWFObject("', '"');
		videoId = copyBetween(videoId, "swf/", "?");
		return executePlugin("dailymotion", VIDEO_URL + videoId);
	}
	else // youtube plugin not installed
		return new VideoDefinition();
}
