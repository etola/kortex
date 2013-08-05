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
#include <kortex/filter.h>
#include <kortex/sse_extensions.h>
#include <kortex/mem_manager.h>
#include <kortex/defs.h>

#include <cstring>

namespace kortex {

    // !!
    // !! do not use these temp filters directly - use them through filter horizontal
    // !!

    void filter_buffer_03_basic(float* buffer, const int& bsz, const float* kernel) {
        for( int i=0; i<bsz; i++ ) {
            float* bi = buffer+i;
            buffer[i] = bi[0]*kernel[0] + bi[1]*kernel[1] + bi[2]*kernel[2];
        }
    }
    void filter_buffer_05_basic(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            bi[0] = bi[0]*kernel[0] + bi[1]*kernel[1] + bi[2]*kernel[2] +
                bi[3]*kernel[3] + bi[4]*kernel[4];
        }
    }
    void filter_buffer_07_basic(float* buffer, const int& bsz, const float* kernel) {
        float *bi = NULL;
        for( int i=0; i<bsz; i++ )  {
            bi = buffer+i;
            bi[0] = bi[0]*kernel[0] + bi[1]*kernel[1] + bi[2]*kernel[2] + bi[3]*kernel[3] +
                bi[4]*kernel[4] + bi[5]*kernel[5] + bi[6]*kernel[6];
        }
    }
    void filter_buffer_09_basic(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            bi[0] =
                bi[ 0]*kernel[ 0] + bi[ 1]*kernel[ 1] + bi[ 2]*kernel[ 2] + bi[ 3]*kernel[ 3] +
                bi[ 4]*kernel[ 4] + bi[ 5]*kernel[ 5] + bi[ 6]*kernel[ 6] + bi[ 7]*kernel[ 7] +
                bi[ 8]*kernel[ 8];
        }
    }
    void filter_buffer_11_basic(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            bi[0] =
                bi[ 0]*kernel[ 0] + bi[ 1]*kernel[ 1] + bi[ 2]*kernel[ 2] + bi[ 3]*kernel[ 3] +
                bi[ 4]*kernel[ 4] + bi[ 5]*kernel[ 5] + bi[ 6]*kernel[ 6] + bi[ 7]*kernel[ 7] +
                bi[ 8]*kernel[ 8] + bi[ 9]*kernel[ 9] + bi[10]*kernel[10];
        }
    }
    void filter_buffer_13_basic(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            bi[0] =
                bi[ 0]*kernel[ 0] + bi[ 1]*kernel[ 1] + bi[ 2]*kernel[ 2] +
                bi[ 3]*kernel[ 3] + bi[ 4]*kernel[ 4] + bi[ 5]*kernel[ 5] +
                bi[ 6]*kernel[ 6] + bi[ 7]*kernel[ 7] + bi[ 8]*kernel[ 8] +
                bi[ 9]*kernel[ 9] + bi[10]*kernel[10] + bi[11]*kernel[11] +
                bi[12]*kernel[12];
        }
    }
    void filter_buffer_15_basic(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            bi[0] =
                bi[ 0]*kernel[ 0] + bi[ 1]*kernel[ 1] + bi[ 2]*kernel[ 2] +
                bi[ 3]*kernel[ 3] + bi[ 4]*kernel[ 4] + bi[ 5]*kernel[ 5] +
                bi[ 6]*kernel[ 6] + bi[ 7]*kernel[ 7] + bi[ 8]*kernel[ 8] +
                bi[ 9]*kernel[ 9] + bi[10]*kernel[10] + bi[11]*kernel[11] +
                bi[12]*kernel[12] + bi[13]*kernel[13] + bi[14]*kernel[14];
        }
    }
    void filter_buffer_g_basic (float* buffer, const int& bsz, const float* kernel, const int& ksize) {
        int i, j;
        float sum;
        float* bij = NULL;
        for( i=0; i<bsz; i++ ) {
            sum = 0.0;
            for( j=0; j<ksize-5; j+=5 ) {
                bij = buffer+i+j;
                sum += bij[0]*kernel[j  ] + bij[1]*kernel[j+1] + bij[2]*kernel[j+2]
                    +   bij[3]*kernel[j+3] + bij[4]*kernel[j+4];
            }
            for( ; j<ksize; j++ ) sum += buffer[i+j]*kernel[j];
            buffer[i]=sum;
        }
    }

    void filter_buffer_basic   (float* buffer, const int& bsz, const float* kernel, const int& ksize) {
        switch(ksize) {
        case  3: filter_buffer_03_basic(buffer,bsz,kernel); return;
        case  5: filter_buffer_05_basic(buffer,bsz,kernel); return;
        case  7: filter_buffer_07_basic(buffer,bsz,kernel); return;
        case  9: filter_buffer_09_basic(buffer,bsz,kernel); return;
        case 11: filter_buffer_11_basic(buffer,bsz,kernel); return;
        case 13: filter_buffer_13_basic(buffer,bsz,kernel); return;
        case 15: filter_buffer_15_basic(buffer,bsz,kernel); return;
        default: filter_buffer_g_basic (buffer,bsz,kernel,ksize);
            return;
        }
    }



