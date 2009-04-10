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
	this.website = "http://www.spike.com/";
	this.ID = "spike.com";
	this.caption = "Spike";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_GET_XML = "http://www.spike.com/ui/xml/mediaplayer/mediagen.groovy?videoId=%1";
	// init result
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get the video title
	result.title = copyBetween(html, "<title>", "-");
	// get the video id
	var videoId = strRemove(url, 0, url.lastIndexOf("/") + 1);
	// download xml
	var xml = http.downloadWebpage(strFormat(URL_GET_XML, videoId));
	// get flv url
	result.URL = copyBetween(xml, "<src>", "</src>");
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x00,0x80,0x49,0x44,0x41,0x54,0x78,0x9c,0xcd,0x92,0x41,0x0e,0xc3,
		0x20,0x0c,0x04,0x67,0x11,0x27,0x3e,0x96,0x37,0xf7,0x63,0x3d,0x45,0xda,0x1c,0xd2,
		0xaa,0x6e,0x02,0x89,0x55,0x2e,0x5d,0x09,0x61,0x2c,0x79,0xbc,0x06,0x64,0xdb,0x4c,
		0xa8,0x02,0x3c,0x1f,0xf5,0xa7,0xe2,0xb6,0xac,0x94,0x99,0xee,0x00,0x9a,0x1d,0x01,
		0xc0,0xef,0xf5,0x82,0x9d,0xce,0xa3,0x1d,0x70,0x61,0xcf,0x10,0x8d,0x64,0x4d,0xd9,
		0xde,0xef,0x40,0x12,0x92,0x3e,0x73,0x85,0x38,0x4a,0xd2,0x17,0x5c,0x12,0xe5,0xd8,
		0xbd,0xe7,0x28,0xe6,0x23,0xdc,0x36,0x75,0xd4,0x2d,0x23,0x49,0xf3,0xaf,0xf0,0x67,
		0x1f,0x29,0xeb,0xa4,0x2d,0x6b,0x1f,0x70,0x07,0x8a,0x85,0x97,0x80,0x23,0xa8,0x57,
		0x98,0x02,0x64,0xb4,0x01,0xed,0x5c,0x6a,0xda,0xb3,0xd8,0xa7,0x0a,0x00,0x00,0x00,
		0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82);
}