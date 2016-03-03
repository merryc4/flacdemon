//
//  CommandMap.h
//  FlacDemon
//
//  Created by merryclarke on 17/02/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef __FlacDemon__CommandMap__
#define __FlacDemon__CommandMap__

#include "includes.h"
#include "FlacDemonNameSpace.h"
#include "SignalHandler.h"

template < typename TType >
using fd_demon_command_function = std::function < int ( TType&, fd_stringvector * ) > ;

template < typename TType >
using fd_commandmap = std::map < std::string, std::function < int ( TType&, fd_stringvector * ) > >;

#define flacdemon_command( classType , commandName , funcName) { commandName , fd_demon_command_function < classType > { &classType::funcName } }

#define flacdemon_command_map( classType , ...) fd_commandmap < classType > { foreach( flacdemon_command, classType , __VA_ARGS__ ) }
#define flacdemon_commander( classType , commander , ...) FlacDemon::CommandMap < classType > ( commander , flacdemon_command_map( classType , __VA_ARGS__ ) )

template < class Target >
class FlacDemon::CommandMap {
private:
    fd_commandmap < Target > commandMap;
    Target * target;
protected:
public:
    CommandMap( Target * iTarget = nullptr ) {
        this->target = iTarget;
    };
    ~CommandMap(){
        //nothing
    };
    void setTargetMap( Target * iTarget , fd_commandmap < Target > map ) {
        this->target = iTarget;
        this->commandMap = map;
        auto f = boost::bind( &FlacDemon::CommandMap < Target >::callCommandHandler, this, _1, _2);
        signalHandler->signals("callCommand")->connect(f);
    };
    void callCommandHandler( const char * signal , void * arg ){
        fd_stringvector * args = ( fd_stringvector * )arg;
        std::string command = args->at(1);
        if( this->commandMap.count( command ) ){
            auto f = this->commandMap.at( command );
            f( ( *this->target ), args );
        }
    };
};

#endif /* defined(__FlacDemon__CommandMap__) */
