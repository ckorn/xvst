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
	this.website = "http://tv.boingboing.net/";
	this.ID = "tv.boingboing.net";
	this.caption = "Boing Boing";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_GET_FLV = "http://video.boingboing.net%1";
	// video information
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get video title
	result.title = copyBetween(html, "<title>", "</title>");
	result.title = strReplace(result.title, "- Boing Boing", "");
	// get flv url
	result.URL = copyBetween(html, "<embed class=", "</embed>");
	result.URL = copyBetween(result.URL, "src='", "'");
	result.URL = strRemove(result.URL, 0, strIndexOf(result.URL, "/video/"));
	// clear and get the final url
	result.URL = cleanUrl(strFormat(URL_GET_FLV, result.URL));
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x00,0x73,0x49,0x44,0x41,0x54,0x78,0x9c,0xb5,0x92,0x51,0x0e,0xc0,
		0x20,0x0c,0x42,0xcb,0xe2,0xfd,0xaf,0xcc,0x3e,0x5c,0x9d,0x36,0xd5,0xe0,0x16,0xf9,
		0xd1,0x18,0x83,0x3c,0x2a,0x48,0xd2,0x7a,0x01,0x75,0x0d,0xc7,0x33,0x5d,0xd2,0xad,
		0x93,0x06,0xa0,0x59,0xcd,0xea,0x91,0x1d,0xa1,0xd7,0x02,0xe7,0x20,0x82,0x58,0x62,
		0x69,0xbb,0x2c,0xfa,0x96,0x41,0x22,0xb7,0xf4,0x2c,0x78,0x1e,0x21,0xd9,0xf6,0x2f,
		0x82,0x18,0x39,0x6a,0xde,0x81,0x88,0x24,0x4d,0x01,0x0b,0xb3,0xdc,0x80,0x1c,0x90,
		0xe2,0x6f,0xcf,0x0d,0x3e,0x4e,0xe1,0xf7,0x47,0x2a,0x4a,0xfb,0x00,0x06,0x8c,0xbe,
		0x93,0xad,0x04,0x0c,0xbd,0x90,0xb4,0x1b,0xe4,0x65,0x29,0x16,0x62,0xd8,0x28,0x04,
		0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82);
}