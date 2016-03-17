/***********************************************************************
 * FileImporter.h : Import files into database
 * part of FlacDemon
 ************************************************************************
 *  Copyright (c) 2016 Meriadoc Clarke.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

#ifndef __FlacDemon__FileImporter__
#define __FlacDemon__FileImporter__

#include <iostream>
#include "FlacDemonAll.h"
#include "File.h"
#include "SignalHandler.h"


class FlacDemon::FileImporter {
protected:
    
public:
    FileImporter();
    ~FileImporter();
    void importFilesFromPath(string*);
    void addFileHandler(const char * signal, void * arg);
    void addFile( FlacDemon::File * file );
};


#endif /* defined(__FlacDemon__FileImporter__) */
