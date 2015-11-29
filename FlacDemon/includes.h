//
//  includes.h
//  FlacDemon
//
//  Created by merryclarke on 24/11/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef FlacDemon_includes_h
#define FlacDemon_includes_h

#include <stdio.h>
#include <sstream>
#include <regex>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include <boost/uuid/uuid.hpp>
#include <thread>
#include <uuid/uuid.h>
#include <mba/diff.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/dict.h>
    //#include <libavresample/avresample.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

#endif
