//
//  FileImporter.cpp
//  FlacDemon
//
//  Created by merryclarke on 23/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "FileImporter.h"

void FlacDemon::FileImporter::importFilesFromPath(string *path){
    if(this->isDirectory(path)){
        this->parseDirectory(path);
    } else {
        this->addFile(path);
    }
    
}
directory* FlacDemon::FileImporter::parseDirectory(string* path){
    cout << "Checking directory " << *path << endl;
    struct dirent *ent;
    DIR* dir;
    string subpath;
    AVCodecID codecID;
    if(path->back() != '/'){
        path->append("/");
    }
    directory* direc = new directory {
        path,
        AV_CODEC_ID_NONE,
        true,
        false
    };

    if ((dir = opendir (path->c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if(ent->d_name[0] == '.'){
                //hidden file
                cout << "skipping hidden file " << ent->d_name << endl;
                continue;
            }
            subpath = *path + ent->d_name;
            cout << subpath << endl;

            if(isDirectory(&subpath)){
                cout << "found directory within directory " << *path << endl;
                //parse
                directory* subDirectory = this->parseDirectory(&subpath);
                if(subDirectory->hasMediaFiles){
                    direc->isAlbumDirectory = false;
                }
                continue;
            } else {
                int error = this->readMediaFile(&subpath, &codecID);

                if(error == -1){ //not a media file
                    continue;
                } else if(error != 0){
                    //more error handling
                    continue;
                }
                if(direc->hasMediaFiles){
                    if(codecID != direc->codecID){
                        direc->codecID = AV_CODEC_ID_NONE;
                        direc->isAlbumDirectory = false;
                    }
                } else {
                    direc->codecID = codecID;
                    direc->hasMediaFiles = true;
                }
            }
        }
        if (direc->isAlbumDirectory) {
            cout << "found album directory " << *path << endl;
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
    }
    return direc;
}
int FlacDemon::FileImporter::isDirectory(string *path){
    return this->isDirectory(path->c_str());
}
int FlacDemon::FileImporter::isDirectory(const char *path){
    struct stat statBuffer;
    int rCode=0;
    if(stat(path, &statBuffer) == -1){
        cout << "error stat-ing file " << path << " errno: " << errno << endl;
        if(errno == ENOENT){
            cout << "File does not exist" << endl;
        }
    } else if(S_ISDIR(statBuffer.st_mode)){
        rCode=1;
    }
    return rCode;
}
void FlacDemon::FileImporter::addFile(string *path){
    cout << "Adding file " << *path << endl;
    AVCodecID codecID;
    this->readMediaFile(path, &codecID);
}
int FlacDemon::FileImporter::readMediaFile(string * path, AVCodecID* codecID){
    return this->readMediaFile(path->c_str(), codecID);
}
int FlacDemon::FileImporter::readMediaFile(const char * path, AVCodecID* codecID){
    AVFormatContext* inputContext = avformat_alloc_context();
//    AVCodecContext** codecContext;
    
    int error;
    AVCodec* inputCodec;
    
     /** Open the input file to read from it. */
     if ((error = avformat_open_input(&inputContext, path, NULL,
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
        return -1;
    }
    
    *codecID = inputCodec->id;
    
    cout << "Found decoder " << &inputCodec->name << endl;
    
    avformat_free_context(inputContext);
    return inputCodec->id;
}







