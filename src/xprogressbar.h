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

#ifndef XPROGRESSBAR_H
#define XPROGRESSBAR_H
//
#include <QRect>
#include <QColor>
#include <QPainter>
#include <QLinearGradient>
#include <QLocale>
//
/*! xProgressBar class */
class xProgressBar : public QObject
{
Q_OBJECT
	private:
		// progress vlues
		float progressValue;	//!< Progress bar value
		int schemaIndex;		//!< Color schema
		bool displayText;		//!< Flag for know if can display a % text or not
		int vSpan;				//!< Vertical span
		int hSpan;				//!< Horitzontal span
		// painter config
		QRect rect;				//!< Display rect
		QPainter *painter;		//!< Painter class
		// text color
		QColor textColor;		//!< Font color
		// progress colors
		QColor backgroundBorderColor;	//!< Background line color
		QColor backgroundColor;			//!< Background color
		QColor gradBorderColor;			//!< Gradient line color
		QColor gradColor1;				//!< Gradient color 1
		QColor gradColor2;				//!< Gradient color 2
		/*! Configure the color schema */
		void setColor();	
	public:
		/*! Class constructor */
		xProgressBar(QRect rect, QPainter *painter, int schemaIndex = 0);
		/*! Paint the xProgressBar */
		void paint();
		/*! Set the color schema */
		void setColorSchema(const int value);
		/*! Set a progress bar value */
		void setValue(const float value);
		/*! Set if should display the % text */
		void setDisplayText(const bool display);
		/*! Set the vertical span */
		void setVerticalSpan(const int value);
		/*! Set the horitzontal span */
		void setHorizontalSpan(const int value);
		/*! Paint the xProgressBar directly to a QPixmap */
		static QPixmap getPixmap(QSize size, float progressValue, bool displayText = false, int schemaIndex = 0);
};
#endif
