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

#include <kortex/string.h>
#include <kortex/check.h>
#include <cstring>

namespace kortex {

    string get_file_extension( const string& file ) {
        size_t found = file.find_last_of(".");
        return file.substr(found+1);
    }


    bool is_exact_match(const char* str1, const char* str2) {
        assert_pointer( str1 );
        assert_pointer( str2 );
        if( !strcmp(str1,str2) ) return true;
        else                     return false;
    }




}

