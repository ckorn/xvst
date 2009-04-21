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
	this.website = "http://www.badjojo.com/";
	this.ID = "badjojo.com";
	this.caption = "BadJojo";
	this.adultContent = true;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_GET_INFO = "http://www.badjojo.com/noheader.php?action=getVideoStyle&videoId=%1";
	// init result
	var result = new VideoDefinition();
	// get videoId
	var videoId = copyBetween(url, ".com/", "/");
	// download information
	var http = new Http();
	var info = http.downloadWebpage(strFormat(URL_GET_INFO, videoId));
	// get video info
	result.title = copyBetween(info, "Title:", "Video ID:");
	result.URL = copyBetween(info, "Video file:", "*/");
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x0e,0x00,0x00,0x00,0x0e,0x08,0x02,0x00,0x00,0x00,0x90,0x2a,0xba,
		0x86,0x00,0x00,0x01,0x2f,0x69,0x43,0x43,0x50,0x49,0x43,0x43,0x20,0x50,0x72,0x6f,
		0x66,0x69,0x6c,0x65,0x00,0x00,0x78,0x01,0x63,0x60,0x60,0x32,0x70,0x74,0x71,0x72,
		0x65,0x12,0x60,0x60,0xc8,0xcd,0x2b,0x29,0x0a,0x72,0x77,0x52,0x88,0x88,0x8c,0x52,
		0x60,0xbf,0xc0,0xc0,0xc1,0xc0,0xcd,0x20,0xcc,0x60,0xcc,0x60,0x9d,0x98,0x5c,0x5c,
		0xe0,0x18,0x10,0xe0,0xc3,0x00,0x04,0x79,0xf9,0x79,0xa9,0x20,0x1a,0x15,0x7c,0xbb,
		0xc6,0xc0,0x08,0x12,0xb9,0xac,0x0b,0x32,0x0b,0x55,0x8e,0x20,0x8f,0x2b,0xb9,0xa0,
		0xa8,0x04,0xa8,0xea,0x0f,0x10,0x1b,0xa5,0xa4,0x16,0x27,0x33,0x30,0x30,0x1a,0x00,
		0xd9,0xd9,0xe5,0x25,0x05,0x40,0x71,0xc6,0x39,0x40,0xb6,0x48,0x52,0x36,0x98,0xbd,
		0x01,0xc4,0x2e,0x0a,0x09,0x72,0x06,0x8a,0x1f,0x01,0xb2,0xf9,0xd2,0x21,0xec,0x2b,
		0x20,0x76,0x12,0x84,0xfd,0x04,0xc4,0x2e,0x02,0x7a,0x02,0xa8,0xe6,0x0b,0x48,0x7d,
		0x3a,0x98,0xcd,0xc4,0x01,0x62,0x27,0x41,0xd8,0x32,0x20,0x76,0x49,0x6a,0x05,0xc8,
		0x5e,0x06,0xe7,0xfc,0x82,0xca,0xa2,0xcc,0xf4,0x8c,0x12,0x05,0x23,0x03,0x03,0x03,
		0x05,0xc7,0x94,0xfc,0xa4,0x54,0x85,0xe0,0xca,0xe2,0x92,0xd4,0xdc,0x62,0x05,0xcf,
		0xbc,0xe4,0xfc,0xa2,0x82,0xfc,0xa2,0xc4,0x92,0xd4,0x14,0xa0,0x5a,0x88,0xfb,0x40,
		0xba,0x18,0x04,0x21,0x0a,0x41,0x21,0xa6,0x61,0x68,0x69,0x69,0xa1,0x09,0x16,0xa5,
		0x22,0x01,0x8a,0x07,0x88,0x71,0x9f,0x03,0xc1,0xe1,0xcb,0x28,0x76,0x06,0x21,0x86,
		0xb0,0x28,0xb9,0xb4,0xa8,0x0c,0xca,0x63,0x64,0x32,0x66,0x60,0x20,0xc4,0x47,0x98,
		0x31,0x47,0x82,0x81,0xc1,0x7f,0x29,0x03,0x03,0xcb,0x1f,0x84,0x98,0x49,0x2f,0x03,
		0xc3,0x02,0x1d,0x06,0x06,0xfe,0xa9,0x08,0x31,0x35,0x43,0x06,0x06,0x01,0x7d,0x06,
		0x86,0x7d,0x73,0x00,0xc3,0xaf,0x50,0x6f,0xe6,0xb3,0xe5,0xe9,0x00,0x00,0x00,0x09,
		0x70,0x48,0x59,0x73,0x00,0x00,0x0b,0x13,0x00,0x00,0x0b,0x13,0x01,0x00,0x9a,0x9c,
		0x18,0x00,0x00,0x01,0x5c,0x49,0x44,0x41,0x54,0x28,0x15,0x35,0x91,0x51,0x6e,0x14,
		0x41,0x0c,0x44,0x5d,0x76,0x67,0xff,0x23,0x56,0xe1,0x60,0x64,0x73,0x03,0x12,0x10,
		0x1c,0x07,0x09,0x72,0x04,0x82,0xb8,0x58,0xb4,0x5c,0x20,0xdb,0xb6,0x79,0x5e,0x44,
		0x6b,0x34,0x3d,0xdd,0x53,0x2e,0x57,0x95,0x75,0x7b,0xff,0x71,0x75,0x94,0xca,0xad,
		0x25,0xcf,0x96,0x5b,0xa5,0x8c,0xcb,0x66,0x39,0xef,0x8b,0xd9,0x92,0xef,0x75,0xc5,
		0xb9,0xb1,0xda,0x72,0x36,0x2b,0x5b,0xd1,0x7b,0x9b,0xa4,0xe4,0x4e,0x76,0x23,0x75,
		0xd7,0x72,0xf8,0xe6,0xaf,0x4b,0x56,0x3d,0x9b,0x38,0xaa,0x56,0xd8,0x5e,0x16,0x51,
		0xe1,0x6d,0x3d,0xad,0xf6,0x1a,0xc2,0x36,0x25,0x8c,0x0e,0x1a,0x19,0xd6,0x95,0x73,
		0xbb,0xf8,0x68,0x6f,0x55,0xf5,0xd0,0x1f,0xd6,0xb4,0x19,0xf0,0x02,0x07,0x7a,0x5b,
		0x85,0x45,0x59,0x86,0x3a,0x85,0x09,0x9f,0x7e,0xcd,0x93,0x0b,0x1f,0x30,0xb8,0x1a,
		0x3e,0x70,0x7f,0x7e,0x7e,0xb7,0xb9,0xb0,0xbb,0x0f,0x9f,0x3c,0x54,0x74,0xb6,0x9d,
		0xb6,0x16,0x00,0xfc,0x82,0xc4,0x2b,0x52,0x61,0x03,0x77,0x7c,0xf8,0x7a,0x77,0x7a,
		0x7a,0xfd,0xfd,0xa3,0x68,0xf6,0x5f,0xf7,0x56,0xae,0x02,0x5e,0x45,0x35,0x56,0xf9,
		0x41,0xc9,0xf9,0xe5,0x1b,0x90,0xe3,0xe9,0x29,0x7a,0x5c,0xa6,0x63,0x00,0x4d,0x90,
		0xa2,0x3b,0x3a,0x46,0x30,0x19,0x92,0xd0,0x80,0xde,0xdf,0x7f,0x39,0xff,0x7a,0x76,
		0x8b,0x0b,0x45,0xb8,0x45,0x1d,0x36,0x29,0xcd,0xf2,0x84,0x4f,0x68,0x9a,0xab,0xf3,
		0xcb,0x33,0x32,0xde,0x3d,0x3c,0x4a,0x71,0x63,0xda,0x44,0x47,0xae,0x04,0x76,0x3c,
		0x7d,0x66,0x8b,0xa1,0xcb,0x0b,0x67,0x05,0x81,0x4f,0x52,0x04,0x87,0xb4,0x7e,0x73,
		0xea,0x34,0x93,0xf3,0x31,0x89,0x8a,0xda,0xa9,0x00,0xc5,0x9a,0xc0,0x6b,0x06,0xb7,
		0xf5,0xe6,0x3a,0x24,0x36,0xc6,0x9f,0xcd,0xb4,0xaa,0x4c,0x7e,0x10,0xc3,0xea,0x35,
		0x1d,0x48,0x9e,0x24,0x7a,0xcb,0x0e,0x80,0xe6,0x38,0x13,0xb5,0x7f,0x61,0x11,0x09,
		0x92,0x0c,0xb3,0xf4,0x25,0x8a,0x19,0xae,0xc5,0x95,0xfb,0x3a,0xf3,0x26,0xa8,0xfa,
		0x0b,0xa9,0x64,0xdc,0x5b,0x62,0x93,0x9e,0xf7,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,
		0x44,0xae,0x42,0x60,0x82);
}
