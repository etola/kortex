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
#include <vector>

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

	inline void in_value( const char* str, int8_t&   v ) { v = (int8_t  )atoi(str);    }
	inline void in_value( const char* str, int16_t&  v ) { v = (int16_t )atoi(str);    }
	inline void in_value( const char* str, int32_t&  v ) { v = (int32_t )atoi(str);    }
	inline void in_value( const char* str, int64_t&  v ) { v = (int64_t )atoi(str);    }
	inline void in_value( const char* str, uint8_t&  v ) { v = (uint8_t )atoi(str);    }
	inline void in_value( const char* str, uint16_t& v ) { v = (uint16_t)atoi(str);    }
	inline void in_value( const char* str, uint32_t& v ) { v = (uint32_t)atoi(str);    }
	inline void in_value( const char* str, uint64_t& v ) { v = (uint64_t)atoi(str);    }
	inline void in_value( const char* str, float  &  v ) { v = (float)   atof(str);    }
	inline void in_value( const char* str, double &  v ) { v =           atof(str);    }
	inline void in_value( const char* str, bool   &  v ) { v =   bool(atoi(str)!=0);   }
	inline void in_value( const char* str, char   &  v ) { v = str[0];                 }
	inline void in_value( const char* str, string &  v ) { v = string(str);            }

	inline string in_str( const int8_t   & v ) { char _b[64]; sprintf(_b, "%d", v ); return string(_b); }
	inline string in_str( const int16_t  & v ) { char _b[64]; sprintf(_b, "%d", v ); return string(_b); }
	inline string in_str( const int32_t  & v ) { char _b[64]; sprintf(_b, "%d", v ); return string(_b); }
	inline string in_str( const int64_t  & v ) { char _b[64]; sprintf(_b, "%ld", v ); return string(_b); }
	inline string in_str( const uint8_t  & v ) { char _b[64]; sprintf(_b, "%d", v ); return string(_b); }
	inline string in_str( const uint16_t & v ) { char _b[64]; sprintf(_b, "%d", v ); return string(_b); }
	inline string in_str( const uint32_t & v ) { char _b[64]; sprintf(_b, "%d", v ); return string(_b); }
	inline string in_str( const uint64_t & v ) { char _b[64]; sprintf(_b, "%ld", v ); return string(_b); }
	inline string in_str( const float    & v ) { char _b[64]; sprintf(_b, "%.*e", OP_DBL_Digs-1, v ); return string(_b); }
	inline string in_str( const double   & v ) { char _b[64]; sprintf(_b, "%.*e", OP_DBL_Digs-1, v ); return string(_b); }
	inline string in_str( const bool     & v ) { char _b[64]; sprintf(_b, "%d", v ); return string(_b); }
	inline string in_str( const char     & v ) { char _b[64]; sprintf(_b, "%c", v ); return string(_b); }
	inline string in_str( const char     * v ) { return string(v); }
    inline string in_str( const string   & v ) { return v; }

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

    string print_array_to_string( std::vector<int>& arr  , bool include_ids, bool horizontal );
    string print_array_to_string( std::vector<float>& arr, bool include_ids, bool horizontal );
    string print_array_to_string( const int& argc, const char** argv );

}

#endif
