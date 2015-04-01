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
#ifndef KORTEX_RECT2_H
#define KORTEX_RECT2_H

namespace kortex {

    struct Rect2f {
        float lx, ly, ux, uy;
        float dx, dy;
        int id;

        Rect2f() { lx = ux = ly = uy = dx = dy = 0.0f; id=0; }
        Rect2f(float xmin, float xmax, float ymin, float ymax) {
            lx = xmin;
            ux = xmax;
            ly = ymin;
            uy = ymax;
            id = 0;
            update();
        }

        void crop(float xmin, float xmax, float ymin, float ymax) {
            if( lx<xmin ) lx=xmin;
            if( ly<ymin ) ly=ymin;
            if( ux>xmax ) ux=xmax;
            if( uy>ymax ) uy=ymax;
        }

        void update() { dx=ux-lx; dy=uy-ly; }

        void update(float x, float y) {
            insert( x, y );
            update();
        }
        void insert( float x, float y ) {
            if( x<lx ) lx = x;
            if( x>ux ) ux = x;
            if( y<ly ) ly = y;
            if( y>uy ) uy = y;
        }

        bool is_inside(float x, float y) const;
        bool is_inside_y(float y) const;
        bool is_inside_x(float x) const;

        bool is_inside(int x, int y) const;
        bool is_inside_y(int y) const;
        bool is_inside_x(int x) const;

    };

    struct Rect2i {
        int lx, ly, ux, uy;
        int dx, dy;
        int id;

        Rect2i() { lx = ux = ly = uy = dx = dy = 0; id = 0; }
        Rect2i(int xmin, int xmax, int ymin, int ymax) {
            init( xmin, xmax, ymin, ymax );
            id = 0;
        }

        void reset_max_region();

        void init(int xmin, int xmax, int ymin, int ymax) {
            lx = xmin;
            ux = xmax;
            ly = ymin;
            uy = ymax;
            update();
        }

        void crop(int xmin, int xmax, int ymin, int ymax) {
            if( lx<xmin ) lx=xmin;
            if( ly<ymin ) ly=ymin;
            if( ux>xmax ) ux=xmax;
            if( uy>ymax ) uy=ymax;
        }

        void update() { dx=ux-lx; dy=uy-ly; }

        void update(int x, int y) {
            insert( x, y );
            update();
        }

        void insert(int x, int y) {
            if( x<lx ) lx = x;
            if( x>ux ) ux = x;
            if( y<ly ) ly = y;
            if( y>uy ) uy = y;
        }

        bool is_inside(int x, int y) const;
        bool is_inside_y(int y) const;
        bool is_inside_x(int x) const;
    };

    /// checks if the smaller rect structure resides within the larger one
    inline bool is_inside( const Rect2i& larger, const Rect2i& smaller ) {
        if( !larger.is_inside( smaller.lx,   smaller.ly   ) ) return false;
        if( !larger.is_inside( smaller.ux-1, smaller.uy-1 ) ) return false;
        return true;
    }

    inline bool does_intersect( const Rect2i& p, const Rect2i& q ) {
        if( p.is_inside( q.lx, q.ly ) ) return true;
        if( p.is_inside( q.lx, q.uy ) ) return true;
        if( p.is_inside( q.ux, q.ly ) ) return true;
        if( p.is_inside( q.ux, q.uy ) ) return true;
        return false;
    }

    void rect_move     ( const Rect2i& irect, int mx, int my, Rect2i& orect  );
    void rect_union    ( const Rect2i& r0, const Rect2i& r1, Rect2i& out );
    bool rect_intersect( const Rect2i& r0, const Rect2i& r1, Rect2i& out );




}

#endif
