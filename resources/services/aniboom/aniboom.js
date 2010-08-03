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
	this.website = "http://www.aniboom.com/";
	this.ID = "aniboom.com";
	this.caption = "Aniboom";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_GET_FLV = "http://www.aniboom.com/animations/player/handlers/animationDetails.aspx?mode=&movieid=%1";
	// init result
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get the video title
	result.title = copyBetween(html, "<meta name=\"title\" content=\"", "\" />");
	// video URL
	var videoId = copyBetween(html, 'animationId=', '"');
	// download the file with the real url
	var info = http.downloadWebpage(strFormat(URL_GET_FLV, videoId));
	// get the video url
	result.URL = cleanUrl(copyBetween(info, "flv=", "&"));
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0b,0x13,0x00,0x00,0x0b,
		0x13,0x01,0x00,0x9a,0x9c,0x18,0x00,0x00,0x02,0x46,0x49,0x44,0x41,0x54,0x38,0x11,
		0x75,0x93,0x4d,0x6b,0x13,0x51,0x14,0x86,0x9f,0x49,0x26,0x33,0x36,0xd1,0x62,0x4c,
		0x9b,0xb6,0x6a,0x6d,0x28,0x62,0x15,0x94,0x2a,0x42,0x71,0x21,0x6e,0x84,0xae,0xdc,
		0xb8,0xa8,0xe8,0x52,0x10,0xdd,0x09,0xfe,0x8b,0xfe,0x03,0xff,0x80,0x20,0x18,0xa5,
		0x9b,0x6e,0xda,0x42,0x21,0x48,0xa1,0xd5,0x22,0x88,0xc5,0x7e,0x68,0xd3,0x34,0x62,
		0x69,0xa7,0x49,0x9b,0x34,0x99,0x49,0x27,0xc9,0x4c,0x9c,0x33,0x81,0xd0,0x68,0x7c,
		0xe1,0x1d,0x0e,0xe7,0x9c,0x79,0xe6,0xde,0x3b,0xe7,0x2a,0x0d,0x4f,0x74,0x90,0xeb,
		0xa5,0x6d,0xa7,0x59,0xd2,0x83,0x0a,0x01,0x45,0xe9,0xd0,0x05,0x81,0x8e,0x59,0x2f,
		0xf9,0xfa,0xd3,0x2a,0x7d,0xef,0x96,0x7d,0x4b,0xfc,0x3f,0x75,0x04,0x64,0x76,0x0d,
		0xde,0x64,0x0b,0x94,0xeb,0x8e,0x6f,0x89,0x25,0xd7,0x49,0x1d,0x01,0x53,0xab,0x19,
		0x7e,0x38,0x21,0x64,0x73,0x62,0x89,0x25,0xd7,0x49,0xff,0x00,0x32,0xfb,0x07,0x4c,
		0xd7,0x4e,0x93,0xb7,0xeb,0xad,0x7e,0x89,0x25,0x27,0xb5,0xbf,0xd5,0x06,0x70,0x1c,
		0x97,0xa9,0xb5,0x2c,0x5f,0x0f,0x4c,0xbf,0x2f,0xaa,0xa9,0x88,0x45,0x92,0x93,0x9a,
		0xf4,0x9c,0x54,0x1b,0x20,0xed,0xed,0x73,0xbe,0x7a,0xaa,0xf5,0xf5,0x89,0x44,0x0c,
		0xb1,0x48,0x56,0x21,0x35,0xe9,0x39,0xa9,0x36,0xc0,0xc2,0xf7,0x75,0x16,0x8c,0x92,
		0x5f,0xef,0xd1,0x55,0x5e,0xc6,0x15,0xdf,0x12,0x8b,0xa4,0x26,0x3d,0x27,0xd5,0x02,
		0xac,0x6d,0x65,0x49,0x9a,0x31,0x0e,0x77,0x2a,0x60,0x3a,0x3c,0xb9,0x18,0xe3,0x52,
		0x5f,0x2f,0x43,0xfd,0x71,0x9e,0x5e,0x8e,0x13,0x28,0x39,0x1c,0xae,0x96,0x48,0x1a,
		0x51,0xd6,0xd2,0xd9,0x16,0xc3,0x47,0x57,0x6b,0x35,0x16,0xd3,0x7b,0xa4,0xe6,0x0a,
		0x60,0x79,0x87,0x17,0x0e,0x90,0x0b,0x97,0x99,0x39,0x36,0xb8,0x35,0x92,0x60,0xa2,
		0x47,0x23,0x59,0x84,0xcc,0x2f,0x9b,0xd4,0xfe,0x2e,0x8b,0xd1,0x0a,0xc3,0x83,0x03,
		0x68,0xa1,0x10,0x3e,0x60,0x2f,0x97,0x67,0xda,0x8a,0x60,0x56,0xf3,0x4d,0xb2,0xe5,
		0xf2,0x36,0xf5,0x9b,0xf9,0x6e,0x9d,0xd1,0x2f,0x9b,0xdc,0x8c,0x9a,0x5c,0x09,0x9f,
		0x23,0x43,0x09,0xf3,0xd8,0x61,0x7a,0x4b,0xe3,0xbe,0x91,0x67,0xf0,0x42,0x3f,0x4a,
		0xbd,0x5e,0x6f,0xcc,0x6e,0x1b,0x4c,0x7c,0xdc,0xc6,0x34,0xaa,0xb0,0x6b,0x43,0xc1,
		0x01,0xb7,0x39,0xc6,0x32,0xc0,0x5d,0x5a,0x90,0x90,0x37,0xce,0xc5,0x4a,0xf3,0xd7,
		0x46,0xf4,0x20,0xc9,0xe7,0xd7,0x18,0x1f,0x3d,0x8f,0x5a,0x28,0x16,0x99,0xdc,0x38,
		0xc0,0xa4,0x41,0xb0,0x4f,0xe3,0xd5,0x9d,0x18,0x8f,0x07,0xe3,0xcc,0x2d,0x6f,0xf3,
		0x61,0xa5,0xc2,0x66,0xce,0xf6,0x5f,0xb4,0xaa,0x4d,0xa0,0x2c,0xd1,0xb4,0x1d,0x26,
		0x67,0x77,0x18,0x1b,0x8a,0xa0,0x1e,0xd5,0x1c,0x96,0x72,0x65,0x7f,0xe9,0xc3,0x67,
		0x74,0xee,0x69,0x36,0xb7,0x13,0xbd,0xbe,0x5f,0x8c,0x9b,0xbc,0x4f,0xad,0xb0,0xb0,
		0xd7,0xc5,0xe7,0xad,0x22,0x9b,0x86,0x85,0x5d,0x6f,0xce,0xc1,0x52,0xba,0x40,0xae,
		0x74,0x8c,0xaa,0xa9,0x2a,0x37,0xce,0x86,0x59,0x3f,0xaa,0xf0,0x50,0xb7,0xb8,0x7b,
		0xf5,0xba,0x0f,0x93,0x47,0xb4,0x3b,0xc2,0xb3,0x07,0x63,0x3c,0x2a,0x5b,0x7c,0xdb,
		0xc8,0xb0,0xf8,0x13,0x66,0xb2,0x3a,0x4b,0xe9,0x22,0x23,0x03,0x11,0xf4,0x90,0x8a,
		0xe2,0xba,0x6e,0xa3,0xea,0xed,0x57,0xb8,0x21,0xcf,0x6a,0xb0,0xf5,0x67,0x85,0xd1,
		0x26,0xb9,0xf9,0x35,0xef,0x8a,0x3b,0x5e,0x7f,0x20,0xa0,0xa0,0x79,0xe7,0xf2,0x07,
		0x1a,0x07,0x1b,0xc1,0xa1,0xee,0x77,0x2c,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,
		0xae,0x42,0x60,0x82);
}