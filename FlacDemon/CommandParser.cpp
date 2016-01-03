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
}
FlacDemon::CommandParser::~CommandParser(){
    
}
void FlacDemon::CommandParser::getCommand() {
    this->parseCommand(this->getInput());
}
string* FlacDemon::CommandParser::getInput() {
	cout << "Enter a command: ";
	string* input = new string();
    switch(demon->interfaceMode){
        case interfaceModeTerminal:
            getline(cin >> ws, *input);
            break;
        case interfaceModeCurses:
            
            break;
        case interfaceModeGUI:
            
            break;
    }
	return input;
}
void FlacDemon::CommandParser::parseCommand(string* command) {
	if (!command->length())
		return;
    
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
    
}
void FlacDemon::CommandParser::setMapForDemon(FlacDemon::Demon* iDemon, std::map<string, demonCommandFunction>* iMap){
    this->commandMap = iMap;
    this->demon = iDemon;
}