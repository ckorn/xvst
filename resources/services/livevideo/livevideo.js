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
	this.website = "http://www.livevideo.com/";
	this.ID = "livevideo.com";
	this.caption = "LiveVideo";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_GET_XML = "http://www.livevideo.com/media/getflashvideo.ashx?cid=%1&path=%2&t=%3&mid=%4&et=%5&f=flash8";
	const EXTRA_INFO = "LVX*7x8yzwe";
	// video information
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get video info
	result.title = copyBetween(html, "<title>", "</title>");
	// remove extra information form title
	result.title = strReplace(result.title, "- LiveVideo.com", "");
	result.title = strReplace(result.title, "- Channel", "");
	// get video ID (from the URL)
	var cid = getToken(url, "/", getTokenCount(url, "/") - 2);
	// get T parameter
	var T = copyBetween(html, "rel=\"videothumbnail\"", "/>");
	T = copyBetween(T, "livevideo.com", "\"");
	// get path parameter
	var path = getToken(T, "/", 2) + "/" + getToken(T, "/", 3);
	// get mid parameter
	var mid = getToken(T, "/", 4);
	mid = strCopy(mid, 0, strIndexOf(mid, "_"));
	// get et parameter
	var et = copyBetween(html, "et%3d", "'");
	// generate the xml url
	var xmlUrl = strFormat(URL_GET_XML, cid, path, T, mid, et);
	// download the xml file
	var xml = http.downloadWebpage(xmlUrl + "&h=" + getMd5(xmlUrl + EXTRA_INFO) + "&?");
	// get the FLV url
	result.URL = cleanUrl(copyBetween(xml, "video_id=", "&"));
	// get cookies
	result.cookies = http.getCookies("|");
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x00,0x67,0x49,0x44,0x41,0x54,0x78,0x9c,0x63,0x60,0xa0,0x01,0xf8,
		0x0f,0xc5,0xc4,0x8a,0x13,0x65,0x08,0x4e,0xcd,0x8c,0x04,0x0c,0x21,0x56,0x2d,0xd1,
		0x2e,0x21,0xe8,0x02,0x62,0xfd,0xc8,0x88,0xc1,0x20,0x41,0x33,0x8a,0x5e,0x16,0x74,
		0xd1,0x0d,0x37,0x5e,0x30,0x48,0x08,0x08,0xa0,0x88,0x09,0x70,0x20,0xd8,0x1a,0xc8,
		0x1c,0x06,0x06,0x06,0x26,0x74,0x03,0xf0,0x69,0xc6,0x06,0x30,0x0c,0xc0,0xa7,0x59,
		0x02,0x8b,0x61,0x38,0x0d,0x20,0x46,0x33,0x4e,0x03,0x88,0xd5,0x8c,0xd5,0x00,0x52,
		0x34,0x33,0x30,0x50,0x21,0x1a,0x29,0x4e,0x48,0x14,0x03,0x00,0x23,0x93,0x13,0x3a,
		0x9f,0x97,0x04,0xc8,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82
	);
}
