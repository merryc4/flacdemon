/***********************************************************************
 * Session.cpp : Session data for received network command
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

#include "Session.h"
FlacDemon::Session::Session(){
    this->stringValues = nullptr;
}
FlacDemon::Session::~Session(){
    if(this->stringValues){
        this->stringValues->clear();
    }
    delete this->stringValues;
}
void FlacDemon::Session::setString(std::string * key, std::string * value){
    if(!key || !value)
        return;
    if(!this->stringValues)
        this->stringValues = new fd_keymap;
    std::string * valuecopy = new std::string(*value);
    this->stringValues->insert(fd_keypair{*key, *valuecopy});
}
void FlacDemon::Session::setString(const char * key, std::string * value){
    std::string tkey = key;
    this->setString(&tkey, value);
}
std::string FlacDemon::Session::getString(std::string *key){
    std::string rvalue = "";
    if(this->stringValues && this->stringValues->count(*key)){
        rvalue = this->stringValues->at(*key);
    }
    return rvalue;
}
std::string FlacDemon::Session::getString(const char * key){
    std::string tkey = key;
    return this->getString(&tkey);
}