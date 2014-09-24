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
#include <kortex/message.h>
#include "message.tcc"

namespace kortex {

    template void matrix_print(const char* str, const int   * A, int ar, int ac, bool legend, bool print_zeros);
    template void matrix_print(const char* str, const float * A, int ar, int ac, bool legend, bool print_zeros);
    template void matrix_print(const char* str, const double* A, int ar, int ac, bool legend, bool print_zeros);

    void print_separator( int nsz, const char* ch ) {
        char sch = '-';
        if( ch ) sch = *ch;
        for( int i=0; i<nsz; i++ )
            printf("%c",sch);
        printf("\n");
    }
    void print_arr( const vector<int>& var, const char* pretag, const char* posttag ) {
        if( pretag  ) printf( "%s ",  pretag  );
        for( unsigned i=0; i<var.size(); i++ )
            printf("%d ", var[i]);
        if( posttag ) printf( "%s", posttag );
        printf("\n");
    }

    void print_mat( const vector<float>& mat, int nr, int nc, const char* pretag  ) {
        if( pretag ) printf( "%s ", pretag );
        for( int r=0; r<nr; r++ ) {
            for( int c=0; c<nc; c++ ) {
                printf( "% 5.2f ", mat[r*nc+c] );
            }
            printf("\n");
        }
    }


}
