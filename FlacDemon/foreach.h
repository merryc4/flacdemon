//
//  foreach.h
//  FlacDemon
//
//  Created by merryclarke on 03/03/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef FlacDemon_foreach_h
#define FlacDemon_foreach_h

#define foreach1( action , staticarg , arg ) action( staticarg , arg )
#define foreach2( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach1( action, staticarg , __VA_ARGS__ )
#define foreach3( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach2( action, staticarg , __VA_ARGS__ )
#define foreach4( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach3( action, staticarg , __VA_ARGS__ )
#define foreach5( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach4( action, staticarg , __VA_ARGS__ )
#define foreach6( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach5( action, staticarg , __VA_ARGS__ )
#define foreach7( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach6( action, staticarg , __VA_ARGS__ )
#define foreach8( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach7( action, staticarg , __VA_ARGS__ )
#define foreach9( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach8( action, staticarg , __VA_ARGS__ )
#define foreach10( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach9( action, staticarg , __VA_ARGS__ )
#define foreach11( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach10( action, staticarg , __VA_ARGS__ )
#define foreach12( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach11( action, staticarg , __VA_ARGS__ )
#define foreach13( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach12( action, staticarg , __VA_ARGS__ )
#define foreach14( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach13( action, staticarg , __VA_ARGS__ )
#define foreach15( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach14( action, staticarg , __VA_ARGS__ )
#define foreach16( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach15( action, staticarg , __VA_ARGS__ )
#define foreach17( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach16( action, staticarg , __VA_ARGS__ )
#define foreach18( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach17( action, staticarg , __VA_ARGS__ )
#define foreach19( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach18( action, staticarg , __VA_ARGS__ )
#define foreach20( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach19( action, staticarg , __VA_ARGS__ )
#define foreach21( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach20( action, staticarg , __VA_ARGS__ )
#define foreach22( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach21( action, staticarg , __VA_ARGS__ )
#define foreach23( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach22( action, staticarg , __VA_ARGS__ )
#define foreach24( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach23( action, staticarg , __VA_ARGS__ )
#define foreach25( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach24( action, staticarg , __VA_ARGS__ )
#define foreach26( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach25( action, staticarg , __VA_ARGS__ )
#define foreach27( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach26( action, staticarg , __VA_ARGS__ )
#define foreach28( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach27( action, staticarg , __VA_ARGS__ )
#define foreach29( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach28( action, staticarg , __VA_ARGS__ )
#define foreach30( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach29( action, staticarg , __VA_ARGS__ )
#define foreach31( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach30( action, staticarg , __VA_ARGS__ )
#define foreach32( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach31( action, staticarg , __VA_ARGS__ )
#define foreach33( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach32( action, staticarg , __VA_ARGS__ )
#define foreach34( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach33( action, staticarg , __VA_ARGS__ )
#define foreach35( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach34( action, staticarg , __VA_ARGS__ )
#define foreach36( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach35( action, staticarg , __VA_ARGS__ )
#define foreach37( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach36( action, staticarg , __VA_ARGS__ )
#define foreach38( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach37( action, staticarg , __VA_ARGS__ )
#define foreach39( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach38( action, staticarg , __VA_ARGS__ )
#define foreach40( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach39( action, staticarg , __VA_ARGS__ )
#define foreach41( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach40( action, staticarg , __VA_ARGS__ )
#define foreach42( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach41( action, staticarg , __VA_ARGS__ )
#define foreach43( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach42( action, staticarg , __VA_ARGS__ )
#define foreach44( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach43( action, staticarg , __VA_ARGS__ )
#define foreach45( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach44( action, staticarg , __VA_ARGS__ )
#define foreach46( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach45( action, staticarg , __VA_ARGS__ )
#define foreach47( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach46( action, staticarg , __VA_ARGS__ )
#define foreach48( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach47( action, staticarg , __VA_ARGS__ )
#define foreach49( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach48( action, staticarg , __VA_ARGS__ )
#define foreach50( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach49( action, staticarg , __VA_ARGS__ )
#define foreach51( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach50( action, staticarg , __VA_ARGS__ )
#define foreach52( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach51( action, staticarg , __VA_ARGS__ )
#define foreach53( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach52( action, staticarg , __VA_ARGS__ )
#define foreach54( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach53( action, staticarg , __VA_ARGS__ )
#define foreach55( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach54( action, staticarg , __VA_ARGS__ )
#define foreach56( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach55( action, staticarg , __VA_ARGS__ )
#define foreach57( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach56( action, staticarg , __VA_ARGS__ )
#define foreach58( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach57( action, staticarg , __VA_ARGS__ )
#define foreach59( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach58( action, staticarg , __VA_ARGS__ )
#define foreach60( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach59( action, staticarg , __VA_ARGS__ )
#define foreach61( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach60( action, staticarg , __VA_ARGS__ )
#define foreach62( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach61( action, staticarg , __VA_ARGS__ )
#define foreach63( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach62( action, staticarg , __VA_ARGS__ )
#define foreach64( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach63( action, staticarg , __VA_ARGS__ )
#define foreach65( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach64( action, staticarg , __VA_ARGS__ )
#define foreach66( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach65( action, staticarg , __VA_ARGS__ )
#define foreach67( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach66( action, staticarg , __VA_ARGS__ )
#define foreach68( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach67( action, staticarg , __VA_ARGS__ )
#define foreach69( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach68( action, staticarg , __VA_ARGS__ )
#define foreach70( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach69( action, staticarg , __VA_ARGS__ )
#define foreach71( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach70( action, staticarg , __VA_ARGS__ )
#define foreach72( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach71( action, staticarg , __VA_ARGS__ )
#define foreach73( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach72( action, staticarg , __VA_ARGS__ )
#define foreach74( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach73( action, staticarg , __VA_ARGS__ )
#define foreach75( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach74( action, staticarg , __VA_ARGS__ )
#define foreach76( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach75( action, staticarg , __VA_ARGS__ )
#define foreach77( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach76( action, staticarg , __VA_ARGS__ )
#define foreach78( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach77( action, staticarg , __VA_ARGS__ )
#define foreach79( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach78( action, staticarg , __VA_ARGS__ )
#define foreach80( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach79( action, staticarg , __VA_ARGS__ )
#define foreach81( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach80( action, staticarg , __VA_ARGS__ )
#define foreach82( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach81( action, staticarg , __VA_ARGS__ )
#define foreach83( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach82( action, staticarg , __VA_ARGS__ )
#define foreach84( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach83( action, staticarg , __VA_ARGS__ )
#define foreach85( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach84( action, staticarg , __VA_ARGS__ )
#define foreach86( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach85( action, staticarg , __VA_ARGS__ )
#define foreach87( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach86( action, staticarg , __VA_ARGS__ )
#define foreach88( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach87( action, staticarg , __VA_ARGS__ )
#define foreach89( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach88( action, staticarg , __VA_ARGS__ )
#define foreach90( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach89( action, staticarg , __VA_ARGS__ )
#define foreach91( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach90( action, staticarg , __VA_ARGS__ )
#define foreach92( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach91( action, staticarg , __VA_ARGS__ )
#define foreach93( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach92( action, staticarg , __VA_ARGS__ )
#define foreach94( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach93( action, staticarg , __VA_ARGS__ )
#define foreach95( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach94( action, staticarg , __VA_ARGS__ )
#define foreach96( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach95( action, staticarg , __VA_ARGS__ )
#define foreach97( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach96( action, staticarg , __VA_ARGS__ )
#define foreach98( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach97( action, staticarg , __VA_ARGS__ )
#define foreach99( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach98( action, staticarg , __VA_ARGS__ )
#define foreach100( action , staticarg , arg , ... ) action( staticarg , arg ) , foreach99( action, staticarg , __VA_ARGS__ )

