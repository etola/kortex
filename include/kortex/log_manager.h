// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
//
// ---------------------------------------------------------------------------
//
// an initial version of this log manager was developed in collaboration with
// Mustafa Ozuysal
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_LOG_MANAGER_H
#define KORTEX_LOG_MANAGER_H

#include <cstdlib>
#include <cstdio>
#include <cstdarg>

namespace kortex {

    class LogManager {
        typedef void(*FatalFnPtr)(const char* group, const char* msg, va_list prm);

    public:
        enum Verbosity { Silent = 0, Cautious, Normal, Informative };

        LogManager( FILE* tinfo_stream, FILE* tlog_stream, FILE* twarn_stream, FILE* terr_stream );
        ~LogManager() { stop_recording(); }

        void info   ( const char* group, const char* msg, ... );
        void log    ( const char* group, const char* msg, ... );
        void warning( const char* group, const char* msg, ... );
        void error  ( const char* group, const char* msg, ... );
#ifdef __GNUC__
        void fatal(const char* group, const char* msg, ...) __attribute__((noreturn));
#else
        __declspec(noreturn) void fatal(const char* group, const char* msg, ...);
#endif

        void set_streams( FILE* tinfo_stream, FILE* tlog_stream,
                          FILE* twarn_stream, FILE* terr_stream ) {
            if( tinfo_stream ) info_stream = tinfo_stream;
            if( tlog_stream  ) log_stream  = tlog_stream;
            if( twarn_stream ) warn_stream = twarn_stream;
            if( terr_stream  ) err_stream  = terr_stream;
        }

        void set_verbosity(Verbosity  verb     ) { verbosity = verb; }
        void set_fatal_fn (FatalFnPtr tfatal_fn) {
            if( tfatal_fn )
                fatal_func = tfatal_fn;
        }

        void start_recording(const char* log_file_name);
        void stop_recording();
        void write_to_log_file(const char* group, const char* msg, va_list argptr);

        void brief( bool bval=true ) { brief_message = bval; }

    private:

        bool  brief_message;

        FatalFnPtr fatal_func;
        Verbosity  verbosity;

        FILE* log_file;

        FILE* info_stream;
        FILE* log_stream;
        FILE* warn_stream;
        FILE* err_stream;
    };

    LogManager* log_man();

    void log_man_recorder_start( const char* file_name );
    void log_man_recorder_stop ();
    void release_log_man();

}


#ifdef __GNUC__
#define function_line_str kortex::format_function_message( __PRETTY_FUNCTION__, __LINE__).c_str()
#else
#define function_line_str kortex::format_function_message( __FUNCTION__, __LINE__).c_str()
#endif

#define logman_info_(...)    kortex::log_man()->info   (function_line_str, __VA_ARGS__)
#define logman_log_(...)     kortex::log_man()->log    (function_line_str, __VA_ARGS__)
#define logman_warning_(...) kortex::log_man()->warning(function_line_str, __VA_ARGS__)
#define logman_error_(...)   kortex::log_man()->error  (function_line_str, __VA_ARGS__)
#define logman_fatal_(...)   kortex::log_man()->fatal  (function_line_str, __VA_ARGS__)

#define logman_info_g(fmt, ...)    logman_info_(fmt, __VA_ARGS__)
#define logman_log_g(fmt, ...)     logman_log_(fmt, __VA_ARGS__)
#define logman_warning_g(fmt, ...) logman_warning_(fmt, __VA_ARGS__)
#define logman_error_g(fmt, ...)   logman_error_(fmt, __VA_ARGS__)
#define logman_fatal_g(fmt, ...)   logman_fatal_(fmt, __VA_ARGS__)

#define logman_info(msg)    kortex::log_man()->info   (function_line_str, "%s", msg)
#define logman_log(msg)     kortex::log_man()->log    (function_line_str, "%s", msg)
#define logman_warning(msg) kortex::log_man()->warning(function_line_str, "%s", msg)
#define logman_error(msg)   kortex::log_man()->error  (function_line_str, "%s", msg)
#define logman_fatal(msg)   kortex::log_man()->fatal  (function_line_str, "%s", msg)


#define logman_log_gvi(prestr, int_arr, asz) {                                  \
    static const int bufsz = 2560;                                              \
    const int* arr = int_arr;                                                   \
    char buf[bufsz];                                                            \
    int nstr = sprintf( buf, "%s [", prestr );                                  \
    for( int i=0; i<asz; i++, arr++ ) nstr += sprintf( buf+nstr, "%d ", *arr ); \
    nstr += sprintf( buf+nstr, "]" );                                           \
    assert_statement( nstr <= bufsz, "buffer overflow" );                       \
    logman_log( buf );                                                          \
    }

#define logman_info_gvi(prestr, int_arr, asz) {                                  \
    static const int bufsz = 2560;                                              \
    const int* arr = int_arr;                                                   \
    char buf[bufsz];                                                            \
    int nstr = sprintf( buf, "%s [", prestr );                                  \
    for( int i=0; i<asz; i++, arr++ ) nstr += sprintf( buf+nstr, "%d ", *arr ); \
    nstr += sprintf( buf+nstr, "]" );                                           \
    assert_statement( nstr <= bufsz, "buffer overflow" );                       \
    logman_info( buf );                                                          \
    }


#define logman_log_gvf(prestr, float_arr, asz) {                                \
    static const int bufsz = 2560;                                              \
    const float* arr = float_arr;                                               \
    char buf[bufsz];                                                            \
    int nstr = sprintf( buf, "%s [", prestr );                                  \
    for( int i=0; i<asz; i++, arr++ ) nstr += sprintf( buf+nstr, "%f ", *arr ); \
    nstr += sprintf( buf+nstr, "]" );                                           \
    assert_statement( nstr <= bufsz, "buffer overflow" );                       \
    logman_log( buf );                                                          \
    }

#define logman_log_gvs(prestr, str_array, asz) {                 \
    logman_log( prestr );                                        \
    const string* sarr = str_array;                              \
    for( int i=0; i<asz; i++, sarr++ )                           \
        logman_log_g( "% 3d/% 3d - %s", i, asz, sarr->c_str() ); \
    }

#endif
