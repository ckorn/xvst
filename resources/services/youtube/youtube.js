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

/* Function called on load plugin */
function RegistVideoService()
{
	this.version = "1.0.0";
	this.minVersion = "2.0.0a";
	this.author = "Xesc & Technology 2009";
	this.website = "http://www.youtube.com/";
	this.ID = "youtube.com";
	this.caption = "Youtube";
	this.adultContent = false;
	this.musicSite = false;
}

/* Function called when the xVST will need to resolve the video URL */
function getVideoInformation(url)
{
	const URL_YOUTBE  = "http://youtube.com/watch?v=%1";
	const URL_GET_FLV = "http://%1/get_video?video_id=%2&t=%3&fmt=%4";
	const STD_VIDEO_RES = 34;
	const HD_VIDEO_RES  = 22;
	// init result
	var result = new VideoDefinition();
	// default URL
	var youTubeURL = url;
	// check if is an embeded video, and get the "real url" of youtube
	if (youTubeURL.indexOf(".youtube.com/v/") != -1)
	{
		var embededID = youTubeURL;
		embededID = strRemove(embededID, 0, embededID.lastIndexOf("/v/") + 3);
		youTubeURL = strFormat(URL_YOUTBE, embededID);
	}
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(youTubeURL);
	delete http;
	// get the video ID and the video HASH
	var vidID =  getUrlParam(youTubeURL, "v");
	var vidHash = copyBetween(html, "\"t\": \"", "\"");
	// check if a HD version is aviable
	var vidRes = STD_VIDEO_RES;
	if (html.indexOf("var isHDAvailable = true;") != -1)
	{
		vidRes = HD_VIDEO_RES;
		// for HD videos the extension is mp4
		result.extension = ".mp4";
	}
	// get the video title
	result.title = copyBetween(html, "<title>YouTube - ", "</title>");
	// build the video url
	result.URL = strFormat(URL_GET_FLV, getUrlHost(youTubeURL), vidID, vidHash, vidRes);
	// check if this video need a login
	result.needLogin = result.title == "Broadcast Yourself.";
	// return the video information
	return result;
}