#define get_macro(  _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8, _arg9, _arg10,\
_arg11, _arg12, _arg13, _arg14, _arg15, _arg16, _arg17, _arg18, _arg19,\
_arg20, _arg21, _arg22, _arg23, _arg24, _arg25, _arg26, _arg27, _arg28,\
_arg29, _arg30, _arg31, _arg32, _arg33, _arg34, _arg35, _arg36, _arg37,\
_arg38, _arg39, _arg40, _arg41, _arg42, _arg43, _arg44, _arg45, _arg46,\
_arg47, _arg48, _arg49, _arg50, _arg51, _arg52, _arg53, _arg54, _arg55,\
_arg56, _arg57, _arg58, _arg59, _arg60, _arg61, _arg62, _arg63, _arg64,\
_arg65, _arg66, _arg67, _arg68, _arg69, _arg70, _arg71, _arg72, _arg73,\
_arg74, _arg75, _arg76, _arg77, _arg78, _arg79, _arg80, _arg81, _arg82,\
_arg83, _arg84, _arg85, _arg86, _arg87, _arg88, _arg89, _arg90, _arg91,\
_arg92, _arg93, _arg94, _arg95, _arg96, _arg97, _arg98, _arg99, _arg100,\
NAME,...) NAME

#define foreach(action,...) get_macro(__VA_ARGS__, foreach99, foreach98, foreach97, foreach96, foreach95, foreach94, foreach93, foreach92, foreach91,\
foreach90, foreach89, foreach88, foreach87, foreach86, foreach85, foreach84, foreach83, foreach82, foreach81,\
foreach80, foreach79, foreach78, foreach77, foreach76, foreach75, foreach74, foreach73, foreach72, foreach71,\
foreach70, foreach69, foreach68, foreach67, foreach66, foreach65, foreach64, foreach63, foreach62, foreach61,\
foreach60, foreach59, foreach58, foreach57, foreach56, foreach55, foreach54, foreach53, foreach52, foreach51,\
foreach50, foreach49, foreach48, foreach47, foreach46, foreach45, foreach44, foreach43, foreach42, foreach41,\
foreach40, foreach39, foreach38, foreach37, foreach36, foreach35, foreach34, foreach33, foreach32, foreach31,\
foreach30, foreach29, foreach28, foreach27, foreach26, foreach25, foreach24, foreach23, foreach22, foreach21,\
foreach20, foreach19, foreach18, foreach17, foreach16, foreach15, foreach14, foreach13, foreach12, foreach11,\
foreach10, foreach9, foreach8, foreach7, foreach6, foreach5, foreach4, foreach3, foreach2, foreach1)\
(action,__VA_ARGS__)

#endif
