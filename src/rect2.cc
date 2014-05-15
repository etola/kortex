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

#include <algorithm>
#include <kortex/rect2.h>

namespace kortex {

    bool Rect2f::is_inside(float x, float y) const {
        if( y>=ly && y<uy && x>=lx && x<ux ) return true;
        else                                 return false;
    }

    bool Rect2f::is_inside_y(float y) const {
        if( y>=ly && y<uy ) return true;
        else                return false;
    }

    bool Rect2f::is_inside_x(float x) const {
        if( x>=lx && x<ux ) return true;
        else                return false;
    }


    bool Rect2f::is_inside(int x, int y) const {
        if( y>=ly && y<uy && x>=lx && x<ux ) return true;
        else                                 return false;
    }

    bool Rect2f::is_inside_y(int y) const {
        if( y>=ly && y<uy ) return true;
        else                return false;
    }

    bool Rect2f::is_inside_x(int x) const {
        if( x>=lx && x<ux ) return true;
        else                return false;
    }
//
//
//


    bool Rect2i::is_inside(int x, int y) const {
        if( x>=lx && x<ux && y>=ly && y<uy ) return true;
        else                                 return false;
    }

    bool Rect2i::is_inside_x(int x) const {
        if( x>=lx && x<ux ) return true;
        else                return false;
    }

    bool Rect2i::is_inside_y(int y) const {
        if( y>=ly && y<uy ) return true;
        else                return false;
    }

//

    void rect_move( const Rect2i& irect, int mx, int my, Rect2i& orect  ) {
        orect.init( mx+irect.lx, mx+irect.ux, my+irect.ly, my+irect.uy );
        orect.id = irect.id;
    }

    void rect_union( const Rect2i& r0, const Rect2i& r1, Rect2i& out ) {
        out = r0;
        out.update( r1.lx, r1.ly );
        out.update( r1.ux, r1.uy );
    }

    bool rect_intersect( const Rect2i& r0, const Rect2i& r1, Rect2i& out ) {
        if( r1.lx > r0.ux ) return false;
        if( r0.lx > r1.ux ) return false;
        if( r1.ly > r0.uy ) return false;
        if( r0.ly > r1.uy ) return false;

        int olx = std::max( r0.lx, r1.lx );
        int oux = std::min( r0.ux, r1.ux );
        int oly = std::max( r0.ly, r1.ly );
        int ouy = std::min( r0.uy, r1.uy );

        out.init(olx, oux, oly, ouy);
        return true;
    }


}