#ifdef KORTEX_WITH_SSE
    void filter_buffer_05_sse_u(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            xmm_a = _mm_loadu_ps(bi);
            xmm_b = _mm_loadu_ps(kernel);
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] + bi[4]*kernel[4];
        }
    }
    void filter_buffer_05_sse_a(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            xmm_a = _mm_loadu_ps(bi);
            xmm_b = _mm_load_ps(kernel);
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] + bi[4]*kernel[4];
        }
    }
    void filter_buffer_07_sse_u(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            xmm_a = _mm_loadu_ps(bi);
            xmm_b = _mm_loadu_ps(kernel);
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] +
                bi[4]*kernel[4] + bi[5]*kernel[5] + bi[6]*kernel[6];
        }
    }
    void filter_buffer_07_sse_a(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            xmm_a = _mm_loadu_ps(bi);
            xmm_b = _mm_load_ps(kernel);
            xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] +
                bi[4]*kernel[4] + bi[5]*kernel[5] + bi[6]*kernel[6];
        }
    }
    void filter_buffer_09_sse_u(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            for( int j=0; j<8; j+=4 ) {
                xmm_a = _mm_loadu_ps(bi+j);
                xmm_b = _mm_loadu_ps(kernel+j);
                xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            }
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] + bi[8]*kernel[8];
        }
    }
    void filter_buffer_09_sse_a(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            for( int j=0; j<8; j+=4 ) {
                xmm_a = _mm_loadu_ps(bi+j);
                xmm_b = _mm_load_ps(kernel+j);
                xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            }
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] + bi[8]*kernel[8];
        }
    }
    void filter_buffer_11_sse_u(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            for( int j=0; j<8; j+=4 ) {
                xmm_a = _mm_loadu_ps(bi+j);
                xmm_b = _mm_loadu_ps(kernel+j);
                xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            }
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] +
                bi[8]*kernel[8] + bi[9]*kernel[9] + bi[10]*kernel[10];
        }
    }
    void filter_buffer_11_sse_a(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            for( int j=0; j<8; j+=4 ) {
                xmm_a = _mm_loadu_ps(bi+j);
                xmm_b = _mm_load_ps(kernel+j);
                xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            }
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] +
                bi[8]*kernel[8] + bi[9]*kernel[9] + bi[10]*kernel[10];
        }
    }
    void filter_buffer_13_sse_u(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            for( int j=0; j<12; j+=4 ) {
                xmm_a = _mm_loadu_ps(bi+j);
                xmm_b = _mm_loadu_ps(kernel+j);
                xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            }
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] +
                bi[12]*kernel[12];
        }
    }
    void filter_buffer_13_sse_a(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            for( int j=0; j<12; j+=4 ) {
                xmm_a = _mm_loadu_ps(bi+j);
                xmm_b = _mm_load_ps (kernel+j);
                xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            }
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] +
                bi[12]*kernel[12];
        }
    }
    void filter_buffer_15_sse_u(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            for( int j=0; j<12; j+=4 ) {
                xmm_a = _mm_loadu_ps(bi+j    );
                xmm_b = _mm_loadu_ps(kernel+j);
                xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            }
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] +
                bi[12]*kernel[12] + bi[13]*kernel[13] + bi[14]*kernel[14];
        }
    }
    void filter_buffer_15_sse_a(float* buffer, const int& bsz, const float* kernel) {
        float* bi = NULL;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        for( int i=0; i<bsz; i++ ) {
            bi = buffer+i;
            xmm_s.pack = _mm_set1_ps(0.0);
            for( int j=0; j<12; j+=4 ) {
                xmm_a = _mm_loadu_ps(bi+j    );
                xmm_b = _mm_load_ps (kernel+j);
                xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            }
            bi[0] = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3] +
                bi[12]*kernel[12] + bi[13]*kernel[13] + bi[14]*kernel[14];
        }
    }
    void filter_buffer_g_sse_u (float* buffer, const int& bsz, const float* kernel, const int& ksize) {
        int ksimdlen = ksize/4*4;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        float sum;
        int i, j;
        for(i=0; i<bsz; i++ ) {
            xmm_s.pack = _mm_set1_ps(0.0);
            for(j=0; j<ksimdlen; j+=4 ) {
                xmm_a = _mm_loadu_ps(buffer+i+j);
                xmm_b = _mm_loadu_ps(kernel+j  );
                xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            }
            sum = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3];
            for(; j<ksize; j++) sum += buffer[i+j] * kernel[j];
            buffer[i] = sum;
        }
    }
    void filter_buffer_g_sse_a (float* buffer, const int& bsz, const float* kernel, const int& ksize) {
        int ksimdlen = ksize/4*4;
        __m128 xmm_a, xmm_b;
        F128 xmm_s;
        float sum;
        int i, j;
        for( i=0; i<bsz; i++ ) {
            xmm_s.pack = _mm_set1_ps(0.0);
            for(j=0; j<ksimdlen; j+=4 ) {
                xmm_a = _mm_loadu_ps(buffer+i+j);
                xmm_b = _mm_load_ps (kernel+j  );
                xmm_s.pack = _mm_add_ps(xmm_s.pack, _mm_mul_ps(xmm_a, xmm_b));
            }
            sum = xmm_s.f[0] + xmm_s.f[1] + xmm_s.f[2] + xmm_s.f[3];
            for(; j<ksize; j++) sum += buffer[i+j] * kernel[j];
            buffer[i] = sum;
        }
    }
    void filter_buffer_sse_u   (float* buffer, const int& bsz, const float* kernel, const int& ksize) {
        switch(ksize) {
        case  3: filter_buffer_03_basic(buffer,bsz,kernel); return;
        case  5: filter_buffer_05_sse_u(buffer,bsz,kernel); return;
        case  7: filter_buffer_07_sse_u(buffer,bsz,kernel); return;
        case  9: filter_buffer_09_sse_u(buffer,bsz,kernel); return;
        case 11: filter_buffer_11_sse_u(buffer,bsz,kernel); return;
        case 13: filter_buffer_13_sse_u(buffer,bsz,kernel); return;
        case 15: filter_buffer_15_sse_u(buffer,bsz,kernel); return;
        default: filter_buffer_g_sse_u(buffer,bsz,kernel,ksize);
            return;
        }
    }
    void filter_buffer_sse_a   (float* buffer, const int& bsz, const float* kernel, const int& ksize) {
        switch(ksize) {
        case  3: filter_buffer_03_basic(buffer,bsz,kernel); return;
        case  5: filter_buffer_05_sse_a(buffer,bsz,kernel); return;
        case  7: filter_buffer_07_sse_a(buffer,bsz,kernel); return;
        case  9: filter_buffer_09_sse_a(buffer,bsz,kernel); return;
        case 11: filter_buffer_11_sse_a(buffer,bsz,kernel); return;
        case 13: filter_buffer_13_sse_a(buffer,bsz,kernel); return;
        case 15: filter_buffer_15_sse_a(buffer,bsz,kernel); return;
        default: filter_buffer_g_sse_a(buffer,bsz,kernel,ksize);
            return;
        }
    }
