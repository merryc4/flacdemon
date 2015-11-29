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
int fd_comparetags(std::string * tag1, std::string * tag2);
const void * characterAtIndex(const void * str, int index, void * context);
int compareCharacters(const void * c1, const void * c2, void * context);

void fd_tolowercase(std::string * str);

#endif /* defined(__FlacDemon__FlacDemonUtils__) */
