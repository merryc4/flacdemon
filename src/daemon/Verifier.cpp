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

#include "Verifier.h"

FlacDemon::Verifier::Verifier(){
    
}
FlacDemon::Verifier::~Verifier(){
    
}
MatchType FlacDemon::Verifier::compareValues( std::string & value1 , std::string & value2 , float * match , bool checkMultiple ){
    MatchType result = MatchTypeNone;
    if( checkMultiple && fd_ismultipletag( value1 ) && fd_ismultipletag( value2 ) ) {
        result = MatchTypeFull;
        if( match ) *match = 1;
    } else {
        result = fd_comparetags( value1 , value2 , match);
    }
    return result;
}
MatchType FlacDemon::Verifier::compareValues( KeyValue & obj1 , KeyValue & obj2 , const char * key , float * match,  bool checkMultiple){
    std::string value1 = obj1.valueForKey( key ), value2 = obj2.valueForKey( key );
    return this->compareValues( value1 , value2 , match , checkMultiple );
}

MatchType FlacDemon::Verifier::compareValues( KeyValue & obj1 , KeyValue & obj2 , const char * key , const char * key2 , float * match, bool checkMultiple){
    std::string value1 = obj1.valueForKey( key ), value2 = obj2.valueForKey( key2 );
    return this->compareValues( value1 , value2 , match , checkMultiple );
}
int FlacDemon::Verifier::verifyAlbum( FlacDemon::Album & album ) {
    fd_keymap * map = album.getScraperKeymap();
    this->scraper.search( * map );
    delete map;
    DiscogsResult * result = nullptr;
    fd_stringvector matches;
    while( ( result = scraper.nextResult() ) ){
        if( this->compare( album, *result ) ) {
            matches.push_back( result->resource_url );
        }
        delete result;
    }
    if(matches.size() == 0) //try other search parameters
        return 0;
    if(matches.size() > 1)
        this->checkMasters( matches );
    else {
        DiscogsMaster * master = this->scraper.getMaster( matches[0] );
        cout << master->title << endl;
        if( this->compare( album , *master ) ){
            //verify album, copy values from discogs
        }
        
        delete master;
    }
    return 0;
}

