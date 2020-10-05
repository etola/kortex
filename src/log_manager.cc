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
#include <string>
#include <ctime>

#ifdef __GNUC__
#include <cxxabi.h>
#include <execinfo.h>
#endif

#include <kortex/check.h>
#include <kortex/log_manager.h>

using std::string;

namespace kortex {

    static LogManager* s_log_man = (LogManager*) new LogManager(stderr, stderr,
                                                                stderr, stderr);

    /// taken from
    /// http://mykospark.net/2009/09/runtime-backtrace-in-c-with-name-demangling/
    string demangle(const char* symbol) {
#ifdef __GNUC__
        char temp[128];
        if( sscanf(symbol, "%*[^(]%*[^_]%127[^)+]", temp) == 1 ) {
            char* demangled = NULL;
            size_t size;
            int status;
            if( (demangled = abi::__cxa_demangle(temp, NULL, &size, &status)) != NULL ) {
                string result(demangled);
                free(demangled);
                return result;
            }
        }
        if( sscanf(symbol, "%127s", temp) == 1 )
            return temp;
#endif
        return symbol;
    }

    void print_trace() {
#ifdef __GNUC__
        void *array[50];
        int size    = backtrace (array, 50);
        char** strings = backtrace_symbols (array, size);
        fprintf( stderr, "\nObtained %d stack frames.\n", size );
        for( int i = 0; i<size; i++ ) {
            fprintf(stderr, "[ %04d ] [ %s ]\n", int(size-i), demangle(strings[i]).c_str() );
        }
        free(strings);
#else
        logman_warning("would have printed the stack in linux");
#endif
    }

    static void _fatal_func(const char* group, const char* msg, va_list prm) {
        fprintf(stderr, "[%s] fatal: ", group);
        vfprintf(stderr, msg, prm);
        fprintf(stderr, "\n");
        print_trace();
    }

    LogManager::LogManager(FILE* tinfo_stream, FILE* tlog_stream, FILE* twarn_stream, FILE* terr_stream)
        : fatal_func(_fatal_func),
          verbosity(LogManager::Normal),
          info_stream(tinfo_stream),
          log_stream(tlog_stream),
          warn_stream(twarn_stream),
          err_stream(terr_stream) {
        log_file = NULL;
        brief_message = false;
    }

    void LogManager::info(const char* group, const char* msg, ...) {
        if( verbosity > LogManager::Normal ) {
            fprintf(info_stream, "[%-40.40s] ", group);
            va_list prm;
            va_start(prm, msg);
            vfprintf(info_stream, msg, prm);
            va_end(prm);
            fprintf(info_stream, "\n");
        }
        va_list argptr;
        va_start(argptr, msg);
        write_to_log_file(nullptr, group, msg, argptr);
        va_end(argptr);
    }

    void LogManager::log(const char* group, const char* msg, ...) {
        if( verbosity > LogManager::Cautious ) {
            if( !brief_message )
                fprintf(log_stream, "[%-40.40s] ", group);
            va_list prm;
            va_start(prm, msg);
            vfprintf(log_stream, msg, prm);
            va_end(prm);
            fprintf(log_stream, "\n");
        }
        va_list argptr;
        va_start(argptr, msg);
        write_to_log_file(nullptr, group, msg, argptr);
        va_end(argptr);
    }

    void LogManager::warning(const char* group, const char* msg, ...) {
        if( verbosity > LogManager::Silent ) {
            fprintf(warn_stream, "[%-40.40s] warning: ", group);
            va_list prm;
            va_start(prm, msg);
            vfprintf(warn_stream, msg, prm);
            va_end(prm);
            fprintf(warn_stream, "\n");
        }
        va_list argptr;
        va_start(argptr, msg);
        write_to_log_file("warning:", group, msg, argptr);
        va_end(argptr);
    }

    void LogManager::error(const char* group, const char* msg, ...) {
        fprintf(err_stream, "[%-40.40s] error: ", group);
        va_list prm;
        va_start(prm, msg);
        vfprintf(err_stream, msg, prm);
        va_end(prm);
        fprintf(err_stream, "\n");

        va_list argptr;
        va_start(argptr, msg);
        write_to_log_file("error:", group, msg, argptr);
        va_end(argptr);
    }

    void LogManager::fatal(const char* group, const char* msg, ...) {
        va_list argptr;
        va_start(argptr, msg);
        write_to_log_file("error:", group, msg, argptr);
        va_end(argptr);

        va_list prm;
        va_start(prm, msg);
        fatal_func(group, msg, prm);
        va_end(prm);

        exit(99);
    }

    void LogManager::write_to_log_file(const char* tag, const char* group, const char* msg, va_list argptr) {
        if( !log_file ) return;

        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        fprintf(log_file, "%02d:%02d:%02d ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        fprintf(log_file, "[%s] ", group);
        if( tag != nullptr )
            fprintf(log_file, "%s ", tag);
        vfprintf(log_file, msg, argptr);
        fprintf(log_file, "\n");
        fflush(log_file);
    }

    void LogManager::start_recording(const char* log_file_name) {
        log_file = fopen(log_file_name, "a");
        if( log_file == NULL ) {
            fprintf( err_stream, "could not open the recording file: %s\n", log_file_name );
            exit(98);
        }
    }

    void LogManager::stop_recording() {
        if( log_file ) {
            fclose(log_file);
            log_file = NULL;
        }
    }

//
//
//

    LogManager* log_man() { return s_log_man; }


    void log_man_recorder_start( const char* file_name ) {
        if( log_man() )
            log_man()->start_recording(file_name);
    }

    void log_man_recorder_stop() {
        if( log_man() )
            log_man()->stop_recording();
    }

    void release_log_man() {
        delete s_log_man;
        s_log_man = NULL;
    }

}
