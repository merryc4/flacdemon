//
//  Session.h
//  FlacDemon
//
//  Created by merryclarke on 27/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef __FlacDemon__Session__
#define __FlacDemon__Session__

#include "includes.h"
#include "FlacDemonNameSpace.h"
#include "typedefs.h"

class FlacDemon::Session{
private:
    fd_keymap * stringValues;
protected:
public:
    Session();
    ~Session();
    
    void setString(std::string * key, std::string * value);
    void setString(const char * key, std::string * value);

    std::string getString(std::string * key);
    std::string getString(const char * key);
};

#endif /* defined(__FlacDemon__Session__) */
