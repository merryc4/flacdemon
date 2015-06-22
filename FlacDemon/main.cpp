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

int main(int argc, const char * argv[])
{

    // insert code here...
    std::cout << "Hello, World!\n";
    
    FlacDemon::Demon* demon = new FlacDemon::Demon();
    
    demon->run();

    delete demon;
    
    
    return 0;
}

