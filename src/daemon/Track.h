/***********************************************************************
 * Track.h : Data for a single audio track
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

#ifndef __FlacDemon__Track__
#define __FlacDemon__Track__

#include <iostream>
#include <time.h>
#include "FlacDemonAll.h"
#include "File.h"
#include "TrackListing.h"

class FlacDemon::Track: public FlacDemon::TrackListing {
protected:
    
public:
    friend FlacDemon::File;
//    using FlacDemon::TrackListing::valueForKey;
    
    FlacDemon::File* file = nullptr;
    struct MediaStreamInfo * mediaStreamInfo = nullptr;

    Track (FlacDemon::File* file = nullptr);
    Track (fd_keymap * keymap);
    ~Track();
    
    void incrementPlaycount ();
    void decrementPlaycount ();
    
    void setFile(FlacDemon::File*);
    void setValuesFromAudioContext(AVCodecContext*);
    
    void initInfo();
    int openFilePath();
    
    virtual std::string keymapFileValue( std::string * key );
    
};

#endif /* defined(__FlacDemon__Track__) */
