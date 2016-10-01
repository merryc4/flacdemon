/***********************************************************************
 * VerifierViewer.h : Let user verify and correct release metadata
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

#ifndef __FlacDemon__VerifierViewer__
#define __FlacDemon__VerifierViewer__

#include "FlacDemonNameSpace.h"
#include "includes.h"
#include "typedefs.h"

class FlacDemon::VerifierViewer {
private:
protected:
public:
    VerifierViewer();
    ~VerifierViewer();
    
    void showAlbum( FlacDemon::Album * album );
};

#endif /* defined(__FlacDemon__VerifierViewer__) */
