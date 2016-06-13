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
#include <vector>

#include <kortex/keyed_value.h>

using std::vector;

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

    void print_separator( int nsz, const char* ch=NULL );
    void print_arr( const vector<int>& var, const char* pretag=NULL, const char* posttag=NULL, bool detailed=false );
    void print_mat( const vector<float>& mat, int nr, int nc, const char* pretag=NULL  );
    void print( const vector<iint>& arr );

    void display_similarity_matrix( const vector<int>& dmatrix, int scale );
    void   print_similarity_matrix( const vector<int>& dmatrix, int scale );

}

#endif
