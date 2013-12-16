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
#include <kortex/check.h>

namespace kortex {

    string format_function_message(const string& str, const int& number) {
        size_t fbegin = str.find("cosmos::");
        size_t fend   = str.find_first_of("(");
        if( fbegin != std::string::npos )
            fbegin += 7;
        else
            fbegin = 0;
        string str2 = str.substr(fbegin,fend-fbegin);
        char buffer[2560];
        sprintf(buffer, "%s(%d)", str2.c_str(), number);
        return string(buffer);
    }

    void assert_array(const char* name, const float* arr, int asz) {
#ifdef DEBUG
        assert_statement_g( is_positive_number(asz), "[%s] array size is weird [asz %d]", name, asz );
        for(int i=0; i<asz; i++) {
            assert_statement_g( is_a_number(arr[i]), "[%s] array contains NaN", name );
        }
#endif
    }

    void assert_array(const char* name, const double* arr, int asz) {
#ifdef DEBUG
        assert_statement_g( is_positive_number(asz), "[%s] array size is weird [asz %d]", name, asz );
        for(int i=0; i<asz; i++) {
            assert_statement_g( is_a_number(arr[i]), "[%s] array contains NaN", name );
        }
#endif
    }


}

