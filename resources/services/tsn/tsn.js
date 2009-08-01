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
	this.minVersion = "0.2.2";
	this.author = "Xesc & Technology 2009";
	this.website = "http://watch.tsn.ca/";
	this.ID = "watch.tsn.ca";
	this.caption = "TSN";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_XML = "http://esi.ctv.ca/datafeed/flv/urlgenjs.aspx?vid=%1";
	// init result
	var result = new VideoDefinition();
	// get videoId
	var videoId = copyBetween(url + "/", "#clip", "/");
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get video title
	result.title = copyBetween(html, "#clip" + videoId, "</dt>");
	result.title = copyBetween(result.title, "Title:'", "'");
	// download xml
	var xml = http.downloadWebpage(strFormat(URL_XML, videoId));
	// get video url
	result.URL = copyBetween(xml, "url:'", "'");
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0b,0x13,0x00,0x00,0x0b,
		0x13,0x01,0x00,0x9a,0x9c,0x18,0x00,0x00,0x01,0xc8,0x49,0x44,0x41,0x54,0x38,0x11,
		0x5d,0x93,0x4b,0x4e,0x02,0x51,0x10,0x45,0x0b,0x7d,0x7e,0xf0,0x83,0x22,0x31,0xd1,
		0x04,0x8d,0x33,0x8d,0x71,0x2c,0x2b,0x70,0x05,0x6e,0xc2,0x11,0x2b,0x60,0x21,0xcc,
		0x60,0xe6,0x48,0x37,0xe0,0x02,0x04,0x27,0xce,0x4c,0x9c,0x48,0x54,0x12,0x4d,0xfc,
		0x82,0x0a,0x02,0x2a,0xde,0xd3,0x52,0xa4,0xb1,0x92,0xdb,0xef,0x53,0xb7,0x6e,0xdd,
		0xee,0xf7,0x3a,0x71,0x6a,0xd6,0xff,0x34,0xb3,0x77,0xe1,0x4d,0x68,0x0b,0x3d,0x81,
		0x48,0x08,0x41,0x48,0x0a,0xf3,0xc2,0x9c,0x30,0x29,0x7c,0x09,0x70,0x3a,0x42,0xa8,
		0xe9,0x41,0x51,0x57,0xe8,0x0b,0x63,0x02,0x45,0x33,0x02,0x45,0x60,0x42,0xa0,0x88,
		0x46,0xcd,0xc1,0x5c,0xc3,0x5f,0x83,0x17,0x4d,0x28,0x9a,0x12,0xe8,0x94,0x12,0xa6,
		0x05,0xba,0x23,0xd8,0x12,0xe8,0xf4,0x2d,0xb0,0xe7,0xd0,0x34,0xca,0x87,0x8c,0x26,
		0xf1,0x22,0xc8,0xbc,0x0e,0x16,0xdd,0x91,0xa6,0x43,0xc1,0x1f,0x16,0xb1,0x08,0xcb,
		0x5a,0x50,0xd4,0x10,0xb0,0xe9,0x41,0x27,0x82,0x02,0x84,0xfe,0x87,0xe7,0x43,0x72,
		0x7f,0xdf,0xd2,0xd9,0xec,0x30,0x4f,0xe2,0xa9,0x52,0xb1,0x89,0x54,0xca,0x52,0xdb,
		0xdb,0xc3,0xfd,0x5e,0xb3,0x69,0x60,0x66,0xc0,0x45,0xb4,0x7e,0x74,0x64,0x89,0xe7,
		0xf3,0xf3,0x3e,0xe4,0x78,0x34,0x2f,0x2e,0x46,0x8a,0xc9,0xdd,0x9f,0x9c,0xd8,0xca,
		0xde,0x5e,0x9c,0x16,0x09,0x86,0xb3,0x83,0x03,0x5b,0x50,0xa7,0x55,0x25,0x6f,0xa4,
		0xd8,0xaa,0xd7,0x6d,0x5d,0xae,0x7a,0x6f,0x1c,0xea,0x5f,0xd0,0xf9,0xb1,0x5a,0x8d,
		0x5c,0xdd,0x8a,0xd3,0x16,0x67,0x4d,0x9c,0xac,0x10,0x1e,0x94,0x60,0x91,0xc9,0xe5,
		0xac,0x2a,0x31,0xc8,0xbb,0xc5,0x62,0x44,0x76,0x01,0x1f,0x9d,0xd3,0x11,0x67,0xab,
		0x50,0x30,0xc6,0x80,0x7d,0xba,0xdf,0xc9,0x22,0xc5,0xcc,0xaf,0xca,0x65,0xaf,0xb1,
		0xcd,0x7c,0xde,0x9e,0xd4,0x04,0xfb,0x35,0x75,0x6f,0x88,0x93,0xd4,0x77,0xc0,0xf5,
		0x65,0xa9,0x64,0x81,0x04,0x22,0x08,0xf0,0x61,0x76,0xa4,0xec,0x1f,0x0a,0x95,0xae,
		0x0a,0x1a,0xb2,0x9c,0x96,0xc3,0x6b,0x71,0x38,0x95,0xac,0x6a,0x88,0xda,0xf1,0xb1,
		0x25,0x0e,0x75,0x4a,0xf1,0x5b,0x48,0x82,0xe3,0x04,0x5c,0x1e,0x3f,0x42,0x3f,0x36,
		0x1f,0x95,0x8a,0x22,0x6c,0x68,0xf8,0x10,0x5e,0x05,0xae,0x29,0x37,0x8f,0x22,0x6e,
		0x67,0x9c,0xec,0x42,0xda,0x1e,0x89,0x40,0x77,0xee,0xbe,0xdf,0x48,0xbe,0x3d,0x97,
		0x8a,0xff,0x03,0xbb,0x88,0xc4,0x85,0xb4,0x1c,0x89,0x48,0xc0,0x09,0xe3,0x4a,0x2d,
		0x09,0x8b,0x02,0x4e,0x70,0x85,0x3b,0xff,0x3b,0x71,0x85,0x13,0xe7,0x6b,0x1a,0x35,
		0x1f,0xbe,0x27,0x1b,0xbc,0x37,0x31,0x3b,0x00,0xc5,0xfc,0x1b,0x88,0x21,0xfa,0xdf,
		0xd5,0x2f,0xfb,0xff,0xac,0x6d,0xc1,0x53,0x7d,0x04,0x00,0x00,0x00,0x00,0x49,0x45,
		0x4e,0x44,0xae,0x42,0x60,0x82);
}
