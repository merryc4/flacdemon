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


class FlacDemon::Scraper {
private:
protected:
    size_t connectionLimit;
    size_t connectionLimitTime;
    size_t connectionCount;
    
    CURL * curlHandle;
    char * curlErrorBuffer;
    
    std::string curlResponse;
    std::string searchBaseUrl;
    
    void curlSetup();
    void curlReset();
    int curlPerform();
    void search( std::string & searchString );
    void getDiscogsAccessToken();
    void authorize();
    
    
public:
    Scraper();
    ~Scraper();
};


size_t curl_write_data( char * buffer , size_t size , size_t nmemb , void * userPointer);

#endif /* defined(__FlacDemon__Scraper__) */
