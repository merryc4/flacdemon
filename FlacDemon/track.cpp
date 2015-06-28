//
//  Track.cpp
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "Track.h"


void FlacDemon::Track::setValuesFromAudioContext(AVCodecContext* codecContext){
    this->bitRate = codecContext->sample_rate;
    this->sampleRate = codecContext->sample_rate;
    this->channels = codecContext->channels;
    this->codecID = codecContext->codec_id;
    
    if(this->codecName != NULL){
        delete this->codecName;
    }
    this->codecName = new string(codecContext->codec->name);
}