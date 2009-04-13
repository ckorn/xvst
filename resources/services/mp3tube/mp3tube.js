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
	this.website = "http://www.mp3tube.net/";
	this.ID = "mp3tube.net";
	this.caption = "Mp3Tube";
	this.adultContent = false;
	this.musicSite = true;
}

function getVideoInformation(url)
{
	const URL_GET_MP3 = "http://storm.mp3tube.net/d.php?file=%1.tube";
	// init result
	var result = new VideoDefinition();
	// set this as "audio file"
	result.isAudioFile = true;
	result.extension = ".mp3";
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get mp3 title
	result.title = copyBetween(html, "<TITLE>", "- Mp3Tube</TITLE>");
	// get mp3 id
	var fileId = copyBetween(html, "swf?id=", "&");
	// clear and get the final url
	result.URL = cleanUrl(strFormat(URL_GET_MP3, fileId));
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x02,0xf0,0x49,0x44,0x41,0x54,0x78,0x9c,0x55,0xc1,0x4f,0x6f,0x54,
		0x55,0x18,0xc0,0xe1,0xdf,0x79,0xcf,0xb9,0x73,0xe7,0x76,0xa6,0xb6,0x96,0xa1,0x4c,
		0x43,0x5b,0x2d,0x05,0xac,0x08,0x41,0x12,0x0a,0x18,0xd3,0x08,0x6d,0x0d,0x09,0xc4,
		0x80,0x10,0x52,0x82,0xc6,0x60,0xe2,0x87,0x70,0xed,0xd6,0xef,0x60,0xa2,0x31,0x2e,
		0xdc,0x98,0xb8,0x63,0x61,0xa2,0x2e,0x9a,0x28,0x8b,0xaa,0x94,0x42,0xed,0xb4,0xd3,
		0x92,0x1a,0xe9,0xb4,0xd8,0xd2,0xa1,0xbd,0x73,0xef,0x3d,0x7f,0x58,0xfb,0x3c,0x2a,
		0xf8,0x22,0xd8,0xcc,0x80,0x02,0x1d,0x5b,0x3c,0x19,0x3e,0x24,0x18,0x04,0x6f,0x41,
		0x47,0x16,0x28,0xb0,0x05,0xf8,0xa0,0x29,0x95,0x14,0x1e,0x41,0xa1,0xf1,0xc1,0x22,
		0xd8,0x3d,0x8c,0x01,0x63,0xc0,0xdb,0x02,0x45,0x41,0xa4,0x3c,0xc1,0x82,0x36,0xe0,
		0x3a,0x00,0x31,0x5a,0x62,0x4a,0x51,0x89,0x34,0xcd,0x11,0x14,0x69,0x27,0x45,0x14,
		0xa8,0x60,0x5d,0x58,0x5f,0x7c,0xce,0xc6,0xd3,0xff,0x18,0x1c,0xe9,0xe6,0xd0,0x70,
		0x0d,0x82,0x60,0x53,0x10,0x0d,0x52,0x82,0xa2,0x03,0x26,0x02,0x15,0x01,0x0a,0xf2,
		0x2c,0xa3,0x54,0x16,0x9c,0xf7,0xa8,0xf0,0x3c,0x84,0xa9,0x89,0xbb,0x8c,0x0c,0x8d,
		0x71,0xe1,0x9d,0xb7,0x38,0x72,0xbc,0xce,0xab,0x7d,0x75,0xbe,0xf9,0xee,0x5b,0x8e,
		0xbd,0xf9,0x3a,0xdd,0x5d,0x15,0x4e,0x9e,0x18,0x47,0xe2,0x0e,0x63,0x27,0x86,0x10,
		0x65,0x88,0xaa,0x80,0xa4,0x10,0x12,0x0c,0x0a,0xca,0x51,0x95,0xe1,0xfa,0x51,0xda,
		0x5b,0x39,0x4b,0x9d,0x27,0x7c,0xf2,0xd9,0x38,0x93,0x17,0xa7,0x68,0x67,0x9b,0x2c,
		0x3e,0x58,0xe1,0x9f,0x66,0xc1,0xc1,0x01,0x61,0xfe,0xc1,0x7d,0x8c,0xea,0x63,0xe6,
		0xe3,0x69,0x48,0x60,0x73,0x7d,0x07,0xa1,0x0c,0xc3,0xa3,0x07,0x68,0xae,0x2d,0xd0,
		0xdf,0xff,0x0a,0xc6,0x58,0x9a,0xab,0x4f,0xf8,0x60,0xe6,0x1c,0xb7,0xef,0x5e,0x61,
		0xa0,0x76,0x88,0xb9,0xdf,0x1e,0x12,0x49,0xcc,0xfb,0x97,0xa6,0xf9,0xfa,0xab,0xef,
		0xb9,0x3f,0xfb,0x88,0xb0,0x67,0x09,0x79,0x8c,0x60,0x3c,0xa7,0xce,0x9c,0x62,0xbf,
		0xd3,0x21,0x4d,0x33,0xac,0xb5,0xac,0xac,0x2e,0x03,0xa0,0x8c,0xe5,0xe6,0x8d,0x0f,
		0xf9,0xfb,0xd1,0x0a,0x3b,0xcf,0x76,0xa8,0x24,0x09,0x17,0xce,0x5e,0xe4,0xc7,0x1f,
		0xee,0xa1,0xe2,0x0a,0x46,0x25,0x88,0xb5,0xc2,0xb9,0xf3,0x97,0xd8,0x6a,0xa5,0xa4,
		0x7b,0x42,0x64,0xfa,0x68,0x2c,0x6f,0x80,0x40,0xee,0x14,0x07,0x5f,0x8b,0x38,0x7c,
		0x78,0x88,0xe5,0xa5,0x06,0x8d,0x46,0x83,0x99,0x9b,0x77,0xf8,0xe5,0xe7,0x59,0x9a,
		0x0b,0x4b,0xac,0xaf,0x6e,0x23,0xa2,0xe0,0xed,0xd3,0x07,0x68,0xb5,0xfe,0xc0,0x84,
		0x40,0xba,0x6d,0xa9,0x4a,0x85,0x95,0xf9,0x16,0x91,0x37,0x60,0xe0,0xd6,0x9d,0xf3,
		0xac,0x35,0xb7,0x99,0xfd,0x69,0x83,0x37,0x4e,0x43,0xbd,0xd6,0xcb,0xaf,0xf7,0x1e,
		0x33,0xd0,0xdf,0x83,0x84,0xe0,0x31,0x25,0x18,0x19,0x1d,0xa5,0xb5,0xf5,0x8c,0x38,
		0xe9,0xa2,0xb0,0x96,0xe6,0xea,0x2a,0xa2,0xc1,0xee,0x3b,0xae,0xdd,0xba,0x4a,0x63,
		0xed,0x31,0x39,0x2f,0x78,0xfa,0xaf,0xe7,0xa3,0x4f,0x6f,0x33,0xf7,0xf0,0x77,0xb6,
		0xf7,0x36,0x31,0x4a,0x1c,0x94,0x84,0x33,0xe3,0x67,0x59,0x98,0x5b,0xa4,0x56,0xaf,
		0xa3,0x13,0x43,0x6b,0x73,0x83,0x10,0xc0,0xf4,0x68,0x7a,0x13,0xb8,0x7c,0x7d,0x82,
		0xa5,0xf5,0x3f,0xa9,0x0d,0x5e,0xe7,0xda,0xb1,0x49,0xa6,0xae,0x4e,0x90,0x94,0x23,
		0x8c,0x68,0x45,0xf6,0x02,0x8e,0x9f,0x3c,0xca,0xfc,0x5f,0x4d,0x72,0x95,0x51,0xa4,
		0xbb,0xc4,0x85,0x41,0xa9,0x40,0x50,0x1e,0x55,0xd6,0x7c,0xf1,0xe5,0xe7,0x54,0x2a,
		0x42,0xbb,0x6d,0xa9,0x6a,0x43,0xb5,0x16,0x11,0x02,0x88,0xb3,0x05,0x71,0x15,0xde,
		0x9b,0x7e,0x97,0xde,0x7a,0xc2,0x6e,0xbe,0xc9,0x91,0xb1,0x41,0xa6,0x2f,0x4f,0x82,
		0x52,0xa0,0x14,0x9d,0x22,0xa5,0xab,0x1b,0x82,0x40,0xb5,0xc7,0x50,0x38,0x47,0xc0,
		0x83,0x2a,0x50,0xce,0x76,0x42,0xf0,0x11,0x5a,0x84,0x74,0x17,0x92,0x0a,0xec,0x77,
		0xa0,0xab,0x0a,0x21,0x78,0x1c,0x0a,0xad,0x1d,0x9e,0x02,0xe7,0x02,0xa2,0x62,0xb4,
		0x68,0xac,0xcb,0x31,0x3a,0xa0,0x42,0x70,0xc1,0x3b,0x07,0x4a,0xe3,0x3c,0x18,0x23,
		0xa8,0xc0,0xff,0x04,0x1c,0xb9,0x6d,0x13,0x47,0x09,0x10,0x93,0x67,0xe0,0x7c,0x46,
		0x92,0x08,0xe2,0xac,0x45,0xb4,0x22,0x48,0x86,0x98,0x82,0x8e,0x6b,0x13,0x94,0x25,
		0xcb,0x33,0x00,0xf2,0x0c,0x94,0xd2,0x94,0xa2,0x12,0x20,0x14,0x79,0x20,0x32,0x90,
		0x94,0x63,0x40,0xf1,0x12,0x6a,0x4d,0x4d,0xea,0x01,0xe1,0x1e,0xcc,0x00,0x00,0x00,
		0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82);
}
