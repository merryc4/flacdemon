//
//  Player.cpp
//  FlacDemon
//
//  Created by merryclarke on 28/09/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "Player.h"

void FlacDemon::Player::setDatabase(FlacDemon::Database * idatabase){
    this->database = idatabase;
}
void FlacDemon::Player::playTrackWithID(long ID){
    this->database->trackForID(ID);
}