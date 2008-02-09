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

#ifndef __PROGRAMVERSION_H__
#define __PROGRAMVERSION_H__

static const QString PROGRAM_VERSION		= "1.5 Alpha"; 	//<! Current program version
static const QString PROGRAM_VERSION_SHORT	= "1.5a"; 		//<! Current program version (short version)

#ifdef Q_OS_WIN32
static const QString CURRENT_OS = "MSWINDOWS"; //<! Microsoft windows version
#endif
#ifdef Q_OS_LINUX 
static const QString CURRENT_OS = "LINUX"; //<! Gnu/Linux version
#endif
#ifdef Q_WS_MAC
static const QString CURRENT_OS = "MACOSX"; //<! MacOS X version
#endif

#ifdef STATIC_BUILD
static const QString COMPILATION_MODE = "STATIC"; //<! Static compilation
#else
static const QString COMPILATION_MODE = "DYNAMIC"; //<! Dynamic compilation
#endif

#endif // __PROGRAMVERSION_H__
