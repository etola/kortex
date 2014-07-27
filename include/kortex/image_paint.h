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
#ifndef KORTEX_IMAGE_PAINT_H
#define KORTEX_IMAGE_PAINT_H

#include <kortex/color.h>

#include <vector>

namespace kortex {

    class Image;
    class Rect2i;

    void draw_point         ( Image& im, int x, int y, ColorName color);
    void draw_line          ( Image& im, int x0, int y0, int x1, int y1, ColorName color);
    void draw_plus          ( Image& im, int x, int y, int ps, ColorName color);
    void draw_square        ( Image& im, int x, int y, int ss, int ori, ColorName color);
    void draw_rectangle     ( Image& im, const Rect2i& rectangle, ColorName col );
    void draw_circle        ( Image& im, int x, int y, float dr, ColorName color );
    void draw_shaded_square ( Image& im, int x0, int y0, int w, float ss, ColorName color);
    void draw_filled_square ( Image& im, int x0, int y0, int w, ColorName color);
    void draw_shaded        ( Image& im, const Image& mask, float ss, ColorName color);


}

#endif
