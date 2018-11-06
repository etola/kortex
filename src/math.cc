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
#include <kortex/matrix.h>
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

    /// returns integer y such that 2^y >= x
    int cast_to_upper_power_2( int x ) {
        int log2y = int(log2(x));
        int y = 1<<log2y;
        if( y < x )
            y = y<<1;
        return y;
    }


    bool is_unit_norm( const float* a, int asz, float eps ) {
        assert_pointer( a );
        assert_pointer_size( asz );
        if( fabs( dot(a, a, asz) - 1.0f ) < eps )
            return true;
        return false;
    }

    float  dot128( const float* a, const float* b ) {
#ifdef WITH_SSE
        float s = sse_dot_128(a,b);
#else
        float s = 0.0f;
        for( int i=0; i<128; i++ )
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
        double one_over_norm_c = 1.0 / std::sqrt( d );
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
        float one_over_norm_c = 1.0f / std::sqrt(d); // inv_sqrt( d );
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

    float  normalize_l2norm3( const float* arr, float* na ) {
        assert_pointer( arr );
        float nrm = l2norm3( arr );
        if( nrm < NRM_EPS ) {
            memset( na, 0, sizeof(*arr)*3 );
            return nrm;
        }
        float inrm = 1.0f/nrm;
        for( int p=0; p<3; p++ )
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
        for( int k=0; k<asz; k++ )
            nrm += sq(a[k]);
        return sqrt(nrm);
    }

    float l2norm ( const float* a, int asz ) {
#ifdef WITH_SSE
        return std::sqrt( sse_sq_sum(a, asz) );
#else
        float nrm = 0.0f;
        for( int k=0; k<asz; k++ ) {
            nrm += *a * *a;
            a++;
        }
        return std::sqrt(nrm);
#endif
    }

    float l2norm_128( const float* a ) {
        assert_pointer( a );
#ifdef WITH_SSE
        if( is_16_byte_aligned(a) ) return std::sqrt( sse_sq_sum_128a(a) );
        else                        return std::sqrt( sse_sq_sum_128u(a) );
        passert_statement( 0, "should not have reached here" );
#else
        float nrm = 0.0f;
        for( int k=0; k<128; k++ ) {
            nrm += *a * *a;
            a++;
        }
        return std::sqrt(nrm);
#endif
    }

    int l2norm_128_sq( const uchar a[128], const uchar b[128] ) {
        int s = 0;
        int d0,d1,d2,d3;
        for( int i=0; i<32; i++ ) {
            d0 = a[0] - b[0];
            d1 = a[1] - b[1];
            d2 = a[2] - b[2];
            d3 = a[3] - b[3];
            a += 4;
            b += 4;
            s += d0*d0 + d1*d1 + d2*d2 + d3*d3;
        }
        return s;
    }

    float l2norm( const float* a, const float* b, int asz ) {
        assert_pointer( a && b );
        assert_pointer_size( asz );
#ifdef WITH_SSE
        return std::sqrt( sse_sq_sum(a,b,asz) );
#else
        float nrm = 0.0f;
        for( int k=0; k<asz; k++ ) {
            nrm += sq( *a - *b );
            a++;
            b++;
        }
        return std::sqrt(nrm);
#endif
    }

    float normalize_l2norm_128(float* arr) {
        assert_pointer( arr );
        float nrm = l2norm_128(arr);
        if( nrm < NRM_EPS ) {
            memset( arr, 0, sizeof(*arr)*128 );
            return nrm;
        }
        float inrm = 1.0f/nrm;
        for(int p=0; p<128; p++ )
            arr[p] *= inrm;
        return nrm;
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
        float sum = 0.0f;
        float v = 2*sigma*sigma;
        for( int x=-sz; x<=sz; x++ ) {
            counter++;
            fltr[counter] = std::exp( -sq(static_cast<float>(x)-mean)/v );
            sum += fltr[counter];
        }
        if( sum != 0.0f ) {
            float isum = 1.0f/sum;
            for( int x=0; x<fsz; x++ )
                fltr[x] *= isum;
        }
    }


    // assuming p(x) = c0 + c1 x + c2 x^2 + ...
    bool find_real_roots_of_polynomial( const vector<double>& coeffs, vector<double>& real_roots ) {
        real_roots.clear();
        int csz = (int)coeffs.size();
        if( csz <= 1 ) {
            return false;
        } else if( csz == 2 ) {
            real_roots.push_back( -coeffs[0]/coeffs[1] );
            return true;
        }
        int dim = csz - 1;
        const static int n_pre_dim = 10;
        passert_statement( dim < n_pre_dim, "non enough static memory allocated" );

        double D[ n_pre_dim * n_pre_dim ];
        memset( D, 0, sizeof(*D)*n_pre_dim*n_pre_dim );

        for( int i=0; i<dim; i++ ) {
            if( i != dim-1 )
                D[ (i+1)*dim+i ] = 1.0;
            D[i] = -coeffs[dim-1-i]/coeffs[dim];
        }

        vector<double> eig_real, eig_imag;
        bool rval = find_eigenvalues( D, dim, eig_real, eig_imag );

        if( !rval ) {
            return false;
        } else {
            for( int i=0; i<dim; i++ ) {
                if( fabs( eig_imag[i] ) < DBL_MIN )
                    real_roots.push_back( eig_real[i] );
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

    // fits a second order polynomial to the point pairs (xp,yp), (xc,yc),
    // (xa,ya) points and returns the value of the x-coordinate where the (min
    // or max) peak occurs.
    double optimize_peak( double xp, double xc, double xa, double yp, double yc, double ya ) {
        if( fabs(yp-yc)<1e-10 && fabs(ya-yc)<1e-10 ) {
            return (xp+xc+xa)/3.0;
        } else {
            double A[] = {xp*xp, xp, 1, xc*xc, xc, 1, xa*xa, xa, 1};
            double iA[9];
            if( !mat_inv_3(A,3,iA,3,1e-30) ) {
                return xc;
            } else {
                double m[] = {yp, yc, ya};
                double a = dot3(iA,   m);
                double b = dot3(iA+3, m);
                if( fabs(a)>1e-10 ) return -b/(2.0*a);
                else                return xc;
            }
        }
    }

    void compute_covariance_3( const double* Xs, int m, int n, double cov[9] ) {
        assert_statement( n==3, "sample dimension has to be 3" );
        assert_statement( m> 3, "not enough samples provided" );

        double x_mean[3] = {0.0, 0.0, 0.0};

        for( int i=0; i<m; i++ ) {
            const double* X = Xs + 3*i;
            x_mean[0] += X[0];
            x_mean[1] += X[1];
            x_mean[2] += X[2];
        }
        x_mean[0] /= m;
        x_mean[1] /= m;
        x_mean[2] /= m;

        double xn[3];
        double xout[9];
        memset( cov, 0, sizeof(*cov)*9 );
        for( int i=0; i<m; i++ ) {
            const double* X = Xs + 3*i;
            xn[0] = X[0] - x_mean[0];
            xn[1] = X[1] - x_mean[1];
            xn[2] = X[2] - x_mean[2];
            mat_mat_trans( xn, 3, 1, xn, 3, 1, xout, 9 );
            for( int j=0; j<9; j++ ) cov[j] += xout[j];
        }

        for( int j=0; j<9; j++ )
            cov[j] /= (m-1);
    }

    int get_closest_sample_index( const vector<float>& samples, const float& val ) {
        passert_statement( samples.size() > 0, "invalid input" );
        int   idx  = 0;
        float dist = fabs( samples[0] - val );
        for( size_t i=1; i<samples.size(); i++ ) {
            float d = fabs( samples[i]-val );
            if( d > dist ) continue;
            dist = d;
            idx  = i;
        }
        return idx;
    }

}
