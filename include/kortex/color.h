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
#ifndef KORTEX_COLOR_H
#define KORTEX_COLOR_H

#include <algorithm>
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
                     COLOR_BROWN,  COLOR_GOLD,    COLOR_SILVER,
                     COLOR_GRAY,
                     COLOR_LRED,    COLOR_LGREEN,   COLOR_LBLUE,
                     COLOR_LCYAN,   COLOR_LMAGENTA, COLOR_LYELLOW,
                     COLOR_LORANGE, COLOR_LPURPLE,  COLOR_LPINK,
                     COLOR_LBROWN,  COLOR_LGOLD,    COLOR_LGRAY,
                     COLOR_WHITE }; // white should be the last color!

    inline uchar cast_to_gray_range( const float& f ) {
        return static_cast<uchar>( std::min( 255.0f, std::max(0.0f, f+0.5f) ) );
    }
    inline uchar cast_to_gray_range( const int  & i ) {
        return static_cast<uchar>( std::min( 255, std::max(0, i) ) );
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

    void get_color(ColorName color, uchar &cr, uchar &cg, uchar &cb);
    inline void get_color(ColorName name, Color& col ) {
        get_color( name, col.r, col.g, col.b );
    }

    inline ColorName get_color( int color ) {
        int mc = color % COLOR_WHITE + 1;
        return ColorName(mc);
    }
    inline void get_color(int color, uchar &cr, uchar &cg, uchar &cb) {
        ColorName cn = get_color( color );
        get_color( cn, cr, cg, cb);
    }

    ColorName get_color( const string&    str   );
    string    get_color( const ColorName& color );

    void hsv_to_rgb( float h, float s, float v, float &r, float &g, float &b );


}

#endif
