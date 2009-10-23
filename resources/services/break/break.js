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
	this.website = "http://www.break.com/";
	this.ID = "break.com";
	this.caption = "Break";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_GET_FLV = "%1%2/%3.flv";
	// video information
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get title
	result.title = copyBetween(html, "<title>", "&nbsp;");
	// get video pre-url
	var preUrl = copyBetween(html, 'var videoPath = "', '"');
	// get variable values
	var sGlobalContentFilePath = copyBetween(html, "sGlobalContentFilePath='", "'");
	var sGlobalFileName = copyBetween(html, "sGlobalFileName='", "'");
	// clear and get the final url
	result.URL = cleanUrl(strFormat(URL_GET_FLV, preUrl, sGlobalContentFilePath, sGlobalFileName));
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x02,0xd8,0x49,0x44,0x41,0x54,0x78,0x9c,0x35,0x92,0xcb,0x6b,0x5d,
		0x55,0x18,0x47,0xd7,0xf7,0xed,0x7d,0xcf,0xb9,0x37,0x09,0x72,0x13,0x9b,0x1a,0x1f,
		0x15,0xd1,0x56,0xac,0x50,0x1c,0x18,0x68,0x10,0x6b,0x10,0x2c,0x54,0x49,0x2c,0x2a,
		0x45,0x5a,0x4b,0x41,0x28,0x3e,0xc0,0x42,0x26,0x4e,0xc4,0x89,0x82,0x14,0x14,0x0b,
		0x1d,0xb4,0x71,0xa8,0x93,0x82,0x10,0xac,0x8a,0x0e,0x7c,0x0c,0x7c,0xe0,0xb8,0x60,
		0x49,0x8d,0x50,0x53,0xaa,0x96,0x68,0xd2,0xdc,0xbc,0x4e,0xef,0xe3,0x9c,0xbd,0xf7,
		0xe7,0xe0,0xc6,0xdf,0x5f,0xf0,0x63,0xad,0x25,0x6c,0x6f,0x3f,0xf0,0x26,0x39,0x86,
		0x79,0x4f,0x8c,0x9e,0x68,0x3d,0x6a,0x08,0x8c,0x09,0xf9,0xeb,0x89,0xee,0x69,0x07,
		0x55,0xc4,0x89,0x22,0xe1,0x4b,0x7a,0x5c,0x07,0x7c,0x0e,0x3c,0x0b,0x1c,0xc3,0x61,
		0xa4,0x86,0x92,0xed,0xf3,0x84,0x8d,0x12,0xbf,0x62,0x98,0x79,0xea,0xe3,0x09,0x6b,
		0x45,0x2c,0xcb,0xc8,0xf6,0x29,0x3a,0x60,0x84,0xc5,0x29,0xb2,0x35,0x41,0x0a,0x37,
		0x0d,0xbc,0x84,0x57,0x45,0x77,0x28,0x7e,0x3c,0xc2,0x4c,0xa2,0x36,0x03,0xfe,0x21,
		0x8f,0xdb,0x99,0xb0,0x69,0x41,0xda,0x35,0x52,0x2f,0xe2,0x3f,0x01,0x5e,0x01,0x3d,
		0xea,0xd0,0x65,0x23,0xcd,0xcb,0x67,0x78,0x40,0x46,0x85,0xec,0x43,0x83,0x43,0xce,
		0xd7,0xd6,0xb2,0xb1,0x9d,0xf7,0x8b,0x48,0x02,0x22,0x88,0x9a,0xa5,0xf5,0xaa,0xd5,
		0x5a,0x8e,0xed,0xee,0x5e,0x30,0x07,0x2c,0x83,0x1c,0x4d,0x54,0x3f,0x79,0x40,0x6b,
		0xd4,0xee,0x0d,0x30,0x85,0x68,0x53,0x76,0x3f,0x30,0x72,0xe0,0xbb,0x2f,0xb4,0x1d,
		0xa3,0x2f,0x8a,0x02,0x55,0xb5,0xa6,0x73,0xa3,0xd7,0xde,0x3d,0xbd,0xe3,0xcf,0x4f,
		0x2f,0xaa,0x85,0x20,0x86,0x45,0x21,0x2e,0x29,0x96,0xb4,0x8b,0xa4,0x8a,0xb8,0x02,
		0xa4,0x2d,0x15,0x2e,0x0c,0xd7,0xdd,0xa6,0x20,0x1f,0xcd,0x9e,0xe7,0xd0,0xe4,0x24,
		0x53,0xd3,0x53,0xf2,0xc7,0x8d,0xbf,0xf5,0x9e,0xd7,0x4e,0x3a,0xad,0xe7,0xb2,0xcd,
		0x5c,0x3c,0x55,0x0e,0x88,0xce,0x11,0x30,0x28,0xc1,0x16,0x13,0xa4,0x42,0x84,0x06,
		0xc2,0x73,0xf5,0x41,0xde,0x69,0x8e,0x71,0xe0,0xb6,0x11,0x7a,0x22,0x6c,0xae,0xae,
		0x62,0xc9,0xfe,0x97,0x36,0x52,0x92,0xbf,0x97,0x90,0xba,0x3f,0x8e,0xc3,0x30,0x31,
		0x68,0x03,0x66,0x80,0x0e,0x0d,0x72,0xe7,0xab,0x27,0x19,0x38,0xf2,0x02,0x8f,0xaa,
		0xd2,0xcc,0x33,0xae,0xbc,0x7f,0x06,0x4b,0x09,0x03,0x03,0x0a,0x90,0xef,0x85,0x14,
		0x34,0x81,0xf3,0xb8,0xbb,0x05,0xb9,0x0b,0x50,0x80,0xa2,0x28,0x38,0x7f,0xf6,0x2c,
		0x4f,0x4f,0x4e,0x72,0xf8,0xe0,0x41,0xbe,0xbd,0xf8,0x39,0x8f,0xcc,0x9c,0xa2,0xb9,
		0xfb,0x3e,0x44,0x24,0x00,0xf3,0x19,0xed,0x39,0x25,0x95,0x0a,0x2a,0x86,0x0e,0x01,
		0x77,0x08,0x08,0x40,0xc3,0x8c,0xe9,0x4e,0xc9,0x5b,0x5b,0x81,0xc7,0x5a,0x05,0x3f,
		0x5e,0xfe,0x15,0x19,0x1c,0x60,0xe8,0xe1,0xbd,0xe0,0x5c,0x1b,0xe4,0xab,0x2e,0x6e,
		0xe5,0x5f,0x22,0x3e,0x41,0x30,0xc2,0x22,0xf8,0xd2,0x00,0xb3,0x84,0x88,0x32,0x30,
		0x3c,0xcc,0xd8,0xae,0x5d,0x3c,0x55,0xcf,0xe8,0xee,0x9f,0xa0,0xbb,0xb6,0xce,0xad,
		0xab,0x57,0x21,0xc5,0x9e,0xc2,0xa5,0x88,0x86,0x59,0xc0,0xcf,0x52,0xf2,0x06,0x59,
		0x47,0xb1,0x4b,0x66,0xe9,0xc9,0xcd,0x8d,0x4d,0xbf,0xb4,0xb1,0x4e,0xe3,0xd8,0x8b,
		0xec,0x39,0xf2,0x3c,0x86,0x10,0xba,0x1d,0x7e,0xf8,0xe0,0x0c,0xf9,0xfc,0xef,0x90,
		0x52,0x14,0x58,0x55,0xb0,0x9b,0x80,0xef,0x73,0xb5,0xca,0x90,0x05,0x52,0x7a,0xa2,
		0xb5,0xb0,0xe0,0x9e,0x99,0x98,0x88,0x15,0xd2,0xf1,0xd0,0xc8,0xc0,0xed,0x31,0xe1,
		0x44,0x4f,0x8c,0x2a,0x08,0x70,0x2b,0x11,0x16,0x0d,0x52,0x01,0xf8,0xb2,0xaf,0xb5,
		0x14,0xec,0x4a,0x13,0x59,0x7e,0x3b,0xe5,0xb7,0xa7,0xad,0xb4,0x14,0x60,0xce,0xf7,
		0x33,0x7e,0xd0,0xc1,0x56,0x06,0x37,0x0d,0x86,0xc0,0x2e,0x0b,0x54,0xe7,0x28,0x59,
		0x07,0x7c,0x0b,0x30,0xa8,0x04,0xf9,0x45,0x90,0x53,0x75,0xd2,0xcb,0x06,0xd7,0xa1,
		0x3a,0xa7,0x64,0x95,0xc1,0x61,0x45,0x3e,0x76,0x74,0xbe,0x0e,0xe4,0x8f,0x1b,0x8c,
		0x27,0xcc,0x8c,0xfe,0x77,0xff,0x4f,0x3f,0xf8,0xca,0x28,0xe7,0x05,0xfe,0x72,0x64,
		0xa3,0x9e,0xf4,0x73,0x17,0x5b,0x75,0xc8,0x37,0x42,0xb8,0x90,0x88,0xd7,0x02,0xd2,
		0x83,0xea,0x86,0xa7,0xfe,0x5b,0x20,0x94,0xe5,0x76,0x51,0xff,0x01,0x9b,0xee,0x4d,
		0xeb,0x75,0xc2,0x3d,0x26,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,
		0x82);
}
