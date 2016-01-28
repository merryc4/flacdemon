//
//  CommandParser.cpp
//  FlacDemon
//
//  Created by merryclarke on 22/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "CommandParser.h"

FlacDemon::CommandParser::CommandParser(){
    this->commands = new std::vector<std::string>;
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
void FlacDemon::CommandParser::startCommandThreads(){
//    for(int i = FDInterfaceStartIterate; i < FDInterfaceMaxIterate; i = (i << 1)){
//        if(i & this->availableInterfaces){
//            
//        }
//    }
    if(this->availableInterfaces & FDInterfaceCommandLine){
        //happens on main thread
    }
    if(this->availableInterfaces & FDInterfaceSocket){
        
    }
}
void FlacDemon::CommandParser::getCommand() {
    std::string * cmd = this->getInput();
    this->parseCommand(cmd);
    std::string * results = sessionManager->getSession()->getString(cmd);
    if(results)
        cout << *results << endl;
}
string* FlacDemon::CommandParser::getInput() {
	cout << "Enter a command: ";
    std::string* input = new std::string();
    getline(cin >> ws, *input);
	return input;
}
void FlacDemon::CommandParser::checkSocketsLoop(){
    
}
void FlacDemon::CommandParser::parseCommand(string* command) {
	if (!command->length())
		return;
    command = new std::string(*command);
    while(command->back() == '\n'){
        command->pop_back();
    }
    
	unsigned long pos = 0, skip;
	string word, *commandWord = nullptr;
    vector<string> args;
    std::map<string, demonCommandFunction>::iterator it;
    
    std::regex reg("[\\s\\'\\\"]");
    std::regex reg2 = reg;
    std::match_results<std::string::iterator> regmatch;

    bool matchQuote = false;
    
    std::string::iterator last = command->end();
    std::string::iterator first;
    
    args.push_back(*command);
    
    do {
        first = command->begin();
        skip = 0;
        if(regex_search(first, last, regmatch, reg2)){
            pos = regmatch.position();
            if(regmatch.str().compare(" ")){
                if(matchQuote){
                    matchQuote = false;
                    skip = 1;
                    reg2 = reg;
                } else {
                    reg2 = regmatch.str();
                    matchQuote = true;
                }
            } else if(matchQuote){
                continue;
            }
            
        } else {
			pos = command->length();
		}
        
        if(pos == 0)
            pos++;
        
        if(pos > command->length())
            break;
        
		word = command->substr(0, pos);
		*command = command->substr(pos+skip, (unsigned long int)(command->length() - (pos+skip)));
        
        pos -= word.length();
        
        if(word.compare(" ") == 0 || matchQuote){
            continue;
        }
        
        if(!commandWord){
            if((it = this->commandMap->find(word)) != this->commandMap->end()){
                commandWord = new string(word);
                cout << "command is '" << word << "'" << endl;                
            } else {
                cout << "command '" << word << "' is unknown" << endl;
                delete command;
                return;
            }
        } else {
            args.push_back(word);
            cout << "argument: " << word << endl;
        }
        
	} while (pos < command->length());
    
    if(it->second){
        (this->demon->*it->second)(&args);
    }
    delete command;
    
}
void FlacDemon::CommandParser::setMapForDemon(FlacDemon::Demon* iDemon, std::map<string, demonCommandFunction>* iMap){
    this->commandMap = iMap;
    this->demon = iDemon;
}