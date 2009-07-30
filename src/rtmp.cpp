#include "rtmp.h"

#include "http.h"
#include "tools.h"

#include  <signal.h>

RTMP::RTMP(QString flvstreamerPath)
{
	setObjectName("RTMP");
	// init internal vars
	init();
	// set up the flvstream app path
	this->flvstreamerPath = flvstreamerPath + "/" + FLVSTREAMER_PATH;
	// create speed avarage
	downloadSpeedAvg = new ArrayAvg(100);
	// create a new qprocess
	flvstreamerProcess = new QProcess();
	// connect signals
	connect(flvstreamerProcess, SIGNAL(started()), this, SLOT(started()));
	connect(flvstreamerProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));
	connect(flvstreamerProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
	connect(flvstreamerProcess, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
}

RTMP::~RTMP()
{
	if (pauseOnDestroyF) pause(); else cancel();

	delete downloadSpeedAvg;
	delete flvstreamerProcess;
}

void RTMP::init()
{
	stopReason = EnumRTMP::NO_STOPPED;
	resuming = false;
	currentDownloadedSize = 0;
	lastDownloadedSize = 0;
	downloadSpeed = 0;
	timeRemaining = 0;
}

bool RTMP::isFlvstreamerInstalled()
{
	return QFile::exists(flvstreamerPath);
}

int RTMP::download(const QString URL, QString destination, QString fileName, bool autoName)
{
	if (!isFlvstreamerInstalled())
		return EnumRTMP::FLVSTREAMER_MISSING;

	// clean destination dir
	destination = QDir::cleanPath(destination);

#ifdef Q_WS_WIN
	// fix Qt4.4.0 bug in windows
	if (destination.indexOf(":/") == -1)
		destination.replace(":", ":/");
#endif

	// check if is already downloading another file
	if (isDownloading())
		return EnumHTTP::ALREADY_DOWNLOADING;

	// create the destination path, if it don't exists
	if (!QDir(destination).exists())
		if (!QDir(destination).mkpath(destination))
			return EnumHTTP::UNABLE_CREATE_DIR;

	// set a default name (only if it is empty)
	if (fileName.isEmpty())
		fileName = "download.file";

	fileName = QFileInfo(fileName).fileName();

	// set destination file name
	fileName = cleanFileName(fileName);

	// get an unique file name for this download
	if (autoName)
		fileName = uniqueFileName(destination + "/" + fileName);
	else
		fileName = destination + "/" + fileName;

	// set file info
	destinationFile = QFileInfo(fileName);

	// set as downloading
	resuming = false;

	qDebug() << flvstreamerPath << QStringList() << "-r" << URL << "-o" << fileName;

	// start download
	flvstreamerProcess->start(flvstreamerPath, QStringList() << "-r" << URL << "-o" << fileName);

	// ok
	return 0;
}

int RTMP::resume(const QString URL, QString fileName)
{
	if (!isFlvstreamerInstalled())
		return EnumRTMP::FLVSTREAMER_MISSING;

	// check if is already downloading another file
	if (isDownloading())
		return EnumHTTP::ALREADY_DOWNLOADING;

	if (!QFile::exists(fileName))
		return EnumHTTP::MISSING_RESUME_FILE;

	// set file info
	destinationFile = QFileInfo(fileName);

	// set as resuming
	resuming = true;

	// resume download
	flvstreamerProcess->start(flvstreamerPath, QStringList() << "-r" << URL << "-e" << "-o" << fileName);

	// ok
	return 0;
}

void RTMP::pause()
{
	if (isDownloading())
	{	
		stopReason = EnumRTMP::USER_PAUSED;
		// kill the flvstreaming
		kill(flvstreamerProcess->pid(), SIGINT);
	}
}

void RTMP::cancel()
{
	if (isDownloading())
	{
		stopReason = EnumRTMP::USER_CANCELLED;
		// kill the flvstreaming
		flvstreamerProcess->kill();
		// remove file from disc
		QFile::remove(destinationFile.absoluteFilePath());
	}
}

void RTMP::pauseOnDestroy(bool pauseOnDestroyF)
{
	this->pauseOnDestroyF = pauseOnDestroyF;
}

bool RTMP::isDownloading()
{
	return flvstreamerProcess->state() != QProcess::NotRunning;
}

int RTMP::getFileSize()
{
	return fileSize;
}

int RTMP::getDownloadSpeed()
{
	return downloadSpeed;
}

int RTMP::getTimeRemaining()
{
	return timeRemaining;
}

QFileInfo RTMP::getDestinationFile()
{
	return destinationFile;
}

void RTMP::started()
{
	init();
	// start timer
	singleTimerShot();
	// send download started signal
	if (resuming)
		emit downloadResumed();
	else // not resuming (starting)
		emit downloadStarted();
}

void RTMP::finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	// user cancelled
	if (stopReason == EnumRTMP::USER_CANCELLED)
		emit downloadCanceled();
	// user paused
	else if (stopReason == EnumRTMP::USER_PAUSED)
		emit downloadPaused(destinationFile);
	// download finished
	else if (stopReason == EnumRTMP::NO_STOPPED)
		emit downloadFinished(destinationFile);
}

void RTMP::readyReadStandardOutput()
{
	parseOutput(QString::fromLocal8Bit(flvstreamerProcess->readAllStandardOutput()));
}

void RTMP::readyReadStandardError()
{
	parseOutput(QString::fromLocal8Bit(flvstreamerProcess->readAllStandardError()));
}

void RTMP::parseOutput(QString output)
{
	output = output.trimmed();

	qDebug() << output;

	// split output into lines
	QStringList lines = output.split("\n");
	// parse each line
	foreach(QString line, lines)
	{
		QStringList tokens = line.split(" ", QString::SkipEmptyParts);
		// parse tokens
		if (!tokens.isEmpty())
		{
			// get fileSze
			if (tokens.at(0) == "filesize")
				fileSize = tokens.at(1).toInt(NULL, 10);
			// get error and send error signal
			else if (tokens.at(0) == "E:" && stopReason == EnumRTMP::NO_STOPPED)
				emit downloadError(tokens.at(1).toInt(NULL, 10));
			// get download event
			else if (tokens.at(0) == "D:")
			{
				currentDownloadedSize = tokens.at(1).toInt(NULL, 10);
				// send signal
				emit downloadEvent(currentDownloadedSize, fileSize);
			}
		}
	}
}

void RTMP::singleTimerShot()
{
	QTimer::singleShot(1000, this, SLOT(timerEvent()));
}

void RTMP::timerEvent()
{
	// calcule download speed
	downloadSpeed = static_cast<int>(downloadSpeedAvg->add(currentDownloadedSize - lastDownloadedSize));
	// time remaining
	if (downloadSpeed != 0)
		timeRemaining = (fileSize - currentDownloadedSize) / downloadSpeed;
	// update counters
	lastDownloadedSize = currentDownloadedSize;
	// next timer shot
	if (isDownloading()) singleTimerShot();
}
