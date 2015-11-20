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
#include <thread>
#include <uuid/uuid.h>

#include "SignalHandler.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/dict.h>
//#include <libavresample/avresample.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

typedef std::map<std::string, std::string *> fd_keymap;

namespace FlacDemon {
    class Demon;
    class CommandParser;
    class FileImporter;
    class File;
    
    class Track;
    class Group;
    class Album;
    
    class Database;
    class Player;
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

typedef std::basic_string <unsigned char> ustring;


#endif