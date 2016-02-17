//
//  FlacDemonInterface.cpp
//  FlacDemon
//
//  Created by merryclarke on 02/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "FlacDemonInterface.h"

std::map< std::string , unsigned long > * commandFlags = new std::map < std::string , unsigned long >{
    { "get all" , fd_interface_libraryupdate },
    { "playing" , fd_interface_playing },
    { "playbackProgress", fd_interface_playbackprogress }
};

FlacDemonInterface::FlacDemonInterface(){
    //init
    this->socketFileDescriptor = -1;
    this->readThread = nullptr;
    this->retryConnectThread = nullptr;
    this->fetchedLibrary = false;
    this->killResponseThread=false;
    this->flags = 0;
    this->commandCursorPosition = 0;
    this->progress = 0;
    this->isSearch = false;
    this->typeSearch = false;
    this->browserOffset = 0;
    this->userCommand = "";
    this->printAlbums = false;
    
    auto f = boost::bind(&FlacDemonInterface::callCommand, this, _1, _2);
    signalHandler->signals("callCommand")->connect(f);
//    this->initialize();
}
FlacDemonInterface::~FlacDemonInterface(){
    //de-init
}
void FlacDemonInterface::initialize(){
    
    
    //redirect cout to log file
    std::ofstream * out = new std::ofstream("fd_interface.log");
    std::cout.rdbuf(out->rdbuf());

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, true);
    refresh(); //clears screen and sets scroll to correct position
    
    if( has_colors() == false || can_change_color() == false ){
        cout << "no colors" << endl;
    }
    cout << "terminal supports " << COLORS << " colors" << endl;

    init_color( COLOR_WHITE, 1000, 1000, 1000 );
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);
    init_pair(3, COLOR_GREEN, COLOR_WHITE);
    
    bkgd(COLOR_PAIR(1));
    
    char msg[] = "FlacDemon NCURSES Interface";
    
    getmaxyx(stdscr, this->maxRows, this->maxColumns);
    
    WINDOW * titleWindow = this->nextwin( 1 );
    this->playbackWindow = this->nextwin( 3 );
    this->commandWindow = this->nextwin( 1 );
    
    refresh();
    
    mvwprintw(titleWindow, 0, (this->maxColumns - strlen(msg)) / 2, "%s", msg);
    
    wrefresh(titleWindow);
    
    this->commandPrompt.assign("Command / Search:");
    this->commandCursorDefault = this->commandPrompt.length();
    this->commandCursorPosition = this->commandCursorDefault;
    mvwprintw(this->commandWindow, 0, 0, this->commandPrompt.c_str());
    wrefresh(this->commandWindow);
    
    size_t row;
    this->browserHeaderWindow = this->nextwin( 2 , &row );
    this->browserRows = this->maxRows - row;
    this->browserWindow = nextwin( this->browserRows );
    
    this->browserPanel = new_panel( this->browserWindow );
}
WINDOW * FlacDemonInterface::nextwin( size_t rowSize , size_t * row ){
    static size_t currentWindowRow = 0;
    WINDOW * window = newwin( rowSize, this->maxColumns, currentWindowRow, 0 );
    wbkgd( window , COLOR_PAIR( 1 ) );

    currentWindowRow += rowSize;
    if( row != nullptr )
        *row = currentWindowRow;
    return window;
}
void FlacDemonInterface::connect(){
    int sockfd, port;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    port = 8995;
    
    if(this->socketFileDescriptor > 2){
        close(this->socketFileDescriptor);
    }
    this->socketFileDescriptor = -1;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERROR opening socket" << std::endl;
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);
    if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        if(!this->retryConnectThread){
            std::cout << "Error: could not connect: " << strerror(errno) << ". Retrying..." << std::endl;
            this->retryConnectThread = new std::thread(&FlacDemonInterface::retryConnect, this);
        }
        return;
    }
    this->socketFileDescriptor = sockfd;
    std::cout << "Connected to server" << std::endl;
    this->onConnect();
}
void FlacDemonInterface::retryConnect(){
    while(this->socketFileDescriptor < 0){
        sleep(3);
        this->connect();
    }
}
void FlacDemonInterface::onConnect(){
    if(!this->fetchedLibrary){
        this->sendCommand("get all");
        this->fetchedLibrary = true;
    }
    if(this->readThread){
        this->killResponseThread = true;
        this->readThread->join();
        this->killResponseThread = false;
    }
    this->readThread = new std::thread(&FlacDemonInterface::readResponse, this);

}
void FlacDemonInterface::callCommand( const char * signal, void * arg ){
    //check some stuff

    fd_stringvector * args = ( fd_stringvector * ) arg;
    
    cout << "call command " << args->front() << endl;
    
    if( this->commandParser.commandType == no_command && this->typeSearch ){
        this->typeSearch = false;
    }
    else if(this->commandParser.commandType == local_command ){
        
    } else if( this->commandParser.commandType == remote_command ){
        this->sendCommand(args->front().c_str());
    }
    
}
void FlacDemonInterface::sendCommand(const char * icommand){
    if(this->socketFileDescriptor < STDERR_FILENO){
        return;
    }
    ssize_t n = send(this->socketFileDescriptor,icommand,strlen(icommand), 0);
    if (n < 0){
        std::cout << "ERROR writing to socket" << std::endl;
        //end socket
        close(this->socketFileDescriptor);
        this->socketFileDescriptor = -1;
    }
}
void FlacDemonInterface::readResponse(){
    ssize_t n;
    char buffer[4096];
    bzero(buffer,4096);
    std::string response="";
    std::string response2="";
    size_t pos;
    cout << "reading ..." << endl;
    do{
        if ((n = recv(this->socketFileDescriptor, buffer, sizeof(buffer), 0)) < 0){
            cout << "ERROR reading from socket" << std::endl;
            continue;
        }
        response.append(buffer, n);

        while((pos = response.find("--data-end--")) != std::string::npos){
            cout << "response found" << endl;
            cout << response << endl;
            cout << response.length() << ":" << (pos + 12) << endl;
            if(response.length() > (pos + 12)){
                response2 = response.substr((pos + 12));
            } else
                response2 = "";
    
            
            response.erase(pos);
            cout << "response 2:\n" << response2 << endl;
                cout << "response 1:\n" << response << endl;
            this->parseResponse(response);
            response = response2;
        }
    }while(n>0 && !this->killResponseThread);
    this->connect();
}
void FlacDemonInterface::parseResponse(std::string response){
//    cout << response << endl;
    std::string tcommand = this->parseCommandFromResponse(&response);
    std::cout << "Got response for command \"" << tcommand << "\"" << std::endl;
    if(commandFlags->count(tcommand)){
        switch (commandFlags->at(tcommand)) {
            case fd_interface_libraryupdate:
                this->parseLibraryUpdate(&response);
                break;
            case fd_interface_playing:
                this->setNowPlaying(this->removeCommandFromResponse(&response));
                break;
            case fd_interface_playbackprogress:
                this->progress = atof(this->removeCommandFromResponse(&response).c_str());
                this->event(fd_interface_printprogress);
                break;
            default:
                break;
        }
        this->setCommandCursor();
    }
}
std::string FlacDemonInterface::parseCommandFromResponse(std::string *response){
    std::istringstream inStream;
    std::string line;
    inStream.str(*response);
    std::getline(inStream, line);
    std::regex reg("\"command\":\"([^\"]+)\"");
    std::smatch regmatch;
    if(std::regex_search(line, regmatch, reg) && regmatch.size()){
        std::ssub_match submatch = regmatch[1];
        return submatch.str();
    }
    return (std::string(""));
}
std::string FlacDemonInterface::removeCommandFromResponse(std::string * response){
    int pos = response->find("\n");
    std::string responseWithoutCommand;
    if(pos != std::string::npos && (pos+1) < response->length()){
        responseWithoutCommand = response->substr( pos + 1 );
    }
    return responseWithoutCommand;
}
void FlacDemonInterface::run(){
    new std::thread(&FlacDemonInterface::userInputLoop, this);

    do{
        this->printFlags();
    }while(1);
}
void FlacDemonInterface::printFlags(){
    std::unique_lock < std::mutex > lock ( this->eventMutex );
    this->eventCV.wait( lock );
    if(ihas_flag(this->flags, fd_interface_printlibrary)){
        cout << "printing library" << endl;
        this->printLibrary(this->browserOffset);
    }
    if(ihas_flag(this->flags, fd_interface_printcommand)){
        cout << "printing command" << endl;
        this->printCommand();
    }
    if(ihas_flag(this->flags, fd_interface_printplaying)){
        cout << "printing now playing" << endl;
        this->printNowPlaying();
    }
    if(ihas_flag(this->flags, fd_interface_printprogress)){
        cout << "printing progress" << endl;
        this->printProgress();
    }
    this->flags = 0;
    this->setCommandCursor();
}
void FlacDemonInterface::event(unsigned long rlflags){
    std::lock_guard<std::mutex> lock( this->eventMutex );
    cout << "event: " << rlflags << endl;
    set_flag rlflags;
    this->eventCV.notify_one();
}
void FlacDemonInterface::setRunLoopFlags(unsigned long rlflags){
    set_flag rlflags;
}
void FlacDemonInterface::userInputLoop(){
    cout << "user input thread running " << endl;
    char ch;
    int c;
    while(true){
        c = getch();
        cout << "got char, numeric: " << c << " keyname: " << keyname(c) << endl;
        switch (c) {
            case KEY_DOWN: //down
                this->changeOffset(1);
                break;
            case KEY_UP: //up
                this->changeOffset(-1);
                break;
            case KEY_PPAGE: //page up
                this->changeOffset( 0 - this->browserRows);
                break;
            case KEY_NPAGE: //page down
                this->changeOffset(( this->browserRows ));
                break;
            case KEY_HOME:
                this->changeOffset( 0 , true );
                break;
            case KEY_END:
                this->changeOffset((int)this->library.count(), true);
                break;
            case 27: //esc
                this->escapeHandler();
                break;
            case 5: //^E
                this->escapeHandler();
                break;
            case 10: //Enter
                this->commandParser.parseCommand( &this->userCommand , true );
                this->userCommand.clear();
                break;
            case 127: //backspace
                if( this->userCommand.length() )
                    this->userCommand.pop_back();
                break;
            
            default:
                ch = (char)c;
                this->userCommand.append(&ch, 1);
                break;
        }
        this->commandCursorPosition = this->commandCursorDefault + this->userCommand.length();
        this->event(fd_interface_printcommand);
        this->commandParser.parseCommand( &this->userCommand , false );
        this->trySearch();

    }
}
void FlacDemonInterface::trySearch(){
    fd_stringvector args = this->commandParser.currentArgs;
    int index = 0;
    if( this->commandParser.commandType == no_command){
        index = 1;
    } else if ( this->commandParser.commandType == search_command){
        index = 2;
    }
    if( index && args.size() > index && args.at(index).length() > 2 ) {
        index--;
        args.erase(args.begin(), args.begin() + index);
        this->typeSearch = true;
        this->library.search(args);
        this->isSearch = true;
        new std::thread(&FlacDemonInterface::waitForSearch, this);
    } else if ( this->typeSearch ){
        this->clearSearch();
    }
}
void FlacDemonInterface::clearSearch(){
    this->isSearch = false;
    this->typeSearch = false;
    this->event(fd_interface_printlibrary);
}
void FlacDemonInterface::escapeHandler(){
    if(this->isSearch && !this->typeSearch){
        this->clearSearch();
        return;
    }
    this->userCommand.clear();
}
void FlacDemonInterface::printLibrary(int offset = 0){
//    cout << "printing library" << endl;
    wclear(this->browserWindow);
    wclear(this->browserHeaderWindow);
    this->printLibraryHeaders();
    this->currentBrowserRow = 0;
    std::string key;
    if( ! this->printAlbums ){
        fd_tracklistingvector * tracks = this->library.allTracks();
        for(fd_tracklistingvector::iterator it = tracks->begin() + offset; it != tracks->end(); it++){
            if(this->isSearch && !(*it)->matchesSearch)
                continue;
            std::vector < std::string > values;
            for(std::vector< std::string >::iterator it2 = libraryTitlesTracks->begin(); it2 != libraryTitlesTracks->end(); it2++){
                key = (*it2);
                fd_standardiseKey(&key);
                values.push_back( ( *it )->valueForKey( &key ) );
            }
            this->printLibraryLine( this->browserWindow , &values );
            if( this->currentBrowserRow > this->browserRows )
                break;
        }
    } else {
        fd_albumvector& albums = this->library.allAlbums();
        for(fd_albumvector::iterator it = albums.begin() + offset; it != albums.end(); it++){
            if(this->isSearch && !(*it)->matchesSearch())
                continue;
            std::vector < std::string > values;
            for(std::vector< std::string >::iterator it2 = libraryTitlesAlbums->begin(); it2 != libraryTitlesAlbums->end(); it2++){
                key = (*it2);
                fd_standardiseKey(&key);
                values.push_back( ( *it )->valueForKey( &key ) );
            }
            this->printLibraryLine( this->browserWindow , &values );
            if( this->currentBrowserRow > this->browserRows )
                break;
        }
    }
    wrefresh(this->browserWindow);
    wrefresh(this->browserHeaderWindow);
}
void FlacDemonInterface::printLibraryHeaders(){
    char title[] = "Library";
    mvwprintw(this->browserWindow, 0, (this->maxColumns - strlen(title)) / 2, title);
    this->currentBrowserRow = 1;
    this->printLibraryLine( this->browserHeaderWindow , libraryTitlesAlbums );
}
void FlacDemonInterface::printLibraryLine( WINDOW * window , std::vector<std::string> *values ){
    int width = (this->maxColumns / values->size());
    int position = 0;
//    cout << "printing line " << this->browserRows << endl;
    
    int pair = 2 + ( this->currentBrowserRow % 2 );
    wattron( window , COLOR_PAIR( pair ) );
    
    for(std::vector< std::string >::iterator it = values->begin(); it != values->end(); it++){
        const char * val = this->formatValue(*it, width);
        mvwprintw(window, this->currentBrowserRow, position, val);
        position+=width;
        mvwprintw(window, this->currentBrowserRow, position, "|");
        position++;
    }
    this->currentBrowserRow++;
    wattroff( window , COLOR_PAIR( pair ) );
}
const char * FlacDemonInterface::formatValue(std::string value, int max){
    if(value.length() > max){
        value = value.substr(0, max-2);
        value.append("..");
    }
    return value.c_str();
}
void FlacDemonInterface::parseLibraryUpdate(std::string *response){
    fd_keymap_vector * results = fd_jsontokeymap_vector(response);
    this->library.libraryUpdate(results);
    this->library.sort("artist");
    this->event(fd_interface_printlibrary);
}
void FlacDemonInterface::changeOffset(int diff, bool absolute){
    if(absolute){
        this->browserOffset = diff;
        diff = 0;
    }
    if( diff != 0 || absolute ){
        if( ( (long)this->browserOffset + diff ) < 0 || this->library.count() < this->browserRows )
            this->browserOffset = 0;
        else if ( ( this->browserOffset + diff + this->browserRows ) > this->library.count() )
            this->browserOffset = ( this->library.count() - this->browserRows );
        else
            this->browserOffset += diff;
        this->event(fd_interface_printlibrary);
    }
}
void FlacDemonInterface::setNowPlaying(std::string ID){
//    int iid;
//    fd_stringtoint(&ID, &iid);
    cout << "setting now playing to " << ID << endl;
    this->nowPlaying = this->library.trackListingForID(ID);
    this->progress = 0;
    this->event(fd_interface_printplaying);
}
void FlacDemonInterface::printNowPlaying(){
    wclear(this->playbackWindow);
    if(this->nowPlaying){
        std::string title = this->nowPlaying->valueForKey("title");
        cout << "setting title to " << title << endl;
        mvwprintw(this->playbackWindow, 0, (this->maxColumns - title.length()) / 2, title.c_str());
    } else {
        char msg2[] = "Nothing Playing";
        mvwprintw(this->playbackWindow, 0, (this->maxColumns - strlen(msg2)) / 2, msg2);
        wclrtobot(this->playbackWindow);
    }
    wrefresh(this->playbackWindow);
}
void FlacDemonInterface::printCommand(){
//    cout << "printing command " << this->command << endl;
    if(this->isSearch && !this->typeSearch)
        this->searchPrompt = "(^E to cancel current search)";
    else
        this->searchPrompt = "";
    mvwprintw(this->commandWindow, 0, 0, "%s%s", this->commandPrompt.c_str(), this->userCommand.c_str());
    wclrtoeol(this->commandWindow);
    mvwprintw(this->commandWindow, 0, this->maxColumns - this->searchPrompt.length(), "%s", this->searchPrompt.c_str());
    wrefresh(this->commandWindow);

//    this->setCommandCursor();
}
void FlacDemonInterface::setCommandCursor(){
    wmove(this->commandWindow, 0, (int)this->commandCursorPosition);
    wrefresh(this->commandWindow);
}
void FlacDemonInterface::printProgress(){
    if(!this->nowPlaying)
        return;
    std::string tracktime = this->nowPlaying->valueForKey("tracktime");
    cout << "printing progress " << this->progress << " tracktime " << tracktime << endl;
    int itime;
    fd_stringtoint( &tracktime , &itime );
    itime = itime / 1000;
    int progressTime = (int)(itime * this->progress);
    std::string formatTime = fd_secondstoformattime(itime);
    std::string formatProgressTime = fd_secondstoformattime(progressTime);
    std::string totalTime = formatProgressTime;
    totalTime.append("/");
    totalTime.append(formatTime);
    mvwprintw(this->playbackWindow, 1, (this->maxColumns - totalTime.length()) / 2, totalTime.c_str());
    wrefresh(this->playbackWindow);
}
void FlacDemonInterface::waitForSearch(){
    cout << "waiting for library search" << endl;
    while (this->library.searching) {
        usleep(100);
    }
    cout << "search wait over" << endl;

    this->event(fd_interface_printlibrary);
}
