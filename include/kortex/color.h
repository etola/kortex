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



}

#endif
