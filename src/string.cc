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
#include <kortex/linux_windows_defs.h>
#include <cstring>

namespace kortex {

    /// returns 0 if strings are identical disregarding case
    int compare_string_nc( const string& str1, const string& str2 ) {
        return strncasecmp( str1.c_str(), str2.c_str(), str2.size() );
    }

    /// returns 0 if strings are identical disregarding case
    int compare_string_nc( const char* str1, const char* str2 ) {
        return strncasecmp(str1,str2,strlen(str2));
    }

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

    bool is_exact_match( const string& str1, const string& str2 ) {
        if( !strcmp(str1.c_str(),str2.c_str()) ) return true;
        else                                     return false;
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

    /// removes leading and trailing spaces (remove_str)
    string trim_string(const string& str, const string& remove_str) {
        const size_t begin_str = str.find_first_not_of(remove_str);
        if (begin_str == std::string::npos) {
            return "";
        }
        const size_t end_str = str.find_last_not_of(remove_str);
        const size_t range   = end_str - begin_str + 1;
        return str.substr(begin_str, range);
    }

    string pad_zeros4( int num ) {
        string nstr = num2str(num);
        if     ( num   < 10 ) nstr = "000" + nstr;
        else if( num  < 100 ) nstr = "00"  + nstr;
        else if( num < 1000 ) nstr = "0"   + nstr;
        return nstr;
    }

    string pad_zeros8( int num ) {
        string nstr = num2str(num);
        if     ( num   <     10 ) nstr = "0000000" + nstr;
        else if( num  <     100 ) nstr = "000000"  + nstr;
        else if( num <     1000 ) nstr = "00000"   + nstr;
        else if( num <    10000 ) nstr = "0000"    + nstr;
        else if( num <   100000 ) nstr = "000"     + nstr;
        else if( num <  1000000 ) nstr = "00"      + nstr;
        else if( num < 10000000 ) nstr = "0"       + nstr;
        return nstr;
    }

    string file_name( const string& str, int num, const string& ext, int num_padding ) {
        switch( num_padding ) {
        case 4:  return string( str + pad_zeros4(num) + "." + ext ); break;
        case 8:
        default: return string( str + pad_zeros8(num) + "." + ext ); break;
        }
    }

    string file_name( const string& str, int num1, int num2, const string& ext, int num_padding ) {
        switch( num_padding ) {
        case 4:  return string( str + pad_zeros4(num1) + "-" + pad_zeros4(num2) + "." + ext ); break;
        case 8:
        default: return string( str + pad_zeros8(num1) + "-" + pad_zeros8(num2) + "." + ext ); break;
        }
    }

    /// replaces all the white_space chars with the fill string. check tst_string for examples.
    string replace_whitespace( const string& str, const string& white_space, const string& fill_str ) {
        string result = trim_string(str);

        // replace sub ranges
        size_t begin_space = result.find_first_of(white_space);

        while( begin_space != std::string::npos ) {
            size_t end_space = result.find_first_not_of(white_space, begin_space);
            size_t range     = end_space - begin_space;
            result.replace(begin_space, range, fill_str);
            size_t new_start = begin_space + fill_str.length();
            begin_space = result.find_first_of(white_space, new_start);
        }
        return result;
    }

}

