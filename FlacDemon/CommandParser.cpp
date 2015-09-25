//
//  CommandParser.cpp
//  FlacDemon
//
//  Created by merryclarke on 22/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "CommandParser.h"

FlacDemon::CommandParser::CommandParser(){

}
FlacDemon::CommandParser::~CommandParser(){
    
}
void FlacDemon::CommandParser::getCommand() {
    this->parseCommand(this->getInput());
}
string* FlacDemon::CommandParser::getInput() {
	cout << "Enter a command: ";
	string* input = new string();
    getline(cin >> ws, *input);
	return input;
}
void FlacDemon::CommandParser::parseCommand(string* command) {
	if (!command->length())
		return;
    
	unsigned long pos;
	string word, *commandWord = NULL;
    vector<string> args;
    std::map<string, demonCommandFunction>::iterator it;
    
	do {
		if ((pos = command->find(" ")) == string::npos){
			pos = command->length();
		}
        if(pos == 0)
            pos++;
        
        if(pos > command->length())
            break;
        
		word = command->substr(0, pos);
		*command = command->substr(pos, (unsigned long int)(command->length() - pos));
        
        if(word.compare(" ") == 0){
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