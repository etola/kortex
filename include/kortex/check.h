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
#ifndef KORTEX_CHECK_H
#define KORTEX_CHECK_H

#include <kortex/log_manager.h>
#include <cmath> // isnan / isinf tests
#include <string>
#include <vector>
using std::string;
using std::vector;

#define function_line_str kortex::format_function_message(__PRETTY_FUNCTION__, __LINE__).c_str()

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

#ifdef DEBUG
#define assert_statement( statement, msg )      { if( !(statement) ) { logman_fatal(msg); } }
#define assert_statement_g(statement, msg, ...) { if( !(statement) ) { logman_fatal_g(msg, __VA_ARGS__); } }
#define assert_pointer(statement)               { if( !(statement) ) { logman_fatal("passed null pointer"); } }
#define assert_pointer_size(psz)                { if( isnan(psz) || psz<=0 ) { logman_fatal_g("passed nonpositive array size [%d]", psz); } }
#define assert_boundary(val,minval,maxval)      { if( !(val>=minval && val<maxval ) ) logman_fatal("out of bounds"); }
#define assert_number(num)                      { if( isnan(num) || isinf(num) ) { logman_fatal("nan/inf test failed"); } }
#define assert_noalias(obj1,obj2)               { if( &obj1 == &obj2 ) { logman_fatal("aliasing is not allowed"); } }
#define assert_noalias_p(obj1,obj2)             { if(  obj1 ==  obj2 ) { logman_fatal("aliasing is not allowed"); } }
#else
#define assert_statement( statement, msg ) { }
#define assert_statement_g(statement, msg, ...) { }
#define assert_pointer(statement)               { }
#define assert_pointer_size(psz) { }
#define assert_boundary(val,minval,maxval) { }
#define assert_number(num) { }
#define assert_noalias(obj1,obj2) { }
#define assert_noalias_p(obj1,obj2) { }
#endif

/// persistent assert -> stays thru runtime
#define passert_statement( statement, msg )      { if( !(statement) ) { logman_fatal(msg); } }
#define passert_statement_g(statement, msg, ...) { if( !(statement) ) { logman_fatal_g(msg, __VA_ARGS__); } }
#define passert_pointer(statement)               { if( !(statement) ) { logman_fatal("passed null pointer"); } }
#define passert_pointer_size(psz)                { if( isnan(psz) || psz<=0 ) { logman_fatal_g("passed nonpositive array size [%d]", psz); } }
#define passert_boundary(val,minval,maxval)      { if( !(val>=minval && val<maxval ) ) logman_fatal("out of bounds"); }
#define passert_number(num)                      { if( isnan(num) || isinf(num) ) { logman_fatal("nan/inf test failed"); } }
#define passert_noalias(obj1,obj2)               { if( &obj1 == &obj2 ) { logman_fatal("aliasing is not allowed"); } }
#define passert_noalias_p(obj1,obj2)             { if(  obj1 ==  obj2 ) { logman_fatal("aliasing is not allowed"); } }

#define switch_fatality() { logman_fatal("unhandled switch case"); }

namespace kortex {

    string format_function_message(const string& str, const int& number);
    void assert_array(const char* name, const float* arr, int asz);

    template <typename T> inline
    bool is_16_byte_aligned(const T* pnt) {
        return ( ((const uintptr_t)(pnt)) & 0x0F ) == 0 ? true : false;
    }

    inline bool is_inside( const int& x, const int& xmin, const int& xmax ) {
        assert_number(x);
        assert_number(xmin);
        assert_number(xmax);
        return ( x >= xmin && x < xmax ) ? true : false;
    }
    inline bool is_inside( const float& x, const float& xmin, const float& xmax ) {
        assert_number(x);
        assert_number(xmin);
        assert_number(xmax);
        return ( x >= xmin && x < xmax ) ? true : false;
    }
    inline bool is_inside( const double& x, const double& xmin, const double& xmax ) {
        assert_number(x);
        assert_number(xmin);
        assert_number(xmax);
        return ( x >= xmin && x < xmax ) ? true : false;
    }
    inline bool is_inside( const double& x, const int& xmin, const int& xmax ) {
        assert_number(x);
        assert_number(xmin);
        assert_number(xmax);
        return ( x >= xmin && x < xmax ) ? true : false;
    }
    inline bool is_inside( const float& x, const int& xmin, const int& xmax ) {
        assert_number(x);
        assert_number(xmin);
        assert_number(xmax);
        return ( x >= xmin && x < xmax ) ? true : false;
    }

    inline bool is_inside( const vector<int>& arr, const int& v ) {
        for( size_t i=0; i<arr.size(); i++ )
            if( arr[i] == v ) return true;
        return false;
    }

    inline bool is_inside( const int* arr, const int& narr, const int& query ) {
        assert_pointer( arr );
        for( int i=0; i<narr; i++ )
            if( arr[i] == query ) return true;
        return false;
    }

    inline bool is_a_number(const int&    v) { return ( isnan(v) ) ? false : true; }
    inline bool is_a_number(const float&  v) { return ( isnan(v) ) ? false : true; }
    inline bool is_a_number(const double& v) { return ( isnan(v) ) ? false : true; }

    inline bool is_positive_number( const int&    v ) { return ( is_a_number(v) && (v>0) ) ? true : false; }
    inline bool is_positive_number( const float&  v ) { return ( is_a_number(v) && (v>0) ) ? true : false; }
    inline bool is_positive_number( const double& v ) { return ( is_a_number(v) && (v>0) ) ? true : false; }

    inline bool is_nonnegative_number( const int&    v ) { return ( is_a_number(v) && (v>=0) ) ? true : false; }
    inline bool is_nonnegative_number( const float&  v ) { return ( is_a_number(v) && (v>=0) ) ? true : false; }
    inline bool is_nonnegative_number( const double& v ) { return ( is_a_number(v) && (v>=0) ) ? true : false; }


}

#endif
