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

    class  Image;
    struct Rect2i;

    void draw_point         ( Image& im, int x, int y, ColorName color, int thickness=0 );
    void draw_line          ( Image& im, int x0, int y0, int x1, int y1, ColorName color, int thickness=0 );
    void draw_plus          ( Image& im, int x, int y, int ps, ColorName color, int thickness=0 );
    void draw_square        ( Image& im, int x, int y, int ss, int ori, ColorName color, int thickness=0 );
    void draw_rectangle     ( Image& im, const Rect2i& rectangle, ColorName col, int thickness=0 );
    void draw_circle        ( Image& im, int x, int y, float dr, ColorName color, int thickness=0 );
    void draw_shaded_square ( Image& im, int x0, int y0, int w, float ss, ColorName color);
    void draw_filled_square ( Image& im, int x0, int y0, int w, ColorName color);
    void draw_shaded        ( Image& im, const Image& mask, float ss, ColorName color);

    void draw_region( Image& im, int xs, int ys, int xe, int ye, ColorName col, int thickness=0 );
    void draw_region_filled( Image& im, int xs, int ys, int xe, int ye, ColorName col );

}

#endif
