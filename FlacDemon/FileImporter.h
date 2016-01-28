//
//  FileImporter.h
//  FlacDemon
//
//  Created by merryclarke on 23/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__FileImporter__
#define __FlacDemon__FileImporter__

#include <iostream>
#include "FlacDemonAll.h"
#include "File.h"
#include "SignalHandler.h"


class FlacDemon::FileImporter {
protected:
    
public:
    FileImporter();
    ~FileImporter();
    void importFilesFromPath(string*);
    void sigtest(const char * signalName);
};


#endif /* defined(__FlacDemon__FileImporter__) */
