/*
*
* This file is part of xVideoServiceThief, 
* an open-source cross-platform Video service download
*
* Copyright (C) 2007 Xesc & Technology
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Xesc <xeskuu@gmail.com>
* Program URL   : http://xviservicethief.sourceforge.net/
*
*/

#ifndef __CONVERTVIDEO_H__
#define __CONVERTVIDEO_H__

#include <QtGui>
//
#include "videoitem.h"

/*! Output format conversion (DivX, MPEG1... ) */
enum OutputFormat
{
	ofAVI, ofWMV, ofMPEG1, ofMPEG2, ofAppleiPod, of3GP, ofMP3
};

/*! Video resolution */
enum VideoResolution
{
	vrOriginal, vr96x72, vr128x96, vr160x120, vr176x120, vr176x144,
	vr192x144, vr240x180, vr320x200, vr320x240, vr352x240, vr352x288, vr480x272,
	vr480x360, vr480x480, vr624x352, vr640x480, vr720x480, vr720x576
};

/*! Audio Sample Ratio */
enum AudioSampleRatio
{
	asrOriginal, asr22050, asr44100
};

/*! Video Frame Rate */
enum VideoFrameRate
{
	vfrOriginal, vfr15, vfr24, vfr25, vfr29_97, vfr30
};

/*! Output Quality */
enum OutputQuality 
{
	oqLower_quality, oqLow_quality, oqNormal_quality,
	oqMedium_quality, oqGood_quality, oqSuperb_quality, oqSame_quality
};

struct VideoConversionConfig
{
	OutputFormat outputFormat;
	VideoResolution videoResolution;
	AudioSampleRatio audioSampleRatio;
	VideoFrameRate videoFrameRate;
	OutputQuality outputQuality;
};

class VideoConverter : public QObject
{
Q_OBJECT
	private:
		QProcess *ffmpegProcess;	//!< ffmpeg process
		QString ffmpegApp;			//!< ffmpeg path (including the app)
		QString workingDir;			//!< where to save converted videos
		VideoConversionConfig convConf;	//!< conversion config
		VideoItem *videoItem;		//!< current video item
		float videoLength;			//!< video time duration (seconds)
		bool deleteOriginalVideo;	//!< delete the input video on finish?
		QString outputAll;			//!< Temporal console output
		/*! Build the ffmpeg command line */
		QStringList getCommandLine();
		/*! Convert & set the video duration to seconds */
		void setVideoDuration(QString strVideoDuration);
		/*! Get the current time conversion */
		void getCurrentTimeConversion(QString strFrame);
		/*! Parse the current output text */
		void parseOutput(QString output);
	public:
		/*! Class constructor */
		VideoConverter(QString ffmpegApp, QString workingDir, 
			VideoConversionConfig convConf, bool deleteOriginalVideo);
		/*! Class destructor */
		~VideoConverter();
		/*! Start a video conversion */
		void startConversionVideo(VideoItem *videoItem);
		/*! Cancel current conversion */
		void cancelConversion();
		/*! Get if possible start a new download */
		bool canStartConversion();
		/*! Get if is converting some video */
		bool isConverting();
		/*! Get if is possible convert videos (ffmpeg installed?) */
		bool ffmpegInstalled();
		/*! Get the ffmpeg app */
		QString getFFmpegApp();
		/*! Get the working dir */
		QString getWorkingDir();
		/*! Get the conversion config */
		VideoConversionConfig getConversionConfig();
		/* Get the flag of delete original video */
		bool getDeleteOriginalVideo();
		/*! Set the ffmpeg app */
		void setFFmpegApp(QString ffmpegApp);
		/*! Set the working dir */
		void setWorkingDir(QString workingDir);
		/*! Set the video conversion config */
		void setConversionConfig(VideoConversionConfig convConf);
		/*! Set the flag for delete original videos on finish the conversion */
		void setDeleteOriginalVideo(bool deleteOriginalVideo);
	private slots:
		/*! ffmpeg started */
		void started();
		/*! ffmpeg finished */
		void finished(int exitCode, QProcess::ExitStatus exitStatus);
		/*! output recived */
		void readyReadStandardOutput();
		/*! output by error recived */
		void readyReadStandardError();
	signals:
		/*! Video item has been modified */
		void videoItemUpdated(VideoItem *videoItem);
		/*! A Video item conversion started */
		void conversionStarted(VideoItem *videoItem);
		/*! A Video item conversion finished */
		void conversionFinished(VideoItem *videoItem);
};

#endif // __CONVERTVIDEO_H__
