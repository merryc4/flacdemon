//
//  typedefs.h
//  FlacDemon
//
//  Created by merryclarke on 10/12/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef FlacDemon_typedefs_h
#define FlacDemon_typedefs_h

#define set_flag this->flags = this->flags |
#define unset_flag this->flags = this->flags & ~
#define has_flag this->flags &

typedef std::pair<std::string, std::string *> fd_keypair;
typedef std::map<std::string, std::string *> fd_keymap;
typedef std::vector< fd_keymap * > fd_keymap_vector;

#endif
