/*
*
* This file is part of xVideoServiceThief,
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 - 2010 Xesc & Technology
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
	this.author = "Xesc & Technology 2010";
	this.website = "http://www.dailymotion.com/";
	this.ID = "dailymotion.com";
	this.caption = "Dailymotion";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	// video information
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get video title
	result.title = copyBetween(html, 'title="', '"');
	// get flv url
	result.URL = cleanUrl(copyBetween(html, '.addVariable("video", "', '");'));
	result.URL = getToken(result.URL, "||", 0);
	result.URL = getToken(result.URL, "@@", 0);
	// clear and get the final flv url
	result.URL = cleanUrl(result.URL);
	// check if need login
	result.needLogin = html.indexOf('addVariable("start"') != -1;
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x01,0x5d,0x49,0x44,0x41,0x54,0x78,0xda,0xa5,0xd3,0xbf,0x4b,0x02,
		0x61,0x18,0x07,0xf0,0x97,0x3b,0xc9,0xa1,0x21,0x70,0x6d,0x77,0x8e,0xc0,0x08,0xa2,
		0x68,0xa9,0xa5,0xb1,0xa5,0xbf,0xa0,0x86,0x68,0x8a,0x0c,0xa2,0x84,0x5a,0xfb,0x41,
		0x36,0x94,0xba,0x04,0x26,0xc8,0xdd,0x50,0x60,0x05,0x35,0x18,0x51,0x89,0x84,0x69,
		0x90,0x87,0x05,0xda,0xa1,0x54,0xd2,0x20,0x9d,0x67,0x59,0xd6,0xd0,0x37,0xde,0x47,
		0x08,0x8e,0x4e,0xe9,0xea,0x85,0xef,0xf8,0x7e,0xf8,0xde,0xf3,0xbc,0xc7,0x9a,0x9d,
		0xc7,0xb3,0x09,0xb0,0xff,0x9c,0xd7,0x14,0xfb,0x3b,0x80,0x4a,0x27,0x38,0x90,0x93,
		0x5d,0xd6,0x91,0xda,0xbd,0x17,0xd0,0xe7,0xa0,0x86,0x19,0x12,0x92,0x0d,0xb5,0x6c,
		0xbf,0x35,0xe4,0xb9,0x10,0x45,0x3c,0xc8,0x08,0x98,0x19,0x76,0xe0,0x2a,0x64,0xb1,
		0x05,0x34,0x37,0xf0,0xc2,0x90,0x97,0x19,0x7c,0x53,0x6d,0x90,0xdc,0x36,0x8b,0x80,
		0x3e,0x44,0x0d,0x32,0x7e,0x06,0xcf,0x88,0xc3,0x1a,0xa0,0x1c,0xfb,0xf0,0x79,0x37,
		0x80,0xe2,0x09,0x01,0xd4,0xc0,0x3f,0x6e,0xc7,0x4d,0x2c,0xf0,0x3b,0x64,0x73,0xb6,
		0x97,0xa6,0x8f,0x87,0x3a,0x10,0x9a,0x6c,0xc5,0xfe,0xbc,0x1d,0xbb,0x2b,0x7d,0x78,
		0xd7,0x32,0xcd,0x91,0xdc,0xe1,0x18,0x4e,0xe5,0x0e,0x40,0x63,0x14,0x65,0x9d,0xe1,
		0x7c,0x55,0x00,0x6e,0x19,0x0e,0x16,0x04,0xca,0x93,0xb2,0x66,0x8e,0x70,0x3d,0xbe,
		0xe1,0x44,0xfa,0xa8,0xdd,0x00,0xf0,0x70,0x20,0xb6,0x28,0xf2,0x26,0x34,0x58,0xf3,
		0xd5,0xe5,0x77,0xb0,0xe7,0x69,0x81,0x9e,0x16,0x4c,0x81,0x0b,0xaf,0x40,0x40,0x32,
		0x20,0xa2,0xc1,0xbb,0x1f,0xa5,0x61,0x99,0x01,0x6f,0xd7,0x34,0x8f,0x6f,0xa0,0x9c,
		0x0d,0xa2,0x11,0x40,0xfb,0xaf,0xc7,0x08,0xa8,0xe1,0xfa,0x67,0x44,0x97,0x44,0x98,
		0xfe,0x64,0xaa,0xec,0x44,0x2a,0xd2,0xc3,0xeb,0xf2,0x18,0x1a,0x94,0xa2,0xf4,0xa8,
		0x08,0x48,0x6c,0x75,0xa3,0x5a,0x88,0x34,0xde,0x06,0xaa,0x12,0x50,0x5e,0x06,0x8a,
		0x2e,0xba,0xac,0x6e,0x77,0x41,0x4b,0x0e,0xa2,0x92,0x99,0xc6,0x47,0xe9,0xf2,0xc7,
		0xc5,0x2f,0x4d,0x68,0x3a,0x4b,0x83,0x2b,0x3c,0xf1,0x00,0x00,0x00,0x00,0x49,0x45,
		0x4e,0x44,0xae,0x42,0x60,0x82);
}
