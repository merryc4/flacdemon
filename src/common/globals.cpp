/***********************************************************************
 * globals.cpp : global variables initialization
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

#include "globals.h"
#include "SignalHandler.h"
#include "SessionManager.h"
#include "includes.h"


const SignalHandler * signalHandler = new SignalHandler();
std::thread::id mainThreadID = std::this_thread::get_id();
SessionManager * sessionManager = new SessionManager();
char * workingDirectory = nullptr;
