//
//  FlacDemonInterface.cpp
//  FlacDemon
//
//  Created by merryclarke on 02/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "FlacDemonInterface.h"

std::vector< std::string > * libraryTitles = new std::vector< std::string >{"id", "Track", "Title", "Album", "Artist", "AlbumArtist", "Playcount"};
std::map< std::string , unsigned long > * commandFlags = new std::map < std::string , unsigned long >{
    { "get all" , fd_interface_libraryupdate },
    { "playing" , fd_interface_playing }
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

    cbreak();
    noecho();
    keypad(stdscr, true);
    refresh(); //clears screen and sets scroll to correct position
    
    char msg[] = "FlacDemon NCURSES Interface";
    char msg2[] = "Nothing Playing";
    
    getmaxyx(stdscr, this->maxRows, this->maxColumns);
    
    WINDOW * titleWindow = newwin(1, this->maxColumns, 0, 0);
    this->playbackWindow = newwin(3, this->maxColumns, 1, 0);
    this->commandWindow = newwin(1, this->maxColumns, 4, 0);
    
    mvwprintw(titleWindow, 0, (this->maxColumns - strlen(msg)) / 2, "%s", msg);
    wrefresh(titleWindow);
    
    mvwprintw(this->playbackWindow, 0, (this->maxColumns - strlen(msg2)) / 2, msg2);
    wrefresh(this->playbackWindow);
    
    this->commandPrompt.assign("Command / Search:");
    this->commandCursorDefault = this->commandPrompt.length();
    this->commandCursorPosition = this->commandCursorDefault;
    mvwprintw(this->commandWindow, 0, 0, this->commandPrompt.c_str());
    wrefresh(this->commandWindow);
    
    int browserRow = 5;
    this->browser = newwin(this->maxRows - browserRow, this->maxColumns, browserRow, 0);
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
void FlacDemonInterface::parseCommand(std::string *iCommand){
    //check some stuff
    this->sendCommand(iCommand->c_str());
}
void FlacDemonInterface::sendCommand(const char * command){
    if(this->socketFileDescriptor < STDERR_FILENO){
        return;
    }
    ssize_t n = send(this->socketFileDescriptor,command,strlen(command), 0);
    if (n < 0){
        std::cout << "ERROR writing to socket" << std::endl;
        //end socket
        close(this->socketFileDescriptor);
        this->socketFileDescriptor = -1;
    }
}
void FlacDemonInterface::readResponse(){
    ssize_t n;
    char buffer[256];
    std::string response="";
    std::string response2="";
    size_t pos;
    do{
        bzero(buffer,256);
        cout << "reading ..." << endl;
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
    std::string command = this->parseCommandFromResponse(&response);
    std::cout << "Got response for command \"" << command << "\"" << std::endl;
    if(commandFlags->count(command)){
        switch (commandFlags->at(command)) {
            case fd_interface_libraryupdate:
                this->parseLibraryUpdate(&response);
                break;
            case fd_interface_playing:
                this->setNowPlaying(this->removeCommandFromResponse(&response));
                break;
            default:
                break;
        }
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
    std::string* input = new std::string();
    do{
        this->eventMutex.lock();
//        std::cout << "Enter Command: ";
//        getline(std::cin >> std::ws, *input);
//        this->sendCommand(input->c_str());
        if(has_flag fd_interface_printlibrary){
            this->printLibrary(0);
        }
        if(has_flag fd_interface_printcommand){
            this->printCommand();
        }
        this->setCommandCursor();
    }while(1);
}
void FlacDemonInterface::event(unsigned long rlflags){
    set_flag rlflags;
    this->eventMutex.unlock();
}
void FlacDemonInterface::setRunLoopFlags(unsigned long rlflags){
    set_flag rlflags;
}
void FlacDemonInterface::userInputLoop(){
    cout << "user input thread running " << endl;
    while(true){
        char c = getch();
//        cout << "got char " << c << ". numeric: " << (int)c << endl;
        switch (c) {
            case 10: //Enter
                this->parseCommand(&this->command);
                this->command.clear();
                break;
            case 127: //backspace
                this->command.pop_back();
                break;
            
            default:
                this->command.append(&c, 1);
                break;
        }
        this->event(fd_interface_printcommand);
    }
}
void FlacDemonInterface::printLibrary(int offset = 0){
//    cout << "printing library" << endl;
    this->browserRows=1; //title always first row
    this->printLibraryHeaders();
    std::string key;
    for(std::map < std::string , FlacDemon::TrackListing * >::iterator it = this->tracks.begin(); it != this->tracks.end(); it++){
        std::vector < std::string > values;
        for(std::vector< std::string >::iterator it2 = libraryTitles->begin(); it2 != libraryTitles->end(); it2++){
            key = (*it2);
            fd_standardiseKey(&key);
            values.push_back(*it->second->valueForKey(&key));
        }
        this->printLibraryLine(&values);
    }
    
    wrefresh(this->browser);
}
void FlacDemonInterface::printLibraryHeaders(){
    char title[] = "Library";
    mvwprintw(this->browser, 0, (this->maxColumns - strlen(title)) / 2, title);
    this->printLibraryLine(libraryTitles);
}
void FlacDemonInterface::printLibraryLine(std::vector<std::string> *values){
    int width = (this->maxColumns / values->size());
    int position = 0;
    this->browserRows++;
    
//    cout << "printing line " << this->browserRows << endl;
    
    for(std::vector< std::string >::iterator it = values->begin(); it != values->end(); it++){
        const char * val = this->formatValue(*it, width);
        mvwprintw(this->browser, this->browserRows, position, val);
        position+=width;
        mvwprintw(this->browser, this->browserRows, position, "|");
        position++;
    }
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
    this->libraryUpdate(results);
}
void FlacDemonInterface::libraryUpdate(fd_keymap_vector * values){
    for(fd_keymap_vector::iterator it = values->begin(); it != values->end(); it++){
        this->updateTrackListing(*it);
    }
    this->event(fd_interface_printlibrary);
}
void FlacDemonInterface::setNowPlaying(std::string ID){
    int iid;
//    fd_stringtoint(&ID, &iid);
    cout << "setting now playing to " << ID << endl;
    FlacDemon::TrackListing * track = this->trackListingForID(ID);
    if(track){
        wclear(this->playbackWindow);
        std::string * title = track->valueForKey("title");
        cout << "setting title to " << *title << endl;
        mvwprintw(this->playbackWindow, 0, (this->maxColumns - title->length()) / 2, title->c_str());
        
        wrefresh(this->playbackWindow);
    }
}
void FlacDemonInterface::updateTrackListing(fd_keymap *ikeymap){
    std::string idKey("id");
    if(!ikeymap->count(idKey)){
        std::cout << "Error: Malformed keymap, has no ID" << std::endl;
        return;
    }
    std::string * id = ikeymap->at(idKey);
    if(this->tracks.count(*id)){
        //update
    } else {
        FlacDemon::TrackListing * trackListing = new FlacDemon::TrackListing(ikeymap);
        this->tracks.insert(std::pair<std::string, FlacDemon::TrackListing * >(*id, trackListing));
    }
}
void FlacDemonInterface::printCommand(){
//    cout << "printing command " << this->command << endl;
    mvwprintw(this->commandWindow, 0, 0, "%s%s", this->commandPrompt.c_str(), this->command.c_str());
    wclrtoeol(this->commandWindow);
    wrefresh(this->commandWindow);

//    this->setCommandCursor();
}
void FlacDemonInterface::setCommandCursor(){
    wmove(this->commandWindow, (int)this->commandCursorPosition, 0);
    wrefresh(this->commandWindow);
}
FlacDemon::TrackListing * FlacDemonInterface::trackListingForID(std::string ID){
    if(this->tracks.count(ID) == 0){
        return nullptr;
    }
    return this->tracks.at(ID);
}