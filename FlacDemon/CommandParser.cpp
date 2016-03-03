//
//  CommandParser.cpp
//  FlacDemon
//
//  Created by merryclarke on 22/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

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
        this->parseCommand(&command);
    }
}
void FlacDemon::CommandParser::getCommand() {
    std::string * cmd = this->getInput();
    this->parseCommand(cmd);
}
string* FlacDemon::CommandParser::getInput() {
	cout << "Enter a command: ";
    std::string* input = new std::string();
    getline(std::cin >> std::ws, *input);
	return input;
}
void FlacDemon::CommandParser::parseCommand( std::string* icommand , bool run ) {
    if (!icommand->length()){
        this->clear();
		return;
    }
    std::string * tcommand = new std::string( *icommand );
    while(tcommand->back() == '\n'){
        tcommand->pop_back();
    }
    fd_strreplace(tcommand, "\t", " ");
    fd_strreplace(tcommand, "\n", " ");
    fd_strreplace(tcommand, "\r", " ");
    while(fd_strreplace(tcommand, "  ", " ")){};
    
    if( run )
        this->history.push_back(*tcommand);

    
	unsigned long pos = 0, skip = 0;
    std::string word;
    fd_stringvector args;
    
    std::regex reg("([\\'\\\"]).*?\\1(?=\\s|$)");
    std::regex reg2 = reg;
    std::smatch regmatch;
//    std::match_results<std::string::iterator> regmatch;

    bool matchQuote = false;
    
    
    args.push_back(*tcommand);
    
    do {
        skip = 0;
        while(tcommand->front() == ' '){
            tcommand->erase(0, 1);
        }
        //        cout << "parse command regex search. first: " << *first << " last: " << *last << endl;
        if(regex_search(*tcommand, regmatch, reg, std::regex_constants::match_continuous )){
            cout << "regex matched" << endl;
            pos = regmatch.length();
            skip = 1;
        } else if ( ( pos = tcommand->find(" ")) == std::string::npos ){
            pos = tcommand->length();
        }
        
        if(pos > tcommand->length())
            break;
        
        word = tcommand->substr( skip, pos - 2 * skip );
        tcommand->erase(0, pos);
        
        if( word.compare(" ") == 0 ){
            continue;
        }
        
        if( args.size() == 1 ){
            this->commandWord = word;
            this->commandArgs = *tcommand;
        }
        
        args.push_back(word);
        
        
    } while ( tcommand->length() );
    this->currentArgs = args;
    this->checkCommand();
    if( run ) {
        signalHandler->call("callCommand", &args);
    }
    delete tcommand;
    
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
void FlacDemon::CommandParser::historyPush ( std::string * icommand ) {
    this->history.push_back( *icommand );
}
void FlacDemon :: CommandParser :: clear() {
    this->commandType = no_command;
    this->commandArgs.clear();
    this->commandWord.clear();
    this->currentArgs.clear();
    
}