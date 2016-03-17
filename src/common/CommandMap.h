/***********************************************************************
 * CommandMap.h : Map command names to functions
 * part of FlacDemon
 ************************************************************************
 *  Copyright (c) 2016 Meriadoc Clarke.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

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
#define flacdemon_command_handler( name ) int name( fd_stringvector * args );

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
