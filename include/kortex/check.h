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
#include <set>
#include <vector>
using std::string;
using std::vector;

#ifdef _WIN32
#ifndef isnan
#define isnan(x) _isnan(x)
#endif
#ifndef isinf
#define isinf(x) (!_finite(x))
#endif
#endif

#ifdef __GNUC__
#ifdef isnan
#undef isnan
#endif
#ifdef isinf
#undef isinf
#endif
#endif

namespace kortex {
    template <typename T>
    inline bool is_finite( const T& v ) {
        return std::isfinite(v);
    }

    template <typename T>
    inline bool is_a_number( const T& v ) {
        return ( std::isnan(v) ) ? false : true;
    }

    template <typename T>
    inline bool is_valid_number( const T& v ) {
        return is_a_number(v) && is_finite(v);
    }
}

#ifdef DEBUG
#define assert_statement( statement, msg )      { if( !(statement) ) { logman_fatal(msg); } }
#define assert_statement_g(statement, msg, ...) { if( !(statement) ) { logman_fatal_g(msg, __VA_ARGS__); } }
#define assert_pointer(statement)               { if( !(statement) ) { logman_fatal("passed null pointer"); } }
#define assert_pointer_size(psz)                { if( !is_valid_number(psz) || psz<=0 ) { logman_fatal_g("passed nonpositive array size [%d]", psz); } }
#define assert_boundary(val,minval,maxval)      { if( !(val>=minval && val<maxval ) ) logman_fatal_g("out of bounds: %f not in [%f,%f)", float(val), float(minval), float(maxval) ); }
#define assert_number(v)                        { if( !is_valid_number(v) ) { logman_fatal("nan/inf test failed"); } }
#define assert_noalias(obj1,obj2)               { if( &obj1 == &obj2 ) { logman_fatal("aliasing is not allowed"); } }
#define assert_noalias_p(obj1,obj2)             { if(  obj1 ==  obj2 ) { logman_fatal("aliasing is not allowed"); } }
#define debug_warning( statement, msg )      { if( !(statement) ) { logman_warning(msg); } }
#define debug_warning_g(statement, msg, ...) { if( !(statement) ) { logman_warning_g(msg, __VA_ARGS__); } }
#else
#define assert_statement( statement, msg ) { }
#define assert_statement_g(statement, msg, ...) { }
#define assert_pointer(statement)               { }
#define assert_pointer_size(psz) { }
#define assert_boundary(val,minval,maxval) { }
#define assert_number(num) { }
#define assert_noalias(obj1,obj2) { }
#define assert_noalias_p(obj1,obj2) { }
#define debug_warning( statement, msg      ) { }
#define debug_warning_g( statement, msg, ... ) { }
#endif

/// persistent assert -> stays thru runtime
#define passert_statement( statement, msg )      { if( !(statement) ) { logman_fatal(msg); } }
#define passert_statement_g(statement, msg, ...) { if( !(statement) ) { logman_fatal_g(msg, __VA_ARGS__); } }
#define passert_pointer(statement)               { if( !(statement) ) { logman_fatal("passed null pointer"); } }
#define passert_pointer_size(psz)                { if( !is_valid_number(psz) || psz<=0 ) { logman_fatal_g("passed nonpositive array size [%d]", psz); } }
#define passert_boundary(val,minval,maxval)      { if( !(val>=minval && val<maxval ) ) logman_fatal_g("out of bounds: %f not in [%f,%f)", float(val), float(minval), float(maxval) ); }
#define passert_number(v)                        { if( !is_valid_number(v) ) { logman_fatal("nan/inf test failed"); } }
#define passert_noalias(obj1,obj2)               { if( &obj1 == &obj2 ) { logman_fatal("aliasing is not allowed"); } }
#define passert_noalias_p(obj1,obj2)             { if(  obj1 ==  obj2 ) { logman_fatal("aliasing is not allowed"); } }

#define switch_fatality() { logman_fatal("unhandled switch case"); }

namespace kortex {

    string format_function_message(const string& str, const int& number);

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

    template <typename T>
    inline bool is_inside( const vector<T>& arr, const T& v ) {
        for( size_t i=0; i<arr.size(); i++ )
            if( arr[i] == v ) return true;
        return false;
    }

    template <typename T>
    inline bool is_inside( const T* arr, const int& narr, const T& query ) {
        assert_pointer( arr );
        assert_pointer_size( narr );
        for( int i=0; i<narr; i++ )
            if( arr[i] == query ) return true;
        return false;
    }

    template <typename T>
    inline bool is_ascending( const vector<T>& arr ) {
        int asz = arr.size();
        if( asz == 1 ) return true;
        for( int i=1; i<asz; i++ )
            if( arr[i] < arr[i-1] )
                return false;
        return true;
    }

    template <typename T>
    inline bool is_descending( const vector<T>& arr ) {
        int asz = arr.size();
        if( asz == 1 ) return true;
        for( int i=1; i<asz; i++ )
            if( arr[i] > arr[i-1] )
                return false;
        return true;
    }

    template <typename T>
    inline bool is_positive_number( const T& v ) {
        return ( is_a_number(v) && (v>T(0)) ) ? true : false;
    }

    template <typename T>
    inline bool is_nonnegative_number( const T& v ) {
        return ( is_a_number(v) && (v>=T(0)) ) ? true : false;
    }

    template<typename T>
    inline bool is_valid_array( const T* A, int asz ) {
        assert_pointer( A );
        assert_pointer_size( asz );
        for( int i=0; i<asz; i++ ) {
            if( !is_a_number(A[i]) ) return false;
            if( !is_finite  (A[i]) ) return false;
        }
        return true;
    }

    template<typename T>
    inline void assert_array(const char* name, const T * arr, int asz) {
#ifdef DEBUG
        assert_statement_g( is_valid_array(arr,asz), "[%s] array contains nan/inf", name );
#endif
    }

    template<typename T>
    bool is_all_zero( const T* A, int asz, T eps=1e-16 ) {
        assert_pointer( A );
        assert_pointer_size( asz );
        for( int i=0; i<asz; i++ )
            if( fabs(A[i]) > eps ) return false;
        return true;
    }

    inline bool has_unique_elements( const vector<int>& arr ) {
        std::set<int> sarr;
        sarr.insert( arr.begin(), arr.end() );
        return bool( sarr.size() == arr.size() );
    }



}

#endif
