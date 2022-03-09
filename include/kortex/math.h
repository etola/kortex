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
#ifndef KORTEX_MATH_H
#define KORTEX_MATH_H

#include <cmath>
#include <vector>
using std::vector;
#include <set>
using std::set;

#include <algorithm>
#include <numeric>

#include <kortex/defs.h>
#include <kortex/types.h>

namespace kortex {

	template<typename T1, typename T2>
	void convert_array(const std::vector<T1>& sarray, std::vector<T2>& darray) {
		darray = std::vector<T2>(sarray.begin(), sarray.end());
	}

    template<typename T> inline
    void clamp_array( T* array, int asz, const T& vmin, const T& vmax ) {
        std::transform( array, array+asz, array, [=](auto v) { return std::clamp(v,vmin,vmax); } );
    }
    template<typename T> inline
    void clamp_min( T* array, int asz, const T& vmin ) {
        std::transform( array, array+asz, array, [=](auto v) { return (v<vmin) ? vmin : v; } );
    }
    template<typename T> inline
    void clamp_max( T* array, int asz, const T& vmax ) {
        std::transform( array, array+asz, array, [=](auto v) { return (v>vmax) ? vmax : v; } );
    }

    int nearest_int( const float & f );
    int nearest_int( const double& f );

    /// returns integer y such that 2^y >= x
    int cast_to_upper_power_2( int x );

    inline float  sq(const float&  v) { return v*v; }
    inline double sq(const double& v) { return v*v; }
    inline int    sq(const int&    v) { return v*v; }

    inline float  cube(const float&  v) { return v*v*v; }
    inline double cube(const double& v) { return v*v*v; }
    inline int    cube(const int&    v) { return v*v*v; }

    float  dot128( const float* a, const float* b );

    double dot ( const double * a, const double* b, int asz );
    float  dot ( const float  * a, const float * b, int asz );

