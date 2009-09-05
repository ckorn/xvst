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
	this.website = "http://videolog.uol.com.br/";
	this.ID = "videolog.uol.com.br";
	this.caption = "Videolog (UOL)";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_XML = "http://videolog.uol.com.br/swfs/related/related-player-publicididade.php?id_video=%1";
	// init result
	var result = new VideoDefinition();
	// create http class
	var http = new Http();
	// get videoId
	var videoId = getUrlParam(url, "id");
	// download html
	var html = http.downloadWebpage(url);
	// get video title
	result.title = copyBetween(html, '<h1 class="titulo-video">', '</h1>');
	// download xml
	var xml = http.downloadWebpage(strFormat(URL_XML, videoId));
	// get video url
	result.URL = copyBetween(xml, "<video>", "</video>");
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0b,0x13,0x00,0x00,0x0b,
		0x13,0x01,0x00,0x9a,0x9c,0x18,0x00,0x00,0x01,0xe7,0x49,0x44,0x41,0x54,0x38,0x11,
		0x63,0xf4,0x3d,0x79,0xe9,0x3f,0x03,0x1e,0xc0,0xcd,0xcc,0xcc,0x90,0xaf,0x24,0xc3,
		0xb0,0xef,0xcd,0x7b,0x86,0x13,0xef,0x3f,0x61,0xa8,0x64,0xc2,0x10,0x41,0x12,0xd0,
		0xe5,0xe3,0x66,0x30,0x11,0xe4,0x65,0x98,0x78,0xef,0x09,0x83,0x89,0x00,0x2f,0x92,
		0x0c,0x82,0xc9,0x82,0x60,0xa2,0xb2,0xec,0x84,0x04,0x18,0x6e,0x7c,0xfd,0xc6,0x60,
		0x2e,0xc0,0xc7,0x20,0xca,0xca,0xca,0x30,0xe5,0xfe,0x53,0x54,0x05,0x50,0x1e,0x86,
		0x0b,0x44,0xd9,0x58,0x19,0x74,0x78,0xb9,0x19,0xd4,0x78,0x38,0x19,0xb2,0x15,0xa4,
		0x18,0x8e,0xbe,0xfb,0xc8,0xb0,0xe6,0xf9,0x6b,0xb8,0x66,0x2f,0x71,0x61,0x38,0x1b,
		0xc4,0x40,0x71,0x81,0x1e,0xd0,0xc9,0xd6,0x40,0x9b,0x85,0x58,0x59,0x18,0x2e,0x7d,
		0xfa,0xc2,0xb0,0xec,0xe9,0x2b,0x86,0x6f,0x7f,0xff,0x32,0xb8,0x8a,0x0a,0x31,0x44,
		0x48,0x8b,0x31,0xf4,0xdf,0x7d,0xcc,0x10,0x2b,0x23,0xce,0xb0,0xed,0xe5,0x5b,0xb8,
		0x21,0x60,0x03,0x40,0x01,0xe5,0x2f,0x21,0xc2,0x20,0xc6,0xce,0xca,0x70,0x1e,0xa8,
		0x91,0x11,0x18,0xac,0x07,0xde,0x7e,0x60,0xa8,0x56,0x95,0x67,0x50,0xe0,0xe2,0x60,
		0x28,0xb8,0x72,0x87,0xc1,0x4a,0x90,0x8f,0xe1,0xf5,0xaf,0x5f,0x0c,0x20,0xb5,0xe9,
		0x40,0x97,0xcd,0x7c,0xf0,0x0c,0x6c,0x08,0x93,0x12,0x50,0x41,0xb8,0x94,0x18,0xc3,
		0x1e,0x60,0x28,0x3f,0xfc,0xfe,0x93,0x41,0x8e,0x93,0x03,0xec,0x85,0x89,0x3a,0x2a,
		0x0c,0xad,0xb7,0x1f,0x32,0xbc,0xfd,0xf5,0x87,0xc1,0x42,0x88,0x8f,0xa1,0xf1,0xd6,
		0x03,0x86,0xd9,0xfa,0x1a,0x60,0x4d,0x1e,0x40,0x17,0xc1,0xc1,0x9e,0xd7,0xef,0xfe,
		0x7f,0xf9,0xf3,0xe7,0xff,0xcb,0x1f,0x3f,0xff,0xaf,0x7a,0xf6,0xf2,0xff,0xbd,0xaf,
		0xdf,0xff,0x83,0xa2,0xf6,0xfe,0xb7,0x6f,0xff,0x37,0x3d,0x7f,0xfd,0x1f,0x17,0xe8,
		0xb8,0xfd,0x10,0xac,0x8e,0x21,0xfc,0xcc,0x55,0xb0,0xa6,0x97,0x3f,0x7e,0xfd,0x8f,
		0x80,0xb2,0x2f,0x7f,0xfa,0x82,0x4b,0x1f,0x5c,0xfc,0xdb,0x9f,0xbf,0x60,0x03,0x58,
		0x40,0x81,0xc4,0xcb,0xc2,0xcc,0x20,0x02,0x0c,0xfd,0xb9,0x06,0xea,0x0c,0x5c,0x40,
		0x3f,0x12,0x03,0x38,0x99,0x99,0x80,0x01,0xce,0xcf,0x00,0x8e,0xc6,0xfa,0x9b,0xf7,
		0xc1,0x7a,0x88,0xd5,0x0c,0xb3,0x20,0x43,0x5e,0x0a,0x62,0xc0,0x63,0x60,0xe0,0xbd,
		0xfc,0xf9,0x0b,0x26,0x4e,0x34,0xcd,0xc6,0xc4,0x04,0x31,0x00,0xa4,0xa3,0x09,0x18,
		0xca,0xc4,0x82,0x07,0xdf,0x7e,0x30,0x64,0x5f,0xbe,0xc5,0x10,0x7e,0xf6,0x2a,0x22,
		0x21,0x81,0x5c,0xf1,0xfd,0xef,0x3f,0x06,0x90,0xdf,0xb0,0x01,0x90,0xdc,0xde,0x37,
		0xef,0x18,0x66,0x3d,0x7c,0x8e,0x22,0x8d,0x92,0x12,0x27,0xde,0x7f,0xcc,0x50,0xa1,
		0x22,0x8f,0xa2,0x00,0x64,0x5b,0xf7,0xdd,0x47,0x0c,0x20,0x0b,0xb0,0x01,0x14,0x03,
		0x8e,0xbd,0xfb,0xc4,0xf0,0xf1,0xf7,0x1f,0x06,0x90,0xdf,0xb0,0xd9,0x86,0xcd,0x00,
		0x00,0xba,0xaa,0xf9,0x77,0xe8,0x6e,0x80,0x69,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,
		0x44,0xae,0x42,0x60,0x82);
}
