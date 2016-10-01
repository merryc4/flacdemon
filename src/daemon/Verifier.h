/***********************************************************************
 * Verifier.cpp : Verify and correct release metadata
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

#ifndef __FlacDemon__Verifier__
#define __FlacDemon__Verifier__

#include "FlacDemonNameSpace.h"
#include "Album.h"
#include "FlacDemonUtils.h"
#include "Scraper.h"
#include "includes.h"
#include "typedefs.h"
#include "KeyValue.h"

#define FLACDEMON_VERIFICATION_TAG_MATCHING_THRESHOLD 0.85
#define FLACDEMON_VERIFICATION_TRACK_MATCHING_THRESHOLD 0.85

#define FLACDEMON_MATCHES_TITLE     1 << 0
#define FLACDEMON_MATCHES_ARTIST    1 << 1
#define FLACDEMON_MATCHES_TRACKS    1 << 2


class FlacDemon::Verifier {
private:
    FlacDemon::Scraper scraper;
    
    MatchType compareValues( std::string & value1 , std::string & value2 , float * match = nullptr, bool checkMultiple = false );
    MatchType compareValues( KeyValue & obj1 , KeyValue & obj2 , const char * key , float * match = nullptr , bool checkMutliple = false);
    MatchType compareValues( KeyValue & obj1 , KeyValue & obj2 , const char * key , const char * key2, float * match = nullptr, bool checkMultiple = false);
    
    uint compare( FlacDemon::Album & album , DiscogsResult & result );
    float compare( FlacDemon::Album & album , DiscogsMaster & master );
    float compare( FlacDemon::TrackListing & trackListing , DiscogsTrackListing & discogsTrackListing);
    
    float compareTracks( FlacDemon::Album & album , DiscogsMaster & master);
    void checkMasters( fd_stringvector & uris );
    
    std::map < std::string , std::vector < DiscogsMaster * > > verifications;
    
protected:
public:
    Verifier();
    ~Verifier();
    
    int verifyAlbum( FlacDemon::Album & album );
};

#endif /* defined(__FlacDemon__Verifier__) */
