//
//  Scraper.h
//  FlacDemon
//
//  Created by merryclarke on 29/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

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
