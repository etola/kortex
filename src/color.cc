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

    void get_color(ColorName color, uchar &cr, uchar &cg, uchar &cb) {
        switch( color ) {
        case COLOR_BLACK    : cr =   0; cg =   0; cb =   0; break;
        case COLOR_WHITE    : cr = 255; cg = 255; cb = 255; break;
        case COLOR_RED      : cr = 255; cg =   0; cb =   0; break;
        case COLOR_GREEN    : cr =   0; cg = 255; cb =   0; break;
        case COLOR_BLUE     : cr =   0; cg =   0; cb = 255; break;
        case COLOR_CYAN     : cr =   0; cg = 255; cb = 255; break;
        case COLOR_YELLOW   : cr = 255; cg = 255; cb =   0; break;
        case COLOR_MAGENTA  : cr = 255; cg =   0; cb = 255; break;
        case COLOR_ORANGE   : cr = 255; cg = 165; cb =   0; break;
        case COLOR_PURPLE   : cr = 128; cg =   0; cb = 128; break;
        case COLOR_PINK     : cr = 255; cg = 192; cb = 203; break;
        case COLOR_BROWN    : cr = 136; cg =  69; cb =  19; break;
        case COLOR_GOLD     : cr = 255; cg = 215; cb =   0; break;
        case COLOR_SILVER   : cr = 190; cg = 190; cb = 190; break;
        case COLOR_GRAY     : cr = 128; cg = 128; cb = 128; break;
        case COLOR_LRED     : cr = 255; cg =  51; cb =  51; break;
        case COLOR_LGREEN   : cr =  51; cg = 255; cb =  51; break;
        case COLOR_LBLUE    : cr =  51; cg =  51; cb = 255; break;
        case COLOR_LCYAN    : cr =  51; cg = 255; cb = 255; break;
        case COLOR_LMAGENTA : cr = 255; cg =  51; cb = 255; break;
        case COLOR_LYELLOW  : cr = 255; cg = 255; cb =  51; break;
        case COLOR_LORANGE  : cr = 255; cg = 153; cb =  51; break;
        case COLOR_LPURPLE  : cr = 178; cg = 102; cb = 255; break;
        case COLOR_LPINK    : cr = 255; cg = 102; cb = 255; break;
        case COLOR_LBROWN   : cr = 205; cg = 133; cb =  63; break;
        case COLOR_LGOLD    : cr = 250; cg = 250; cb = 210; break;
        case COLOR_LGRAY    : cr =  90; cg =  90; cb =  90; break;
        default             : cr = 255; cg =   0; cb =   0; break;
        }
    }


}
