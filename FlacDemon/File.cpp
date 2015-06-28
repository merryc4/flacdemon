//
//  File.cpp
//  FlacDemon
//
//  Created by merryclarke on 28/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "File.h"
FlacDemon::File::File(string* path){
    this->codecID = AV_CODEC_ID_NONE;
    this->flags = 0;
    this->metadata = NULL;
    this->files = NULL;
    this->path = new string;
    
    if(path)
        this->setPath(path);
}
string* FlacDemon::File::getPath(){
    return this->path;
}
void FlacDemon::File::setPath(string* iPath){
    this->path->assign(*iPath);
    if(this->checkDirectory()){
        this->setToDirectory(true);
        this->parse();
    } else {
        this->readMediaInfo();
    }
}

void FlacDemon::File::parse(){
    if(this)
    cout << "Checking directory " << this->path << endl;
    struct dirent *ent;
    DIR* dir;
    string subpath;
    if(path->back() != '/'){
        path->append("/");
    }
    
    if ((dir = opendir (this->path->c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if(ent->d_name[0] == '.'){
                //hidden file
                cout << "skipping hidden file " << ent->d_name << endl;
                continue;
            }
            subpath = *path + ent->d_name;
            cout << subpath << endl;
            
            FlacDemon::File* tFile = new FlacDemon::File(&subpath);
            if(tFile->error) //handle separate error codes
                continue;
            
            this->addFile(tFile);

        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
    }
}
void FlacDemon::File::addFile(FlacDemon::File * file){
    this->files->push_back(file);
}
bool FlacDemon::File::checkExists(struct stat * buffer){
    bool makeBuffer = false;
    if(!buffer){
        makeBuffer = true;
        buffer = new struct stat;
    }
    this->exists = false;
    if(stat(path->c_str(), buffer) == -1){
        cout << "error stat-ing file " << path << " errno: " << errno << endl;
        if(errno == ENOENT){
            cout << "File does not exist" << endl;
        }
        return false;
    }
    this->exists = true;
    if(makeBuffer)
        delete buffer;
    return true;
}
bool FlacDemon::File::checkDirectory(){
    struct stat buffer;
    bool isDir = false;
    if(this->checkExists(&buffer) && S_ISDIR(buffer.st_mode)){
        isDir = true;
    }
    this->setToDirectory(isDir);
    return isDir;
}
void FlacDemon::File::setToDirectory(bool setToDir){
    if (setToDir) {
        this->flags = this->flags | FLACDEMON_FILE_IS_DIRECTORY;
        this->files = new vector<FlacDemon::File*>;
    } else {
        this->flags = this->flags & ~ FLACDEMON_FILE_IS_DIRECTORY;
        if(this->files){
            delete this->files;
            this->files = NULL;
        }
    }
}
bool FlacDemon::File::isDirectory(){
    return this->flags & FLACDEMON_FILE_IS_DIRECTORY;
}
bool FlacDemon::File::isMediaFile(){
    return this->flags & FLACDEMON_FILE_IS_MEDIA;
}
bool FlacDemon::File::isAlbumDirectory(){
    return this->flags & FLACDEMON_DIRECTORY_IS_ALBUM;
}
int FlacDemon::File::readMediaInfo(){
    AVFormatContext* inputContext = avformat_alloc_context();
    //    AVCodecContext** codecContext;
    
    int error;
    AVCodec* inputCodec;
    
    /** Open the input file to read from it. */
    if ((error = avformat_open_input(&inputContext, this->path->c_str(), NULL,
                                     NULL)) < 0) {
        cout << "could not open input file" << endl;
        //         fprintf(stderr, "Could not open input file '%s' (error '%s')\n",
        //                 path, get_error_text(error));
        inputContext = NULL;
        return error;
    }
    
    /** Get information on the input file (number of streams etc.). */
    if ((error = avformat_find_stream_info(inputContext, NULL)) < 0) {
        cout << "Could not open find stream info" << endl;
        //         fprintf(stderr, "Could not open find stream info (error '%s')\n",
        //                 get_error_text(error));
        avformat_close_input(&inputContext);
        return error;
    }
    
    /** Make sure that there is only one stream in the input file. */
    if ((inputContext)->nb_streams != 1) {
        fprintf(stderr, "Expected one audio input stream, but found %d\n",
                inputContext->nb_streams);
        avformat_close_input(&inputContext);
        return AVERROR_EXIT;
    }
    
    /** Find a decoder for the audio stream. */
    if (!(inputCodec = avcodec_find_decoder((inputContext)->streams[0]->codec->codec_id))) {
        fprintf(stderr, "Could not find input codec\n");
        avformat_close_input(&inputContext);
        return AVERROR_EXIT;
    }
    
    if(inputCodec->type != AVMEDIA_TYPE_AUDIO){
        cout << "skipping none audio file " << *path << endl;
        this->flags = this->flags & ~ FLACDEMON_FILE_IS_MEDIA;
    }
    
    this->codecID = inputCodec->id;
    av_dict_copy(&this->metadata, inputContext->metadata, 0);
    
    this->printMetaDataDict(this->metadata);
    
    cout << "Found decoder " << inputCodec->name << endl;
    
    avformat_free_context(inputContext); //this calls free on metadata dict so need to copy fields to new dict
    return inputCodec->id;
}
void FlacDemon::File::printMetaDataDict(AVDictionary *dict){
    AVDictionaryEntry *t = NULL;
    while ((t = av_dict_get(dict, "", t, AV_DICT_IGNORE_SUFFIX))){
        cout << t->key << " : " << t->value << endl;
    }
    cout <<endl;
}

