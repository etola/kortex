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
#ifndef KORTEX_BIT_OPERATIONS_H
#define KORTEX_BIT_OPERATIONS_H

#include <assert.h>
#include <stdint.h>
#include <kortex/types.h>

namespace kortex {

    inline bool have_same_sign(const int& x, const int& y) {
        return ((x ^ y) < 0);
    }

    inline bool power_of_two(const int& v) {
        return v && !(v & (v - 1));
    }

    inline uint64_t pack64(const int a[64]) {
        uint64_t r = (uint64_t)(0);
        for( int i=0; i<64; i++ )
            r =  r | ( (uint64_t)(a[i]&1) << i );
        return r;
    }

    inline uint64_t pack64(const uchar a[64]) {
        uint64_t r = (uint64_t)(0);
        for( int i=0; i<64; i++ )
            r =  r | ( (uint64_t)(a[i]&1) << i );
        return r;
    }
    inline uint64_t pack64(const float a[64]) {
        uint64_t r = (uint64_t)(0);
        for( int i=0; i<64; i++ )
            r =  r | ( (uint64_t)(((int)(a[i]))&1) << i );
        return r;
    }

    inline void unpack64(const uint64_t& a, uchar ua[64]) {
        for( int i=0; i<64; i++ )
            ua[63-i] = ( 1 & ( a >> i ) );
    }
    inline void unpack64(const uint64_t& a, int ua[64]) {
        for( int i=0; i<64; i++ )
            ua[63-i] = ( 1 & ( a >> i ) );
    }

    inline int extract_k_bits(const uchar* word, const unsigned& starting_bit_no, const unsigned& k) {
        assert( k<=16 );
        unsigned starting_byte  = starting_bit_no/8;
        unsigned remaining_bits = starting_bit_no - starting_byte*8;
        unsigned n_bytes_to_read = (remaining_bits + k + 7 ) / 8;
        uint32_t bit_mask = (1<<k)-1;
        assert( n_bytes_to_read * 8 >= remaining_bits + k );
        assert( n_bytes_to_read < 4 );
        uint32_t a = 0;
        for(unsigned c=0; c<n_bytes_to_read; c++ ) {
            a += word[starting_byte+c];
            if( c!=n_bytes_to_read-1 )
                a <<= 8;
        }
        unsigned shift_bits = (n_bytes_to_read*8-remaining_bits-k);
        a >>= shift_bits;
        return (int)(a & bit_mask);
    }

    inline int hamming_64(const uchar* a, const uchar * b) {
        uint64_t* ta = (uint64_t*)a;
        uint64_t* tb = (uint64_t*)b;
        return __builtin_popcountll( (*ta)^(*tb) );
    }

    inline int hamming_128(const uchar* a, const uchar* b) {
        uint64_t* ta = (uint64_t*)a;
        uint64_t* tb = (uint64_t*)b;
        int s = 0;
        s += __builtin_popcountll( (*ta++) ^ (*tb++) );
        s += __builtin_popcountll( (*ta++) ^ (*tb++) );
        return s;
    }

    inline int hamming_256(const uchar* a, const uchar* b) {
        uint64_t* ta = (uint64_t*)a;
        uint64_t* tb = (uint64_t*)b;
        int s = 0;
        s += __builtin_popcountll( (*ta++) ^ (*tb++) );
        s += __builtin_popcountll( (*ta++) ^ (*tb++) );
        s += __builtin_popcountll( (*ta++) ^ (*tb++) );
        s += __builtin_popcountll( (*ta++) ^ (*tb++) );
        return s;
    }


}

#endif
