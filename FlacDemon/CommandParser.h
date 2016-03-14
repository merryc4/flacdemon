/***********************************************************************
 * CommandParser.h : Command line parser
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
