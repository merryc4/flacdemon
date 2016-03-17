/***********************************************************************
 * tests.cpp : Automated development tests
 * part of FlacDemon
 ************************************************************************
 *  Copyright (c) 2016 Meriadoc Clarke.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/


#include "tests.h"

std::mutex testMutex;

void run_tests(){
    //add tests here
//    test_comparetags();
//    test_insertsql();
//    demon->database->fillDatabase(50000);
//    cout << *demon->database->getJSONForID(1) << endl;
//    cout << *demon->database->getAll() << endl;
//    cout << *demon->database->getValue(1, new std::string("title"));
    
//    std::string all = *demon->database->getAll();
//    fd_keymap_vector * test = fd_jsontokeymap_vector(&all);

    
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