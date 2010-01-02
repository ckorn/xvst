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
	this.version = "2.0.1";
	this.minVersion = "2.0.0a";
	this.author = "Xesc & Technology 2009";
	this.website = "http://www.clipfish.de/";
	this.ID = "clipfish.de";
	this.caption = "Clipfish";
	this.adultContent = false;
	this.musicSite = false;
}

function getVideoInformation(url)
{
	const URL_GET_XML = "http://www.clipfish.de/video_n.php?vid=%1";
	// video information
	var result = new VideoDefinition();
	// download webpage
	var http = new Http();
	var html = http.downloadWebpage(url);
	// get the video title
	result.title = copyBetween(html, "<span class=\"cf-headline cf-bold\">", "</span>");
	// get the video id
	var vidId = copyBetween(html, "var video = {id: ", " ");
	// download xml
	var xml = http.downloadWebpage(strFormat(URL_GET_XML, vidId));
	// build final url
	result.URL = copyBetween(xml, "=", "&");
	// return the video information
	return result;
}

//http://www.clipfish.de/suche/?search=Will+Smith
//http://www.clipfish.de/suche/Will+Smith/video/bestertreffer/2/listenansicht/?search=Will%20Smith
function searchVideos(keyWord, pageIndex)
{
	const URL_SEARCH = "http://www.clipfish.de/suche/%1/video/bestertreffer/%2/listenansicht/?search=%1";
	const HTML_SEARCH_START = '<ul class="cf-video-list cf-list-linear">'; //"<!-- GENERIC BOX BEGIN -->";
	const HTML_SEARCH_FINISH = '<!-- GENERIC BOX END -->';
	const HTML_SEARCH_SEPARATOR = '<li id="cf-video-item_';
	const HTML_ACCESS_DENIED = "http://bilder.clipfish.de/v2/access_denied.jpg";
	// replace all spaces for "+"
	keyWord = strReplace(keyWord, " ", "+");
	// init search results object
	var searchResults = new SearchResults();
	// init http object
	var http = new Http();
	var html = http.downloadWebpage(strFormat(URL_SEARCH, keyWord, pageIndex));
	// access denied?
	if (strIndexOf(htmlResults, HTML_ACCESS_DENIED) != -1)
		return searchResults;
	// get the search summary
	var tmp = copyBetween(html, '>Deine Suche nach', 'Treffer ergeben.');
	var summary = "Deine Suche nach " + cleanwhitespace(cleartags(tmp,1)) + ' Treffer ergeben.';
	var tmp = copyBetween(html, "Suchergebnisse", "</span></span>");
	var summary = summary + " (" + cleanwhitespace(cleartags(tmp,1)) + ")";
	searchResults.setSummary(summary);
	// get results html block
	var htmlResults = copyBetween(html, HTML_SEARCH_START, HTML_SEARCH_FINISH);
	if (strIndexOf(htmlResults,HTML_SEARCH_SEPARATOR) != -1) 
		htmlResults = strRemove(htmlResults, 0, strIndexOf(htmlResults,HTML_SEARCH_SEPARATOR));
	// if we found some results then...
	if (htmlResults != "")
	{
		var block = "";
		// iterate over results
		while ((block = copyBetween(htmlResults, HTML_SEARCH_SEPARATOR, HTML_SEARCH_SEPARATOR)) != "")
		{
			parseResultItem(searchResults, block);
			htmlResults = strRemove(htmlResults, 0, block.toString().length + HTML_SEARCH_SEPARATOR.toString().length);
		}
		// get last result
		parseResultItem(searchResults, htmlResults);
	}
	// return search results
	return searchResults;
}

