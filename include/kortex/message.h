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
#ifndef KORTEX_MESSAGE_H
#define KORTEX_MESSAGE_H

#include <iostream>
#include <iomanip>

namespace kortex {

    struct MessageFormat {
        static const int word_width       = 14;
        static const int number_precision =  4;
        static const int number_width     = number_precision + 5; // +2 for . and -
        static const int line_width       = 80;
        static std::ostream& apply(std::ostream& os) {
            os.width(number_width);
            os.precision(number_precision);
            return os;
        }
        static std::ostream& apply(std::ostream& os, std::ios_base::fmtflags flags) {
            os.setf(flags);
            return apply(os);
        }
    };

    template<typename T>
    void matrix_print(const char* str, const T* A, int ar, int ac, bool legend, bool print_zeros);

}

#endif
