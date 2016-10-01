/***********************************************************************
 * Scraper.h : Retrieve release information from online database
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

#ifndef __FlacDemon__Scraper__
#define __FlacDemon__Scraper__

#include "FlacDemonAll.h"
#include "DiscogsTypes.h"

class FlacDemon::Scraper {
private:
protected:
    size_t connectionLimit;
    size_t connectionLimitTime;
    size_t connectionCount;
    size_t resultsArrayIndex;
    
    CURL * curlHandle;
    char * curlErrorBuffer;
    const char * curlUrl;
    
    std::string curlResponse;
    std::string searchBaseUrl;
    
//    std::string masterBaseUrl;
    
    std::map< std::string , json_int_t > currentPageInfo;
    
    json_t * jsonResultsArray;
    
    void curlSetup();
    void curlReset( std::string & url );
    void curlReset( const char * url = nullptr );
    int curlPerform();
    json_t * curlGetJson( std::string & url );
    void search( std::string & searchString );
    void getDiscogsAccessToken();
    void authorize();
    
public:
    Scraper();
    ~Scraper();
    
    void search( fd_keymap & values );
    DiscogsMaster * getMaster( std::string & releaseUrl );
    DiscogsResult * nextResult();
    void clear();
};


size_t curl_write_data( char * buffer , size_t size , size_t nmemb , void * userPointer);

#endif /* defined(__FlacDemon__Scraper__) */