    template<typename T1, typename T2>
    inline double dot3( const T1* a, const T2* b ) {
        return (double)(a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
    }

    inline bool is_unit_norm_3( const double a[3], double eps = 1e-5 ) {
        if( fabs( dot3(a,a) - 1.0 ) < eps )
            return true;
        return false;
    }

    inline bool is_unit_norm_3( const float  a[3], float  eps = 1e-5f ) {
        if( fabs( dot3(a,a) - 1.0f ) < eps )
            return true;
        return false;
    }

    inline bool is_unit_norm_128( const float a[128], float eps = 1e-5f ) {
        if( fabs( dot128(a,a)-1.0f ) < eps )
            return true;
        return false;
    }

    bool is_unit_norm( const float* a, int asz, float eps = 1e-5f );

    inline float  l2norm_sq3( const float * a ) { return dot3(a,a); }
    inline double l2norm_sq3( const double* a ) { return dot3(a,a); }

    inline float  l2norm_sq3( const float * a, const float * b ) {
        return sq( a[0]-b[0] ) + sq( a[1]-b[1] ) + sq( a[2]-b[2] );
    }
    inline double l2norm_sq3( const double * a, const double * b ) {
        return sq( a[0]-b[0] ) + sq( a[1]-b[1] ) + sq( a[2]-b[2] );
    }
    inline double l2norm_sq3( const float * a, const double * b ) {
        return sq( a[0]-b[0] ) + sq( a[1]-b[1] ) + sq( a[2]-b[2] );
    }


    inline float  l2norm3( const float * a ) { return sqrtf( dot3(a,a) ); }
    inline double l2norm3( const double* a ) { return sqrt ( dot3(a,a) ); }

    inline float  l2norm3( const float * a, const float* b ) {
        return std::sqrt( sq( a[0]-b[0] ) + sq( a[1]-b[1] ) + sq( a[2]-b[2] ) );
    }
    inline double l2norm3( const double * a, const float* b ) {
        return std::sqrt( sq( a[0]-b[0] ) + sq( a[1]-b[1] ) + sq( a[2]-b[2] ) );
    }
    inline double l2norm3( const float * a, const double* b ) {
        return std::sqrt( sq( a[0]-b[0] ) + sq( a[1]-b[1] ) + sq( a[2]-b[2] ) );
    }
    inline double l2norm3( const double * a, const double* b ) {
        return std::sqrt( sq( a[0]-b[0] ) + sq( a[1]-b[1] ) + sq( a[2]-b[2] ) );
    }


    float l2norm_128( const float* a );
    float normalize_l2norm_128(float* arr);

    inline double l2norm( const double& u, const double& v ) {
        return sqrt( u*u + v*v );
    }

    void   scale( float* arr, int asz, float v );
    float  l2norm( const float * a, int asz );
    double l2norm( const double* a, int asz );

    float l2norm( const float* a, const float* b, int asz );
    float normalize_l2norm( float * arr, int asz );

    int l2norm_128_sq( const uchar a[128], const uchar b[128] );

    float  normalize_l2norm3( float * arr );
    double normalize_l2norm3( double* arr );
    double normalize_l2norm3( const double* a, double* na );
    float  normalize_l2norm3( const float * a, float * na );

    void cross3(const float * a, const float * b, float * c );
    void cross3(const double* a, const double* b, double* c );
    bool cross3_normalized( const float * a, const float * b, float * c );
    bool cross3_normalized( const double* a, const double* b, double* c );

    void gaussian_1d( float* fltr, const int& fsz, const float& mean, const float& sigma );

    // assuming p(x) = c0 + c1 x + c2 x^2 + ...
    bool find_real_roots_of_polynomial( const vector<double>& coeffs, vector<double>& real_roots );

    int absmax( const double* arr, int narr, double &mn );

    // fits a second order polynomial to the point pairs (xp,yp), (xc,yc),
    // (xa,ya) points and returns the value of the x-coordinate where the (min
    // or max) peak occurs.
    double optimize_peak( double xp, double xc, double xa, double yp, double yc, double ya );

    void compute_covariance_3( const double* Xs, int m, int n, double cov[9] );


    template <typename T> inline int sign(T v) { return (T(0) < v) - (v<T(0)); }

    inline void boolean_to_index_array( const vector<bool>& bool_array, vector<int>& index_array ) {
        index_array.clear();
        for( int i=0; i<(int)bool_array.size(); ++i ) {
            if( !bool_array[i] ) continue;
            index_array.push_back(i);
        }
    }
    template<typename T> inline
    void set_to_vector( const std::set<T>& a_set, vector<T>& a_vector ) {
        a_vector.insert( a_vector.end(), a_set.begin(), a_set.end() );
    }
    template<typename T> inline
    void vector_to_set( const std::vector<T>& a_vector, std::set<T>& a_set ) {
        a_set.insert( a_vector.begin(), a_vector.end() );
    }


    template<typename T1, typename T2> inline
    double angle_between_in_dot( const T1* n0, const T2* n1 ) {
        return std::max( T1(-1.0), std::min( (T1)dot3(n0,n1), T1(1.0) ) );
    }

    template<typename T1, typename T2> inline
    double angle_between( const T1* n0, const T2* n1 ) {
        double dp =  angle_between_in_dot(n0,n1);
        return acos(dp) * DEGREES;
    }

    template<typename T1, typename T2> inline
    double sym_angle_between_in_dot( const T1* n0, const T2* n1 ) {
        return std::min( (T1)fabs(dot3(n0,n1)), T1(1.0) );
    }

    template<typename T1, typename T2> inline
    double sym_angle_between( const T1* n0, const T2* n1 ) {
        double dp =  sym_angle_between_in_dot(n0,n1);
        return acos(dp) * DEGREES;
    }

    template<typename T> inline
    void make_unique( vector<T>& arr ) {
        std::sort(arr.begin(), arr.end());
        auto last = std::unique( arr.begin(), arr.end() );
        arr.erase(last, arr.end());
    }

    template<typename T> inline
    bool is_unique( const vector<T>& arr ) {
        vector<T> tarr = arr;
        kortex::make_unique(tarr);
        return bool(tarr.size() == arr.size());
    }

    int get_closest_sample_index( const vector<float>& samples, const float& val );

    void fit_gaussian( const vector<float>& arr, float& mean, float& sigma );

}

#endif
