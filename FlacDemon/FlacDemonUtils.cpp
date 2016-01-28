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
std::string * fd_keymap_vectortojson(fd_keymap_vector * ikeymap_vector){
    //probably need a json string escape function
    std::stringstream ss;
    for(fd_keymap_vector::iterator it = ikeymap_vector->begin(); it != ikeymap_vector->end(); it++){
        ss << "{\n";
        for(fd_keymap::iterator it2 = (*it)->begin(); it2 != (*it)->end(); it2++){
            ss << "\t\"" << it2->first << "\":\"" << *(it2->second) << "\",\n";
        }
        ss << "},\n";
    }
    
    ss.ignore(2);
    std::string * json = new std::string(ss.str());
    return json;
}
std::string * fd_keymaptojson(fd_keymap * ikeymap){
    //probably need a json string escape function
    fd_keymap_vector kmv{ikeymap};
    return fd_keymap_vectortojson(&kmv);
}
fd_keymap_vector * fd_jsontokeymap_vector(std::string * json){
    
//    std::vector< std::string > objects{*json};
//    std::vector< std::string > splitobjects;
//    std::vector< std::string > temp;
    fd_keymap_vector * rkeymap_vector = new fd_keymap_vector;
    fd_keymap * tkeymap;
//    for (std::vector< std::string >::iterator it = objects.begin(); it != objects.end(); it++) {
//        temp = fd_splitjsondescriptor(&(*it));
//        splitobjects.insert(splitobjects.end(), temp.begin(), temp.end());
//    }
    
    std::vector < std::string > objects = fd_splitjsondescriptor(json);
    std::istringstream inStream;
    std::string line, key, value;
    size_t pos, pos2, offset;
    for(std::vector < std::string >::iterator it = objects.begin(); it != objects.end(); it++){
        tkeymap = new fd_keymap;
        rkeymap_vector->push_back(tkeymap);
        inStream.str(*it);
        do{
            std::getline(inStream, line);
            if((pos = line.find("\":\"")) != std::string::npos){
                pos2 = line.find("\"");
                if(pos2 == pos)
                    pos2 = 0;
                offset = pos2 ? 1 : 0;
                key = line.substr(pos2+offset, (pos - pos2 - offset));
                pos2 = line.find("\"", pos+3);
                value = line.substr(pos+3, (pos2 - pos - 3));
                tkeymap->insert(fd_keypair(key, new std::string(value)));
            }
        }while (!inStream.eof());
    }
    return rkeymap_vector;
}
std::vector< std::string > fd_splitjsondescriptor(std::string * json){
    std::regex reg("[{}]");
    std::match_results<std::string::iterator> regmatch;
    std::string::iterator last = json->end();
    std::string::iterator first = json->begin();
    int openBracketCount = 0, closeBracketCount = 0, depth = 0;
    std::vector< std::string > objects;
    while(std::regex_search(first, last, regmatch, reg) && first != last){
        first += (regmatch.position() + 1);
        if(regmatch.str().compare("{") == 0){
            openBracketCount++;
        } else {
            closeBracketCount++;
        }
        depth = openBracketCount - closeBracketCount;
        if(depth == 0){
            objects.push_back(std::string(json->begin(), first));
        }
    }
    
    return objects;
}
void waitfor0(int * value){
    while (*value == 0) {
        usleep(50);
    }
}
void waitfor0(bool * value){
    while (*value == false) {
        usleep(50);
    }
}
int isMainThread(){
    return std::this_thread::get_id() == mainThreadID;
}