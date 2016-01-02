//
//  CommandParser.h
//  FlacDemon
//
//  Created by merryclarke on 22/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__CommandParser__
#define __FlacDemon__CommandParser__

#include <iostream>
#include "FlacDemonNameSpace.h"
#include "FlacDemon.h"

class FlacDemon::CommandParser {
protected:
    std::vector<string>*commands;
    std::map<string, demonCommandFunction>* commandMap;
    FlacDemon::Demon *demon;

public:
    CommandParser();
    ~CommandParser();
    
    void getCommand();
    string* getInput();
    void parseCommand(string* command);
    void setMapForDemon(FlacDemon::Demon*, std::map<string, demonCommandFunction>*);
};

#endif /* defined(__FlacDemon__CommandParser__) */
