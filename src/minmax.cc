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
#include <kortex/minmax.h>
#include <kortex/check.h>
#include <cfloat>

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

    int min( const vector<float>& arr, float& min_val ) {
        int narr = arr.size();
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

    int max( const vector<float>& arr, float& max_val ) {
        int narr = arr.size();
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

    void find_2_minima( const float* cost, int np, int rad, int matches[2] ) {
        assert_pointer( cost );
        assert_statement_g( is_positive_number(np),  "[np %d] is not positive", np );
        assert_statement_g( is_positive_number(rad), "[rad %d] is not positive", rad );
        assert_array( "find_2_minima", cost, np );
        matches[0] = matches[1] = -1;
        float mv = FLT_MAX;
        for(int i=0; i<np; i++) {
            if( cost[i] < mv ) {
                mv = cost[i];
                matches[0] = i;
            }
        }
        int li=matches[0]-rad; if(li<  0) li=0;
        int ui=matches[0]+rad; if(ui>=np) ui=np;
        mv = FLT_MAX;
        for(int i=0; i<np; i++ ) {
            if( i>=li && i<ui ) continue;
            if( mv > cost[i] ) {
                mv = cost[i];
                matches[1] = i;
            }
        }
    }



}

