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
float fd_comparetags(std::string * tag1, std::string * tag2){
    if(!tag1->length() || !tag2->length())
        return 0;
    tag1 = new std::string(*tag1); fd_tolowercase(tag1);
    tag2 = new std::string(*tag2); fd_tolowercase(tag2);
    struct varray * ses = varray_new(sizeof(struct diff_edit), NULL);
    int numElements;
    int editDistance = diff(tag1, 0, (int)tag1->length(), tag2, 0, (int)tag2->length(), characterAtIndex, compareCharacters, NULL, 0, ses, &numElements, NULL);
    std::cout << "edit distance " << editDistance << std::endl;
    int match_count = 0;
    int nonmatch_count = 0;
    int diff_delete = 0;
    for(int i = 0; i < numElements; i++){
        struct diff_edit * e = (struct diff_edit *)varray_get(ses, i);
        switch (e->op) {
            case DIFF_MATCH:
                if(diff_delete){
                    nonmatch_count+=diff_delete;
                    diff_delete = 0;
                }
                match_count+=e->len;
                break;
            case DIFF_DELETE:
                diff_delete += e->len;
                break;
            case DIFF_INSERT:
                if(diff_delete){
                    nonmatch_count += abs((e->len - diff_delete));
                    diff_delete = 0;
                } else {
                    nonmatch_count += e->len;
                }
                break;
        }
    }
    
    float percent = 1.0 * (match_count - nonmatch_count) / tag1->length();
    free(tag1);
    free(tag2);
    return percent;
}

const void * characterAtIndex(const void * str, int index, void * context){
    return &((std::string *)str)->at(index);
}
int compareCharacters(const void * c1, const void * c2, void * context){
    return (*((char*)c1)) != (*((char*)c2));
}
void fd_tolowercase(std::string * str){
    transform(str->begin(), str->end(), str->begin(), ::tolower);
}
void fd_strreplace(std::string * str, std::string * search, std::string * replace){
    fd_strreplace(str, search->c_str(), replace->c_str());
}
void fd_strreplace(std::string * str, const char * search, const char * replace){
    size_t pos = str->find(search);
    str->erase(pos, strlen(search));
    str->insert(pos, replace);
}
std::string fd_sqlescape(std::string isql) { //pointer version might save memory
    return regex_replace(isql, std::regex("'"), "''");
}
std::string * fd_keymaptojson(fd_keymap * ikeymap){
    //probably need a json string escape function
    std::stringstream ss;
    ss << "{\n";
    for(fd_keymap::iterator it = ikeymap->begin(); it != ikeymap->end(); it++){
        ss << "\t\"" << it->first << "\":\"" << *(it->second) << "\",\n";
    }
    ss << "}";
    std::string * json = new std::string(ss.str());
    return json;
}
