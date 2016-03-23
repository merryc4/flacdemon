/***********************************************************************
 * FlacDemonNameSpace.h : FlacDemon namespace definition
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

#ifndef FlacDemon_FlacDemonNameSpace_h
#define FlacDemon_FlacDemonNameSpace_h

namespace FlacDemon {
    class Demon;
    class CommandParser;
    class TCPHandler;
    class NCURSESHandler;
    class CommandManager;
    class FileImporter;
    class Scraper;
    class File;
    
    class TrackListing;
    class Track;
    class Group;
    class Album;
    class Library;
    class LibraryListing;
    
    class Database;
    class Player;
    
    class Session;
    class CommandMapBase;
    template < class Target > class CommandMap;
    
    class Verifier;
}

#endif
