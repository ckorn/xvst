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
	this.version = "1.0.1";
	this.minVersion = "2.0.0a";
	this.author = "Supelex Technologies";
	this.website = "http://www.xhamster.com/";
	this.ID = "xhamster.com";
	this.caption = "Xhamster";
	this.adultContent = true;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	// http://88.208.24.133/flv2/1276861797:vja1tohWpIY/190185_Ciglik_Cigliga_Orgazm.flv
	
	const FLV_URL = "%1flv2/%2";
	
	// video information
	var result = new VideoDefinition();
	// get the html
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get video title
	result.title = copyBetween(html, "<title>", "</title>");
	// get the flv url
	var srv = copyBetween(html, '\'srv\': \'', '\',');
	var file = copyBetween(html, '\'file\': \'', '\',');
	result.URL = strFormat(FLV_URL, srv, file);
	// return the video information
	return result;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0b,0x13,0x00,0x00,0x0b,
		0x13,0x01,0x00,0x9a,0x9c,0x18,0x00,0x00,0x03,0x16,0x49,0x44,0x41,0x54,0x38,0x11,
		0xad,0x93,0x6d,0x68,0x53,0x57,0x1c,0xc6,0x9f,0x7b,0x6f,0x72,0x6f,0xda,0xdb,0xa4,
		0xb1,0xb1,0x29,0x9a,0x69,0xa9,0xf5,0x6d,0xb3,0xe2,0xdc,0x44,0xfd,0xa0,0x1b,0x6e,
		0x33,0x30,0x57,0x5f,0xa8,0x93,0xa9,0x0c,0x94,0x16,0x51,0xa6,0xc5,0x29,0x5b,0xbf,
		0xa8,0x53,0xa9,0x8a,0x28,0xe2,0x26,0x1b,0xb8,0xae,0x3a,0xfa,0x65,0x48,0x9d,0x8a,
		0xaf,0x28,0x9d,0x16,0x23,0x22,0x43,0x5b,0xb5,0xea,0x02,0x05,0x6d,0x63,0x3b,0x6d,
		0x7d,0x69,0xde,0x73,0x73,0x93,0xdc,0xfb,0x2c,0x09,0xf4,0x83,0x1d,0xfb,0xb6,0xe7,
		0xc3,0x39,0x7f,0x0e,0xcf,0xef,0xe1,0xff,0x3f,0x87,0x03,0xfc,0x5f,0xb2,0xdb,0xf0,
		0x7d,0xed,0x2c,0xc9,0x28,0xb0,0x60,0xe6,0x7f,0x65,0x56,0x39,0x84,0xae,0xea,0x4a,
		0x51,0x57,0x65,0xd8,0x87,0x3d,0x62,0xae,0x50,0xac,0x10,0xbe,0xad,0x71,0xd5,0x3b,
		0x3d,0x4e,0xf1,0xa3,0x19,0x4a,0x87,0x20,0xc2,0x22,0x00,0xb2,0x28,0xe0,0x1b,0x49,
		0xc0,0x76,0x41,0x80,0x35,0x5b,0xcf,0x5e,0xf8,0xb1,0x7b,0xba,0x77,0x81,0x47,0xae,
		0xaa,0x90,0xae,0x0d,0x07,0x64,0x7d,0x80,0xcd,0x0a,0xc9,0x5b,0x55,0x90,0x79,0x99,
		0x34,0x30,0xad,0x24,0x8d,0x40,0x5c,0xc1,0xc9,0x63,0x5b,0xa0,0x96,0x59,0x20,0x14,
		0xc8,0x48,0xbd,0x26,0xb6,0xad,0xdb,0x05,0x7f,0x8c,0x50,0xcd,0x22,0xdc,0xfe,0x3b,
		0xae,0x3d,0x7d,0x61,0x16,0xe6,0xd8,0x7c,0x07,0xaa,0x43,0x60,0x7f,0x28,0x63,0xcc,
		0xf2,0xa4,0x30,0x21,0x6b,0x5a,0x6d,0x33,0xa1,0x0e,0xde,0x85,0x54,0xba,0x02,0xa2,
		0xba,0x14,0x4a,0xf2,0x16,0x3e,0x78,0x0d,0x2c,0xe9,0x23,0xdc,0x8e,0x28,0xd2,0x34,
		0xff,0xc8,0xc1,0x6f,0xc8,0xa6,0x60,0xd5,0x7c,0x17,0xd8,0xeb,0x6b,0x66,0xe3,0xf3,
		0x04,0xcf,0x34,0x78,0xd9,0xb9,0xb3,0x92,0xa9,0x83,0xa5,0x6c,0x69,0x3c,0xcc,0x0f,
		0x23,0x06,0xd3,0xaf,0x5a,0x58,0xa9,0x80,0xd9,0x71,0xe4,0x37,0x60,0x97,0x82,0x65,
		0x93,0x47,0x21,0xf0,0x83,0xd7,0xc2,0xe3,0x47,0x7f,0xa1,0x22,0x5b,0xe9,0x5b,0x20,
		0x31,0x50,0x67,0xa1,0xb1,0x43,0xe2,0x76,0x97,0x40,0x11,0x12,0x93,0xe1,0x67,0xec,
		0xf4,0x0a,0xe6,0x58,0x19,0x9a,0x2c,0x62,0x51,0x3e,0xc4,0xe3,0xc0,0x86,0x75,0x9f,
		0x7a,0xf8,0xb4,0xfb,0x3e,0x9b,0x97,0x17,0xf3,0x5a,0xad,0x95,0xda,0x61,0x27,0xcd,
		0x7b,0x5f,0xd2,0xcc,0xe8,0x34,0x53,0x69,0x52,0xff,0x91,0xbc,0xec,0xa0,0xfe,0xb5,
		0xc0,0xd6,0xf7,0x44,0xa6,0xe2,0x51,0xee,0x5d,0xfd,0x36,0x5d,0x36,0xd4,0x63,0xb6,
		0x47,0xe8,0xf1,0x77,0xdc,0x60,0xdf,0xe3,0x87,0x6c,0x5e,0x3b,0x86,0xbd,0x4d,0xf3,
		0x98,0xbc,0xb2,0x86,0x34,0x0d,0xe6,0x95,0x36,0x69,0x98,0x26,0xcd,0x58,0x13,0x23,
		0x17,0x16,0xf3,0x62,0xad,0xc2,0x40,0xcf,0x63,0xc6,0x23,0x61,0xce,0x29,0x13,0x83,
		0xa2,0x69,0x20,0x1c,0x8f,0x47,0x11,0x78,0x70,0x13,0xf6,0xec,0x7b,0x46,0x34,0x0b,
		0x38,0x77,0x1f,0x22,0x09,0xa2,0xfb,0xd4,0x05,0x74,0x9d,0x3b,0x8f,0xa3,0xfb,0x4f,
		0x43,0x28,0xac,0x43,0xf2,0x85,0x8e,0xf1,0x65,0x45,0xe8,0xf7,0xfd,0x0c,0x4d,0xd3,
		0xa0,0x1b,0x18,0x40,0xf9,0x28,0x7c,0xb1,0xa9,0xe6,0x7d,0xf6,0x3e,0x79,0xc4,0x43,
		0x35,0x2e,0xfe,0xbe,0x71,0x2a,0x13,0x99,0x14,0xf5,0xa4,0xce,0xcf,0x27,0x4e,0xe7,
		0x14,0x87,0x8b,0x9d,0xed,0xb7,0x98,0xce,0x36,0xd4,0xd6,0x30,0x99,0x3f,0xbd,0x6b,
		0x61,0x38,0x38,0xc8,0x3d,0x1b,0x3f,0x63,0x69,0x21,0x56,0xe6,0xef,0xa1,0xbc,0x18,
		0xbf,0x1e,0xd9,0x56,0xc7,0xce,0xcb,0xc7,0x79,0xf6,0xbb,0x4f,0x18,0x4e,0xe8,0xf9,
		0xee,0xc3,0x91,0x38,0xdb,0x7d,0xd7,0xf3,0x75,0x2c,0x63,0xf0,0xfa,0xae,0x79,0xec,
		0xbe,0x74,0x80,0xa7,0x9a,0xf6,0xf1,0x2d,0x15,0x27,0x73,0xb0,0x94,0x5b,0x5c,0x0e,
		0xb4,0xdd,0xf9,0xf3,0xee,0xdc,0x8a,0x71,0xa3,0x27,0xcc,0xa9,0x5e,0x03,0x7b,0x69,
		0x05,0x02,0x81,0x3e,0x5c,0xbd,0x7a,0x11,0x7e,0xbf,0x1f,0xe9,0x64,0x0a,0x63,0xc6,
		0x8d,0x47,0x89,0xbb,0x1c,0xf7,0x1f,0xf9,0xb1,0xb9,0x61,0xb7,0xcf,0x26,0x63,0xf9,
		0x90,0x86,0x4c,0x8e,0xcf,0x6b,0x92,0x1b,0xea,0x44,0x27,0x5a,0x1b,0x6b,0xe7,0xf3,
		0x79,0x4f,0x17,0x43,0xb1,0x38,0x83,0xd1,0x04,0x43,0x51,0x8d,0x03,0xc1,0x10,0x87,
		0x06,0x7a,0x79,0x64,0x6b,0x35,0x27,0x39,0xf1,0x5b,0x89,0x0a,0x75,0x98,0xfb,0xd7,
		0xee,0x56,0x51,0xbf,0x78,0xaa,0x2d,0x78,0x6c,0xc7,0x4a,0xde,0x69,0x6b,0x65,0x47,
		0xfb,0x39,0x9e,0x38,0xb8,0x9e,0x2b,0x66,0xda,0x87,0xb2,0x33,0x7f,0x35,0x12,0xc8,
		0xff,0x85,0x91,0x87,0xe5,0x4e,0x2c,0x2c,0xb6,0x60,0xed,0x3b,0x15,0x45,0x4e,0xab,
		0x08,0xe1,0xaf,0x27,0xb1,0xd0,0xa0,0x8e,0x96,0xfe,0x08,0xae,0x8c,0xf4,0xfe,0x03,
		0x42,0xc8,0x84,0xf1,0xba,0xeb,0x08,0x86,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,
		0xae,0x42,0x60,0x82);
}