//
//  FlacDemonNameSpace.h
//  FlacDemon
//
//  Created by merryclarke on 22/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef FlacDemon_FlacDemonNameSpace_h
#define FlacDemon_FlacDemonNameSpace_h

#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include <boost/uuid/uuid.hpp>

#include "SignalHandler.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/dict.h>
}

namespace FlacDemon {
    class Demon;
    class CommandParser;
    class FileImporter;
    class File;
    
    class Track;
    class Group;
    class Album;
    
    class Database;
};

using namespace std;

typedef int (FlacDemon::Demon::*demonCommandFunction) (vector<string>*);

struct MediaStreamInfo {
    unsigned long bitRate;
    unsigned long sampleRate;
    unsigned long channels;
    unsigned long duration;
    AVCodecID codecID;
};

extern const SignalHandler * signalHandler;

#endif