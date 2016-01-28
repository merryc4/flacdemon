//
//  Session.cpp
//  FlacDemon
//
//  Created by merryclarke on 27/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "Session.h"
FlacDemon::Session::Session(){
    this->stringValues = nullptr;
}
FlacDemon::Session::~Session(){
    if(this->stringValues){
        for(fd_keymap::iterator it = this->stringValues->begin(); it != this->stringValues->end(); it++){
            delete it->second;
        }
        this->stringValues->clear();
    }
    delete this->stringValues;
}
void FlacDemon::Session::setString(std::string * key, std::string * value){
    if(!this->stringValues)
        this->stringValues = new fd_keymap;
    std::string * valuecopy = new std::string(*value);
    this->stringValues->insert(fd_keypair{*key, valuecopy});
}
void FlacDemon::Session::setString(const char * key, std::string * value){
    std::string tkey = key;
    this->setString(&tkey, value);
}
std::string * FlacDemon::Session::getString(std::string *key){
    if(this->stringValues->count(*key)){
        return this->stringValues->at(*key);
    }
    return nullptr;
}
std::string * FlacDemon::Session::getString(const char * key){
    std::string tkey = key;
    return this->getString(&tkey);
}