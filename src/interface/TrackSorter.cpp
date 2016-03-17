/***********************************************************************
 * TrackSorter.cpp : Sort track arrays
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

#include "TrackSorter.h"
static fd_stringvector * sortKeys = new fd_stringvector {"genre", "composer", "albumartist", "album", "disc", "track"};

template < class T >
TrackSorter < T > :: TrackSorter(std::string sortKey){
    if(sortKey.compare("artist") == 0)
        sortKey = "albumartist";
    this->currentSortKey = sortKey;
}


template < class T >
bool TrackSorter < T >::operator()( T a, T b ){
    bool isDiff = false, nextKey = true;
    int diff;
    std::string sortKey;
    
    for( fd_stringvector::iterator it = std::find(sortKeys->begin(), sortKeys->end(), this->currentSortKey); it != sortKeys->end() && !isDiff && nextKey; it++){
        sortKey = (*it);
        std::string value1, value2;
        if( ! (isDiff = ( ( diff = fd_strnumbercompare( &( value1 = a->valueForKey( &sortKey ) ) , &( value2 = b->valueForKey( &sortKey ) ) ) ) < 0 )) ){
            nextKey = (diff == 0);
        }
    }
    return isDiff;
};

