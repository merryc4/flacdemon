//
//  FlacDemonNameSpace.h
//  FlacDemon
//
//  Created by merryclarke on 22/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef FlacDemon_FlacDemonNameSpace_h
#define FlacDemon_FlacDemonNameSpace_h

#include "includes.h"
#include "avincludes.h"
#include "typedefs.h"
#include "SignalHandler.h"
#include "FlacDemonUtils.h"

namespace FlacDemon {
    class Demon;
    class CommandParser;
    class TCPHandler;
    class NCURSESHandler;
    class FileImporter;
    class File;
    
    class Track;
    class Group;
    class Album;
    
    class Database;
    class Player;
};

using std::cout;
using std::string;
using std::endl;

typedef int (FlacDemon::Demon::*demonCommandFunction) (std::vector<string>*);
typedef std::vector< FlacDemon::File * > fd_filevector;

struct MediaStreamInfo {
    unsigned long bitRate;
    unsigned long sampleRate;
    unsigned long channels;
    unsigned long duration;
    AVCodecID codecID;
};

extern const SignalHandler * signalHandler;
extern FlacDemon::Demon * demon;

typedef std::basic_string <unsigned char> ustring;


//demon flags
#define demon_set_flag demon->flags |=
#define demon_unset_flag demon->flags = demon->flags & ~
#define demon_has_flag demon->flags &

#endif