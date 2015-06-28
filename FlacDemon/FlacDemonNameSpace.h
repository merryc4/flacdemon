//
//  FlacDemonNameSpace.h
//  FlacDemon
//
//  Created by merryclarke on 22/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef FlacDemon_FlacDemonNameSpace_h
#define FlacDemon_FlacDemonNameSpace_h
#endif

#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
    
}

namespace FlacDemon {
    class Demon;
    class CommandParser;
    class FileImporter;
    
    class Track;
    class Group;
};

using namespace std;

typedef int (FlacDemon::Demon::*demonCommandFunction) (vector<string>*);

