#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([flacdemon], [0.1], [meriadoc.clarke@gmail.com])
AC_CONFIG_SRCDIR([src/daemon/daemon.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET

AC_LANG_PUSH([C++])
# Checks for libraries.
#check for standard libraries here?

#AC_CHECK_LIB([mba] , [diff] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -lmba"] , [ AC_MSG_ERROR( Library dependency missing: libmba ) ] )
AC_CHECK_LIB([sqlite3] , [sqlite3_open_v2] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -lsqlite3"] , [ AC_MSG_ERROR( Library dependency missing: libsqlite3 ) ] )
AC_CHECK_LIB([uuid] , [uuid_generate_random] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -luuid"] , [ AC_MSG_ERROR( Library dependency missing: libuuid ) ] )
AC_CHECK_LIB([curl] , [curl_global_init] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -lcurl"] , [ AC_MSG_ERROR( Library dependency missing: libcurl ) ] )
AC_CHECK_LIB([jansson] , [json_loads] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -ljansson"] , [ AC_MSG_ERROR( Library dependency missing: libjansson ) ] )
AC_CHECK_LIB([ao] , [ao_initialize] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -lao"] , [ AC_MSG_ERROR( Library dependency missing: libao ) ] )

AC_CHECK_LIB([avcodec] , [avcodec_find_decoder] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -lavcodec"] , [ AC_MSG_ERROR( Library dependency missing: libavcodec ) ] )
AC_CHECK_LIB([avformat] , [avformat_alloc_context] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -lavformat"] , [ AC_MSG_ERROR( Library dependency missing: libavformat ) ] )
AC_CHECK_LIB([avutil] , [av_dict_get] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -lavutil"] , [ AC_MSG_ERROR( Library dependency missing: libavutil ) ] )
AC_CHECK_LIB([swresample] , [swr_init] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -lswresample"] , [ AC_MSG_ERROR( Library dependency missing: libswresample ) ] )
#AC_CHECK_LIB([pthread] , [initialize_pthread] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -lpthread"] , [ AC_MSG_ERROR( Library dependency missing: libpthread) ] )


AC_CHECK_LIB([curses] , [initscr] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -lcurses"], [ AC_MSG_ERROR( Library dependency missing: libcurses ) ] )
AC_CHECK_LIB([panel] , [hide_panel] , [flacdemon_LDFLAGS="$flacdemon_LDFLAGS -lpanel"] , [ AC_MSG_ERROR( Library dependency missing: libpanel ) ] )

#AC_CHECK_LIB([] , [] , [] , [ AC_MSG_ERROR( Library dependency missing: ) ] )

flacdemoninterface_LDFLAGS="$flacdemon_LDFLAGS"
AC_SUBST(flacdemon_LDFLAGS)
AC_SUBST(flacdemoninterface_LDFLAGS)

# Checks for header files.
AC_HEADER_STDC

AC_CHECK_HEADER([uuid/uuid.h], [], [ AC_MSG_ERROR( Missing required header uuid/uuid.h) ])
#AC_CHECK_HEADER([mba/diff.h], [], [ AC_MSG_ERROR( Missing required header mba/diff.h) ])
AC_CHECK_HEADER([sqlite3.h], [], [ AC_MSG_ERROR( Missing required header sqlite3.h) ])
AC_CHECK_HEADER([sys/types.h], [], [ AC_MSG_ERROR( Missing required header sys/types.h) ])
AC_CHECK_HEADER([sys/stat.h], [], [ AC_MSG_ERROR( Missing required header sys/stat.h) ])
AC_CHECK_HEADER([dirent.h], [], [ AC_MSG_ERROR( Missing required header dirent.h) ])
AC_CHECK_HEADER([libavformat/avformat.h], [], [ AC_MSG_ERROR( Missing required header libavformat/avformat.h) ])
AC_CHECK_HEADER([libavcodec/avcodec.h], [], [ AC_MSG_ERROR( Missing required header libavcodec/avcodec.h) ])
AC_CHECK_HEADER([libavutil/avutil.h], [], [ AC_MSG_ERROR( Missing required header libavutil/avutil.h) ])
AC_CHECK_HEADER([libavutil/dict.h], [], [ AC_MSG_ERROR( Missing required header libavutil/dict.h) ])
AC_CHECK_HEADER([libavutil/opt.h], [], [ AC_MSG_ERROR( Missing required header libavutil/opt.h) ])
AC_CHECK_HEADER([libswresample/swresample.h], [], [ AC_MSG_ERROR( Missing required header libswresample/swresample.h) ])
AC_CHECK_HEADER([jansson.h], [], [ AC_MSG_ERROR( Missing required header jansson.h) ])

#AC_CHECK_HEADER([], [], [ AC_MSG_ERROR( Missing required header ) ])


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero getcwd gethostbyname modf socket strchr strerror strpbrk strtol])

AC_OUTPUT(Makefile build/daemon/Makefile build/interface/Makefile)