uint FlacDemon::Verifier::compare( FlacDemon::Album & album , DiscogsResult & result ){
    
    float titleMatchValue = 0;
    float artistMatchValue = 0;
    
    MatchType titleMatchType = this->compareValues( album, result ,"album", "albumtitle" , & titleMatchValue);
    MatchType artistMatchType = this->compareValues( album, result, "artist", & artistMatchValue , true);
    
    //check styles?
    uint isMatch = 1;
    switch (titleMatchType) {
        case MatchTypeNone:
            isMatch = 0;
            break;
        case MatchTypeFull:
            //nothing
            break;
        case MatchTypePartial:
            if( titleMatchValue < FLACDEMON_VERIFICATION_TAG_MATCHING_THRESHOLD )
                isMatch = 0;
            break;
        case MatchTypeAContainsB:
        case MatchTypeBContainsA:
            cout << "Error: Unhandled match type for " << album.valueForKey("album") << endl;
            isMatch = 0;
            break;
    }
    
    switch (artistMatchType) {
        case MatchTypeNone:
            isMatch = 0;
            break;
        case MatchTypeFull:
            //nothing
            break;
        case MatchTypePartial:
            if( artistMatchValue < FLACDEMON_VERIFICATION_TAG_MATCHING_THRESHOLD )
                isMatch = 0;
            break;
        case MatchTypeAContainsB:
        case MatchTypeBContainsA:
            cout << "Error: Unhandled match type for " << album.valueForKey("artist") << endl;
            isMatch = 0;
            break;
    }
    
    return isMatch;
    
    //TODO check match types
    
}
void FlacDemon::Verifier::checkMasters( fd_stringvector & uris ){
    for(fd_stringvector::iterator it = uris.begin(); it != uris.end(); it++){
        
    }
}
float FlacDemon::Verifier::compare( FlacDemon::Album & album , DiscogsMaster & master ){

    float titleMatchValue = 0;
    MatchType titleMatchType = this->compareValues( album , master , "album" , "title" , &titleMatchValue );

    float trackMatchValue = this->compareTracks( album, master );
    
    uint isMatch = 1;
    switch (titleMatchType) {
        case MatchTypeNone:
            isMatch = 0;
            break;
        case MatchTypeFull:
            //nothing
            break;
        case MatchTypePartial:
            if( titleMatchValue < FLACDEMON_VERIFICATION_TAG_MATCHING_THRESHOLD )
                isMatch = 0;
            break;
        case MatchTypeAContainsB:
        case MatchTypeBContainsA:
            cout << "Error: Unhandled match type for " << album.valueForKey("artist") << endl;
            isMatch = 0;
            break;
    }
    
    if( isMatch )
        iset_flag(master.flags, FLACDEMON_MATCHES_TITLE);
    
    isMatch = 0;
    if( trackMatchValue > FLACDEMON_VERIFICATION_TRACK_MATCHING_THRESHOLD ){
        isMatch = 1;
        iset_flag(master.flags, FLACDEMON_MATCHES_TRACKS);
    }
    
    return isMatch;
}
float FlacDemon::Verifier::compare( FlacDemon::TrackListing & trackListing , DiscogsTrackListing & discogsTrackListing){
    
    float matchValue = 0;
    MatchType titleMatch = this->compareValues( trackListing , discogsTrackListing , "title" , & matchValue);
    if( titleMatch == MatchTypeFull || ( titleMatch == MatchTypePartial && matchValue > FLACDEMON_VERIFICATION_TAG_MATCHING_THRESHOLD )){
        return matchValue;
    } else {
        //TODO check other fields if titleMatch is other type or matchValue is not above threshold
    }
    return 0;
}
float FlacDemon::Verifier::compareTracks( FlacDemon::Album & album , DiscogsMaster & master){
    std::map< FlacDemon::TrackListing * , DiscogsTrackListing * > trackMap;
    std::vector< FlacDemon::TrackListing * > unpairedTracks;
    std::vector< int > trackNumbers;
    
    //check for record style track numbers A1, A2, B1 etc

    uint found = 0;
    
    //nested iteration through each set of tracks creating pairs of tracks
    for( fd_tracklistingvector::iterator it = album.tracks.begin(); it != album.tracks.end(); it++){
        found = 0;
        for( std::vector < DiscogsTrackListing * >::const_iterator it2 = master.tracks.begin(); it2 != master.tracks.end(); it2++ ){
            DiscogsTrackListing & discogsTrackListing = *( *it2 );
            FlacDemon::TrackListing & trackListing = *(*it);
            if( this->compare( trackListing , discogsTrackListing ) > FLACDEMON_VERIFICATION_TAG_MATCHING_THRESHOLD ){
                //stuff
                found++;
                if( found > 1 ){
                    cout << "Error: more than one matching track for " << (*it)->valueForKey("title") << endl;
                    return 0;
                }
                int position = 0;
                if( fd_stringtoint(discogsTrackListing.position, &position) == 0 ){
                    trackNumbers.push_back(position);
                }
                trackMap.insert(std::make_pair((*it), & discogsTrackListing ));
            }
        }
        if( !found )
            unpairedTracks.push_back( (*it) );
    }
    
    //TODO need to check if any of the tracks from discogs are not paired
    
    std::sort(trackNumbers.begin(), trackNumbers.end());
    std::vector< int > missingTrackNumbers;
    std::vector< int > ::iterator it = trackNumbers.begin();
    for(int count = 1 ; it != trackNumbers.end() ; count++ ){
        if( count == *it ){
            it++;
        } else {
            missingTrackNumbers.push_back( count );
        }
    }
    
    //calculate matching proportion
    
    if( unpairedTracks.size() || missingTrackNumbers.size() )
        return 0;
    
    return 1;
}