function parseResultItem(searchResults, html)
{
	const VIDEO_URL = "http://www.clipfish.de";
	// vars
	var tmp, videoUrl, imageUrl, title, description, duration, rating;
	// get video url
	videoUrl = VIDEO_URL + copyBetween(html, '<a href="', '"');
	// get title and image url
	title = trim(copyBetween(html, 'title="', '"')," ");
	imageUrl = copyBetween(html, '<img src="', '"');
	//if (strIndexOf(imageUrl, "default.jpg") == -1) // if is not a "default.jpg"...
	//	imageUrl = copyBetween(tmp, 'thumb="', '"');
	// get video description
	tmp = copyBetween(html, '<div class="cf-video-list-item-info cf-gray">', '</div>') ;
	description = trim(cleartags(copyBetween(tmp,"<p>","</p>"),0)," ");
	description = (strIndexOf(description,"Video-L&auml;nge:") == -1 ? description : "Keine Beschreibung vorhanden.");
	// get video duration
	duration = convertToSeconds(trim(cleartags(copyBetween(tmp, "Video-L&auml;nge:", '</span>'),0)," "));
	// get rating
	rating = getrating(copyBetween(html, '<div class="cf-voting-fishes">', '</div>'));
	// add to results list
	searchResults.addSearchResult(videoUrl, imageUrl, title, description, duration, rating);
}

function getrating(text)
{
	var rating = 0
	var i = 1
	while (i < 6)
	{
		var part = getToken(text, '<img',i);
		if (strIndexOf(part,"cf-voting-fish-full") != -1)
		{
			rating = rating + 1
		}
		if (strIndexOf(part,"cf-voting-fish-half") != -1)
		{
			rating = rating + 0.5
		}
		i++
	}
	return rating;
}

function cleanwhitespace(text)
{
	text = trim(text," ");
	text = trim(text,"	");
	text = cleandoublevalue(text," ");
	text = cleandoublevalue(text,"	");
	text = strReplace(text,"\n","");
	text = strReplace(text,"\r","");
	return text;
}

function trim(text,value)
{
	while(strIndexOf(text,value) == 0) {
		text = strRemove(text,0,1);
	}
	while(strLastIndexOf(text,value,-1) == text.toString().length - 1) {
		text = strRemove(text,text.toString().length - 1,1);
	}
	return text;
}

function cleandoublevalue(text,value)
{
	while (strIndexOf(text,value+value) != -1) {
		text = strReplace(text,value+value,value);
	}
	return text;
}

