//
//  main.cpp
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include <iostream>
#include "FlacDemonNameSpace.h"
#include "FlacDemon.h"

#define FDOPTIONS_USE_FLACS_DIR 0

const SignalHandler * signalHandler = new SignalHandler();
FlacDemon::Demon * demon = nullptr;

int main(int argc, const char * argv[])
{

    // insert code here...
    std::cout << "Hello, World!\n";
    
#ifdef FDOPTIONS_USE_FLACS_DIR
    char dir[] = "/Users/merryclarke/Documents/Xcode Projects/FlacDemon/";
#else
    char dir[] = "/mnt/Backup/Storage/FLACS/";
#endif
    
    cout << "working directory: " << dir << endl;
    
    chdir(dir);
    
    demon = new FlacDemon::Demon();
    
    initGlobals();
    
    demon->run();

    delete demon;
    delete signalHandler;
    
    
    return 0;
}

