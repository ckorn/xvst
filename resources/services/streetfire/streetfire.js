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
	this.website = "http://videos.streetfire.net/";
	this.ID = "videos.streetfire.net";
	this.caption = "StreetFire";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_GET_FLV = "http://%1.streetfire.net/flv/%2/%3-.flv";
	// init result
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get the video title
	var title = copyBetween(html, "<title>", "</title>").toString();
	title = strRemove(title, title.lastIndexOf("-"), title.length);
	// set video title
	result.title = title;
	// get the video subdomain (we assume which the video subdomain is the same as preview subdomain)
	var previewUrl = copyBetween(html, "[img]", "[/img]");
	var subDomain = getToken(getUrlHost(previewUrl), ".", 0);
	// get the video hash
	var videoHash = copyBetween(html, 'vidiac.swf?video=', '"');
	// get video sub-dir
	var subDir = strCopy(videoHash, 0, 3);
	// build final video url
	result.URL = strFormat(URL_GET_FLV, subDomain, subDir, videoHash);
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0b,0x13,0x00,0x00,0x0b,
		0x13,0x01,0x00,0x9a,0x9c,0x18,0x00,0x00,0x01,0x27,0x49,0x44,0x41,0x54,0x38,0x11,
		0x63,0xfc,0xff,0xff,0x3f,0x03,0x25,0x80,0x89,0x12,0xcd,0x20,0xbd,0x03,0x66,0x00,
		0xc8,0xdf,0x60,0xbf,0xb3,0x60,0xf1,0xc2,0xff,0x85,0x89,0x89,0x0c,0xef,0x2e,0x5f,
		0xc6,0x22,0x05,0x11,0x9a,0xfc,0xfc,0x39,0x43,0x45,0x7d,0x3d,0x43,0x5a,0x5a,0x1a,
		0x03,0x23,0x5a,0x20,0x82,0x35,0xff,0x5c,0xb0,0x00,0xa7,0xe6,0x59,0x20,0x19,0x63,
		0x63,0x86,0x33,0x67,0xce,0x30,0x82,0x98,0xc8,0x2e,0xf8,0xbf,0xab,0xbb,0x9b,0x01,
		0x9f,0xe6,0x63,0x40,0x0d,0x77,0x05,0x04,0x18,0xde,0x9f,0x39,0x03,0xd2,0x0b,0x06,
		0x70,0x17,0xec,0x9b,0x38,0xf1,0xff,0x9d,0x82,0x02,0x98,0x38,0x06,0xfd,0x1c,0x28,
		0xd2,0x00,0xc4,0xcb,0x81,0xae,0xd3,0x34,0x30,0x00,0xcb,0xeb,0xeb,0xeb,0x43,0x62,
		0xe1,0xf8,0xc2,0x85,0xff,0x0f,0xe3,0xd1,0x0c,0x52,0x3d,0x19,0x88,0x8b,0x8a,0x8a,
		0x18,0x24,0xe4,0xe5,0x19,0xde,0xbf,0x7f,0xcf,0x20,0x29,0x29,0x09,0x12,0x66,0x64,
		0x59,0x01,0xd4,0x7c,0x32,0x21,0x01,0xc4,0x01,0x83,0x0f,0x30,0x06,0x12,0xbd,0x0d,
		0xc8,0xd6,0x75,0x70,0x60,0xf0,0xf5,0xf5,0x05,0x8b,0x0a,0x0a,0x0a,0x32,0x88,0x89,
		0x89,0x81,0xd9,0x8c,0x0e,0x0e,0x0e,0x28,0x49,0xf1,0xf3,0xe7,0xcf,0x0c,0xbc,0xbc,
		0xbc,0x0c,0x20,0x1a,0x06,0x40,0xfc,0x7a,0x60,0xa8,0xc3,0x00,0x50,0x0f,0x88,0x09,
		0x0e,0x44,0x78,0x18,0x80,0x44,0x5e,0xbd,0x7a,0xf5,0xff,0xda,0xb5,0x6b,0x20,0x26,
		0x4e,0x80,0xac,0x19,0xa4,0x08,0x39,0x25,0x12,0xd4,0xac,0xa5,0xa5,0x05,0xd2,0x03,
		0xb6,0x19,0xc4,0x00,0x01,0x64,0x17,0xa0,0x78,0x05,0x22,0x8d,0x95,0x44,0x31,0x00,
		0x39,0x1d,0xa0,0x48,0x60,0xd5,0x8a,0x45,0x10,0xd9,0x0b,0x58,0xa4,0x09,0x0b,0x01,
		0x00,0xf8,0x5f,0x5d,0x84,0xc7,0x18,0x57,0xe6,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,
		0x44,0xae,0x42,0x60,0x82);
}