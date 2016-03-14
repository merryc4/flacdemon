/***********************************************************************
 * main.cpp : Interface main()
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

#include <iostream>
#include "FlacDemonInterface.h"

FlacDemonInterface * interface;

std::thread::id mainThreadID;
const SignalHandler * signalHandler = new SignalHandler();

int main(int argc, const char * argv[]) {
    mainThreadID = std::this_thread::get_id();

    initGlobals();
    sleep(2); //debug only, allows gdb to attach before initialising interface
    interface = new FlacDemonInterface();

    if(argc < 2 || *(argv[1])=='1'){
        interface->initialize();
    }

    if(argc < 3 || *(argv[2])=='1'){
        interface->connect();
    }

    interface->run();
    return 0;
}