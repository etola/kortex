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
#include <kortex/image_paint.h>
#include <kortex/image.h>
#include <kortex/color.h>
#include <kortex/defs.h>
#include <kortex/rect2.h>

namespace kortex {

    void draw_point( Image& im, int x, int y, ColorName color, int thickness ) {
        im.assert_type( IT_U_PRGB | IT_U_IRGB );

        uchar cr, cg, cb;
        get_color(color, cr, cg, cb);
        im.set( x, y, thickness, cr, cg, cb );
    }

    void draw_line( Image& im, int x0, int y0, int x1, int y1, ColorName color, int thickness ) {
        im.assert_type( IT_U_PRGB | IT_U_IRGB );
        float dx = x1 - x0;
        float dy = y1 - y0;
        float sz = sqrt(dx*dx + dy*dy);
        dx /= sz;
        dy /= sz;
        uchar cr, cg, cb;
        get_color(color, cr, cg, cb);
        float y=y0, x=x0;
        while(1) {
            if( !im.is_inside( (int)x, (int)y ) ) break;
            im.set( (int)x, (int)y, thickness, cr, cg, cb);
            y += dy;
            x += dx;
            if( dy > 0 ) { if( y > y1 ) break; }
            else         { if( y < y1 ) break; }
            if( dx > 0 ) { if( x > x1 ) break; }
            else         { if( x < x1 ) break; }
        }
    }

    void draw_plus( Image& im, int x, int y, int ps, ColorName color, int thickness ) {
        draw_line(im, x-ps, y, x+ps, y, color, thickness );
        draw_line(im, x, y-ps, x, y+ps, color, thickness );
    }

    void draw_square( Image& im, int x, int y, int ss, int ori, ColorName color, int thickness ) {
        double dr = sqrt(2.0) * ss;
        double o  = ori*RADIANS;
        int rx0 = (int)( x + dr * cos( o + PI_4 ) );
        int ry0 = (int)( y + dr * sin( o + PI_4 ) );
        int rx1 = (int)( x + dr * cos( o - PI_4 ) );
        int ry1 = (int)( y + dr * sin( o - PI_4 ) );
        int rx2 = (int)( x + dr * cos( o - PI_34) );
        int ry2 = (int)( y + dr * sin( o - PI_34) );
        int rx3 = (int)( x + dr * cos( o + PI_34) );
        int ry3 = (int)( y + dr * sin( o + PI_34) );

        draw_line( im, rx0, ry0, rx1, ry1, color, thickness );
        draw_line( im, rx1, ry1, rx2, ry2, color, thickness );
        draw_line( im, rx2, ry2, rx3, ry3, color, thickness );
        draw_line( im, rx3, ry3, rx0, ry0, color, thickness );
    }

    void draw_region( Image& im, int xs, int ys, int xe, int ye, ColorName col, int thickness ) {
        draw_line( im, xs, ys, xe, ys, col, thickness );
        draw_line( im, xe, ys, xe, ye, col, thickness );
        draw_line( im, xe, ye, xs, ye, col, thickness );
        draw_line( im, xs, ye, xs, ys, col, thickness );
    }

    void draw_region_filled( Image& im, int xs, int ys, int xe, int ye, ColorName col ) {
        im.assert_type( IT_U_PRGB | IT_U_IRGB );
        uchar cr, cg, cb;
        get_color(col, cr, cg, cb);

#pragma omp parallel for
        for( int y=ys; y<ye; y++ ) {
            for( int x=xs; x<xe; x++ ) {
                im.set( x, y, cr, cg, cb );
            }
        }

    }

    void draw_rectangle( Image& im, const Rect2i& rectangle, ColorName col, int thickness ) {
        draw_line( im, rectangle.lx, rectangle.ly, rectangle.lx, rectangle.uy, col, thickness );
        draw_line( im, rectangle.lx, rectangle.uy, rectangle.ux, rectangle.uy, col, thickness );
        draw_line( im, rectangle.ux, rectangle.ly, rectangle.ux, rectangle.uy, col, thickness );
        draw_line( im, rectangle.lx, rectangle.ly, rectangle.ux, rectangle.ly, col, thickness );
    }

    void draw_circle( Image& im, int x, int y, float dr, ColorName color, int thickness ) {
        im.assert_type( IT_U_PRGB | IT_U_IRGB );

        uchar cr, cg, cb;
        get_color(color, cr, cg, cb);
        int sy = y + dr;
        int sx = x;

        for(int a=5; a<=360; a+=5 ) {
            int cy = (int)(y + dr * cos(a*RADIANS));
            int cx = (int)(x + dr * sin(a*RADIANS));
            if( im.is_inside(sx,sy) && im.is_inside(cx,cy) )
                draw_line( im, sx, sy, cx, cy, color, thickness );
            sx = cx;
            sy = cy;
        }
    }

    void draw_shaded_square( Image& im, int x0, int y0, int w, float ss, ColorName color ) {
        im.assert_type( IT_U_PRGB | IT_U_IRGB );

        uchar cr, cg, cb;
        get_color(color, cr, cg, cb);
        uchar pr, pg, pb;
        int hw = w/2;
        for(int y=y0-hw; y<=y0+hw; y++) {
            for(int x=x0-hw; x<=x0+hw; x++) {
                if( !im.is_inside(x,y) )
                    continue;
                im.get(x,y,pr,pg,pb);
                pr = ss * cr + (1.0f-ss) * pr;
                pg = ss * cg + (1.0f-ss) * pg;
                pb = ss * cb + (1.0f-ss) * pb;
                if( pr>255 ) pr = 255;
                if( pg>255 ) pg = 255;
                if( pb>255 ) pb = 255;
                im.set(x, y, pr, pg, pb);
            }
        }
    }

    void draw_filled_square( Image& im, int x0, int y0, int w, ColorName color) {
        im.assert_type( IT_U_PRGB | IT_U_IRGB );

        uchar cr, cg, cb;
        get_color(color, cr, cg, cb);
        int hw = w/2;
        for(int y=y0-hw; y<=y0+hw; y++) {
            for(int x=x0-hw; x<=x0+hw; x++) {
                if( !im.is_inside(x,y) )
                    continue;
                im.set(x, y, cr, cg, cb);
            }
        }
    }


    void draw_shaded( Image& im, const Image& mask, float ss, ColorName color) {
        im.assert_type( IT_U_PRGB | IT_U_IRGB );
        passert_statement( check_dimensions(im, mask), "im mask dimension must be the same" );

        uchar cr, cg, cb;
        get_color(color, cr, cg, cb);

        uchar pr, pg, pb;
        for(int y=0; y<im.h(); y++) {
            for(int x=0; x<im.w(); x++) {
                if( mask.getu(x, y) ) {
                    im.get(x,y,pr,pg,pb);
                    pr = ss * cr + (1.0f-ss) * pr;
                    pg = ss * cg + (1.0f-ss) * pg;
                    pb = ss * cb + (1.0f-ss) * pb;
                    if( pr>255 ) pr = 255;
                    if( pg>255 ) pg = 255;
                    if( pb>255 ) pb = 255;
                    im.set(x, y, pr, pg, pb);
                }
            }
        }
    }



}
