//
//  Verifier.h
//  FlacDemon
//
//  Created by merryclarke on 02/03/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef __FlacDemon__Verifier__
#define __FlacDemon__Verifier__

#include "FlacDemonNameSpace.h"
#include "includes.h"
#include "typedefs.h"

class FlacDemon::Verifier {
private:
protected:
public:
    Verifier();
    ~Verifier();
    
    void showAlbum( FlacDemon::Album * album );
};

#endif /* defined(__FlacDemon__Verifier__) */
