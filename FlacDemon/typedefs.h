/***********************************************************************
 * typedefs.h : Common flacdemon types
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

#ifndef FlacDemon_typedefs_h
#define FlacDemon_typedefs_h

#define set_flag this->flags = this->flags |
#define unset_flag this->flags = this->flags & ~
#define has_flag this->flags &

#define iset_flag(_flags, _flag) _flags = _flags | _flag
#define iunset_flag(_flags, _flag) _flags = _flags & ~ _flag
#define ihas_flag(_flags, _flag) _flags & _flag

#define FlacDemonMetaDataMultipleValues "FlacDemonMetaDataMultipleValues"

#include "FlacDemonNameSpace.h"

typedef std::pair < std::string, std::string > fd_keypair;
typedef std::map < std::string, std::string > fd_keymap;
typedef std::vector < fd_keymap * > fd_keymap_vector;
typedef std::basic_string <unsigned char> fd_ustring;


typedef std::vector < std::string > fd_stringvector;
typedef std::vector < FlacDemon::File * > fd_filevector;
typedef std::vector < FlacDemon::Track * > fd_trackvector;
typedef std::vector < FlacDemon::TrackListing * > fd_tracklistingvector;
typedef std::vector < FlacDemon::LibraryListing * > fd_librarylistingvector;
typedef std::vector < FlacDemon::Album * > fd_albumvector;
#endif
