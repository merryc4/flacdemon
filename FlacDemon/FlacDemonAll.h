//
//  FlacDemonAll.h
//  FlacDemon
//
//  Created by merryclarke on 27/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef FlacDemon_FlacDemonAll_h
#define FlacDemon_FlacDemonAll_h

#include "includes.h"
#include "avincludes.h"
#include "globals.h"
#include "typedefs.h"
#include "SignalHandler.h"
#include "FlacDemonUtils.h"
#include "SessionManager.h"
#include "FlacDemonNameSpace.h"

using std::cout;
using std::string;
using std::endl;

typedef int (FlacDemon::Demon::*demonCommandFunction) (std::vector<string>*);

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

typedef std::basic_string <unsigned char> ustring;


//demon flags
#define demon_set_flag demon->flags |=
#define demon_unset_flag demon->flags = demon->flags & ~
#define demon_has_flag demon->flags &

#endif
