/***********************************************************************
 * Session.h : Session data for received network command
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

#ifndef __FlacDemon__Session__
#define __FlacDemon__Session__

#include "includes.h"
#include "FlacDemonNameSpace.h"
#include "typedefs.h"

class FlacDemon::Session{
private:
    fd_keymap * stringValues;
protected:
public:
    Session();
    ~Session();
    
    void setString(std::string * key, std::string * value);
    void setString(const char * key, std::string * value);

    std::string getString(std::string * key);
    std::string getString(const char * key);
};

#endif /* defined(__FlacDemon__Session__) */
