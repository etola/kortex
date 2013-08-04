// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See licence.txt file for licence information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_STRING_H
#define KORTEX_STRING_H

#include <string>
using std::string;

namespace kortex {

    string get_file_extension( const string& file );

    bool is_exact_match(const char* str1, const char* str2);

}

#endif
