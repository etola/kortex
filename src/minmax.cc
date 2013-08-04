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

#include <kortex/minmax.h>
#include <kortex/check.h>

namespace kortex {

    int max( const float* arr, const int& narr, float& max_val ) {
        passert_pointer( arr );
        passert_pointer_size( narr );
        int max_ind = 0;
        max_val = arr[0];
        for( int i=1; i<narr; i++ ) {
            if( arr[i] <= max_val ) continue;
            max_val = arr[i];
            max_ind = i;
        }
        return max_ind;
    }

    int min( const float* arr, const int& narr, float& min_val ) {
        passert_pointer( arr );
        passert_pointer_size( narr );
        int min_ind = 0;
        min_val = arr[0];
        for( int i=1; i<narr; i++ ) {
            if( arr[i] >= min_val ) continue;
            min_val = arr[i];
            min_ind = i;
        }
        return min_ind;
    }

}

