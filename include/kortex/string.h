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

    string get_file_extension( const string& file );

    bool is_exact_match(const char* str1, const char* str2);

    string num2str(const int&    v);
    string num2str(const float&  m, const int& precision);
    string num2str(const double& m, const int& precision);

    /// removes leading and trailing spaces (remove_str)
    string trim_string(const string& str, const string& remove_str);

}

#endif
