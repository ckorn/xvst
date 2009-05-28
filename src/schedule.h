#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QtCore>
//
#include "options.h"

#ifdef Q_WS_MACX
static const QString SCHEDULE_FILE = "/../Application Support/xVideoServiceThief/schedules.plist";
#else
static const QString SCHEDULE_FILE = "/schedules.conf";
#endif

/*! Schedule item */
class ScheduleItem : public QObject
{
	private:
		QString comment;
		QTime startTime;
		QTime endTime;
		bool enabled;
		bool active;
	public:
		/*! Class constructor */
		ScheduleItem(bool enabled, QTime startTime, QTime endTime, QString comment = QString());
		/*! Set the start time */
		void setStartTime(QTime value);
		/*! Get the start time */
		QTime getStartTime();
		/*! Set the end time */
		void setEndTime(QTime value);
		/*! Get the end time */
		QTime getEndTime();
		/*! Set if this item is enabled */
		void setEnabled(bool value);
		/*! Get comment */
		QString getComment();
		/*! Set comment */
		void setComment(QString value);
		/*! Get if this item is enabled */
		bool isEnabled();
		/*! Get if this item is active (current time is between start and end times and is enabled) */
		bool isActive();
};

/*! Schedule controller */
class ScheduleController : public QObject
{
	private:
		QSettings::Format schedulesFileFormat;	//! Schedule file format
		QString schedulesFile;					//! Schedule file path
		QList<ScheduleItem *> *schedules;		//! List of schedules items
		bool enabled;							//! Flag for know if this controller is enabled
	public:
		/*! Class constructor */
		ScheduleController(QString schedulesBasePath, QSettings::Format format);
		/*! Class destructor */
		~ScheduleController();
		/*! Detrmine if we are into some of the configured ranges */
		bool canStart();
		/*! Save the current Schedule configuration */
		void save();
		/*! Open a saved Schedule configuration */
		void load();
		/*! Get if is enabled or disabled */
		bool isEnabled();
		/*! Set as enabled or disabled */
		void setEnabled(bool value);
		/*! Set as enabled */
		void enable();
		/*! Set as disabled */
		void disable();
		/*! Add a new schedule item */
		void addNewSchedule(bool enabled, QTime startTime, QTime endTime, QString comment = QString());
		/*! Remove a schedule item */
		void removeSchedule(int index);
		/*! Remove a schedule item */
		void removeSchedule(ScheduleItem *schedule);
		/*! Remove all schedules */
		void removeAllSchedules();
		/*! Get a schedule item */
		ScheduleItem *getScheduleItem(int index);
		/*! Get schedule count */
		int getSchedulesCount();
};

#endif // SCHEDULE_H
