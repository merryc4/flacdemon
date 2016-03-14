/***********************************************************************
 * FlacDemon.h : Main class and command handler
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

#ifndef __FlacDemon__FlacDemon__
#define __FlacDemon__FlacDemon__

#include "FlacDemonAll.h"
#include "CommandParser.h"
#include "ncursesHandler.h"
#include "FileImporter.h"
#include "Database.h"
#include "Player.h"
#include "TCPHandler.h"
#include "Session.h"
#include "Scraper.h"
#include "includes.h"
#include "tests.h"

#include "CommandMap.h"


enum flacdemonInterfaceMode {
    interfaceModeTerminal = 0,
    interfaceModeCurses,
    interfaceModeGUI
};

using namespace std;

class FlacDemon::Demon : public FlacDemon::CommandMap < FlacDemon::Demon > {
    friend FlacDemon::CommandParser;
private:
    
protected:
    vector<string>* commands;
    FlacDemon::TCPHandler * tcpHandler;
    FlacDemon::CommandParser* commandParser;
    FlacDemon::FileImporter* fileImporter;
    FlacDemon::Player * player;
public:
    int flags;
    int interfaceMode;
    
    FlacDemon::Database* database;
    FlacDemon::Scraper * scraper;

    Demon();
    ~Demon();
    
    void run();
    int add( fd_stringvector * args);
    int play( fd_stringvector * args);
    int stop( fd_stringvector * args);
    int set( fd_stringvector * args);
    int get( fd_stringvector * args);
    int verify( fd_stringvector * args);
};

#endif /* defined(__FlacDemon__FlacDemon__) */
