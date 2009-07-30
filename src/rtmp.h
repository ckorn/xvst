#ifndef RTMP_H
#define RTMP_H

#include <QtCore>

#ifdef Q_OS_WIN32
static const QString FLVSTREAMER_PATH = "flvstreamer.exe";	//!< flvstreamer app (Win32)
#else
static const QString FLVSTREAMER_PATH = "flvstreamer";		//!< flvstreamer app (*unix)
#endif

class ArrayAvg;

namespace EnumRTMP
{
	enum Error
	{
		FLVSTREAMER_MISSING = 100,	//100
	};

	enum StopReason
	{
		NO_STOPPED = 100,			//100
		DOWNLOAD_FINISHED,			//101
		USER_CANCELLED,				//102
		USER_PAUSED,				//103
	};
}

/*! flvstreamer Qt4 wrapper */
class RTMP : public QObject
{
Q_OBJECT
	private:
		QProcess *flvstreamerProcess;	//!< flvstreamer process
		QString flvstreamerPath;		//!< base dir where flvstreamer is located (ie: /users/xesk/.xvst/)
		ArrayAvg *downloadSpeedAvg;		//!< Download speed avg calculator
		QFileInfo destinationFile;		//!< Destination file information
		EnumRTMP::StopReason stopReason;	//!< Flag for know if the user aborted
		bool resuming;				//!< Flag for know if is being resumed
		int fileSize;				//!< File size to download
		int downloadSpeed;			//!< Current download speed (bytes second)
		int internalTimer;			//!< Internal timer id
		int currentDownloadedSize;	//!< Currend downloaded size
		int lastDownloadedSize;		//!< Last downloaded size
		int timeRemaining;			//!< Time remaining in seconds
		bool pauseOnDestroyF;		//!< Should pause the download instead of cancel it?
		/*! Parse the current output text */
		void parseOutput(QString output);
		/*! Single timer shot */
		void singleTimerShot();
		/*! Init internal variables */
		void init();
	public:
		/*! Class constructor */
		RTMP(QString flvstreamerPath);
		/*! Class destructor */
		~RTMP();
		/*! Return if flvstreamer is installed (detected) */
		bool isFlvstreamerInstalled();
		/*! Start a new asynchronously download */
		int download(const QString URL, QString destination, QString fileName = "", bool autoName = true);
		/*! Resume a previous asynchronously download */
		int resume(const QString URL, QString fileName);
		/*! Pause the current asynchronously download */
		void pause();
		/*! Cancel current download */
		void cancel();
		/*! Pause on destroy the RTMP class (only if is downloading) */
		void pauseOnDestroy(bool pauseOnDestroyF = true);
		/*! Get if is downloading */
		bool isDownloading();
		/*! Get file size */
		int getFileSize();
		/*! Get download speed */
		int getDownloadSpeed();
		/*! Get time remaining */
		int getTimeRemaining();
		/*! Get the destination file name */
		QFileInfo getDestinationFile();
	private slots:
		/*! flvstreamer started */
		void started();
		/*! flvstreamer finished */
		void finished(int exitCode, QProcess::ExitStatus exitStatus);
		/*! output recived */
		void readyReadStandardOutput();
		/*! output by error recived */
		void readyReadStandardError();
		/*! Internal timer event */
		void timerEvent();
	signals:
		/*! when a download started */
		void downloadStarted();
		/*! when a download finished */
		void downloadFinished(const QFileInfo destFile);
		/*! when a download has been paused */
		void downloadPaused(const QFileInfo destFile);
		/*! when a download has been resumed */
		void downloadResumed();
		/*! when a download file has been canceled */
		void downloadCanceled();
		/*! an error ocurred during the download process */
		void downloadError(int error);
		/*! when the rtmp read data */
		void downloadEvent(int pos, int max);
};

#endif // RTMP_H
