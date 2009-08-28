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
	this.version = "1.0.1";
	this.minVersion = "2.0.0a";
	this.author = "Xesc & Technology 2009";
	this.website = "http://www.5min.com/";
	this.ID = "5min.com";
	this.caption = "5min";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	// init result
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get video title
	result.title = copyBetween(html, '<h1 class="videoTitle">', '</h1>');
	// get video url
	result.URL = copyBetween(html, "videoUrl=", "&");
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x03,0x00,0x00,0x00,0x28,0x2d,0x0f,
		0x53,0x00,0x00,0x00,0x09,0x50,0x4c,0x54,0x45,0xff,0xff,0x00,0x00,0x00,0x00,0x81,
		0x81,0x00,0x2e,0x08,0x3b,0xd0,0x00,0x00,0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,
		0x0b,0x13,0x00,0x00,0x0b,0x13,0x01,0x00,0x9a,0x9c,0x18,0x00,0x00,0x00,0x39,0x49,
		0x44,0x41,0x54,0x18,0x19,0x9d,0x8d,0x31,0x12,0x00,0x30,0x08,0xc2,0x94,0xff,0x3f,
		0x5a,0xd3,0x09,0xae,0x4b,0xaf,0x6c,0xe6,0x08,0x56,0xfd,0x46,0xc4,0xe5,0x26,0x06,
		0xe4,0x07,0xfc,0x02,0xc7,0xf0,0x8d,0x6d,0x28,0x36,0xd0,0x02,0xf0,0x32,0x40,0xb7,
		0x52,0xd9,0x2b,0x0a,0x4c,0xbc,0x67,0x00,0x1f,0x7b,0x00,0x3a,0xc4,0x1b,0x27,0x51,
		0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82);
}
