//
//  LibraryListing.h
//  FlacDemon
//
//  Created by merryclarke on 22/02/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef __FlacDemon__LibraryListing__
#define __FlacDemon__LibraryListing__

#include "includes.h"
#include "typedefs.h"
#include "FlacDemonNameSpace.h"

class FlacDemon::LibraryListing {
private:
protected:
    bool isSearchMatch;
public:
    bool matchesSearch() {
        return this->isSearchMatch;
    };
    std::string valueForKey (const char * key);
    virtual std::string valueForKey (std::string * key) = 0;
    virtual void setValueForKey(std::string * value, std::string* key) = 0;
};

#endif /* defined(__FlacDemon__LibraryListing__) */
