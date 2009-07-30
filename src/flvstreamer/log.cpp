/*  FLVStreamer
 *  Copyright (C) 2008-2009 Andrej Stepanchuk, The Flvstreamer Team
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with FLVStreamer; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "log.h"

#define MAX_PRINT_LEN	2048

extern int debuglevel;

FILE *fmsg = stderr;

void LogSetOutput(FILE *file)
{
	fmsg = file;
}

void LogPrintf(const char *format, ...)
{
        char str[MAX_PRINT_LEN]="";
        va_list args;
        va_start(args, format);
        vsnprintf(str, MAX_PRINT_LEN-1, format, args);
        va_end(args);

        if ( debuglevel==LOGCRIT )
          return;

        fprintf(fmsg, "%s", str);
	#ifdef _DEBUG
	fflush(fmsg);
	#endif
}

void Log(int level, const char *format, ...)
{
        char str[MAX_PRINT_LEN]="";
        va_list args;
        va_start(args, format);
        vsnprintf(str, MAX_PRINT_LEN-1, format, args);
        va_end(args);

	// Filter out 'no-name'
	if ( debuglevel<LOGALL && strstr(str, "no-name" ) != NULL )
	  return;

        if ( level <= debuglevel )
          fprintf(fmsg, "\r%s: %s\n", level==LOGDEBUG?"DEBUG":(level==LOGERROR?"ERROR":(level==LOGWARNING?"WARNING":(level==LOGCRIT?"CRIT":"INFO"))), str);
  
	#ifdef _DEBUG
	fflush(fmsg);
	#endif
}

void LogHex(const char *data, unsigned long len)
{
        if ( debuglevel==LOGCRIT )
          return;
	for(unsigned long i=0; i<len; i++) {
		LogPrintf("%02X ", (unsigned char)data[i]);
	}
	LogPrintf("\n");
}

void LogHexString(const char *data, unsigned long len)
{
        if ( debuglevel==LOGCRIT )
          return;
        for(unsigned long i=0; i<len; i++) {
                LogPrintf("%02X ", (unsigned char)data[i]);
        }
        LogPrintf("\n");

        for(unsigned long i=0; i<len; i++) {
                LogPrintf("%c", (unsigned char)data[i]);
        }
        LogPrintf("\n");
}

