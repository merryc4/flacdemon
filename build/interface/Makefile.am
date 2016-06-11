AUTOMAKE_OPTIONS = subdir-objects
AM_CXXFLAGS = --pedantic -Wall -std=c++11

bin_PROGRAMS = flacdemoninterface
flacdemoninterface_SOURCES = ../../src/common/Album.cpp ../../src/common/CommandParser.cpp ../../src/common/CommandManager.cpp ../../src/common/FlacDemonUtils.cpp ../../src/common/LibraryListing.cpp ../../src/common/Session.cpp ../../src/common/SessionManager.cpp ../../src/common/SignalHandler.cpp ../../src/common/TrackListing.cpp ../../src/interface/FlacDemonInterface.cpp ../../src/interface/Library.cpp ../../src/interface/TrackSorter.cpp ../../src/interface/Verifier.cpp ../../src/interface/interface.cpp

flacdemoninterface_CPPFLAGS=-I../../src/headers -I../../src/common -I../../src/daemon -I../../dependencies/libmba-0.9.1/src
flacdemoninterface_LDFLAGS += -L../../dependencies/libmba-0.9.1 -lmba 
