/***********************************************************************
 * FlacDemonAll.h : all header files and data structures
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

#ifndef FlacDemon_FlacDemonAll_h
#define FlacDemon_FlacDemonAll_h

#include "includes.h"
#include "avincludes.h"
#include "netincludes.h"
#include "globals.h"
#include "typedefs.h"
#include "SignalHandler.h"
#include "FlacDemonUtils.h"
#include "SessionManager.h"
#include "FlacDemonNameSpace.h"

using std::cout;
using std::string;
using std::endl;

struct MediaStreamInfo {
    unsigned long bitRate;
    unsigned long sampleRate;
    unsigned long channels;
    unsigned long duration;
    AVCodecID codecID;
};

extern const SignalHandler * signalHandler;
extern FlacDemon::Demon * demon;
extern SessionManager * sessionManager;

//demon flags
#define demon_set_flag demon->flags |=
#define demon_unset_flag demon->flags = demon->flags & ~
#define demon_has_flag demon->flags &

#endif
