/***********************************************************************
 * FlacDemonUtils.h : Any none class functions. Mostly string manipulation
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
#ifndef __FlacDemon__FlacDemonUtils__
#define __FlacDemon__FlacDemonUtils__

#include "includes.h"
#include "typedefs.h"
#include "netincludes.h"
#include "globals.h"

#define FLACDEMON_MULTIPLE_TAG_MATCH_THRESHOLD 0.7

enum MatchType {
    MatchTypeNone = 0,
    MatchTypeFull,
    MatchTypePartial,
    MatchTypeAContainsB,
    MatchTypeBContainsA
};

extern std::vector< std::string > * fd_numbers;

void initGlobals();
int fd_stringtoint(const std::string & str, int * value);
MatchType fd_comparetags(const char * tag1, const char * tag2, float * matchProportion = nullptr);
MatchType fd_comparetags(std::string & tag1, std::string & tag2, float * matchProportion = nullptr);
uint fd_ismultipletag( std::string & tag , bool isArtis = false);
const void * characterAtIndex(const void * str, int index, void * context);
int compareCharacters(const void * c1, const void * c2, void * context);

void fd_tolowercase(std::string & str);
int fd_strreplace(std::string & str, std::string & search, std::string & replace , bool global = false );
int fd_strreplace(std::string & str, const char * search, const char * replace , bool global = false );
int fd_strnumbercompare(std::string & str1, std::string & str2);

std::string fd_sqlescape(std::string isql);

std::string fd_keymap_vectortojson(fd_keymap_vector* ikeymap_vector);
std::string fd_keymaptojson(fd_keymap * ikeymap);
fd_keymap_vector * fd_jsontokeymap_vector(std::string & json);
std::vector< std::string > fd_splitjsondescriptor(std::string & json);
json_t * fd_decodeJSON( std::string & jsonString );
void waitfor0(int * value);
void waitfor0(bool * value);

int isMainThread();

std::string fd_standardiseKey(std::string & key);
std::string fd_secondstoformattime(int seconds);

std::vector < std::string > fd_splitstring(std::string & str, const char * delim);
std::vector < std::string > fd_splitstring(std::string & str, std::string delim);
fd_keymap fd_parsekeyvaluepairs( std::string & str , const char * pairSeparator , const char * keyValueSeparator );
bool searchPredicate(char a, char b);

std::string fd_uuid();

#endif /* defined(__FlacDemon__FlacDemonUtils__) */
