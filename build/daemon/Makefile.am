AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = --pedantic -Wall -std=c++11

bin_PROGRAMS = flacdemon
flacdemon_SOURCES = ../../src/common/Album.cpp ../../src/common/CommandParser.cpp ../../src/common/CommandManager.cpp ../../src/common/FlacDemonUtils.cpp ../../src/common/Group.cpp ../../src/common/LibraryListing.cpp ../../src/common/Session.cpp ../../src/common/SessionManager.cpp ../../src/common/SignalHandler.cpp ../../src/common/TrackListing.cpp ../../src/daemon/Database.cpp ../../src/daemon/File.cpp ../../src/daemon/FileImporter.cpp ../../src/daemon/FlacDemon.cpp ../../src/daemon/Player.cpp ../../src/daemon/Scraper.cpp ../../src/daemon/TCPHandler.cpp ../../src/daemon/Track.cpp ../../src/daemon/daemon.cpp

AM_CPPFLAGS=-I../../src/headers -I../../src/common -I../../src/daemon
#AM_LDFLAGS=-L../../dependencies/libmba-0.9.1/ -lmba
flacdemon_LDFLAGS += -L../../dependencies/libmba-0.9.1 -lmba
