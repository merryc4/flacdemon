/***********************************************************************
 * LibraryListing.cpp : Virtual class for any class to be displayed by the library
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


#ifndef __FlacDemon__LibraryListing__
#define __FlacDemon__LibraryListing__

#include "includes.h"
#include "typedefs.h"
#include "FlacDemonNameSpace.h"
#include "KeyValue.h"

class FlacDemon::LibraryListing : public KeyValue {
private:
protected:
    bool isSearchMatch;
public:
    bool matchesSearch() {
        return this->isSearchMatch;
    };
    virtual void setValueForKey(std::string & value, std::string & key) = 0;
};

#endif /* defined(__FlacDemon__LibraryListing__) */
