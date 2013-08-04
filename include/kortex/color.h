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
#ifndef KORTEX_COLOR_H
#define KORTEX_COLOR_H

#include <kortex/types.h>

namespace kortex {

    struct Color {
        uchar r, g, b;
        Color() { r = g = b = 0; }
        Color( uchar r_, uchar g_, uchar b_ ) : r(r_), g(g_), b(b_) {}
    };

    enum ColorName { COLOR_BLACK=0,
                     COLOR_RED,    COLOR_GREEN,   COLOR_BLUE,
                     COLOR_CYAN,   COLOR_MAGENTA, COLOR_YELLOW,
                     COLOR_ORANGE, COLOR_PURPLE,  COLOR_PINK,
                     COLOR_BROWN,  COLOR_GOLD,    COLOR_NAVY,
                     COLOR_GRAY,
                     COLOR_WHITE }; // white should be the last color!

    inline uchar cast_to_gray_range( const float& f ) {
        return static_cast<uchar>( std::min( 255.0f, std::max(0.0f, f+0.5f) ) );
    }

    inline uchar rgb_to_gray_u(const uchar& r, const uchar& g, const uchar& b) {
        return cast_to_gray_range( 0.299f*r + 0.587f*g + 0.114f*b );
    }
    inline uchar rgb_to_gray_u(const float& r, const float& g, const float& b) {
        return cast_to_gray_range( 0.299f*r + 0.587f*g + 0.114f*b );
    }

    inline float rgb_to_gray_f(const uchar& r, const uchar& g, const uchar& b) {
        return std::min( 255.0f, static_cast<float>(0.299f*r + 0.587f*g + 0.114f*b) );
    }
    inline float rgb_to_gray_f(const float& r, const float& g, const float& b) {
        return std::min( 255.0f, 0.299f*r + 0.587f*g + 0.114f*b );
    }

    inline void get_color(ColorName color, uchar &cr, uchar &cg, uchar &cb) {
        switch( color ) {
        case COLOR_BLACK  : cr =   0; cg =   0; cb =   0; break;
        case COLOR_WHITE  : cr = 255; cg = 255; cb = 255; break;
        case COLOR_RED    : cr = 255; cg =   0; cb =   0; break;
        case COLOR_GREEN  : cr =   0; cg = 255; cb =   0; break;
        case COLOR_BLUE   : cr =   0; cg =   0; cb = 255; break;
        case COLOR_CYAN   : cr =   0; cg = 255; cb = 255; break;
        case COLOR_YELLOW : cr = 255; cg = 255; cb =   0; break;
        case COLOR_MAGENTA: cr = 255; cg =   0; cb = 255; break;
        case COLOR_ORANGE : cr = 255; cg = 165; cb =   0; break;
        case COLOR_PURPLE : cr = 160; cg =  32; cb = 240; break;
        case COLOR_PINK   : cr = 255; cg =  20; cb = 147; break;
        case COLOR_BROWN  : cr = 165; cg =  42; cb =  42; break;
        case COLOR_GOLD   : cr = 255; cg = 215; cb =   0; break;
        case COLOR_NAVY   : cr =   0; cg =   0; cb = 128; break;
        case COLOR_GRAY   : cr = 190; cg = 190; cb = 190; break;
        default           : cr = 255; cg =   0; cb =   0; break;
        }
    }

    inline void get_color(int color, uchar &cr, uchar &cg, uchar &cb) {
        int mc = color % COLOR_WHITE + 1;
        get_color( (ColorName)mc, cr, cg, cb);
    }

    void hsv_to_rgb( float h, float s, float v, float &r, float &g, float &b );


}

#endif
