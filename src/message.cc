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

#include <cmath>
#include <kortex/check.h>
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
    void print_arr( const vector<int>& var, const char* pretag, const char* posttag, bool detailed ) {
        static const int bufsz = 2560;
        char buf[bufsz];
        int nstr = 0;
        if( pretag  ) nstr = sprintf( buf+nstr, "%s ",  pretag  );
        for( unsigned i=0; i<var.size(); i++ )
            nstr += sprintf( buf+nstr, "%d ", var[i] );
        if( posttag ) nstr += sprintf( buf+nstr, "%s", posttag );
        assert_statement( nstr <= bufsz, "buffer overflow" );

        if( detailed )
            logman_log( buf );
        else
            printf( "%s\n", buf );
    }

    void print( const vector<iint>& arr ) {
        for( int i=0; i<(int)arr.size(); i++ ) {
            printf( "% 3d % 4d\n", arr[i].id, arr[i].val );
        }
        printf( "\n" );
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

    void display_similarity_matrix( const vector<int>& dmatrix, int scale ) {

        int nf = sqrt(dmatrix.size());
        assert_statement( nf*nf == (int)dmatrix.size(), "invalid matrix data" );

        const int interval = 10;

        printf("%4s|", "-" );
        for( int i=0; i<nf; i++ ) {
            if( i!=0 && i%interval==0 ) printf(" |%d|", i/10 );
            printf(" %d",i%10);
        }
        printf("\n");

        for( int i=0; i<nf; i++ ) {
            if( i%interval==0 && i!=0 ) {
                printf("|% 3d|", i );
                for( int j=0; j<nf; j++ ) {
                    if( j%interval==0 && j!=0 ) printf("  | ");
                    printf("--");
                }
                printf("\n");
            }

            printf("% 4d|", i%10);
            for( int j=0; j<nf; j++ ) {
                if( j%interval==0 && j!=0 ) printf("  | ");

                if( i == j ) {
                    printf( " ." );
                } else {
                    int v = std::min( 9,dmatrix[ i*nf+j ]/scale );
                    if( v ) printf( " %d", v );
                    else    printf( " ." );
                }
            }
            printf("|\n");
        }
        printf("\n");
    }

    void print_similarity_matrix( const vector<int>& dmatrix, int scale ) {

        int nf = sqrt(dmatrix.size());
        assert_statement( nf > 0, "empty matrix" );
        assert_statement( nf*nf == (int)dmatrix.size(), "matrix has to be square" );

        const int interval = 10;

        static const int bufsz = 2560;
        int  nstr = 0;
        char buf[bufsz];

        nstr += sprintf( buf+nstr, "%4s|", "-" );
        for( int i=0; i<nf; i++ ) {
            if( i!=0 && i%interval==0 ) nstr += sprintf( buf+nstr, " |%d|", i/10 );
            nstr += sprintf( buf+nstr, " %d",i%10 );
        }
        logman_log( buf );

        for( int i=0; i<nf; i++ ) {
            nstr = 0;
            if( i%interval==0 && i!=0 ) {
                nstr += sprintf( buf+nstr, "|% 3d|", i );
                for( int j=0; j<nf; j++ ) {
                    if( j%interval==0 && j!=0 ) nstr += sprintf( buf+nstr, "  | " );
                    nstr += sprintf( buf+nstr, "--" );
                }
                logman_log( buf );
            }

            nstr = 0;
            nstr += sprintf( buf, "% 4d|", i%10 );
            for( int j=0; j<nf; j++ ) {
                if( j%interval==0 && j!=0 ) nstr += sprintf( buf+nstr, "  | " );

                if( i == j ) {
                    nstr += sprintf( buf+nstr, " ." );
                } else {
                    int v = std::min( 9,dmatrix[ i*nf+j ]/scale );
                    if( v ) nstr += sprintf( buf+nstr, " %d",v );
                    else    nstr += sprintf( buf+nstr, " ."  );
                }
            }
            nstr += sprintf( buf+nstr, "|");
            logman_log( buf );
        }
        logman_log( "" );
    }


}