function cleartags(text, killopen)
{
	var cleartext = text
	if (killopen==1) {
		if (strIndexOf(cleartext,">") < strIndexOf(cleartext,"<")) {
			cleartext = strRemove(cleartext,0,strIndexOf(cleartext,">")+1);
		}
		if (strLastIndexOf(cleartext,">",-1) < strLastIndexOf(cleartext,"<",-1)) {
			var tmp = strLastIndexOf(cleartext,"<",-1);
			cleartext = strRemove(cleartext,tmp,cleartext.toString().length-tmp);
		}
	}
	while (strIndexOf(cleartext,">") != -1 && strIndexOf(cleartext,"<") != -1) {
		var start = strIndexOf(cleartext,"<");
		var stop = strIndexOf(cleartext,">");
		cleartext = strRemove(cleartext,start,stop-start+1);
	}
	return cleartext;
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
		0x61,0x00,0x00,0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0b,0x13,0x00,0x00,0x0b,
		0x13,0x01,0x00,0x9a,0x9c,0x18,0x00,0x00,0x01,0x80,0x49,0x44,0x41,0x54,0x38,0x11,
		0xc5,0x52,0x3d,0x48,0x42,0x51,0x18,0x3d,0xe6,0x43,0xc1,0x48,0x11,0x84,0x20,0xcb,
		0x48,0x08,0x8c,0x0a,0x9a,0x9c,0x82,0x86,0xd7,0x52,0x41,0xe4,0x16,0x0d,0xae,0x2d,
		0xee,0x61,0xcd,0x25,0x35,0x3f,0x88,0xc6,0x0c,0x5a,0x05,0x41,0x5a,0x72,0x08,0xa2,
		0xa1,0xa6,0xd0,0xe8,0x4d,0x45,0xa6,0x52,0x14,0x58,0x4a,0x92,0x82,0xd8,0x3d,0xdf,
		0xf0,0x30,0xa1,0x16,0x07,0x0f,0xdc,0x77,0xdf,0xbd,0xf7,0x7c,0xbf,0xe7,0xb3,0xb5,
		0x15,0xd0,0x03,0x06,0x7a,0xb0,0x15,0x53,0xad,0xdb,0x41,0xeb,0xb3,0x0a,0xbb,0xc7,
		0x8d,0x46,0xa1,0x04,0x67,0xc0,0xff,0xeb,0xb9,0x92,0x39,0x47,0x3d,0x6f,0x5a,0x77,
		0xde,0x65,0x1d,0x96,0x03,0x1a,0x3c,0xc6,0xe2,0x62,0x38,0x61,0x24,0x50,0xd8,0x49,
		0x60,0xe6,0x22,0x25,0xe4,0x97,0xc3,0x63,0xd9,0x3f,0xce,0xb2,0xe2,0x9c,0x8e,0x79,
		0xe7,0x1c,0xf3,0x43,0x4a,0xe0,0x21,0xbf,0x10,0x11,0x52,0x28,0x9d,0x94,0xbd,0x9e,
		0xbb,0x97,0x9d,0x1f,0x1a,0xb6,0xaa,0x35,0x7c,0xe5,0x4c,0x09,0x40,0xbe,0x7f,0x2b,
		0x06,0xdf,0x46,0x04,0xda,0xfb,0x69,0x4a,0xa2,0xf1,0xe0,0x5b,0x8f,0xe0,0xf5,0x28,
		0x09,0xd7,0x74,0x48,0x8c,0xbb,0xcb,0x60,0x79,0xce,0x40,0x18,0x93,0x27,0x86,0x55,
		0x9e,0xe6,0x9a,0x0d,0xc1,0x3d,0x1f,0x46,0x53,0x95,0x50,0x3e,0x30,0xe0,0x50,0xe9,
		0xd9,0x3d,0x43,0x92,0xea,0xed,0x9c,0x2e,0x6f,0xbc,0x63,0x06,0xe4,0x0d,0x6f,0x46,
		0x2d,0x63,0x46,0xb1,0xf5,0x5d,0xc6,0x7f,0xe7,0x80,0x35,0xb3,0xb9,0x54,0xe4,0x2f,
		0xd8,0xbe,0x9f,0x8a,0x6d,0xca,0x47,0xb0,0xcb,0x41,0x63,0x0f,0xb5,0xab,0x1b,0xd5,
		0xd0,0x35,0x3c,0xc4,0xb6,0xc1,0x1e,0x05,0x95,0xac,0xa5,0x7d,0x43,0x38,0xec,0x7e,
		0xe7,0xbf,0xc6,0x4e,0x57,0x2f,0xaf,0x11,0xd8,0x8d,0x2b,0xf2,0x94,0x18,0x53,0x26,
		0xde,0x53,0x4a,0xae,0x4a,0x26,0x0b,0xef,0x8a,0x2e,0x8d,0x16,0x2f,0x1d,0x1f,0x6b,
		0x90,0x9a,0xc5,0x32,0x1c,0xa3,0x23,0x8a,0xb8,0x28,0x43,0xe2,0x5d,0xd2,0xc1,0x55,
		0xbf,0x33,0xf1,0xa6,0xa4,0x26,0x18,0xc8,0x5c,0x8d,0x4a,0xa6,0x83,0x2a,0x33,0x42,
		0xe3,0x54,0x71,0x74,0x99,0x41,0x37,0x1a,0xcf,0x25,0x71,0x46,0xdd,0xc9,0x21,0xc6,
		0x15,0x8f,0x0e,0x59,0x26,0xd1,0x7f,0x19,0x7f,0x00,0xa3,0x7c,0xb3,0xa1,0xdd,0xa1,
		0xcb,0xde,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82);
}
