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
#ifndef KORTEX_STRING_H
#define KORTEX_STRING_H

#include <string>
using std::string;

#include <cfloat>

#ifdef DBL_DECIMAL_DIG
#define OP_DBL_Digs (DBL_DECIMAL_DIG)
#else
#ifdef DECIMAL_DIG
#define OP_DBL_Digs (DECIMAL_DIG)
#else
#define OP_DBL_Digs (DBL_DIG + 3)
#endif
#endif

namespace kortex {

    inline void in_value( const char* str, float  & v ) { v = (float)atof(str);       }
    inline void in_value( const char* str, double & v ) { v =        atof(str);       }
    inline void in_value( const char* str, int    & v ) { v =        atoi(str);       }
    inline void in_value( const char* str, size_t & v ) { v =        atoi(str);       }
    inline void in_value( const char* str, bool   & v ) { v =   bool(atoi(str)!=0);   }
    inline void in_value( const char* str, char   & v ) { v = str[0];                 }
    inline void in_value( const char* str, string & v ) { v = string(str);            }

    inline string in_str( const int   & v ) {
        char buffer[1024];
        sprintf( buffer, "%d", v );
        return string(buffer);
    }
    inline string in_str( const float & v ) {
        char buffer[1024];
        sprintf( buffer, "%.*e", OP_DBL_Digs-1, v );
        return string(buffer);
    }
    inline string in_str( const double& v ) {
        char buffer[1024];
        sprintf( buffer, "%.*e", OP_DBL_Digs-1, v );
        return string(buffer);
    }
    inline string in_str( const size_t& v ) {
        char buffer[1024];
        sprintf( buffer, "%zd", v );
        return string(buffer);
    }
    inline string in_str( const bool  & v ) {
        char buffer[1024];
        sprintf( buffer, "%d", v );
        return string(buffer);
    }
    inline string in_str( const char  & v ) {
        char buffer[1024];
        sprintf( buffer, "%c", v );
        return string(buffer);
    }
    inline string in_str( const string& v ) {
        return v;
    }
    inline string in_str( const char* v ) {
        return string(v);
    }

    /// returns 0 if strings are identical disregarding case
    int compare_string_nc( const string& str1, const string& str2 );
    int compare_string_nc( const char*   str1, const char*   str2 );

    string get_file_extension( const string& file );
    string get_file_name     ( const string& file );
    string get_file_root     ( const string& file );
    string get_folder_name   ( const string& file );

    bool is_exact_match( const char  * str1, const char  * str2 );
    bool is_exact_match( const string& str1, const string& str2 );

    string num2str(const int&    v);
    string num2str(const float&  m, const int& precision);
    string num2str(const double& m, const int& precision);

    /// removes leading and trailing spaces (remove_str)
    string trim_string(const string& str, const string& remove_str=" \t");

    string pad_zeros4( int num );
    string pad_zeros8( int num );

    string file_name( const string& str, int num, const string& ext, int num_padding=4 );
    string file_name( const string& str, int num0, int num1, const string& ext, int num_padding=4 );

    /// replaces all the white_space chars with the fill string. check tst_string for examples.
    string replace_whitespace( const string& str, const string& white_space, const string& fill_str );

    string resolve_full_path( const string& istr );

}

#endif
