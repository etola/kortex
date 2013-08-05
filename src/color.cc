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
#include <kortex/color.h>
#include <cmath>

namespace kortex {

    void hsv_to_rgb( float h, float s, float v, float &r, float &g, float &b ) {
        if( s == 0.0f ) {
            r = g = b = v;
            return;
        }
        h /= 60.0f;
        int   i = floor( h );
        float f = h - i;
        float p = v * ( 1 - s );
        float q = v * ( 1 - s * f );
        float t = v * ( 1 - s * ( 1 - f ) );

        switch( i ) {
        case  0: r = v; g = t; b = p; break;
        case  1: r = q; g = v; b = p; break;
        case  2: r = p; g = v; b = t; break;
        case  3: r = p; g = q; b = v; break;
        case  4: r = t; g = p; b = v; break;
        default: r = v; g = p; b = q; break;
        }
    }

}
