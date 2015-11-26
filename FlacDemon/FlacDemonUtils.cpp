//
//  FlacDemonUtils.cpp
//  FlacDemon
//
//  Created by merryclarke on 24/11/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "FlacDemonUtils.h"

std::vector< std::string > * fd_numbers;

void initGlobals(){
    fd_numbers = new std::vector< std::string >{"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
}

int fd_stringtoint(std::string * str, int * value){
    if(!str)
        return 0;
    
    const char * cstr = str->c_str();
    int val = std::atoi(cstr);
    if(val || cstr[0] == '0'){
        *value = val;
        return 0;
    }
    std::string * numberStr = new std::string(*str);
    transform(numberStr->begin(), numberStr->end(), numberStr->begin(), ::tolower);

    int i = 0;
    for(std::vector<std::string>::iterator it = fd_numbers->begin(); it != fd_numbers->end(); it++){
        if(numberStr->compare((*it)) == 0){
            *value = i;
            return 0;
        }
        i++;
    }
    
    return 1;
}