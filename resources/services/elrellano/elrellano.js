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
	this.website = "http://www.elrellano.com/";
	this.ID = "elrellano.com";
	this.caption = "elRellano";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	// check if is an audio or video file
	if (url.toString().indexOf("/audios_online/") != -1)		
		return getAudioDefinition(url);
	else // is a video file
		return getVideoDefinition(url);
}

function getVideoDefinition(url)
{
	const URL_XML = "http://www.elrellano.com/video_playlist.php?id=%1&sinespacios=%2";
	// video information
	var result = new VideoDefinition();
	// get id and sinespacios vars	
	var id = copyBetween(url, "/videos_online/", "/");
	var sinespacios = copyBetween(url, strFormat("/%1/", id), ".html");
	// download xml
	var http = new Http();
	var xml = http.downloadWebpage(strFormat(URL_XML, id, sinespacios));
	// get information block
	var track = copyBetween(xml, "<track>", "</track>");
	// get video information
	result.title = copyBetween(track, "<title>", "</title>");
	result.title = strReplace(result.title, "-", " ");
	result.URL = copyBetween(track, "<location>", "</location>");
	// return the video information
	return result;
}

function getAudioDefinition(url)
{
	const URL_MP3 = "http://www.elrellano.com";
	// video information
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// configure audio
	result.isAudioFile = true;
	result.extension = ".mp3";
	// get audio information
	result.title = copyBetween(html, "<title>", "-");
	result.URL = URL_MP3 + copyBetween(html, "'file','", "'");
	// return the audio information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0b,0x13,0x00,0x00,0x0b,
		0x13,0x01,0x00,0x9a,0x9c,0x18,0x00,0x00,0x03,0x89,0x49,0x44,0x41,0x54,0x38,0x11,
		0x55,0x52,0x5f,0x4c,0x5b,0x75,0x14,0xfe,0x6e,0xef,0x6d,0x6f,0x29,0xed,0xbd,0xf4,
		0x1f,0xed,0xba,0xce,0x0e,0x03,0xa3,0xa5,0xb2,0x52,0xf6,0x07,0x96,0x65,0x76,0xda,
		0xa4,0xe8,0x8c,0x3a,0x33,0x59,0xa3,0xd1,0x45,0x9c,0x83,0x2c,0x3e,0x4d,0xd4,0x3d,
		0xe8,0x93,0x89,0x2e,0x26,0x6c,0x31,0x3e,0x98,0x98,0x6d,0xfa,0xa0,0x4b,0xe6,0x02,
		0xb2,0x05,0x46,0xb7,0x07,0x82,0x48,0xdd,0x86,0x53,0x84,0x61,0x61,0x1d,0xa5,0xd4,
		0x4e,0x28,0xc5,0xf2,0xa7,0x2d,0xf4,0x1f,0xf4,0xd6,0x7b,0x9b,0x48,0xf0,0x3c,0x9c,
		0x9c,0x9c,0xef,0x9c,0xef,0x77,0xce,0x77,0x7e,0x24,0xb6,0x58,0x6f,0x6f,0xbf,0xc9,
		0x1f,0x49,0x68,0x22,0xa1,0x87,0xcb,0x5b,0xd2,0x9b,0x61,0xa1,0x50,0x50,0x0c,0x3f,
		0x8c,0xec,0x58,0x9e,0x0f,0x6d,0xa4,0xd3,0xe9,0x9c,0x00,0x88,0x36,0x51,0x3e,0xf0,
		0x8d,0x0e,0x0d,0x9c,0x74,0xd5,0x04,0xce,0x9c,0x69,0xff,0x6a,0x6b,0x5e,0x88,0xf9,
		0xe6,0x9d,0x9d,0x9d,0x3f,0x7a,0x5c,0x35,0x6c,0xe0,0xf4,0xa9,0xb7,0x3d,0xa9,0x42,
		0xc1,0x24,0xe4,0x37,0x09,0x86,0xee,0xdc,0x6b,0x9b,0x0b,0x4e,0x6a,0x4f,0xb9,0x0f,
		0x40,0x9c,0x8d,0x9d,0x7e,0xb3,0xa5,0xf5,0xa2,0x50,0xf0,0x9f,0xbd,0x76,0xa2,0xcd,
		0x11,0xf8,0x63,0xf0,0xe0,0xc9,0x23,0xb5,0xa8,0x78,0xc2,0x70,0xf0,0x56,0xaf,0xc7,
		0x29,0x60,0x9b,0x04,0x7d,0x7d,0x7d,0xcf,0x6b,0x34,0x6a,0x39,0x25,0x21,0xf1,0x51,
		0x8b,0x0b,0x52,0x6e,0xed,0x9d,0xfe,0x81,0xc1,0x4f,0x85,0xa2,0x8f,0xcf,0x9d,0xb7,
		0xad,0xc5,0x42,0x1d,0x6e,0xa7,0x15,0xcc,0x76,0x1d,0xe2,0x8b,0x0b,0x78,0xff,0x83,
		0xb3,0xe9,0xff,0x11,0x78,0x3c,0x37,0x13,0x22,0x11,0x81,0x7c,0x22,0x85,0xb9,0xd8,
		0x12,0x32,0xc9,0x45,0x9c,0xfb,0xbc,0xc3,0xf9,0x28,0xf4,0xcf,0x1b,0xfe,0xb1,0x31,
		0xef,0xfa,0xda,0x8a,0x46,0xa7,0xd7,0x00,0x62,0x0a,0x54,0x3e,0x8d,0x60,0x28,0x2a,
		0xf4,0x83,0x28,0x7a,0xde,0x59,0xac,0xd6,0x6f,0x48,0x6e,0xa3,0xa5,0xca,0xb4,0x0d,
		0xf6,0x3a,0x1b,0xf4,0x4a,0x1a,0x65,0x0c,0x83,0xfd,0x0d,0x07,0xd0,0x79,0xbd,0x07,
		0xa9,0xdc,0x3a,0x82,0xc1,0x30,0xe4,0x34,0x81,0x95,0x74,0x1e,0xbf,0x8c,0xce,0x34,
		0xcf,0x04,0x26,0x3a,0x49,0x5e,0x1c,0x4a,0xa5,0xd5,0xbe,0x35,0x3f,0x3b,0xdb,0xae,
		0x90,0xd1,0x12,0x7b,0x7d,0x3d,0xd4,0x4a,0x06,0x2c,0xcb,0xe0,0xd9,0x06,0x7e,0x64,
		0x71,0x1e,0xeb,0xa4,0x14,0xa3,0x13,0x33,0x90,0xc9,0xe5,0x88,0xf3,0xe2,0x8f,0x8c,
		0x4f,0x21,0x9e,0x58,0xf1,0xfe,0x70,0xf5,0x6a,0x54,0xd4,0xd5,0xd5,0x53,0x79,0xd7,
		0xeb,0xbd,0x5c,0xce,0xd2,0x72,0x9d,0x56,0x89,0x58,0xe4,0x31,0x18,0x19,0x05,0xeb,
		0xae,0x27,0x21,0x2d,0x65,0x51,0xa0,0x59,0x9e,0xb0,0x0c,0xfb,0xec,0x16,0x4c,0x04,
		0xfe,0xc6,0xcc,0x5f,0x11,0xbc,0xf0,0x74,0x3d,0xd8,0x12,0xea,0x4b,0x92,0x12,0x87,
		0xf8,0x09,0x94,0x19,0xf3,0x2e,0x03,0xe3,0x9b,0xf4,0xef,0xdf,0xa6,0x94,0x41,0xa1,
		0x50,0x20,0x9f,0xe7,0xb0,0x94,0xcc,0xa0,0xb6,0xa6,0x12,0x0c,0x53,0x8a,0xfb,0x0f,
		0xfc,0x48,0x2c,0xc5,0x60,0xd0,0xb0,0xf8,0x69,0xf8,0x01,0x7c,0xfe,0x69,0xb8,0xdd,
		0xcd,0xb0,0xd6,0xed,0x6d,0x27,0x7d,0xbe,0x5f,0x95,0x17,0x2f,0x7f,0xfb,0x9e,0x82,
		0xcc,0x1a,0xb3,0xb9,0x1c,0x28,0xfe,0x2e,0x12,0x9a,0x46,0x36,0x2f,0xc2,0xa5,0xef,
		0xbb,0xf1,0xd9,0x85,0xaf,0x41,0x52,0x12,0xf8,0xf9,0x97,0x45,0x5c,0x0e,0x46,0xbd,
		0x0a,0x43,0xf7,0xff,0x84,0xce,0xb0,0x1d,0xa5,0x25,0x69,0xb7,0x88,0x20,0x88,0xe8,
		0xf1,0xe6,0xa3,0x5f,0x94,0x28,0xcb,0x11,0x8d,0xc6,0x20,0xe6,0x2f,0xe1,0x9b,0x9a,
		0xc5,0x46,0x6a,0x05,0xf3,0xf3,0x73,0xc5,0x35,0x2a,0x8d,0x6a,0xd0,0x24,0x81,0xbe,
		0x9f,0x47,0x10,0x0a,0x47,0xe0,0x6c,0xdc,0x0d,0x73,0xb5,0x05,0xad,0xad,0x67,0x35,
		0x94,0x70,0x85,0x64,0x2a,0x45,0x4d,0x4d,0x87,0x51,0xbd,0xd3,0x88,0x1b,0x83,0xa3,
		0xf8,0xb0,0xed,0x38,0x9a,0x9f,0xa9,0x03,0x45,0x91,0x28,0x80,0x44,0x85,0x41,0x03,
		0x2e,0xbb,0x86,0x40,0x88,0xc5,0x6a,0x26,0x07,0x56,0x21,0xe7,0x75,0x61,0x11,0x0e,
		0x2f,0xa0,0x48,0x40,0x70,0x54,0xf2,0xd8,0xd1,0x97,0x56,0xaf,0x5c,0xf9,0x4e,0x5e,
		0x6b,0xae,0x44,0xff,0xd0,0x6f,0x3c,0x38,0x8b,0xd7,0x5f,0x74,0x40,0x59,0x26,0x45,
		0x81,0x23,0x70,0xdb,0x3b,0x82,0x0c,0xbf,0xe2,0xcb,0xce,0x46,0xf4,0x0c,0xfc,0x0e,
		0x11,0x45,0x7d,0x62,0x32,0xe9,0xa6,0x49,0x61,0x82,0xee,0xee,0x2e,0xbf,0x5a,0x5b,
		0x5e,0x35,0x31,0xf9,0xc8,0xbe,0x1c,0x8f,0x83,0xe3,0x3f,0xa8,0xd9,0x6a,0x85,0x4a,
		0xad,0x42,0x3a,0xcb,0xe1,0xfa,0xe0,0x38,0xc6,0xa7,0xc2,0x78,0xa5,0xc9,0x81,0xc7,
		0xe1,0x30,0x74,0xac,0x04,0x63,0xc1,0x45,0x47,0xc7,0xf9,0x8e,0x64,0x91,0x40,0x20,
		0xe1,0x38,0xee,0x55,0x8b,0xc5,0xbc,0x5b,0xd0,0x60,0xaf,0xcd,0x0a,0x93,0x9e,0x05,
		0x5d,0x22,0xc3,0x61,0x47,0x03,0xf6,0xd4,0x9a,0xa1,0x57,0xc9,0x71,0xe4,0x90,0x0d,
		0x17,0x2e,0x5d,0xc3,0xbb,0x27,0x8e,0xa1,0x4c,0x5a,0x80,0x24,0xbf,0xda,0x54,0x5c,
		0x41,0x20,0xb0,0xdb,0xed,0xed,0x2e,0xd7,0x73,0x87,0x33,0x89,0x05,0xe3,0xbd,0xe1,
		0x61,0xec,0x7b,0xaa,0x02,0x7b,0xea,0xaa,0x41,0xe4,0x53,0x10,0x93,0x1c,0x9a,0x1a,
		0xab,0x80,0x8d,0x1c,0xd6,0xb3,0x59,0xec,0xd0,0x48,0x71,0xc8,0xd6,0x80,0xa5,0xc5,
		0x65,0xfc,0x0b,0x9d,0x7c,0x5e,0xc3,0x36,0xe9,0x38,0xcd,0x00,0x00,0x00,0x00,0x49,
		0x45,0x4e,0x44,0xae,0x42,0x60,0x82);
}
