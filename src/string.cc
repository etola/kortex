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
#include <kortex/string.h>
#include <kortex/check.h>
#include <cstring>

namespace kortex {

    string get_file_extension( const string& file ) {
        size_t found = file.find_last_of(".");
        return file.substr(found+1);
    }

    string get_file_name( const string& file ) {
        size_t found     = file.find_last_of("/\\");
        string file_name = file.substr(found+1);
        return file_name;
    }

    string get_file_root( const string& file ) {
        string file_name = get_file_name(file);
        size_t found     = file_name.find_last_of(".");
        string file_root = file_name.substr(0,found);
        return file_root;
    }

    string get_folder_name( const string& file ) {
        size_t found = file.find_last_of("/\\");
        string folder_name = "";
        if( found == string::npos ) folder_name = ".";
        else                        folder_name = file.substr(0,found);
        return folder_name;
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

    string trim_string(const string& str, const string& remove_str) {
        const size_t begin_str = str.find_first_not_of(remove_str);
        if (begin_str == std::string::npos) {
            return "";
        }
        const size_t end_str = str.find_last_not_of(remove_str);
        const size_t range   = end_str - begin_str + 1;
        return str.substr(begin_str, range);
    }


}