/* Function called when the xVST will need to get the video service icon */
function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x01,0xde,0x49,0x44,0x41,0x54,0x78,0x9c,0x9d,0x93,0xbf,0x4b,0x23,
		0x41,0x1c,0xc5,0x3f,0x1b,0x13,0x74,0x89,0x26,0xd9,0x84,0x43,0x44,0xf0,0xc7,0x0a,
		0x8a,0x58,0x84,0x10,0x82,0x70,0x9c,0x8d,0x58,0x09,0xda,0xed,0x7f,0x20,0xd8,0x58,
		0x9b,0xca,0xce,0x26,0x85,0xbd,0x85,0xb5,0x68,0x27,0xda,0x04,0x12,0xb0,0x97,0x60,
		0xad,0x85,0x41,0x9b,0x44,0xe5,0xb2,0x88,0xab,0x20,0x68,0x7c,0x57,0x4c,0x12,0xf7,
		0x8e,0x93,0x93,0x7b,0x30,0xec,0xcc,0xec,0xcc,0xe3,0xf3,0xe6,0x3b,0x63,0x49,0x12,
		0x80,0x65,0x59,0x48,0xea,0x7d,0xbf,0xaa,0x08,0x40,0xbd,0x5e,0x07,0xa0,0x54,0x2a,
		0x11,0x1e,0x7f,0x49,0xea,0xa8,0x58,0x2c,0x0a,0x90,0xe7,0x79,0xf2,0x7d,0x5f,0xae,
		0xeb,0x0a,0x90,0xeb,0xba,0xea,0x50,0xaa,0x56,0xab,0xc9,0x71,0x1c,0x85,0xd5,0x33,
		0xf0,0x7d,0xbf,0xb7,0x48,0x92,0x5c,0xd7,0x95,0xef,0xfb,0x72,0x1c,0x47,0x95,0x4a,
		0xe5,0x53,0x83,0x48,0x97,0xc4,0x71,0x9c,0x7f,0xd2,0x56,0xab,0xd5,0xcf,0x23,0x84,
		0x31,0xff,0x16,0x21,0x9f,0xcf,0x0b,0xd0,0x1f,0x5b,0x64,0x75,0xab,0xf0,0xbf,0x8a,
		0x02,0x70,0x74,0x04,0xc5,0x22,0xfc,0x6c,0x41,0x22,0x0e,0x99,0x6f,0xd0,0xd7,0x07,
		0x91,0x08,0x34,0x1b,0x90,0x4c,0xc1,0xec,0x2c,0xe4,0xb2,0xf0,0xfd,0x07,0x24,0x93,
		0x70,0x79,0x09,0x9e,0xd7,0xe1,0x19,0x19,0x91,0x2e,0x2e,0x3e,0xb8,0x66,0x66,0xa4,
		0x54,0x4a,0x02,0xe9,0xf1,0x51,0xca,0x66,0x4d,0x7f,0x7a,0x5a,0x1a,0x1b,0x93,0x0e,
		0x0f,0xa5,0xcd,0xcd,0x50,0x15,0xf2,0x79,0x29,0x16,0x33,0x9b,0x97,0x97,0x4d,0x5b,
		0x59,0x91,0x0a,0x05,0xa9,0x5e,0x97,0x8e,0x8f,0xa5,0xf9,0x79,0x29,0x9d,0x96,0xf6,
		0xf7,0xa5,0xb9,0x39,0x69,0x61,0x21,0x74,0x06,0x85,0x02,0x9c,0x9f,0xc3,0xd3,0x13,
		0xe4,0x72,0x70,0x7a,0x0a,0xe5,0x32,0x8c,0x8e,0xc2,0xed,0x2d,0xac,0xae,0xc2,0xd9,
		0x19,0x34,0x9b,0xb0,0xb8,0x08,0xed,0x36,0x4c,0x4d,0x41,0xbb,0x6d,0x6e,0x22,0x12,
		0xd8,0x36,0xc4,0x62,0x30,0x38,0x08,0xd7,0xd7,0x66,0xf1,0xdd,0x1d,0xc4,0xe3,0xb0,
		0xb1,0x01,0x41,0x00,0xc3,0xc3,0xf0,0xf2,0x02,0xef,0xef,0x30,0x34,0x14,0x3a,0xc4,
		0xae,0x49,0x34,0x0a,0x96,0x65,0x8c,0x5a,0x2d,0x98,0x98,0x30,0x73,0x03,0x03,0xd0,
		0xdf,0x0f,0xe9,0x34,0x6c,0x6d,0x81,0xeb,0x42,0x26,0x03,0x74,0xde,0x02,0xb6,0xcd,
		0x6f,0x46,0xb6,0x0d,0x3b,0x3b,0xf4,0xfe,0xed,0xed,0xc1,0xeb,0x2b,0x9c,0x9c,0xc0,
		0xc1,0x01,0xac,0xaf,0x9b,0xb8,0x3d,0x82,0xc9,0x49,0x78,0x78,0x30,0xf9,0x9e,0x9f,
		0x61,0x6d,0xcd,0x44,0xb9,0xbf,0x37,0xe5,0xdc,0xdd,0x85,0xab,0x2b,0xb8,0xb9,0x31,
		0xb1,0xca,0x65,0x58,0x5a,0x02,0xe0,0xe3,0x22,0x6d,0x6f,0x43,0xa3,0x61,0xf0,0x83,
		0xc0,0x64,0xb5,0x2c,0x43,0xf1,0xf6,0x06,0x89,0x84,0x31,0x0d,0x02,0x18,0x1f,0x37,
		0x51,0x80,0x5f,0x6c,0x3f,0x59,0xf2,0x0b,0xfd,0xd6,0x66,0x00,0x00,0x00,0x00,0x49,
		0x45,0x4e,0x44,0xae,0x42,0x60,0x82);
}
