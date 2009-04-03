#ifndef __CONVERTVIDEOTYPES_H__
#define __CONVERTVIDEOTYPES_H__

/*! Output format conversion (DivX, MPEG1... ) */
enum OutputFormat
{
	ofAVI, ofWMV, ofMPEG1, ofMPEG2, ofMP4, ofAppleiPod, ofSonyPSP, of3GP, ofMP3
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

#endif // __CONVERTVIDEOTYPES_H__

