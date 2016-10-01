/***********************************************************************
 * DiscogsTypes.h : Classes to describe types from discogs api
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

#ifndef __FlacDemon__DiscogsTypes__
#define __FlacDemon__DiscogsTypes__

#include "includes.h"
#include "KeyValue.h"

/**
 * DISCOGS TYPE
 * Parent class for all discogs type classes
 */

class DiscogsType : public KeyValue {
protected:
    
    std::map < std::string, std::string & > _values;
    size_t _id;
    
    void parse( json_t * json );
    
    template < class T >
    void parseArray( json_t * json , std::vector < T * > & values);
    
    void copyValues( const DiscogsType & copyFrom );
 
public:
    const size_t & id;
    const std::map < std::string, std::string & > & values;
    
    DiscogsType( json_t * json );
    virtual ~DiscogsType();
    
    using KeyValue::valueForKey;
    std::string valueForKey( const std::string & key );
};

/**
 * DISCOGS RESULT
 * Describes a search result
 */
 

class DiscogsResult : public DiscogsType {
private:
    
    std::string _thumb;
    std::string _year;
    std::string _country;
    std::string _title;
    std::string _uri;
    std::string _catno;
    std::string _resource_url;
    std::string _type;
    std::string _albumtitle;
    std::string _artist;
    
    void parseTitle();
    
protected:
public:
    const std::string & thumb;
    const std::string & year;
    const std::string & country;
    const std::string & title;
    const std::string & uri;
    const std::string & catno;
    const std::string & resource_url;
    const std::string & type;
    const std::string & albumtitle;
    const std::string & artist;

    DiscogsResult( json_t * json );
    DiscogsResult( const DiscogsResult & copyFrom );
    DiscogsResult & operator=( const DiscogsResult & that );
    ~DiscogsResult();

};


/** DISCOGS SHORT ARTIST
 * Describes artist name , resource url and id for master release
 */
 

class DiscogsShortArtist : public DiscogsType {
private:
    std::string _name;
    std::string _resource_url;
public:
    const std::string & name;
    const std::string & resource_url;
    
    DiscogsShortArtist( json_t * json );
    DiscogsShortArtist( const DiscogsShortArtist & copyFrom );
    DiscogsShortArtist & operator=( const DiscogsShortArtist & that );
    ~DiscogsShortArtist();
};

/** DISCOGS TRACKLISTING
 * Describes a tracklisting on a release / master
 */

class DiscogsTrackListing : public DiscogsType {
private:
    
    std::string _duration;
    std::string _position;
    std::string _type_;
    std::string _title;
    
    std::vector < DiscogsShortArtist * > _artists;
    
protected:
public:
    
    const std::string & title;
    const std::string & duration;
    const std::string & position;
    const std::string & type_;
    
    const std::vector < DiscogsShortArtist * > & artists;
    
    DiscogsTrackListing( json_t * json);
    DiscogsTrackListing( const DiscogsTrackListing & copyFrom );
    DiscogsTrackListing & operator = ( const DiscogsTrackListing & that );
    ~DiscogsTrackListing();
    
};



/** DISCOGS MASTER
 * Describes a master release
 */

class DiscogsMaster : public DiscogsType {
private:
    
    std::string _title;
    std::string _main_release;
    std::string _main_release_url;
    std::string _versions_url;
    std::string _uri;
    std::string _year;
    std::string _resource_url;
    std::string _data_quality;
    
    std::vector < std::string > _style_tags;
    std::vector < DiscogsShortArtist * > _artists;
    std::vector < DiscogsTrackListing * > _tracks;
    
protected:
public:
    const std::string & title;
    const std::string & main_release;
    const std::string & main_release_url;
    const std::string & versions_url;
    const std::string & uri;
    const std::string & year;
    const std::string & resource_url;
    const std::string & data_quality;
    
    const std::vector < std::string > & style_tags;
    const std::vector < DiscogsShortArtist * > & artists;
    const std::vector < DiscogsTrackListing * > & tracks;
    
    uint flags;
    
    DiscogsMaster( json_t * json );
    DiscogsMaster( const DiscogsMaster & copyFrom );
    DiscogsMaster & operator = ( const DiscogsMaster & that );
    ~DiscogsMaster();
};


#endif /* defined(__FlacDemon__DiscogsTypes__) */
