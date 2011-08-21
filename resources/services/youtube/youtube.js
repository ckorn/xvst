/*
*
* This file is part of xVideoServiceThief,
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 - 2011 Xesc & Technology
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
	this.version = "3.0.1";
	this.minVersion = "2.0.0a";
	this.author = "Xesc & Technology 2011";
	this.website = "http://www.youtube.com/";
	this.ID = "youtube.com";
	this.caption = "YouTube";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_YOUTBE = "http://www.youtube.com/watch?v=%1";
	// init result
	var result = new VideoDefinition();
	// default URL
	var youTubeURL = strReplace(url, "watch#!", "watch?");
	// replace "popup" watch for standard watch
	youTubeURL = strReplace(youTubeURL, "watch_popup?", "watch?");
	// check if is an embeded video, and get the "real url" of youtube
	if (youTubeURL.toString().indexOf(".youtube.com/v/") != -1)
	{
		var embededID = youTubeURL;
		embededID = strRemove(embededID, 0, embededID.lastIndexOf("/v/") + 3);
		youTubeURL = strFormat(URL_YOUTBE, embededID);
	}
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(youTubeURL);
	// get cookies
	result.cookies = http.getCookies("|");
	// get the video title
	result.title = copyBetween(html, "<title>", "</title>");
	result.title = normalizeSpaces(result.title);
	result.title = strReplace(result.title, "\n", "");
	result.title = strReplace(result.title, " - YouTube", "");
	// check if this video need a login
	result.needLogin = strIndexOf(html, "signing_in") != -1;
	// if we can continue (no loggin needed)
	if (result.needLogin) return result;
	// get the video URL and extension
	var videoInfo = getVideoUrlAndExtension(html);
	result.URL = videoInfo.url;
	result.extension = videoInfo.extension;
	// return the video information
	return result;
}

function getVideoUrlAndExtension(html)
{
	// init result
	var result = { url:null, extension:null };
	// get the flashVars value
	var flashVars = "?" + copyBetween(html, 'flashvars="', '"');
	// convert each "&amp;" into "&"
	flashVars = strReplace(flashVars, "&amp;", "&");
	// get an array with all fmt_stream_map values
	var fmt_stream_map_arr = splitString(getUrlParam(flashVars, "url_encoded_fmt_stream_map"), "url%3D", false);
	// default selected video quality
	var selectedFormat = -1;
	// detect the better quality
	for (var n = 0; n < fmt_stream_map_arr.length && selectedFormat == -1; n++)
	{
		fmt_stream_map_arr[n] = "?url=" + cleanUrl(fmt_stream_map_arr[n]).toString();
		// remove the last "," (if exists)
		if (strLastIndexOf(fmt_stream_map_arr[n], ",") == fmt_stream_map_arr[n].toString().length - 1)
			fmt_stream_map_arr[n] = strRemove(fmt_stream_map_arr[n], fmt_stream_map_arr[n].toString().length - 1, 1);
		// check video type
		var vtype = getToken(getUrlParam(fmt_stream_map_arr[n], "type"), ";", 0);
		// is known format?
		if (vtype == "video/x-flv" || vtype == "video/mp4")
		{
			selectedFormat = n;
			// configure video extension
			result.extension = extensionFromVideoType(vtype);
		}
	}
	// no format selected?
	if (selectedFormat == -1) selectedFormat = 0;
	// get the host url
	var urlHost = getToken(fmt_stream_map_arr[selectedFormat], "?", 1);
	urlHost = strReplace(urlHost, "url=", "");
	// leave only the parameters
	fmt_stream_map_arr[selectedFormat] = getToken(fmt_stream_map_arr[selectedFormat], "?", 2);
	// get url parts
	var urlParts = splitString(fmt_stream_map_arr[selectedFormat], "&", false);
	// set the url host
	result.url = urlHost + "?";
	// build the initial url
	for (var n = 0; n < urlParts.length; n++)
	{
		var pname = getToken(urlParts[n], "=", 0).toString();
		var pvalue = getToken(urlParts[n], "=", 1).toString();
		var duplicatedPname = strIndexOf(result.url, pname + "=") != -1;
		// is an excluded param?		
		if (!duplicatedPname && pname != "fexp" && pname != "quality" && pname != "fallback_host" && pname != "type") 
			result.url += pname + "=" + pvalue + "&";
	}
	// remove the last &
	if (strLastIndexOf(result.url, "&") == result.url.length - 1)
		urlInitial = strRemove(result.url, result.url.length - 1, 1);
	// get extra (optional) params
	var ptchn = getUrlParam(flashVars, "ptchn");
	if (ptchn != "") result.url += "ptchn=" + ptchn + "&";
	var ptk = getUrlParam(flashVars, "ptk");
	if (ptk != "") result.url += "ptk=" + ptk;
	// configure the video extension (if is not yet configured)
	if (!result.extension)
	{
		var vtype = getToken(getUrlParam(fmt_stream_map_arr[selectedFormat], "type"), ";", 0);
		// configure video extension
		result.extension = extensionFromVideoType(vtype);
	}
	// return 
	return result;
}

function extensionFromVideoType(vtype)
{
	if (vtype == "video/x-flv") return ".flv";
	if (vtype == "video/mp4") return ".mp4";
	if (vtype == "video/webm") return ".webm";
	// default extension
	return ".flv";
}

/* 
	This function "normalizeSpaces(str)" will be deprecated on next xVST version
	and replaced with the new "simplifyString(str)" function (added in xVST 2.3.1)
*/
function normalizeSpaces(str)
{
	var result = "";
	var parts = splitString(str, " ", false);
	// join again each part
	for (var n = 1; n < parts.length; n++)
		result += parts[n] + " ";
	// return the normalized string
	return trimString(result);
}

