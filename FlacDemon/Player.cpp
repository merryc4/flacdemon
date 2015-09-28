//
//  Player.cpp
//  FlacDemon
//
//  Created by merryclarke on 28/09/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "Player.h"

FlacDemon::Player::Player(){
    ao_initialize();
    this->defaultDriverID = ao_default_driver_id();
    this->device = ao_open_live(this->defaultDriverID, &this->sampleFormat, NULL);
    cout << "default driver: " << defaultDriverID << endl;
}
FlacDemon::Player::~Player(){
    
}
void FlacDemon::Player::setDatabase(FlacDemon::Database * idatabase){
    this->database = idatabase;
}
void FlacDemon::Player::playTrackWithID(long ID){
    FlacDemon::Track * track = this->database->trackForID(ID);
    if(track){
        this->playTrack(track);
    }
}
void FlacDemon::Player::playTrack(FlacDemon::Track * track){
    if(!track->openFilePath() || !track->file->isMediaFile()){
        return;
    }
    
    //move variables to members of the class where possible

//    AVCodecContext * codecContext = track->file->formatContext->streams[0]->codec;
//    
//    if(!codecContext){
//        cout << "error getting codec context";
//        return;
//    }

    
    /* find the mpeg audio decoder */
    AVCodec * codec = avcodec_find_decoder(track->file->codecID);
    if (!codec) {
        cout << "Codec not found" << endl;
        return;
    }
    AVCodecContext * codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        cout << "Could not allocate audio codec context" << endl;
        return;
    }
    /* open it */
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        cout << "could not open codec" << endl;

    }
    AVFrame * frame = av_frame_alloc();
    AVPacket packet;
    av_init_packet(&packet);
    packet.data = NULL;
    packet.size = 0;
    
    int error, endoffile=0, gotFrame=0, samplelength=0;
    while(endoffile == 0 && error >=0 ){
        if((error = av_read_frame(track->file->formatContext, &packet)) < 0){
            if(error == AVERROR_EOF){
                endoffile = 1;
            } else {
                cout << "error reading frame from file " << track->valueForKey("filepath") << endl;
                return;
            }
        }
        
        
        if((error = avcodec_decode_audio4(codecContext, frame, &gotFrame, &packet)) < 0){
            char errbuf[255];
            av_strerror(error, errbuf, sizeof(errbuf));
            cout << "could not read decode audio frame: " << *track->valueForKey("filepath") << ". " << errbuf << endl;
            av_free_packet(&packet);
        }
        samplelength = error;
        if(ao_play(this->device, (char*)frame->extended_data[0], frame->linesize[0]) == 0){
            cout << "ao play error" << endl;
            //close device
            error = -1;
        }
    }
    
    av_free_packet(&packet);
    
    //av_frame_free(&frame);
}