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
	this.website = "http://www.youporn.com/";
	this.ID = "youporn.com";
	this.caption = "YouPorn";
	this.adultContent = true;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	// video information
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url + "/?user_choice=Enter");
	// get video title
	result.title = copyBetween(html, "<title>", "- Free Porn Videos");
	// get the flv url
	result.URL = copyBetween(html, "<div id=\"download\">", "</div");
	result.URL = copyBetween(result.URL, "<a href=\"", "\"");
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x01,0xa8,0x49,0x44,0x41,0x54,0x78,0x9c,0xa5,0xd2,0xbd,0x6b,0x15,
		0x41,0x14,0x05,0xf0,0xdf,0xbe,0xaf,0x3c,0x13,0x0c,0x46,0x5b,0x09,0xd8,0x2a,0x82,
		0x55,0x1a,0x89,0xa8,0x85,0x96,0x16,0x96,0xb1,0x57,0xc1,0xc6,0xbf,0xc0,0x3f,0x40,
		0x2b,0x0b,0x11,0x49,0x9d,0xca,0xda,0x42,0x41,0xb0,0x10,0x2d,0x45,0xf0,0x03,0x51,
		0x14,0x91,0x04,0x35,0x1a,0x35,0x98,0xbc,0xf7,0x66,0xf7,0x8d,0xc5,0x5d,0xe3,0x06,
		0x52,0x28,0x5e,0x18,0x76,0x76,0xb8,0xe7,0xcc,0x39,0x67,0x2e,0xff,0x59,0x05,0xe4,
		0x1b,0x37,0xb3,0x56,0x8b,0xf1,0xf8,0xef,0x50,0xad,0x16,0x39,0x2b,0x2e,0x9e,0x2f,
		0x3a,0x20,0x65,0x36,0xd7,0xff,0xed,0xea,0x5d,0x93,0x20,0x08,0x06,0x89,0x9f,0x43,
		0xca,0x8a,0x7e,0x2f,0xbe,0x65,0x49,0xb7,0x43,0x51,0x30,0x4a,0xdb,0xc1,0xbd,0x1e,
		0x45,0x6a,0x10,0x6c,0x0e,0xd9,0x33,0xc3,0xf4,0x6e,0xde,0x7f,0x60,0x7a,0x9a,0xbd,
		0x33,0x7c,0x59,0x0b,0xa2,0xd9,0xd9,0x90,0x4d,0xd8,0x5c,0xf9,0x18,0x98,0x2d,0x82,
		0xf5,0x4d,0x4e,0xcc,0x33,0x3f,0xc7,0x95,0x6b,0x1c,0x3e,0xc4,0xe9,0xe3,0xdc,0xb9,
		0xcf,0xd7,0x35,0xce,0x9d,0xdd,0xae,0x60,0x71,0x89,0x97,0x6f,0x22,0x0e,0x30,0xaa,
		0x48,0x29,0xa4,0x8e,0xca,0x58,0x90,0xca,0x58,0xf0,0xe0,0x31,0xb7,0x96,0x62,0x5f,
		0x8d,0x19,0x96,0x0d,0x82,0x61,0x15,0x87,0x39,0xf3,0x63,0x83,0xc1,0x28,0x1a,0xcb,
		0x8a,0x54,0xbf,0xcc,0xf2,0x27,0x9e,0xbf,0xae,0xcf,0xc7,0x81,0xd9,0xb2,0x30,0x4c,
		0x64,0xf4,0x27,0xb8,0xb0,0xc0,0xbe,0x99,0x3f,0x37,0x95,0x35,0xc1,0xb1,0x39,0x8e,
		0x1c,0x6c,0x10,0xa4,0x86,0x82,0x32,0xf3,0x6e,0x85,0x27,0x2f,0x28,0x5a,0x4c,0xf4,
		0x6a,0x0b,0x39,0x54,0xc0,0xd4,0x14,0x9d,0x6e,0xa8,0x58,0xfd,0x1e,0x98,0x2d,0x05,
		0x0a,0xee,0x3d,0xe2,0xe9,0xab,0xf8,0x3d,0x73,0x92,0x53,0x47,0xc3,0x52,0x15,0x8d,
		0xee,0x3e,0x64,0xf1,0x36,0x93,0x7d,0xba,0x5d,0x26,0xfa,0x0d,0x82,0x8d,0x44,0xa7,
		0xcd,0xa5,0x05,0x0e,0xec,0x47,0xae,0x83,0x4a,0x75,0x58,0x39,0x64,0x17,0x6d,0xb4,
		0x6b,0x5b,0x65,0x83,0xa0,0x1c,0x47,0xf2,0xd7,0x97,0x22,0x87,0xaa,0x0a,0x55,0xab,
		0x6b,0x61,0xe1,0xf2,0x55,0x3e,0x7f,0xa3,0xd5,0x26,0xd5,0x96,0xc6,0xcd,0x10,0xcb,
		0x1c,0xcf,0xf8,0xec,0x6d,0x0c,0x4a,0x51,0x84,0xfc,0xdf,0x93,0xb8,0xbc,0x1a,0xfb,
		0x6e,0x27,0x06,0x0b,0x72,0xbb,0x41,0x30,0x48,0x21,0xaf,0xd3,0xb2,0x63,0xf5,0xea,
		0xa8,0xca,0x3a,0xaf,0xa2,0x60,0x50,0xee,0xdc,0xfb,0xaf,0xf5,0x0b,0xd3,0x9b,0xb8,
		0x71,0x32,0x21,0x88,0x6e,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,
		0x82);
}