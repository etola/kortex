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
#ifndef KORTEX_MATH_H
#define KORTEX_MATH_H

#include <cmath>

namespace kortex {

    inline float  sq(const float&  v) { return v*v; }
    inline double sq(const double& v) { return v*v; }
    inline int    sq(const int&    v) { return v*v; }

    double dot ( const double * a, const double* b, int asz );

    inline float  dot3( const float * a, const float * b ) {
        return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
    }
    inline double dot3( const double* a, const double* b ) {
        return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
    }
    inline double dot3( const float * a, const double* b ) {
        return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
    }

    inline float  l2norm_sq3( const float * a ) { return dot3(a,a); }
    inline double l2norm_sq3( const double* a ) { return dot3(a,a); }

    inline float  l2norm3( const float * a ) { return sqrt(dot3(a,a)); }
    inline double l2norm3( const double* a ) { return sqrt(dot3(a,a)); }


    float  normalize_l2norm3( float * arr );
    double normalize_l2norm3( double* arr );
    double normalize_l2norm3( const double* a, double* na );

    void cross3(const float * a, const float * b, float * c );
    void cross3(const double* a, const double* b, double* c );
    void cross3_normalized( const float* a, const float* b, float* c );

}

#endif
