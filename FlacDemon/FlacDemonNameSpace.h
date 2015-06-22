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

namespace FlacDemon {
    class Demon;
    class CommandParser;
};

using namespace std;

typedef int (FlacDemon::Demon::*demonCommandFunction) (vector<string>*);

