/***********************************************************************
 * LibraryTitle.cpp : Library titles initialization
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

#include "LibraryTitles.h"

fd_stringvector * libraryTitlesTracks = new fd_stringvector{"id", "Track", "Disc", "Title", "Album", "Artist", "AlbumArtist", "Playcount", "Verified"};
fd_stringvector * libraryTitlesAlbums = new fd_stringvector{"albumuuid", "Album", "Artist", "AlbumArtist", "Tracks", "Playcount", "Verified"};
fd_stringvector * libraryTitlesAlbumCopyFromTrack = new fd_stringvector { "album" , "artist" , "albumartist" , "year" };