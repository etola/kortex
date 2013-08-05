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
#include <kortex/math.h>
#include <kortex/check.h>
#include <kortex/defs.h>

#include <cstring>

namespace kortex {

    double dot( const double * a, const double* b, int asz ) {
        double s = 0.0;
        for( int i=0; i<asz; i++ )
            s += a[i]*b[i];
        return s;
    }

    void cross3(const float * a, const float * b, float * c ) {
        assert_pointer( a && b && c );
        assert_noalias_p( a, c );
        assert_noalias_p( b, c );
        c[0] = a[1]*b[2] - a[2]*b[1];
        c[1] = a[2]*b[0] - a[0]*b[2];
        c[2] = a[0]*b[1] - a[1]*b[0];
    }
    void cross3(const double* a, const double* b, double* c ) {
        assert_pointer( a && b && c );
        assert_noalias_p( a, c );
        assert_noalias_p( b, c );
        c[0] = a[1]*b[2] - a[2]*b[1];
        c[1] = a[2]*b[0] - a[0]*b[2];
        c[2] = a[0]*b[1] - a[1]*b[0];
    }

    void cross3_normalized( const float* a, const float* b, float* c ) {
        cross3(a, b, c);
        float d = dot3(c, c);
        float one_over_norm_c = 1.0f / sqrt(d); // inv_sqrt( d );
        passert_statement_g( is_a_number(one_over_norm_c), "[d %f] 1 / dot product resulted in NaN", d );
        c[0] *= one_over_norm_c;
        c[1] *= one_over_norm_c;
        c[2] *= one_over_norm_c;
    }

    float normalize_l2norm3( float* arr ) {
        assert_pointer( arr );
        float nrm = l2norm3( arr );
        if( nrm < NRM_EPS ) {
            memset( arr, 0, sizeof(*arr)*3 );
            return nrm;
        }
        float inrm = 1.0f/nrm;
        for(int p=0; p<3; p++ )
            arr[p] *= inrm;
        return nrm;
    }
    double normalize_l2norm3( double* arr ) {
        assert_pointer( arr );
        double nrm = l2norm3( arr );
        if( nrm < NRM_EPS ) {
            memset( arr, 0, sizeof(*arr)*3 );
            return nrm;
        }
        double inrm = 1.0/nrm;
        for(int p=0; p<3; p++ )
            arr[p] *= inrm;
        return nrm;
    }
    double normalize_l2norm3( const double* arr, double* na ) {
        assert_pointer( arr );
        double nrm = l2norm3( arr );
        if( nrm < NRM_EPS ) {
            memset( na, 0, sizeof(*arr)*3 );
            return nrm;
        }
        double inrm = 1.0/nrm;
        for(int p=0; p<3; p++ )
            na[p] = inrm * arr[p];
        return nrm;
    }

    void gaussian_1d(float* fltr, const int& fsz, const float& mean, const float& sigma) {
        passert_pointer( fltr );
        passert_pointer_size( fsz );
        passert_statement( sigma > 0.0f, "sigma should be positive" );
        int sz = (fsz-1)/2;
        int counter=-1;
        float sum = 0.0;
        float v = 2*sigma*sigma;
        for( int x=-sz; x<=sz; x++ ) {
            counter++;
            fltr[counter] = exp((-(x-mean)*(x-mean))/v);
            sum += fltr[counter];
        }
        if( sum != 0 ) {
            float isum = 1/sum;
            for( int x=0; x<fsz; x++ )
                fltr[x] *= isum;
        }
    }




}