#endif


    void filter_buffer(float* buffer, const int& bsz, const float* kernel, const int& ksize, const MemoryMode& mode) {
        // buffer should be padded with +-ksize -- see filter_horizontal for
        // example use.
#ifdef KORTEX_WITH_SSE
        switch( mode ) {
        case MM_16_ALIGNED  : filter_buffer_sse_a(buffer, bsz, kernel, ksize); break;
        case MM_16_UNALIGNED: filter_buffer_sse_u(buffer, bsz, kernel, ksize); break;
        default:              logman_fatal("something is wrong here"); break;
        }
#else
        filter_buffer_basic(buffer, bsz, kernel, ksize);
#endif
    }



    /// filling the halfsize regions with 0 --> otherwise blending produces saturated results
    void filter_hor(const float* im, const int& w, const int& h, const float* kernel, const int& ksize,
                    float* out) {
        float buffer[MAX_IMAGE_DIM];
        passert_statement( w+ksize < MAX_IMAGE_DIM, "w+ksize is larger than max buffer size" );
        int halfsize = ksize / 2;
        MemoryMode opmode = get_alignment(kernel); // buffer is already loaded with mm_loadu
        for( int r=0; r<h; r++ ) {
            int rw = r*w;
            memset( buffer,            0,     sizeof(*buffer)*halfsize );
            memcpy( buffer+halfsize,   im+rw, sizeof(*im)*w            );
            memset( buffer+halfsize+w, 0,     sizeof(*buffer)*halfsize );
            filter_buffer(buffer, w, kernel, ksize, opmode);
            memcpy(out+rw, buffer, w*sizeof(*im));
        }
    }


    void filter_hor_par( const float* im, const int& w, const int& h, const float* kernel, const int& ksize,
                         float* out ) {
        passert_statement( w+ksize < MAX_IMAGE_DIM, "w+ksize is larger than max buffer size" );
        int halfsize = ksize / 2;
        MemoryMode opmode = get_alignment(kernel); // buffer is already loaded with mm_loadu
#pragma omp parallel for
        for( int r=0; r<h; r++ ) {
            float buffer[MAX_IMAGE_DIM];
            int rw = r*w;
            memset( buffer,            0,     sizeof(*buffer)*halfsize );
            memcpy( buffer+halfsize,   im+rw, sizeof(*buffer)*w        );
            memset( buffer+halfsize+w, 0,     sizeof(*buffer)*halfsize );
            filter_buffer(buffer, w, kernel, ksize, opmode );
            memcpy( out+rw, buffer, w*sizeof(*out) );
        }
    }


    void filter_ver( const float* im, const int& w, const int& h, const float* kernel, const int& ksize,
                     float* out ) {
        passert_statement( h+ksize < MAX_IMAGE_DIM, "h+ksize is larger than max buffer size" );

        float buffer0[MAX_IMAGE_DIM], buffer1[MAX_IMAGE_DIM], buffer2[MAX_IMAGE_DIM], buffer3[MAX_IMAGE_DIM];
        float buffer4[MAX_IMAGE_DIM], buffer5[MAX_IMAGE_DIM], buffer6[MAX_IMAGE_DIM], buffer7[MAX_IMAGE_DIM];

        int halfsize = ksize / 2;
        int i, c, r;

        MemoryMode opmode = get_alignment(kernel);

        const float* imp;
        for( c=0; c<w-8; c+=8 ) {
            imp=im+c;
            memset( buffer0, 0, sizeof(*buffer0)*halfsize );
            memset( buffer1, 0, sizeof(*buffer0)*halfsize );
            memset( buffer2, 0, sizeof(*buffer0)*halfsize );
            memset( buffer3, 0, sizeof(*buffer0)*halfsize );
            memset( buffer4, 0, sizeof(*buffer0)*halfsize );
            memset( buffer5, 0, sizeof(*buffer0)*halfsize );
            memset( buffer6, 0, sizeof(*buffer0)*halfsize );
            memset( buffer7, 0, sizeof(*buffer0)*halfsize );

            for( i=0; i<h; i++ ) {
                imp = im+i*w+c;
                buffer0[halfsize+i] = imp[0];
                buffer1[halfsize+i] = imp[1];
                buffer2[halfsize+i] = imp[2];
                buffer3[halfsize+i] = imp[3];
                buffer4[halfsize+i] = imp[4];
                buffer5[halfsize+i] = imp[5];
                buffer6[halfsize+i] = imp[6];
                buffer7[halfsize+i] = imp[7];
            }

            memset( buffer0+halfsize+h, 0, sizeof(*buffer0)*halfsize );
            memset( buffer1+halfsize+h, 0, sizeof(*buffer0)*halfsize );
            memset( buffer2+halfsize+h, 0, sizeof(*buffer0)*halfsize );
            memset( buffer3+halfsize+h, 0, sizeof(*buffer0)*halfsize );
            memset( buffer4+halfsize+h, 0, sizeof(*buffer0)*halfsize );
            memset( buffer5+halfsize+h, 0, sizeof(*buffer0)*halfsize );
            memset( buffer6+halfsize+h, 0, sizeof(*buffer0)*halfsize );
            memset( buffer7+halfsize+h, 0, sizeof(*buffer0)*halfsize );

            filter_buffer( buffer0, h, kernel, ksize, opmode );
            filter_buffer( buffer1, h, kernel, ksize, opmode );
            filter_buffer( buffer2, h, kernel, ksize, opmode );
            filter_buffer( buffer3, h, kernel, ksize, opmode );
            filter_buffer( buffer4, h, kernel, ksize, opmode );
            filter_buffer( buffer5, h, kernel, ksize, opmode );
            filter_buffer( buffer6, h, kernel, ksize, opmode );
            filter_buffer( buffer7, h, kernel, ksize, opmode );

            for( r=0; r<h; r++ ) {
                out[r*w+c  ] = buffer0[r];
                out[r*w+c+1] = buffer1[r];
                out[r*w+c+2] = buffer2[r];
                out[r*w+c+3] = buffer3[r];
                out[r*w+c+4] = buffer4[r];
                out[r*w+c+5] = buffer5[r];
                out[r*w+c+6] = buffer6[r];
                out[r*w+c+7] = buffer7[r];
            }
        }
        for(; c<w; c++ ) {
            memset( buffer0, 0, sizeof(*buffer0)*halfsize );
            for( i=0; i<h; i++ ) buffer0[halfsize+i  ] = im[i*w+c ];
            memset( buffer0+halfsize+h, 0, sizeof(*buffer0)*halfsize );
            filter_buffer(buffer0, h, kernel, ksize, get_alignment(kernel) );
            for( r=0; r<h; r++ ) out[r*w+c] = buffer0[r];
        }
    }

    void filter_ver_par(const float* im, const int& w, const int& h, const float* kernel, const int& ksize,
                        float* out ) {
        passert_statement( h+ksize < MAX_IMAGE_DIM, "h+ksize is larger than max buffer size" );
        int halfsize = ksize / 2;
        MemoryMode opmode = get_alignment(kernel);
#pragma omp parallel for
        for( int c=0; c<w; c++ ) {
            float buffer[MAX_IMAGE_DIM];
            const float* imp = im+c;
            int i, r;
            memset( buffer,            0, sizeof(*buffer)*halfsize );
            for( i=0; i<h; i++ ) {
                imp = im+i*w+c;
                buffer[halfsize+i] = imp[0];
            }
            memset( buffer+h+halfsize, 0, sizeof(*buffer)*halfsize );
            filter_buffer( buffer, h, kernel, ksize, opmode );
            for( r=0; r<h; r++ ) {
                out[r*w+c  ] = buffer[r];
            }
        }
    }

    void filter_hv( const float* im, const int& w, const int& h, const float* kernel, const int& ksize, float* out ) {
        filter_hor(im, w,h,kernel,ksize,out);
        filter_ver(out,w,h,kernel,ksize,out);
    }

    void filter_hv_par(const float* im, const int& w, const int& h, const float* kernel, const int& ksize, float* out) {
        filter_hor_par(im, w,h,kernel,ksize,out);
        filter_ver_par(out,w,h,kernel,ksize,out);
    }


}
