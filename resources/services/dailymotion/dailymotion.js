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
	this.website = "http://www.dailymotion.com/";
	this.ID = "dailymotion.com";
	this.caption = "Dailymotion";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_GET_FLV = "http://www.dailymotion.com%1";
	// video information
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get video title
	result.title = copyBetween(html, "<h1 class=\"nav\">", "</h1>");
	// get flv url
	result.URL = cleanUrl(copyBetween(html, ".addVariable(\"video\", \"", "\");"));
	result.URL = getToken(result.URL, "||", 0);
	result.URL = getToken(result.URL, "@@", 0);
	result.URL = strFormat(URL_GET_FLV, result.URL);
	// clear and get the final flv url
	result.URL = cleanUrl(result.URL);
	// check if need login
	result.needLogin = html.indexOf("addVariable(\"start\"") != -1;
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x01,0x88,0x49,0x44,0x41,0x54,0x78,0x9c,0x95,0x92,0xdd,0x2b,0x43,
		0x71,0x18,0xc7,0x3f,0x36,0x9b,0x6d,0x38,0x4d,0x79,0xcf,0xbc,0xbb,0x93,0x22,0x93,
		0x4b,0x25,0x29,0xee,0x5c,0xb9,0x70,0xef,0x8a,0xff,0x05,0x37,0x72,0x29,0x0a,0x17,
		0x5e,0xae,0x24,0x8a,0x0b,0x8a,0x56,0x56,0x62,0x68,0x92,0x32,0x43,0x6b,0x36,0x87,
		0x33,0x67,0x9b,0x73,0x7e,0x2e,0xa6,0x69,0xce,0xd9,0xe4,0xa9,0xdf,0xc5,0xef,0x79,
		0xfa,0x7e,0x9e,0xef,0xf3,0xf4,0x40,0x81,0x78,0x3a,0x9c,0x12,0x85,0xea,0x7f,0x46,
		0xe2,0x94,0x3f,0x01,0x96,0x7c,0x05,0x21,0xf7,0x08,0x80,0x9b,0x35,0x6f,0x41,0x88,
		0x29,0x40,0x0d,0xcd,0x08,0xc4,0x28,0x4f,0xd7,0x10,0x13,0x7e,0xd4,0xe0,0x40,0x5e,
		0x48,0xb1,0x59,0x32,0xad,0x75,0xe2,0xdf,0x1a,0xa4,0xc6,0x06,0x1b,0x6b,0x12,0x25,
		0x69,0xe5,0x7f,0x0e,0xca,0xa4,0x1d,0xfa,0xc7,0xc0,0x62,0x85,0xc6,0x56,0x8d,0xcb,
		0x33,0xff,0xff,0x00,0x58,0x2e,0x38,0x5e,0x87,0x44,0x0c,0xc2,0xf7,0xd6,0xbc,0x62,
		0x53,0xc0,0xf9,0xc1,0xbc,0x10,0x72,0x8a,0xa6,0xe6,0xcc,0xbf,0xde,0xa3,0x11,0x57,
		0xac,0x5c,0x1d,0x2d,0x98,0xee,0xc1,0x00,0xf0,0xed,0x2d,0xa3,0x46,0x76,0xa9,0x6b,
		0xfb,0x1e,0x47,0xff,0xa4,0xa1,0x1a,0x82,0x27,0x4b,0x24,0x63,0x01,0x03,0x24,0x07,
		0x70,0xb3,0x33,0x29,0xda,0xbb,0xde,0x70,0xb6,0x00,0x2e,0xd0,0x35,0xe8,0xf0,0x7c,
		0x30,0x32,0x91,0xc4,0xf6,0x7e,0xc8,0xfe,0x5c,0x27,0x2f,0xe7,0xb3,0xc2,0x14,0x90,
		0x8c,0x05,0x44,0xe4,0x76,0x1f,0x77,0x65,0x24,0xa7,0x43,0xa9,0x5d,0x07,0xa0,0xdc,
		0x59,0x84,0x26,0x6c,0xc8,0x81,0x69,0x73,0x07,0xa9,0xd7,0x6b,0xa2,0x8f,0x77,0x34,
		0x56,0x3d,0x9a,0x8d,0x4a,0x89,0x3d,0xd3,0x38,0x1a,0xcf,0x5d,0x6a,0x16,0xa0,0x84,
		0xb6,0x79,0x88,0x16,0x99,0x8a,0xd5,0x14,0xb8,0x1c,0x7a,0xf6,0x1f,0x0f,0x2e,0x66,
		0xc7,0x30,0x1c,0x92,0xd4,0xa2,0xff,0x4e,0x01,0xe0,0x90,0xc0,0x2d,0x7f,0x12,0x97,
		0x41,0x7d,0xf6,0x19,0x1d,0x24,0xc2,0x07,0x78,0x87,0x7b,0xe1,0x99,0xcc,0x4b,0xff,
		0x88,0x95,0x70,0xe6,0xa8,0x00,0xa4,0x1a,0x2f,0xe5,0x9e,0xa1,0x6c,0xcd,0xe0,0x59,
		0x28,0xab,0x82,0x74,0x08,0x12,0x2b,0x5c,0x6c,0xfa,0x70,0xd5,0xf6,0x51,0xd1,0xe4,
		0xc6,0xea,0xec,0xc6,0x51,0x3d,0x8e,0xbd,0xb2,0x3b,0x47,0xf3,0x05,0xe6,0x5e,0x90,
		0xe3,0xb9,0x35,0xa0,0xb9,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,
		0x82);
}
