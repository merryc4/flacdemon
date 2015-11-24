//
//  FlacDemonUtils.h
//  FlacDemon
//
//  Created by merryclarke on 24/11/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__FlacDemonUtils__
#define __FlacDemon__FlacDemonUtils__

#include "includes.h"

extern std::vector< std::string > * fd_numbers;

void initGlobals();
int fd_stringtoint(std::string * str, int * value);

#endif /* defined(__FlacDemon__FlacDemonUtils__) */
