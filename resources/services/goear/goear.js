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
	this.website = "http://www.goear.com/";
	this.ID = "goear.com";
	this.caption = "GoEar";
	this.adultContent = false;
	this.musicSite = true;
}

function getVideoInformation(url)
{
	const URL_GET_XML = "http://www.goear.com/files/xmlfiles/%1/secm%2.xml";
	// init result
	var result = new VideoDefinition();
	// set this as "audio file"
	result.isAudioFile = true;
	result.extension = ".mp3";
	// get sound id
	var songId = copyBetween(url, "/listen/", "/");
	var subId = strCopy(songId, 0, 1);
	// download xml
	var http = new Http();
	var xml = http.downloadWebpage(strFormat(URL_GET_XML, subId, songId));
	// get sound artits and title
	result.title = copyBetween(xml, 'artist="', '"');
	if (result.title != "") result.title += " - ";
	result.title += copyBetween(xml, 'title="', '"');
	// get sound url
	result.URL = copyBetween(xml, 'path="', '"');
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0b,0x13,0x00,0x00,0x0b,
		0x13,0x01,0x00,0x9a,0x9c,0x18,0x00,0x00,0x02,0x5a,0x49,0x44,0x41,0x54,0x38,0x11,
		0x75,0x53,0xcf,0x4b,0x1b,0x51,0x10,0x9e,0x9a,0x92,0x0d,0x1a,0x45,0x48,0xea,0x21,
		0xa5,0xa5,0x94,0x7a,0x48,0x82,0xa0,0xa0,0x04,0x41,0x7a,0x11,0x04,0x15,0xf4,0x22,
		0x28,0xfe,0x11,0x5e,0x44,0xa8,0x96,0xea,0xa5,0x14,0x0f,0x82,0x1e,0x5a,0x82,0x07,
		0x4b,0xa9,0xd0,0x58,0x21,0xe0,0x45,0x68,0x91,0x80,0x10,0xb0,0xd5,0x60,0xb0,0x6a,
		0xa3,0x48,0x88,0x36,0x48,0x42,0x7e,0x94,0xc4,0x24,0x9b,0x34,0xd9,0x6c,0xbe,0xbe,
		0x7d,0x36,0x4b,0xcd,0x8f,0x39,0xbc,0x7d,0xf3,0xe6,0x9b,0x6f,0xbf,0x99,0x37,0x8f,
		0x50,0xc7,0x12,0x89,0x2c,0x96,0x96,0xf6,0xe0,0x76,0xff,0xaa,0x83,0xb8,0x3d,0x6e,
		0xa0,0x1a,0x96,0x48,0x94,0x68,0x6e,0xce,0x4d,0xc9,0xa4,0x4c,0x7d,0x7d,0x8f,0x39,
		0x42,0x14,0x51,0x03,0xc9,0x8e,0x2a,0xe9,0xe3,0xf1,0x2c,0x06,0x07,0x3f,0xa0,0xa7,
		0x67,0x15,0x37,0x37,0x79,0x84,0x42,0x19,0xcc,0xce,0xba,0x70,0x7e,0x1e,0xe7,0xd0,
		0x54,0x2a,0x0f,0x59,0x2e,0xa9,0x69,0x77,0x08,0x72,0xb9,0x22,0x26,0x27,0x9d,0xd0,
		0x6a,0x5f,0xc1,0xeb,0x0d,0x21,0x12,0xc9,0xa0,0xbf,0xff,0x23,0xd6,0xd6,0xbc,0x28,
		0xb1,0x1c,0x97,0xeb,0x12,0x1e,0x4f,0x08,0xc5,0x62,0x1d,0x02,0xa7,0xd3,0x07,0x41,
		0x78,0x89,0xe9,0xe9,0xaf,0x48,0xa7,0xf3,0x18,0x1d,0x75,0x60,0x6c,0x6c,0x13,0x92,
		0x24,0x43,0x89,0x4d,0x4d,0x7d,0x41,0x34,0x2a,0xaa,0x7f,0x57,0x36,0xaa,0x02,0x45,
		0x7a,0x47,0x87,0x1d,0xad,0xad,0xaf,0x71,0x76,0x16,0x83,0xc3,0x71,0x0a,0xa3,0xf1,
		0x0d,0x4e,0x4e,0x22,0xcc,0x8f,0xa3,0xb3,0x73,0x15,0x87,0x87,0xe1,0x3b,0xc9,0x8a,
		0xa3,0x36,0x91,0x49,0x26,0xbf,0x3f,0x4c,0x43,0x43,0x16,0x6a,0x6e,0x16,0x68,0x79,
		0xf9,0x1b,0x75,0x75,0x3d,0xa2,0xb6,0x36,0x3d,0xad,0xac,0x7c,0xa7,0xee,0x6e,0x13,
		0x59,0x2c,0x0f,0xaa,0x1a,0xa9,0x12,0xb8,0x5c,0x57,0x24,0x08,0xf7,0x68,0x7c,0xdc,
		0x42,0xb1,0x98,0x48,0x81,0x40,0x8c,0x86,0x87,0x9f,0x11,0x6b,0x24,0xed,0xee,0x5e,
		0xd1,0xc0,0xc0,0x53,0xd2,0xe9,0x34,0xb5,0x09,0x64,0x99,0xc8,0xeb,0x0d,0xb3,0x72,
		0x04,0x32,0x99,0x9a,0x89,0x75,0x9c,0x01,0x35,0xd4,0xd2,0xa2,0x63,0x04,0x39,0x62,
		0x3d,0x20,0xbd,0x5e,0x5b,0x95,0xac,0x1c,0xa8,0x0a,0x24,0xa9,0xc4,0x01,0x82,0x70,
		0x9f,0x1a,0x1b,0x15,0xf0,0xad,0xdf,0xd4,0xa4,0x65,0xc4,0x44,0xa2,0x28,0xf1,0x78,
		0xe5,0xc2,0x09,0x34,0x4c,0x99,0xd5,0x6a,0x64,0x31,0x89,0xa2,0xd1,0x0c,0x99,0xcd,
		0xca,0x9e,0xe8,0xf8,0x38,0x46,0x06,0x43,0x23,0xb5,0xb7,0x1b,0x68,0x67,0x27,0x40,
		0x85,0x02,0x93,0x5a,0x61,0xaa,0x02,0x9b,0xed,0x21,0xc9,0xac,0x96,0xbd,0xbd,0x6b,
		0xde,0x38,0x9b,0xed,0x09,0x6d,0x6d,0xf9,0x48,0xab,0x6d,0xa0,0xf9,0xf9,0xe7,0xac,
		0x0f,0x97,0xe4,0xf3,0x29,0xa5,0x55,0x58,0xf9,0x5e,0x2e,0x2e,0x7e,0xb3,0x2b,0x5c,
		0x84,0xd5,0xfa,0x0e,0x99,0x4c,0x01,0x47,0x47,0x61,0x98,0x4c,0x8b,0xb0,0xdb,0x3d,
		0x1c,0xb2,0xb9,0xf9,0x13,0x0b,0x0b,0xbb,0x48,0x24,0xfe,0x94,0x53,0xf8,0x57,0x9d,
		0x03,0x26,0x0f,0x33,0x33,0x3b,0xac,0xda,0x17,0xd8,0xd8,0x38,0xe5,0xc1,0xf5,0xf5,
		0x1f,0xe8,0xed,0x7d,0x8f,0x83,0x83,0x6b,0xee,0x07,0x83,0x37,0xf5,0x09,0x14,0x44,
		0x3a,0x2d,0x61,0x64,0xe4,0x13,0xcc,0xe6,0xb7,0x6c,0x8c,0xb3,0x3c,0x69,0x7f,0x3f,
		0x8c,0xed,0x6d,0x3f,0x9b,0x7f,0xee,0x56,0x2d,0xaa,0x82,0x72,0x24,0x18,0x4c,0x62,
		0x62,0xe2,0x33,0x57,0x93,0x4a,0xdd,0xca,0x55,0x46,0xf9,0xff,0x07,0x54,0x52,0x1e,
		0xc6,0x3f,0xfb,0x0b,0xdb,0xdc,0x55,0x57,0x93,0x3a,0xb4,0xb0,0x00,0x00,0x00,0x00,
		0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82);
}
