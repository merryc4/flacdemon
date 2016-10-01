/***********************************************************************
 * DiscogsTypes.cpp : Classes to describe types from discogs api
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

#include "DiscogsTypes.h"


/** DISCOGS TYPE 
 */

DiscogsType::DiscogsType(json_t * json) : id( _id ), values( _values ){
  
    json_t * jsonId = json_object_get( json, "id" );
    
    if( json_is_integer( jsonId ) ){
        this->_id = json_integer_value( jsonId );
    }
}
DiscogsType::~DiscogsType(){
    
}
void DiscogsType::copyValues( const DiscogsType & copyFrom ) {
    for( std::map < std::string , std::string & > :: const_iterator it = copyFrom.values.begin(); it != copyFrom.values.end(); it++ ){
        this->values.at( it->first ).assign( it->second );
    }
    this->_id = copyFrom.id;
}
std::string DiscogsType::valueForKey( const std::string & key ){
    if( this->_values.count( key ))
        return this->_values.at( key );
    return "";
}
void DiscogsType::parse( json_t *json ){
    std::string key, value;
    json_t * jsonValue = nullptr;
    
    for ( std::map < std::string , std::string & >::iterator it = this->_values.begin(); it != this->_values.end(); it++ ){
        jsonValue = json_object_get( json , it->first.c_str() );
        if (json_is_string(jsonValue)){
            this->_values.at( it->first ).assign( json_string_value( jsonValue ) );
        }
    }
}

template < class T >
void DiscogsType::parseArray( json_t * json , std::vector < T * > & values){
    
    json_t * jsonValue = nullptr;
    size_t index = 0;
    
    if( json_array_size( json ) ){
        json_array_foreach( json, index, jsonValue ){
            T * t = new T(jsonValue);
            values.push_back( t );
        }
    }
}

/** DISCOGS RESULT
 */

DiscogsResult::DiscogsResult( json_t * json ) : DiscogsType::DiscogsType( json ),    thumb( _thumb ),
                                                                                                            year( _year ),
                                                                                                            country( _country ),
                                                                                                            title( _title ),
                                                                                                            uri( _uri ),
                                                                                                            catno( _catno ),
                                                                                                            resource_url( _resource_url ),
                                                                                                            type( _type ),
                                                                                                            albumtitle( _albumtitle ),
                                                                                                            artist( _artist )
{
    
    this->_values.insert( std::pair< std::string, std::string & >{ "thumb" , this->_thumb } );
    this->_values.insert( std::pair< std::string, std::string & >{ "year" , this->_year } );
    this->_values.insert( std::pair< std::string, std::string & >{ "country" , this->_country } );
    this->_values.insert( std::pair< std::string, std::string & >{ "title" , this->_title } );
    this->_values.insert( std::pair< std::string, std::string & >{ "uri" , this->_uri } );
    this->_values.insert( std::pair< std::string, std::string & >{ "catno" , this->_catno } );
    this->_values.insert( std::pair< std::string, std::string & >{ "resource_url" , this->_resource_url } );
    this->_values.insert( std::pair< std::string, std::string & >{ "type" , this->_type } );
    this->_values.insert( std::pair< std::string, std::string & >{ "albumtitle" , this->_albumtitle } );
    this->_values.insert( std::pair< std::string, std::string & >{ "artist" , this->_artist } );
    
    this->parse(json);
    this->parseTitle();
}
DiscogsResult::DiscogsResult( const DiscogsResult & copyFrom ) : DiscogsResult::DiscogsResult( nullptr ){
    this->copyValues( copyFrom );
}
DiscogsResult & DiscogsResult::operator=( const DiscogsResult & that ){
    if( this != & that ){
        this->copyValues( that );
    }
    return * this;
}
DiscogsResult::~DiscogsResult(){
    
}
void DiscogsResult::parseTitle(){
    size_t pos = 0;
    if( ( pos = this->title.find(" - ") ) ){
        this->_artist = this->title.substr( 0 , pos );
        pos += 3;
        if( pos < this->title.length() )
            this->_albumtitle = this->title.substr( pos );
    }
}

/** DISCOGS SHORT ARTIST
 */

