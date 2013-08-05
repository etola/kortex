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
#ifdef  WITH_SSE

#include <kortex/check.h>
#include <kortex/sse_extensions.h>

namespace kortex {

    float sse_dot_128a( const float* a, const float* b ) {
        assert_statement( (is_16_byte_aligned(a) && is_16_byte_aligned(b)),
                          "both arrays must be 16-byte aligned - call sse128u_dot instead" );
        __m128 xmm_a, xmm_b;
        F128   xmm_s;
        int j;
        xmm_s.pack = _mm_set1_ps(0.0);
        for( j=0; j<128; j+=4 ) {
            xmm_a = _mm_load_ps(a+j);
            xmm_b = _mm_load_ps(b+j);
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a,xmm_b));
        }
        return xmm_s.f[0]+xmm_s.f[1]+xmm_s.f[2]+xmm_s.f[3];
    }
    /// A 128x128 : b 128x1
    void sse_mat_vec_mul_128a( const float* A, const float* b, float* c ) {
        assert_pointer( A && b && c );
        assert_statement( (is_16_byte_aligned(A) && is_16_byte_aligned(b)),
                          "both arrays must be 16-byte aligned - call sse128u_dot instead" );
        for( int r=0; r<128; r++ )
            c[r] = sse_dot_128a(A+r*128, b);
    }

    float sse_sq_sum_128a(const float* a) {
        assert_statement( is_16_byte_aligned(a), "array must be 16-byte aligned - call sse128u_sq_sum instead" );
        __m128 xmm_a;
        F128   xmm_s;
        int j;
        xmm_s.pack = _mm_set1_ps(0.0);
        for( j=0; j<128; j+=4 ) {
            xmm_a = _mm_load_ps(a+j);
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a,xmm_a));
        }
        return xmm_s.f[0]+xmm_s.f[1]+xmm_s.f[2]+xmm_s.f[3];
    }

    float sse_sq_sum_128u(const float* a) {
        assert_statement( !is_16_byte_aligned(a), "array is 16-byte aligned - call sse128a_sq_sum instead" );
        __m128 xmm_a;
        F128   xmm_s;
        int j;
        xmm_s.pack = _mm_set1_ps(0.0);
        for( j=0; j<128; j+=4 ) {
            xmm_a = _mm_loadu_ps(a+j);
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a,xmm_a));
        }
        return xmm_s.f[0]+xmm_s.f[1]+xmm_s.f[2]+xmm_s.f[3];
    }

    float sse_sq_sum_u( const float* a, int sz ) {
        int ksimdlen = sz/4*4;
        __m128 xmm_a;
        F128   xmm_s;
        int j;
        xmm_s.pack = _mm_set1_ps(0.0);
        for( j=0; j<ksimdlen; j+=4 ) {
            xmm_a = _mm_loadu_ps((const float*)(a+j));
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a,xmm_a));
        }
        float sum = xmm_s.f[0]+xmm_s.f[1]+xmm_s.f[2]+xmm_s.f[3];
        for(; j<sz; j++ )
            sum += a[j]*a[j];
        return sum;
    }

    float sse_sq_sum_a( const float* a, int sz ) {
        int ksimdlen = sz/4*4;
        __m128 xmm_a;
        F128   xmm_s;
        int j;
        xmm_s.pack = _mm_set1_ps(0.0);
        for( j=0; j<ksimdlen; j+=4 ) {
            xmm_a = _mm_load_ps((const float*)(a+j));
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a,xmm_a));
        }
        float sum = xmm_s.f[0]+xmm_s.f[1]+xmm_s.f[2]+xmm_s.f[3];
        for(; j<sz; j++ )
            sum += a[j]*a[j];
        return sum;
    }

    float sse_sq_sum( const float* a, int sz ) {
        switch( is_16_byte_aligned(a) ) {
        case true : return sse_sq_sum_a(a,sz); break;
        case false: return sse_sq_sum_u(a,sz); break;
        }
    }





    /// sum_i_{i=0:19} (a_i)^2
    float sse_sq_sum_20a(const float* a) {
        assert_statement( is_16_byte_aligned(a), "a is not 16byte aligned" );
        __m128 xmm_a;
        F128   xmm_s;
        xmm_s.pack = _mm_set1_ps(0.0);
        for(int i=0; i<5; i++) {
            xmm_a  = _mm_load_ps(a + 4*i);
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a,xmm_a));
        }
        return xmm_s.f[0]+xmm_s.f[1]+xmm_s.f[2]+xmm_s.f[3];
    }

    /// a <= a*v
    void sse_scale_20a(float* a, float v) {
        assert_statement( is_16_byte_aligned(a), "a is not 16byte aligned" );
        float v4[4] __attribute__ ((__aligned__(16)));
        for(int i=0; i<4; i++) v4[i] = v;
        __m128 xmm_a, xmm_c;
        __m128 xmm_b = _mm_load_ps(v4);
        for(int i=0; i<5; i++) {
            xmm_a = _mm_load_ps(a + 4*i);
            xmm_c = _mm_mul_ps(xmm_a,xmm_b);
            _mm_store_ps(a + 4*i, xmm_c);
        }
    }

    void  sse_scale_a( float* a, int asz, float v ) {
        assert_statement( is_16_byte_aligned(a), "a is not 16byte aligned" );
        float v4[4] __attribute__ ((__aligned__(16)));
        for( int i=0; i<4; i++ ) v4[i] = v;

        __m128 xmm_a, xmm_c;
        __m128 xmm_b = _mm_load_ps(v4);
        int ksimdlen = asz/4*4;
        int i;
        for( i=0; i<ksimdlen; i+=4 ) {
            xmm_a = _mm_load_ps(a + 4*i);
            xmm_c = _mm_mul_ps(xmm_a,xmm_b);
            _mm_store_ps(a + 4*i, xmm_c);
        }
        for( ; i<asz; i++ )
            a[i] *= v;
    }

    void  sse_scale_u( float* a, int asz, float v ) {
        assert_statement( !is_16_byte_aligned(a), "a is 16byte aligned" );
        float v4[4];
        for( int i=0; i<4; i++ ) v4[i] = v;
        __m128 xmm_a, xmm_c;
        __m128 xmm_b = _mm_loadu_ps(v4);
        int ksimdlen = asz/4*4;
        int i;
        for( i=0; i<ksimdlen; i+=4 ) {
            xmm_a = _mm_loadu_ps(a + 4*i);
            xmm_c = _mm_mul_ps(xmm_a,xmm_b);
            _mm_storeu_ps(a + 4*i, xmm_c);
        }
        for( ; i<asz; i++ )
            a[i] *= v;
    }

    void sse_scale( float* a, int asz, float v ) {
        switch( is_16_byte_aligned(a) ) {
        case true : return sse_scale_a(a,asz,v); break;
        case false: return sse_scale_u(a,asz,v); break;
        }
    }

    float sse_sum( const float* a, int sz ) {
        switch( is_16_byte_aligned(a) ) {
        case true : return sse_sum_a(a,sz); break;
        case false: return sse_sum_u(a,sz); break;
        }
    }

    float sse_sum_u( const float* a, int sz ) {
        assert_pointer( a );
        assert_statement_g( is_positive_number(sz), "[sz %d] must be a positive number", sz );
        assert_statement( !is_16_byte_aligned(a), "pointer is 16byte aligned: call sse_sum_a" );
        int ksimdlen = sz/4*4;
        __m128 xmm_a;
        F128   xmm_s;
        float sum;
        int j;
        xmm_s.pack = _mm_set1_ps(0.0);
        for( j=0; j<ksimdlen; j+=4 ) {
            xmm_a = _mm_loadu_ps((const float*)(a+j));
            xmm_s.pack = _mm_add_ps(xmm_s.pack, xmm_a);
        }
        sum = xmm_s.f[0]+xmm_s.f[1]+xmm_s.f[2]+xmm_s.f[3];
        for(; j<sz; j++ ) sum+=a[j];
        return sum;
    }

    float sse_sum_a( const float* a, int sz ) {
        assert_pointer( a );
        assert_statement_g( is_positive_number(sz), "[sz %d] must be a positive number", sz );
        assert_statement( is_16_byte_aligned(a), "pointer is not 16byte aligned: call sse_sum_u" );
        int ksimdlen = sz/4*4;
        __m128 xmm_a;
        F128   xmm_s;
        float sum;
        int j;
        xmm_s.pack = _mm_set1_ps(0.0);
        for( j=0; j<ksimdlen; j+=4 ) {
            xmm_a = _mm_load_ps((const float*)(a+j));
            xmm_s.pack = _mm_add_ps(xmm_s.pack, xmm_a);
        }
        sum = xmm_s.f[0]+xmm_s.f[1]+xmm_s.f[2]+xmm_s.f[3];
        for(; j<sz; j++ ) sum+=a[j];
        return sum;
    }

    float sse_sq_sum_a(const float* a, const float* b, int sz) {
        assert_pointer( a && b );
        assert_pointer_size( sz );
        assert_statement( is_16_byte_aligned(a) && is_16_byte_aligned(b),
                          "arrays not aligned" );
        int ksimdlen = sz/4*4;
        __m128 xmm_a, xmm_b, xmm_c;
        F128   xmm_s;
        int j;
        xmm_s.pack = _mm_set1_ps(0.0);
        for( j=0; j<ksimdlen; j+=4 ) {
            xmm_a = _mm_load_ps((const float*)(a+j));
            xmm_b = _mm_load_ps((const float*)(b+j));
            xmm_c = _mm_sub_ps(xmm_a,xmm_b);
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_c,xmm_c));
        }
        float sum = xmm_s.f[0]+xmm_s.f[1]+xmm_s.f[2]+xmm_s.f[3];
        float d;
        for(; j<sz; j++ ) {
            d=a[j]-b[j];
            sum+=d*d;
        }
        return sum;
    }

    float sse_sq_sum_u(const float* a, const float* b, int sz) {
        assert_pointer( a && b );
        assert_pointer_size( sz );
        assert_statement( !is_16_byte_aligned(a) && !is_16_byte_aligned(b),
                          "arrays are aligned - call aligned version" );
        int ksimdlen = sz/4*4;
        __m128 xmm_a, xmm_b, xmm_c;
        F128   xmm_s;
        int j;
        xmm_s.pack = _mm_set1_ps(0.0);
        for( j=0; j<ksimdlen; j+=4 ) {
            xmm_a = _mm_loadu_ps((const float*)(a+j));
            xmm_b = _mm_loadu_ps((const float*)(b+j));
            xmm_c = _mm_sub_ps(xmm_a,xmm_b);
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_c,xmm_c));
        }
        float sum = xmm_s.f[0]+xmm_s.f[1]+xmm_s.f[2]+xmm_s.f[3];
        float d;
        for(; j<sz; j++ ) {
            d=a[j]-b[j];
            sum+=d*d;
        }
        return sum;
    }

    float sse_sq_sum( const float* a, const float* b, int sz ) {
        switch( is_16_byte_aligned(a) && is_16_byte_aligned(b) ) {
        case true : return sse_sq_sum_a(a,b,sz); break;
        case false: return sse_sq_sum_u(a,b,sz); break;
        }
    }






}

#endif
