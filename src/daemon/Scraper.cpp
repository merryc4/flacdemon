/***********************************************************************
 * Scraper.cpp : Retrieve release information from online database
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

#include "Scraper.h"

fd_stringvector pageKeys = {"items", "page" , "pages" , "per_page"};
fd_stringvector searchKeys = {"type","title","artist", "release_title","credit","anv","label","genre","style","country","year","format","catno","barcode","track","submitter","contributor"}; //TODO revise what all these are
fd_keymap searchKeyMap = { { "release_title" , "album" } };


FlacDemon::Scraper::Scraper(){
    connectionLimit = 240; //limit for discogs
    connectionLimitTime = 60; //seconds
    connectionCount = 0;
    this->curlSetup();
    std::string srch = "Nirvana";
    
    for(std::vector < std::string >::iterator it = pageKeys.begin(); it != pageKeys.end(); it++){
        this->currentPageInfo.insert(std::make_pair((*it), 0));
    }
    
    this->clear();
    
//    this->search( srch );

}

FlacDemon::Scraper::~Scraper(){
    
}

void FlacDemon::Scraper::curlSetup(){
    this->curlHandle = curl_easy_init();
    this->curlErrorBuffer = new char[ CURL_ERROR_SIZE ];
    this->searchBaseUrl = "https://api.discogs.com/database/search?per_page=100&type=master&q=";
    
    struct curl_slist *headers=NULL;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    curl_slist_append(headers, "User-Agent: FlacDaemon/0.1");
    char authHeader[] = "Authorization: Discogs key=wSIuirHInweGSLaGhEbG, secret=detqjfDvDIeipjvzhffnLCFfvbGTsyfo";
    curl_slist_append(headers,  authHeader);
    
    curl_easy_setopt( this->curlHandle , CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt( this->curlHandle , CURLOPT_WRITEFUNCTION , curl_write_data );
    curl_easy_setopt( this->curlHandle , CURLOPT_WRITEDATA , & this->curlResponse );
    curl_easy_setopt( this->curlHandle , CURLOPT_HEADER , 0 );
    curl_easy_setopt( this->curlHandle , CURLOPT_ERRORBUFFER , this->curlErrorBuffer );
    
    this->curlReset();
}

void FlacDemon::Scraper::curlReset( std::string & url ){
    this->curlReset( url.c_str() );
}

void FlacDemon::Scraper::curlReset( const char * url ){
    this->curlResponse.assign("");
    bzero(this->curlErrorBuffer, CURL_ERROR_SIZE);
    this->curlUrl = (url ? url : "" );
    curl_easy_setopt( this->curlHandle, CURLOPT_URL, this->curlUrl);
}

int FlacDemon::Scraper::curlPerform(){
    CURLcode result;
    cout << "performing discogs query " << this->curlUrl << endl;
    if ( ( result = curl_easy_perform( this->curlHandle ) ) != CURLE_OK ) {
        cout << "Error: curl returned error code " << result << ": " << this->curlErrorBuffer << endl;
        return 1;
    }
//    cout << "curl result: " << this->curlResponse << endl;
    return 0;
}

json_t * FlacDemon::Scraper::curlGetJson( std::string & url ){
    
    this->curlReset( url );
    
    if ( this->curlPerform() ) return nullptr;
    
    json_t * json = fd_decodeJSON( this->curlResponse );
    if( ! json )
        return nullptr; //TODO error reported in fd_decodeJSON, should handle it here though
    
    char * jsonStr = json_dumps(json, JSON_INDENT(4) | JSON_SORT_KEYS);
    cout << jsonStr << endl;
    free(jsonStr);
    
    return json;
}

void FlacDemon::Scraper::getDiscogsAccessToken(){
    if ( this->curlPerform() ){
        //error
        return;
    }
//    cout << "curl result: " << this->curlResponse << endl;
}

void FlacDemon::Scraper::authorize(){
    
}

void FlacDemon::Scraper::search( fd_keymap & values ){
    
    std::stringstream ss;
    
    for( fd_stringvector::iterator it = searchKeys.begin(); it != searchKeys.end(); it++){
        std::string key, value;
        if( searchKeyMap.count( *it ))
            key = searchKeyMap.at( *it );
        else
            key = ( *it );
        if( values.count( key ) ){
            //TODO check for sane value
            value = values.at( key );
            ss << "&" << (*it) << "=" << curl_easy_escape(this->curlHandle, value.c_str(), 0);
        }
    }
    std::string srch = ss.str();
    this->search( srch );
}

void FlacDemon::Scraper::search( std::string & searchString ){
    std::string url = this->searchBaseUrl;
    url.append( searchString );

    json_t * json = nullptr;
    if( ! ( json = this->curlGetJson( url ) ) )
        return;
    
    json_t * jsonPagination = nullptr;
    
    if(json_is_object(json) && (jsonPagination = json_object_get(json, "pagination"))){
        json_t * jsonTemp = nullptr;
        
        for(std::vector < std::string >::iterator it = pageKeys.begin(); it != pageKeys.end(); it++){
            if( ( jsonTemp = json_object_get(jsonPagination, it->c_str())) ){
                this->currentPageInfo.at( (*it) ) = json_integer_value( jsonTemp );
                //TODO if json_integer_value fails it returns 0 so this is ok but should check for fail vs actual 0 value
            }
        }
        
        size_t resultsLength = 0;
        
        if((this->jsonResultsArray = json_object_get(json, "results")) && (resultsLength = json_array_size( this->jsonResultsArray ))){
            //TODO something
        } else {
            cout << "No results found or error occured" << endl;
        }
    }
}

DiscogsMaster * FlacDemon::Scraper::getMaster( std::string & releaseUrl ) {

    json_t * json = nullptr;
    if( ! ( json = this->curlGetJson( releaseUrl ) ) )
        return nullptr;
    
    
    if( json_is_object( json ) ){
        return new DiscogsMaster( json );
    }
    
    return nullptr;
    
}

DiscogsResult * FlacDemon::Scraper::nextResult(){
    json_t * jsonResult = nullptr;
    DiscogsResult * result = nullptr;
    
    //TODO Check pagination
    
    if((jsonResult = json_array_get(this->jsonResultsArray, this->resultsArrayIndex))){
        this->resultsArrayIndex++;
        result = new DiscogsResult( jsonResult );
    }
    
    return result;
}
void FlacDemon::Scraper::clear(){
    this->resultsArrayIndex = 0;
    this->jsonResultsArray = nullptr;
}

size_t curl_write_data( char * buffer , size_t size , size_t nmemb , void * userPointer){
    std::string * responseString = ( std::string * ) userPointer;
    responseString->append( buffer );
    return size * nmemb;
}