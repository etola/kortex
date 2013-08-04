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

}