function searchVideos(keyWord, pageIndex)
{
	const URL_SEARCH = "http://www.youtube.com/results?search_query=%1&page=%2&hl=%3";
	const HTML_SEARCH_START = '<div id="search-results">';
	const HTML_SEARCH_FINISH = '<span id="search-pva-content">';
	const HTML_SEARCH_SEPARATOR = '<div class="result-item *sr ">';
	const HTML_SEARCH_SUMMARY_START = '<p class="num-results">';
	const HTML_SEARCH_SUMMARY_END = '</p>';
	// replace all spaces for "+"
	keyWord = strReplace(keyWord, " ", "+");
	// init search results object
	var searchResults = new SearchResults();
	// init http object
	var http = new Http();
	var html = http.downloadWebpage(strFormat(URL_SEARCH, keyWord, pageIndex, searchResults.getUserLanguage()));
	// get the search summary
	var summary = copyBetween(html, HTML_SEARCH_SUMMARY_START, HTML_SEARCH_SUMMARY_END);
	searchResults.setSummary(cleanSummary(summary));
	// get results html block
	var htmlResults = copyBetween(html, HTML_SEARCH_START, HTML_SEARCH_FINISH);
	// if we found some results then...
	if (htmlResults != "")
	{
		var block = "";
		// iterate over results
		while ((block = copyBetween(htmlResults, HTML_SEARCH_SEPARATOR, HTML_SEARCH_SEPARATOR)) != "")
		{
			parseResultItem(searchResults, block);
			htmlResults = strRemove(htmlResults, 0, block.toString().length);
		}
		// get last result
		parseResultItem(searchResults, htmlResults);
	}
	// return search results
	return searchResults;
}

function cleanSummary(summary)
{
	// remove all "\n"
	summary = strReplace(summary, "\n", "");
	// remove unused </span>
	summary = strReplace(summary, "</span>", '');
	// return cleanned summary
	return summary;
}

function parseResultItem(searchResults, html)
{
	const VIDEO_URL = "http://www.youtube.com";
	// vars
	var tmp, videoUrl, imageUrl, title, description, duration, rating;
	// get video url
	videoUrl = VIDEO_URL + copyBetween(html, 'href="', '"');
	// get image url
	tmp = copyBetween(html, '<img', '>') ;
	imageUrl = copyBetween(tmp, 'src="', '"');
	if (strIndexOf(imageUrl, "default.jpg") == -1) // if is not a "default.jpg"...
		imageUrl = copyBetween(tmp, 'thumb="', '"');
	imageUrl = "http:" + imageUrl;
	// get video title
	title = copyBetween(html, 'dir="ltr" title="', '"');
	// get video description
	description = copyBetween(html, '<p id="video-description-', '</p>');
	description = copyBetween(description + '|', '>', '|');
	// get video duration
	duration = convertToSeconds(copyBetween(html, '<span class="video-time">', '</span>'));
	// get rating
	tmp = copyBetween(html, '<button class="master-sprite ratingVS', '>');
	rating = copyBetween(tmp, 'title="', '"');
	// add to results list
	searchResults.addSearchResult(videoUrl, imageUrl, title, description, duration, rating);
}

function convertToSeconds(text)
{
	// how many ":" exists?
	var count = getTokenCount(text, ":");
	// get mins and seconds
	var h = new Number(h = count == 3 ? getToken(text, ":", 0) * 3600 : 0);
	var m = new Number(getToken(text, ":", count - 2) * 60);
	var s = new Number(getToken(text, ":", count - 1));
	// convert h:m:s to seconds
	return h + m + s;
}

