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
	this.website = "http://www.yuvutu.com/";
	this.ID = "yuvutu.com";
	this.caption = "Yuvutu";
	this.adultContent = true;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_GET_HTML = "http://yuvutu.com/modules.php?name=Video&op=view&video_id=%1&name=Video&proceed=yes";
	// video information
	var result = new VideoDefinition();
	// get the video id
	var videoId = getUrlParam(url, "video_id");
	// get the html
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(strFormat(URL_GET_HTML, videoId));
	// get video title
	result.title = copyBetween(html, "class=\"videoTitle\"", "<td");
	result.title = copyBetween(result.title, "</span>", "</td>");
	// get the flv url
	result.URL = copyBetween(html, "&file=", "&");
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x02,0x0f,0x49,0x44,0x41,0x54,0x78,0x9c,0xa5,0x92,0x4b,0x48,0x94,
		0x61,0x14,0x86,0x9f,0xef,0x9b,0x7f,0xa6,0x42,0xcd,0xc6,0x7f,0x46,0x65,0xc6,0xd0,
		0x48,0x6c,0x34,0xd1,0x4c,0x14,0x34,0x62,0x92,0x6e,0xe2,0x80,0x30,0x51,0x42,0x12,
		0xb4,0xaa,0xad,0x0b,0x83,0xf6,0xb5,0xab,0x75,0xb4,0x8c,0x10,0x72,0x63,0x1b,0x23,
		0x08,0x8c,0xa1,0xc4,0x18,0x4a,0xba,0xe1,0xad,0xb4,0xc6,0xdb,0x80,0xe2,0x25,0x8b,
		0xca,0x19,0x2f,0xff,0x77,0x5a,0x0c,0x54,0xdb,0xd1,0x67,0x77,0x16,0xef,0xc3,0x0b,
		0xe7,0x55,0x22,0x22,0xec,0x02,0xbd,0x9b,0xf0,0x8e,0x05,0x33,0x13,0xc2,0xc8,0x2b,
		0xb3,0x33,0xc1,0x68,0xdc,0x70,0xe7,0xfa,0x26,0x1f,0x06,0x33,0x02,0xeb,0xe3,0xa0,
		0x61,0x29,0x29,0xd4,0x9c,0xd0,0x14,0x95,0x2a,0x16,0xa6,0xe1,0xdd,0xc8,0x32,0xf3,
		0xb5,0x4f,0xb0,0x73,0xf2,0x68,0xb7,0xdb,0xf1,0x28,0x0f,0x03,0x6b,0x03,0x24,0xd2,
		0x5f,0x59,0x7d,0x7e,0x86,0xa5,0xd9,0x52,0x66,0x27,0x84,0xd7,0xcf,0x0c,0x56,0x62,
		0x4c,0x78,0x74,0x77,0x8b,0x8e,0x2e,0x8b,0x4b,0x5d,0x16,0x2f,0x1f,0x3b,0xf4,0x3e,
		0xdc,0x22,0x7e,0xf3,0x3e,0x52,0x9e,0xa0,0x62,0x5f,0x05,0x41,0x4f,0x90,0x1b,0x89,
		0x6e,0xa6,0x92,0x2b,0x9c,0x8e,0x35,0x93,0x6b,0x29,0x46,0xe3,0x86,0xd4,0x6f,0x41,
		0xd7,0xb7,0x68,0xec,0x22,0xc5,0xf8,0x1b,0xc3,0xf7,0x65,0x18,0x1f,0x76,0x08,0xe6,
		0x16,0xd2,0x79,0xf0,0x22,0x2b,0x5b,0xdf,0x88,0xad,0xc5,0x18,0xfe,0x39,0xcc,0xc4,
		0xfa,0x27,0x2e,0x94,0xb5,0xd2,0x71,0x3e,0xc4,0xb6,0x31,0x34,0xb5,0x69,0xae,0xdd,
		0x76,0xa3,0x83,0x87,0x15,0xa1,0x06,0xcd,0xfc,0xa4,0xf0,0xfe,0x85,0x43,0x72,0x4a,
		0x38,0x52,0xaf,0xb9,0x7a,0xac,0x0d,0xbf,0xe5,0xa7,0x7f,0xb5,0x9f,0xde,0xa5,0x5e,
		0x94,0x28,0x3a,0xcb,0xa2,0x94,0x94,0xee,0x41,0x44,0xf0,0x05,0x14,0x65,0x55,0x0a,
		0xed,0x72,0xc3,0xf1,0x16,0x17,0xa9,0x5f,0xf0,0xf4,0x81,0x43,0x7a,0x1d,0x6a,0x4f,
		0x41,0x4d,0x7e,0x88,0x70,0x7e,0x98,0xa1,0x1f,0x43,0xf4,0x2d,0xf7,0x51,0x9d,0x53,
		0x4d,0xd3,0xfe,0x66,0x0c,0x99,0xd9,0x18,0xf3,0xdf,0x1b,0x2b,0x1b,0x15,0x76,0xb1,
		0x62,0x7a,0xcc,0xe0,0x2d,0xca,0xdc,0x2e,0xdc,0x44,0x7d,0x51,0x00,0x52,0x4e,0x8a,
		0x88,0x1d,0xa1,0x40,0xf9,0xf0,0xec,0x15,0xb4,0x86,0xc5,0x19,0x61,0x34,0x6e,0xb0,
		0x00,0x7c,0x01,0x45,0x65,0xa3,0x26,0xf9,0xc5,0x10,0xaa,0xd7,0xf8,0x83,0x0a,0x80,
		0xf0,0x81,0x30,0x55,0x39,0x55,0x2c,0x6e,0x2e,0x12,0xb1,0x23,0x00,0x94,0x94,0x2b,
		0x7c,0x41,0xc5,0xdb,0x98,0x21,0x31,0x66,0xb0,0x8c,0x03,0x1b,0x69,0x70,0x1c,0xc1,
		0x72,0x43,0xed,0x49,0x17,0xda,0x95,0xa9,0x17,0xf0,0x04,0xe8,0x09,0xf5,0x90,0x32,
		0x29,0xea,0x72,0xeb,0x00,0x38,0x74,0x54,0xd1,0x7d,0xcf,0xc3,0xfc,0xa4,0xc1,0x5b,
		0xa8,0x60,0xee,0xb3,0x91,0x5b,0x57,0x36,0xe5,0x72,0x45,0x5a,0xba,0x5b,0x37,0x64,
		0x65,0x41,0xb2,0xc2,0xca,0xf3,0x2a,0x0a,0x8a,0xa1,0xe1,0x9c,0xe6,0x6c,0xa7,0x0b,
		0xbb,0x38,0xbb,0x65,0x2a,0x11,0x11,0x67,0x1b,0x94,0xe2,0x6f,0xf5,0xac,0x05,0xd9,
		0xc7,0xfe,0xf1,0x07,0xd0,0xe9,0xf9,0x57,0x6c,0x53,0x20,0xfd,0x00,0x00,0x00,0x00,
		0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82);
}
