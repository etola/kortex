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

#endif
