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
#include "FlacDemonNameSpace.h"
#include "File.h"

#define FLACDEMON_METADATA_MULTIPLE_VALUES "FlacDemonMetaDataMultipleValues"



class FlacDemon::FileImporter {
protected:
    
public:
    void importFilesFromPath(string*);
};


#endif /* defined(__FlacDemon__FileImporter__) */
