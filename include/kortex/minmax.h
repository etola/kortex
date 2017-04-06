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
#ifndef KORTEX_MINMAX_H
#define KORTEX_MINMAX_H

#include <vector>
using std::vector;

#include <kortex/check.h>

namespace kortex {

    template<typename T>
    int min( const T* arr, const int& narr, T& min_val ) {
        passert_pointer( arr );
        passert_pointer_size( narr );
        if( narr == 0 ) return -1;
        int min_ind = 0;
        min_val = arr[0];
        for( int i=1; i<narr; i++ ) {
            if( arr[i] >= min_val ) continue;
            min_val = arr[i];
            min_ind = i;
        }
        return min_ind;
    }

    template<typename T>
    int max( const T* arr, const int& narr, T& max_val ) {
        passert_pointer( arr );
        passert_pointer_size( narr );
        if( narr == 0 ) return -1;
        int max_ind = 0;
        max_val = arr[0];
        for( int i=1; i<narr; i++ ) {
            if( arr[i] <= max_val ) continue;
            max_val = arr[i];
            max_ind = i;
        }
        return max_ind;
    }

    template<typename T>
    int min( const vector<T>& arr, T& min_val ) {
        int narr = (int)arr.size();
        if( narr == 0 ) return -1;
        int min_ind = 0;
        min_val = arr[0];
        for( int i=1; i<narr; i++ ) {
            if( arr[i] >= min_val ) continue;
            min_val = arr[i];
            min_ind = i;
        }
        return min_ind;
    }

    template<typename T>
    int max( const vector<T>& arr, T& max_val ) {
        int narr = (int)arr.size();
        if( narr == 0 ) return -1;
        int max_ind = 0;
        max_val = arr[0];
        for( int i=1; i<narr; i++ ) {
            if( arr[i] <= max_val ) continue;
            max_val = arr[i];
            max_ind = i;
        }
        return max_ind;
    }

    void find_2_minima( const float* cost, int np, int rad, int matches[2] );
}

#endif
