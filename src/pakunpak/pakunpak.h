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

#ifndef __PAKUNPAK_H__
#define __PAKUNPAK_H__

#include <string>
#include <vector>
#include <fstream>

#include <iostream>

class Packer
{
	private:
		std::vector<std::string> *files; //!< List with all files to add
	public:
		/*! Class constructor */
		Packer();
		/*! Class destructor */
		~Packer();
		/*! Make the package */
		void buildPackage(const std::string packageFile);
		/*! Add file to be packed */
		void addFile(const std::string fileName);
};

class Unpacker
{
	public:
		/*! Class constructor */
		Unpacker();
		/*! Class destructor */
		~Unpacker();
};

#endif // __PAKUNPAK_H__
