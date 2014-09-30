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

namespace kortex {

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


}

#endif
