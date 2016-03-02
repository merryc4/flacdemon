//
//  LibraryListing.cpp
//  FlacDemon
//
//  Created by merryclarke on 22/02/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "LibraryListing.h"

std::string FlacDemon::LibraryListing::valueForKey (const char * key) {
    std::string tkey = key;
    return this->valueForKey(&tkey);
};