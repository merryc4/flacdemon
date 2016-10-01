/***********************************************************************
 * CommandParser.cpp : Command line parser
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

#include "CommandParser.h"

fd_stringvector * daemonCommands = new fd_stringvector{"add", "play", "stop", "get"};
fd_stringvector * interfaceCommands = new fd_stringvector{"search", "sort", "show", "verify"};

FlacDemon::CommandParser::CommandParser(){
    this->availableInterfaces = FDInterfaceCommandLine;
    
    auto f = boost::bind(&FlacDemon::CommandParser::signalReceiver, this, _1, _2);
    signalHandler->signals("interfaceAvailable")->connect(f);
    signalHandler->signals("runCommand")->connect(f);
}
FlacDemon::CommandParser::~CommandParser(){
    
}
void FlacDemon::CommandParser::signalReceiver(const char * signalName, void * arg){
    cout << "signal received: " << signalName << endl;
    if(strcmp(signalName, "interfaceAvailable") == 0){
        int interface = *((int *) arg);
        this->availableInterfaces |= interface;
    } else if (strcmp(signalName, "runCommand") == 0){
        std::string command = (char*)arg;
        this->parseCommand(command);
    }
}
bool FlacDemon::CommandParser::getCommand() {
    std::string * cmd = this->getInput();
    
    if( cmd )
        this->parseCommand(*cmd);
    bool ok = !!cmd;
    delete cmd;

    return ok;
}
std::string * FlacDemon::CommandParser::getInput() {
	cout << "Enter a command: ";
    std::string* input = new std::string();
    std::getline(std::cin >> std::ws, *input);
    if (std::cin.eof())
    {
        // rare for keyboard input - e.g. ^D on UNIX/Linux, ^Z Windows
        // can happen with pipes/redirects - e.g. echo 10 20 | ./my_app
        std::cerr << "expected value for input but encountered end of input\n";
        return nullptr;
    }
    else
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
	return input;
}
void FlacDemon::CommandParser::parseCommand( std::string & icommand , bool run ) {
    if (!icommand.length()){
        this->clear();
		return;
    }
    std::string tcommand = icommand;
    while(tcommand.back() == '\n'){
        tcommand.pop_back();
    }
    fd_strreplace(tcommand, "\t", " ");
    fd_strreplace(tcommand, "\n", " ");
    fd_strreplace(tcommand, "\r", " ");
    while(fd_strreplace(tcommand, "  ", " ")){};
    
    if( run ){
        this->history.push_back(tcommand);
        this->currentCommand = tcommand;
    }

    
	unsigned long pos = 0, skip = 0;
    std::string word;
    fd_stringvector args;
    
    std::regex reg("([\\'\\\"]).*?\\1(?=\\s|$)");
    std::regex reg2 = reg;
    std::smatch regmatch;
//    std::match_results<std::string::iterator> regmatch;
    
    
    args.push_back(tcommand);
    
    do {
        skip = 0;
        while(tcommand.front() == ' '){
            tcommand.erase(0, 1);
        }
        //        cout << "parse command regex search. first: " << *first << " last: " << *last << endl;
        if(regex_search(tcommand, regmatch, reg, std::regex_constants::match_continuous )){
            cout << "regex matched" << endl;
            pos = regmatch.length();
            skip = 1;
        } else if ( ( pos = tcommand.find(" ")) == std::string::npos ){
            pos = tcommand.length();
        }
        
        if(pos > tcommand.length())
            break;
        
        word = tcommand.substr( skip, pos - 2 * skip );
        tcommand.erase(0, pos);
        
        if( word.compare(" ") == 0 ){
            continue;
        }
        
        if( args.size() == 1 ){
            this->commandWord = word;
            this->commandArgs = tcommand;
        }
        
        args.push_back(word);
        
        
    } while ( tcommand.length() );
    this->currentArgs = args;
    this->checkCommand();
    if( run ) {
        commandManager->call(args);
    }
}
CommandType FlacDemon::CommandParser::checkCommand(){
    if(this->commandWord == "s" || this->commandWord == "search"){
        return ( this->commandType = search_command );
    }
    for(fd_stringvector::iterator it = daemonCommands->begin(); it != daemonCommands->end(); it++){
        if ( it->find(this->commandWord) != std::string::npos ) {
            return ( this->commandType = daemon_command );
        }
    }
    for(fd_stringvector::iterator it = interfaceCommands->begin(); it != interfaceCommands->end(); it++){
        if ( it->find(this->commandWord) != std::string::npos ) {
            return ( this->commandType = interface_command );
        }
    }
    return ( this-> commandType = no_command );
}
void FlacDemon::CommandParser::historyPush ( std::string & icommand ) {
    this->history.push_back( icommand );
}
void FlacDemon :: CommandParser :: clear() {
    this->commandType = no_command;
    this->commandArgs.clear();
    this->commandWord.clear();
    this->currentArgs.clear();
    
}