function getVideoServiceIcon()
{
	return new Array(
		0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
		0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x06,0x00,0x00,0x00,0x1f,0xf3,0xff,
		0x61,0x00,0x00,0x01,0xb2,0x49,0x44,0x41,0x54,0x78,0xda,0xa5,0x92,0xbf,0x4b,0x82,
		0x61,0x10,0xc7,0x25,0x70,0xb1,0xa1,0x34,0x08,0x1a,0xb2,0x70,0x28,0xa1,0x48,0xc4,
		0x7e,0x38,0xe8,0x10,0x38,0x05,0x0d,0x0d,0x2a,0x19,0x0d,0x51,0x0e,0x2d,0x12,0x0e,
		0x49,0xd0,0x54,0x14,0xfe,0x15,0x51,0x82,0x41,0x21,0x26,0x3a,0x48,0xb8,0x35,0x08,
		0x91,0x4b,0xe0,0x10,0x81,0x15,0x0d,0x8d,0x06,0x0d,0x2e,0x0d,0x9f,0xb8,0xf7,0xe0,
		0x41,0x24,0xa2,0xe8,0x0b,0xc7,0xdd,0xbd,0xdc,0x7d,0xee,0x9e,0xf7,0x79,0x6c,0xa0,
		0xb2,0xd9,0x6c,0xc6,0xff,0x45,0x56,0x75,0xab,0xd5,0xb2,0x1a,0xb3,0xd9,0xac,0x78,
		0xc9,0xff,0x02,0x50,0x65,0x32,0x19,0xab,0x39,0x1a,0x8d,0xd2,0x6e,0xb7,0xf1,0x78,
		0x3c,0x92,0x8b,0x37,0x9b,0x35,0x1a,0x0d,0x9c,0x4e,0xe7,0xb7,0x00,0x69,0x32,0x45,
		0x22,0x69,0x94,0x6f,0xd2,0x50,0xab,0xd5,0x7e,0x02,0x18,0xfd,0x0c,0xd0,0x23,0xfe,
		0x0a,0xf0,0xed,0x11,0x02,0x81,0x80,0xe4,0x62,0x3d,0x80,0x7f,0x4a,0x01,0x57,0x57,
		0x30,0x31,0x01,0xae,0x21,0x18,0x77,0xcb,0x38,0x98,0x9f,0x87,0x60,0x10,0xc6,0xdc,
		0x30,0x33,0x03,0xf1,0x38,0x64,0x8f,0xe1,0xe6,0x06,0xee,0xef,0xe1,0xf2,0xb2,0x0b,
		0x30,0x32,0x02,0x0f,0x0f,0x18,0x4d,0x4e,0xc2,0xe0,0x20,0xc8,0xba,0x1f,0x1f,0xe0,
		0xf3,0x49,0xac,0x43,0xdc,0x6e,0xb8,0xb8,0x80,0xdd,0x5d,0x03,0xd0,0x89,0x76,0x3b,
		0x96,0x96,0x96,0xd4,0x96,0x97,0x61,0x6e,0x0e,0x9e,0x9e,0xa0,0x5c,0x86,0x85,0x05,
		0x70,0xb9,0xe0,0xfc,0x1c,0xa6,0xa6,0x20,0x1c,0xee,0x02,0xcc,0xce,0x42,0x5f,0x1f,
		0x74,0x3a,0x3a,0xfd,0xed,0x0d,0x4e,0x4e,0xe0,0xfa,0x1a,0xce,0xce,0xe0,0xfd,0x5d,
		0xe3,0xd3,0x53,0x78,0x7d,0x85,0xe7,0x67,0xa9,0xef,0xd9,0xa0,0xbf,0x1f,0x3e,0x3f,
		0x35,0xae,0xd7,0xe1,0xe8,0x08,0x72,0x39,0x28,0x14,0x60,0x6d,0x4d,0x7d,0xb5,0x0a,
		0x8f,0x8f,0x20,0x2f,0x75,0x60,0xa0,0x07,0xe0,0x70,0x60,0xb6,0xb9,0xbb,0x83,0x74,
		0x1a,0xf2,0x79,0x28,0x95,0x60,0x73,0x13,0x2a,0x15,0xb8,0xbd,0xd5,0x9f,0xb9,0xb7,
		0x27,0x0f,0xa5,0x0b,0x10,0x0a,0x59,0x00,0x03,0x6b,0x36,0x01,0x04,0xa0,0xab,0x8b,
		0x8a,0x45,0xd8,0xdf,0xd7,0xf8,0xe5,0x05,0x86,0x87,0xbb,0x00,0xeb,0xeb,0x30,0x3d,
		0x0d,0x8b,0x8b,0xe0,0xf5,0xea,0xf5,0x45,0x22,0x7a,0x7d,0x7e,0x3f,0xac,0xac,0x48,
		0xac,0x6b,0x27,0x93,0x30,0x3a,0x0a,0x89,0x84,0x01,0xa8,0x0e,0x0f,0x61,0x7b,0x1b,
		0x52,0x29,0xd8,0xd8,0x80,0xd5,0x55,0x29,0x52,0x8b,0xc5,0x60,0x6b,0x0b,0x76,0x76,
		0xf4,0x38,0x07,0x07,0xa6,0xed,0x0b,0xd9,0x64,0x1a,0x67,0x99,0x78,0xec,0x66,0x00,
		0x00,0x00,0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82);
}
