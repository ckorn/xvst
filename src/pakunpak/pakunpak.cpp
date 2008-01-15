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

#include "pakunpak.h"

/* Support functions */

std::string getFileName(const std::string fileName)
{
  size_t found = fileName.find_last_of("/\\");
  return fileName.substr(found + 1);
}

/* Packer */

Packer::Packer()
{
	files = new std::vector<std::string>;
}

Packer::~Packer()
{
	files->clear();
	delete files ;
}

void Packer::buildPackage(const std::string packageFile)
{
	if (files->empty()) return;

	std::ofstream *package = new std::ofstream(packageFile.c_str(), std::ios::binary);
	
	if (package->is_open())
	{
		// write package header
		char *header_id = new char[4];
		header_id = "XPK";
		package->write(header_id, sizeof(header_id)); // package header id
		delete[] header_id;
		
		// add files into the package
		for (int n = 0; n < files->size(); n++)
		{
			// get the file name
			std::string fileName = getFileName(files->at(n));
			int fileNameLength = fileName.length();
			
			// open file
			std::ifstream *file = new std::ifstream(files->at(n).c_str(), std::ios::binary);
			if (file->is_open())
			{
				// get the file size
				file->seekg(0, std::ios::end);
				int fileSize = file->tellg();
				file->seekg(0, std::ios::beg);
	
				// allocate file in memory
				char *buffer = new char[fileSize];
				file->read(buffer, fileSize);
				
				// write data into the package
				package->write(reinterpret_cast<char *>(&fileNameLength), sizeof(fileNameLength)); // file name length
				package->write(fileName.c_str(), fileNameLength); // file name
				package->write(reinterpret_cast<char *>(&fileSize), sizeof(fileSize)); // file size
				package->write(buffer, fileSize); // file data
				
				// close file
				file->close();
				
				// delete buffer
				delete[] buffer;
			}
			delete file;
		}
	}
	delete package ;
}

void Packer::addFile(const std::string fileName)
{
	files->push_back(fileName);
}

/* Unpacker class */

Unpacker::Unpacker()
{
}

Unpacker::~Unpacker()
{
}
