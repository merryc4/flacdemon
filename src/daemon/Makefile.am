AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = --pedantic -Wall -std=c++11

bin_PROGRAMS = flacdemon
flacdemon_SOURCES = ../common/Album.cpp ../common/CommandParser.cpp ../common/FlacDemonUtils.cpp ../common/LibraryTitles.cpp ../common/Group.cpp ../common/LibraryListing.cpp ../common/Session.cpp ../common/SessionManager.cpp ../common/SignalHandler.cpp ../common/TrackListing.cpp Database.cpp File.cpp FileImporter.cpp FlacDemon.cpp Player.cpp Scraper.cpp TCPHandler.cpp Track.cpp daemon.cpp

#flacdemon_CPPFLAGS=$flacdemon_CPPFLAGS -I../headers -I../common
AM_CPPFLAGS=-I../headers -I../common
 
