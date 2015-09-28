//
//  Player.h
//  FlacDemon
//
//  Created by merryclarke on 28/09/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__Player__
#define __FlacDemon__Player__

#include <stdio.h>
#include "FlacDemonNameSpace.h"
#include "Database.h"

class FlacDemon::Player {
private:
    FlacDemon::Database * database;
protected:
    
public:
    void setDatabase(FlacDemon::Database * database);
    void playTrackWithID(long ID);
};

#endif /* defined(__FlacDemon__Player__) */
