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
#include "FlacDemonAll.h"

enum FDInterfaceMethods {
    FDInterfaceStartIterate = 1,
    FDInterfaceCommandLine = 1,
    FDInterfaceSocket = 2,
    FDInterfaceMaxIterate = 4
};

enum CommandType {
    no_command,
    daemon_command,
    interface_command,
    search_command
};

class FlacDemon::CommandParser {
protected:
    int availableInterfaces;
    size_t historySize;
    fd_stringvector history;
public:
    
    CommandType commandType;
    std::string commandWord;
    std::string commandArgs;
    fd_stringvector currentArgs;
    
    CommandParser();
    ~CommandParser();

    void signalReceiver(const char * signalName, void * arg);

    void getCommand();
    std::string * getInput();
    void parseCommand( std::string* icommand , bool run = true );
    CommandType checkCommand();
    void historyPush ( std::string * icommand );
    void clear();
};

#endif /* defined(__FlacDemon__CommandParser__) */
