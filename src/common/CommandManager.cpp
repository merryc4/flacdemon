/***********************************************************************
 * CommandManager.cpp : Manage all command maps
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


#include "CommandManager.h"

FlacDemon::CommandManager::CommandManager() {
	// TODO Auto-generated constructor stub
}

FlacDemon::CommandManager::~CommandManager() {
	// TODO Auto-generated destructor stub
}

void FlacDemon::CommandManager::add( FlacDemon::CommandMapBase * map ) {
    this->commandMaps.insert( std::pair< std::string , FlacDemon::CommandMapBase * >{ fd_uuid(), map } );
}
void FlacDemon::CommandManager::call( fd_stringvector & args ) {
    //call command maps
    for( std::map< std::string , FlacDemon::CommandMapBase * >::iterator it = this->commandMaps.begin(); it != this->commandMaps.end(); it++){
        it->second->callCommandHandler(args);
    }
}