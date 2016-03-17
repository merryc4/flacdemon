/***********************************************************************
 * FlacDemonInterface.h : Main class of any interface to the daemon
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

#ifndef __FlacDemon__FlacDemonInterface__
#define __FlacDemon__FlacDemonInterface__

#include "includes.h"

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "ncurses.h"
#include "panel.h"
#include <condition_variable>

#include "FlacDemonUtils.h"
#include "TrackListing.h"
#include "Library.h"
#include "CommandParser.h"
#include "LibraryTitles.h"
#include "CommandMap.h"


#define fd_interface_printlibrary   1 << 0
#define fd_interface_libraryupdate  1 << 1
#define fd_interface_printcommand   1 << 2
#define fd_interface_playing        1 << 3
#define fd_interface_printplaying   1 << 4
#define fd_interface_playbackprogress 1 << 5
#define fd_interface_printprogress  1 << 6
#define fd_interface_printalbum     1 << 7
#define fd_interface_verifyview     1 << 8

#define COLOR_GREY  8

#define COLOR_PAIR_BLACK_WHITE  1
#define COLOR_PAIR_GREY_WHITE   2
#define COLOR_PAIR_BLUE_WHITE   3
#define COLOR_PAIR_GREEN_WHITE  4

using std::cout;
using std::endl;

class FlacDemonInterface : public FlacDemon::CommandMap < FlacDemonInterface >{
private:
    int socketFileDescriptor;
    std::thread * readThread;
    std::thread * retryConnectThread;
//    std::mutex socketMutex;
    std::mutex eventMutex;    
    std::condition_variable eventCV;
    
    WINDOW * browserWindow;
    WINDOW * browserHeaderWindow;
    WINDOW * commandWindow;
    WINDOW * playbackWindow;
    
    WINDOW * verifyWindow;
    WINDOW * albumViewWindow;
    
    PANEL * browserPanel;
    PANEL * verifyPanel;
    
    FlacDemon::Library library;
    
    unsigned long flags;
    
    int maxColumns;
    int maxRows;
    int browserRows;
    int currentBrowserRow;
    size_t browserOffset;
    std::map < std::string , size_t > columnWidths;
    
    bool printAlbums;
    bool disableColors;
    bool colorsOn;
    
    size_t commandCursorPosition;
    size_t commandCursorDefault;
    
    FlacDemon::CommandParser commandParser;
    std::string commandPrompt;
    std::string searchPrompt;
    std::string userCommand;
    
    bool fetchedLibrary;
    bool killResponseThread;
    bool isSearch;
    bool typeSearch;
    
    
    FlacDemon::TrackListing * nowPlaying;
    float progress;
    
    FlacDemon::Album * currentViewAlbum;
    
protected:
    
public:
    FlacDemonInterface();
    ~FlacDemonInterface();
    void initialize();
    WINDOW * nextwin( size_t rowSize , size_t * row = nullptr , int reset = -1);
    void connect();
    void retryConnect();
    void onConnect();
    void run();
    void printFlags();
    void event(unsigned long rlflags = 0);
    void setRunLoopFlags(unsigned long flags);
    void userInputLoop();
    void trySearch();
    void clearSearch();
    void escapeHandler();
    fd_stringvector& libraryTitles();
    void printLibrary(int offset);
    void printLibraryHeaders();
    void printLibraryLine( WINDOW * window , std::vector< std:: string > * values);
    void callCommand( const char * signal, void * args );
    void sendCommand(const char * command);
    
    int search ( fd_stringvector * args );
    int show ( fd_stringvector * args );
    int verify ( fd_stringvector * args );
    
    void readResponse();
    void parseResponse(std::string response);
    std::string parseCommandFromResponse(std::string * response);
    std::string removeCommandFromResponse(std::string * response);
    const char * formatValue(std::string value, int max);
    void parseLibraryUpdate(std::string * response);
    void changeOffset(int diff, bool absolute = false);
    void setColumnWidths();
    void setNowPlaying(std::string ID);
    void printNowPlaying();
    void printCommand();
    void setCommandCursor();
    void printProgress();
    void waitForSearch();
    void setColor( WINDOW * window , int attr , bool onoff );
    void printAlbum( WINDOW * window , FlacDemon::Album * album );
    void printVerifySuggestions( FlacDemon::Album * album );
};

#endif /* defined(__FlacDemon__FlacDemonInterface__) */
