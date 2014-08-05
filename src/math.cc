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
#include <kortex/sse_extensions.h>
#include <kortex/mem_unit.h>
#include <kortex/linear_algebra.h>

#include <cstring>
#include <cfloat>
#include <climits>

namespace kortex {

    int nearest_int( const float& f ) {
        if     ( f > INT_MAX ) return INT_MAX;
        else if( f < INT_MIN ) return INT_MIN;
        // return (int)( f + 0.49999997f ); // --> works only on positive values
        return (int)rint(f);
    }

    int nearest_int( const double& f ) {
        if     ( f > INT_MAX ) return INT_MAX;
        else if( f < INT_MIN ) return INT_MIN;
        return (int)rint(f);
    }

    bool is_unit_norm( const float* a, int asz, float eps ) {
        assert_pointer( a );
        assert_pointer_size( asz );
        if( fabs( dot(a, a, asz) - 1.0f ) < eps )
            return true;
        return false;
    }

    float  dot128( const float* a, const float* b ) {
        float s = 0.0f;
#ifdef WITH_SSE
        s = sse_dot_128(a,b);
#else
        for( int i=0; i<asz; i++ )
            s += a[i]*b[i];
#endif
        return s;
    }

    double dot( const double * a, const double* b, int asz ) {
        double s = 0.0;
        for( int i=0; i<asz; i++ )
            s += a[i]*b[i];
        return s;
    }
    float  dot( const float  * a, const float * b, int asz ) {
        float  s = 0.0f;
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

    void cross3_normalized( const double* a, const double* b, double* c ) {
        cross3( a, b, c );
        double d = dot3( c, c );
        if( d < 1e-16 ) {
            memset( c, 0, sizeof(*c)* 3 );
            return;
        }
        double one_over_norm_c = 1.0 / sqrt( d );
        passert_statement_g( is_a_number(one_over_norm_c), "[d %f] 1 / dot product resulted in NaN", d );
        c[0] *= one_over_norm_c;
        c[1] *= one_over_norm_c;
        c[2] *= one_over_norm_c;
    }

    void cross3_normalized( const float* a, const float* b, float* c ) {
        cross3(a, b, c);
        float d = dot3(c, c);
        if( d < 1e-16 ) {
            memset( c, 0, sizeof(*c)* 3 );
            return;
        }
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

    void scale( float* arr, int asz, float v ) {
        assert_pointer( arr );
        assert_pointer_size( asz );
        assert_number( v );
#ifdef WITH_SSE
        sse_scale( arr, asz, v );
#else
        for( int i=0; i<asz; i++ )
            arr[i] *= v;
#endif
    }

    double l2norm( const double* a, int asz ) {
        double nrm = 0.0;
        for( int k=0; k<asz; a++ )
            nrm += sq(a[k]);
        return sqrt(nrm);
    }

    float l2norm ( const float* a, int asz ) {
#ifdef WITH_SSE
        return sqrt( sse_sq_sum(a, asz) );
#else
        float nrm = 0.0f;
        for( int k=0; k<asz; k++ ) {
            nrm += *a * *a;
            a++;
        }
        return sqrt(nrm);
#endif
    }

    float  normalize_l2norm( float* arr, int asz ) {
        float nrm = l2norm( arr, asz );
        if( nrm < NRM_EPS ) {
            memset( arr, 0, sizeof(*arr)*asz );
            return nrm;
        }
        scale( arr, asz, 1.0f/nrm );
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


    // assuming p(x) = c0 + c1 x + c2 x^2 + ...
    bool find_real_roots_of_polynomial( const vector<double>& coeffs, vector<double>& real_roots ) {
        real_roots.clear();
        int csz = coeffs.size();
        if( csz <= 1 ) {
            return false;
        } else if( csz == 2 ) {
            real_roots.push_back( -coeffs[0]/coeffs[1] );
            return true;
        }
        int dim = csz - 1;
        const static int n_pre_dim = 10;
        assert_statement( dim < n_pre_dim, "non enough static memory allocated" );
        double D[ n_pre_dim * n_pre_dim ];
        memset( D, 0, sizeof(*D)*n_pre_dim*n_pre_dim );
        double eigs_r[n_pre_dim], eigs_i[n_pre_dim];

        for( int i=0; i<dim; i++ ) {
            if( i != dim-1 )
                D[ (i+1)*dim+i ] = 1.0;

            D[i] = -coeffs[dim-1-i]/coeffs[dim];
        }

        MemUnit mem;
        bool rval = mat_eigenvalues_upper_hessenberg( D, dim, eigs_r, eigs_i, n_pre_dim, mem );
        if( !rval ) {
            return false;
        } else {
            for( int i=0; i<dim; i++ ) {
                if( fabs(eigs_i[i]) < DBL_MIN )
                    real_roots.push_back( eigs_r[i] );
            }
        }

        return true;
    }

    int absmax( const double* arr, int narr, double &mn ) {
        assert_pointer( arr );
        assert_pointer_size( narr );

        mn       = fabs(arr[0]);
        int maxi = 0;
        for( int i=1; i<narr; i++ ) {
            if( fabs(arr[i])>mn ) {
                mn   = fabs(arr[i]);
                maxi = i;
            }
        }
        return maxi;
    }




}
