//
//  tests.cpp
//  FlacDemon
//
//  Created by merryclarke on 28/11/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "tests.h"

std::mutex testMutex;

void run_tests(){
    //add tests here
//    test_comparetags();
//    test_insertsql();
//    demon->database->fillDatabase(50000);
//    cout << *demon->database->getJSONForID(1) << endl;
}
void test_comparetags(){
    std::string tag1 = "compare test string";
    std::string tag2 = "compare test a string";
    fd_comparetags(&tag1, &tag2);
}
void test_insertsql(){
   
}
void lockMutex(){
    if(!testMutex.try_lock())
        return;
//    std::lock_guard<std::mutex> lock(testMutex);
//    sleep(10);
}