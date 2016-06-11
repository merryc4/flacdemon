/***********************************************************************
 * CommandManager.h : Manage all command maps
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

#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include "includes.h"
#include "FlacDemonNameSpace.h"
#include "FlacDemonUtils.h"
#include "CommandMapBase.h"

class FlacDemon::CommandManager {
private:
protected:
    std::map < std::string , FlacDemon::CommandMapBase * > commandMaps;
public:
    CommandManager();
    virtual ~CommandManager();
    void add( FlacDemon::CommandMapBase * map );
    void call( fd_stringvector & );
};

#endif /* COMMANDMANAGER_H_ */
