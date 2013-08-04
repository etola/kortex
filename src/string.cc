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

    string num2str(const int& v) {
        char buffer[256];
        sprintf(buffer, "%d", v);
        return string(buffer);
    }

    string num2str(const float& m, const int& precision) {
        float  n      = fabs(m);
        string retval = num2str(int(n));
        retval += ".";
        n -= int(n);
        for( int d=0; d<precision; d++ ) {
            n = 10*n;
            retval += num2str(int(n));
            n -= int(n);
        }
        if( m<0 ) retval="-"+retval;
        return retval;
    }

    string num2str(const double& m, const int& precision) {
        double n = fabs(m);
        string retval = num2str(int(n));
        retval += ".";
        n -= int(n);
        for( int d=0; d<precision; d++ ) {
            n = 10*n;
            retval += num2str(int(n));
            n -= int(n);
        }
        if( m<0 ) retval="-"+retval;
        return retval;
    }



}

