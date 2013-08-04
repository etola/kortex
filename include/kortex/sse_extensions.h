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
#ifdef  WITH_SSE

#ifndef KORTEX_SSE_EXTENSIONS_H
#define KORTEX_SSE_EXTENSIONS_H

#include <xmmintrin.h>
#include <emmintrin.h>

namespace kortex {

    typedef union F128 {
        __m128 pack;
        float f[4];
    } F128;

    float sse_dot_128a( const float* a, const float* b );
    void  sse_mat_vec_mul_128a( const float* A, const float* b, float* c );

    /// returns sum( a^2 )
    float sse_sq_sum_128u(const float* a);
    float sse_sq_sum_128a(const float* a);
    float sse_sq_sum_20a (const float* a);
    float sse_sq_sum_a( const float* a, int sz );
    float sse_sq_sum_u( const float* a, int sz );
    float sse_sq_sum  ( const float* a, int sz );

    /// returns sum( (a-b)^2 )
    float sse_sq_sum  ( const float* a, const float* b, int sz );
    float sse_sq_sum_a( const float* a, const float* b, int sz );
    float sse_sq_sum_u( const float* a, const float* b, int sz );

    /// a <= a*v
    void  sse_scale_20a(float* a, float v);
    void  sse_scale  ( float* a, int asz, float v );
    void  sse_scale_u( float* a, int asz, float v );
    void  sse_scale_a( float* a, int asz, float v );

    /// sum a[i]
    float sse_sum  ( const float* a, int sz );
    float sse_sum_a( const float* a, int sz );
    float sse_sum_u( const float* a, int sz );

}

#endif
#endif
