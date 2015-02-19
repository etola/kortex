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
#include <cfloat>

namespace kortex {

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

