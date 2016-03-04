//
//  Scraper.cpp
//  FlacDemon
//
//  Created by merryclarke on 29/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "Scraper.h"

FlacDemon::Scraper::Scraper(){
    connectionLimit = 240; //limit for discogs
    connectionLimitTime = 60; //seconds
    connectionCount = 0;
    this->curlSetup();
    std::string srch = "Nirvana";
    this->search( srch );
}
FlacDemon::Scraper::~Scraper(){
    
}
void FlacDemon::Scraper::curlSetup(){
    this->curlHandle = curl_easy_init();
    this->curlErrorBuffer = new char[ CURL_ERROR_SIZE ];
    this->searchBaseUrl = "https://api.discogs.com/database/search?type=release&";
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
void FlacDemon::Scraper::curlReset(){
    this->curlResponse = "";
}
int FlacDemon::Scraper::curlPerform(){
    CURLcode result;
    if ( ( result = curl_easy_perform( this->curlHandle ) ) != CURLE_OK ) {
        cout << "Error: curl returned error code " << result << ": " << this->curlErrorBuffer << endl;
        return 1;
    }
    cout << "curl result: " << this->curlResponse << endl;
    return 0;
}
void FlacDemon::Scraper::getDiscogsAccessToken(){
    if ( this->curlPerform() ){
        //error
        return;
    }
    cout << "curl result: " << this->curlResponse << endl;
}
void FlacDemon::Scraper::authorize(){
    
}
void FlacDemon::Scraper::search( std::string & searchString ){
    std::string url = this->searchBaseUrl;
    url.append( searchString );
    curl_easy_setopt( this->curlHandle , CURLOPT_URL , url.c_str() );

    if ( this->curlPerform() ) return;
    
    json_t * json = fd_decodeJSON( this->curlResponse );
    
}

size_t curl_write_data( char * buffer , size_t size , size_t nmemb , void * userPointer){
    std::string * responseString = ( std::string * ) userPointer;
    responseString->append( buffer );
    return size * nmemb;
}