DiscogsShortArtist::DiscogsShortArtist( json_t * json ) :  DiscogsType::DiscogsType( json ),
                                                                                            name( _name ),
                                                                                            resource_url( _resource_url )
{
    this->_values.insert( std::pair< std::string, std::string & >{ "name" , this->_name } );
    this->_values.insert( std::pair< std::string, std::string & >{ "resource_url" , this->_resource_url } );
    
    this->parse( json );
}
DiscogsShortArtist::DiscogsShortArtist( const DiscogsShortArtist & copyFrom ) : DiscogsShortArtist::DiscogsShortArtist( nullptr ){
    this->copyValues( copyFrom );
}
DiscogsShortArtist & DiscogsShortArtist::operator=( const DiscogsShortArtist & that ){
    if( this != & that ){
        this->copyValues( that );
    }
    return * this;
}
DiscogsShortArtist::~DiscogsShortArtist(){

}
/** DISCOGS TRACK LISTING
 */

DiscogsTrackListing::DiscogsTrackListing( json_t * json ) :   DiscogsType::DiscogsType( json ),
                                                                                title( _title ),
                                                                                duration( _duration ),
                                                                                position( _position ),
                                                                                type_( _type_ ),
                                                                                artists( _artists )

{
    this->_values.insert( std::pair< std::string, std::string & >{ "duration" , this->_duration } );
    this->_values.insert( std::pair< std::string, std::string & >{ "position" , this->_position } );
    this->_values.insert( std::pair< std::string, std::string & >{ "type_" , this->_type_ } );
    this->_values.insert( std::pair< std::string, std::string & >{ "title" , this->_title } );
    
    this->parse( json );
    this->parseArray( json_object_get( json, "artists" ), this->_artists );
}
DiscogsTrackListing::DiscogsTrackListing( const DiscogsTrackListing & copyFrom ) : DiscogsTrackListing::DiscogsTrackListing( nullptr ){
    this->copyValues( copyFrom );
    this->_artists = copyFrom.artists;
}
DiscogsTrackListing & DiscogsTrackListing::operator=( const DiscogsTrackListing & that ){
    if( this != & that ){
        this->copyValues( that );
        this->_artists = that.artists;
    }
    return * this;
}
DiscogsTrackListing::~DiscogsTrackListing(){
//    for( std::vector< DiscogsShortArtist & >::iterator it = this->_artists.begin(); it != this->_artists.end(); it++ ){
//        delete &(*it);
//    }
}
#pragma DiscogsMaster
/** DISCOGS MASTER
 */

DiscogsMaster::DiscogsMaster( json_t * json ):    DiscogsType::DiscogsType( json ),
                                                                    title( _title ),
                                                                    main_release(_main_release),
                                                                    main_release_url(_main_release_url),
                                                                    versions_url(_versions_url),
                                                                    uri(_uri),
                                                                    year(_year),
                                                                    resource_url(_resource_url),
                                                                    data_quality(_data_quality),
                                                                    style_tags( _style_tags ),
                                                                    artists( _artists ),
                                                                    tracks( _tracks )

{
    this->_values.insert( std::pair< std::string, std::string & >{ "title" , this->_title } );
    this->_values.insert( std::pair< std::string, std::string & >{ "main_relese" , this->_main_release } );
    this->_values.insert( std::pair< std::string, std::string & >{ "main_release_url" , this->_main_release_url } );
    this->_values.insert( std::pair< std::string, std::string & >{ "versions_url" , this->_versions_url } );
    this->_values.insert( std::pair< std::string, std::string & >{ "uri" , this->_uri } );
    this->_values.insert( std::pair< std::string, std::string & >{ "year" , this->_year } );
    this->_values.insert( std::pair< std::string, std::string & >{ "resource_url" , this->_resource_url } );
    this->_values.insert( std::pair< std::string, std::string & >{ "data_quality" , this->_data_quality } );

    this->parse( json );
    
    this->parseArray( json_object_get( json, "artists" ) , this->_artists );
    this->parseArray( json_object_get( json, "tracklist" ) , this->_tracks );
    
    this->flags = 0;
}
DiscogsMaster::DiscogsMaster( const DiscogsMaster & copyFrom ) : DiscogsMaster::DiscogsMaster( nullptr ){
    this->copyValues( copyFrom );
    
    this->flags = copyFrom.flags;
    this->_artists = copyFrom.artists;
    this->_tracks = copyFrom.tracks;
}
DiscogsMaster & DiscogsMaster::operator=( const DiscogsMaster & that ){
    if( this != & that ){
        this->copyValues( that );
        
        this->flags = that.flags;
        this->_artists = that.artists;
        this->_tracks = that.tracks;
    }
    return * this;
}
DiscogsMaster::~DiscogsMaster(){
    
}
