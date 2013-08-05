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
#include <kortex/check.h>

namespace kortex {

    template<typename T>
    float bilinear_interpolation(const T* img, const int& w, const int& h, const int& nc, const int& c, const float& x, const float& y) {
        assert_pointer( img );
        passert_statement_g( x>=0 && x<=w-1 && y>=0 && y<=h-1, "[x %f][y %f] [w %d] [h %d]", x, y, w, h );

        int mnx = (int)floor( x );
        int mny = (int)floor( y );
        int mxx = mnx+1;
        int mxy = mny+1;

        assert_statement( is_inside(mnx,0,w) && is_inside(mxx,0,w), "coords oob" );
        assert_statement( is_inside(mny,0,h) && is_inside(mxy,0,h), "coords oob" );

        float alfa = mxx - x;
        float beta = mxy - y;

        if( alfa < 0.0001f ) alfa = 0.0f;
        if( beta < 0.0001f ) beta = 0.0f;
        if( alfa > 0.9999f ) alfa = 1.0f;
        if( beta > 0.9999f ) beta = 1.0f;

        int mnyw = mny * w * nc;
        int mxyw = mxy * w * nc;

        if( alfa < 0.0001 ) return float(beta * img[mnyw+mxx*nc+c] + (1-beta) * img[mxyw+mxx*nc+c]);
        if( alfa > 0.9999 ) return float(beta * img[mnyw+mnx*nc+c] + (1-beta) * img[mxyw+mnx*nc+c]);
        if( beta < 0.0001 ) return float(alfa * img[mxyw+mnx*nc+c] + (1-alfa) * img[mxyw+mxx*nc+c]);
        if( beta > 0.9999 ) return float(alfa * img[mnyw+mnx*nc+c] + (1-alfa) * img[mnyw+mxx*nc+c]);

        return float( beta*(alfa * img[mnyw+mnx*nc+c] + (1-alfa)*img[mnyw+mxx*nc+c] )
                      +(1-beta)*(alfa * img[mxyw+mnx*nc+c] + (1-alfa)*img[mxyw+mxx*nc+c] ) );
    }

    /// assumes the position to compute interp is between n1 and n2 and t away from n1.
    float bicubic_interpolation_1d(const float& n0, const float& n1, const float& n2, const float& n3, const float& t) {
        float c0 = 2*n1;
        float c1 = n2-n0;
        float c2 = 2*n0-5*n1+4*n2-n3;
        float c3 = -n0+3*n1-3*n2+n3;
        float t2 = t*t;
        return (c3*t2*t + c2*t2 + c1*t + c0)*0.5;
    }

    /// computes the bicubic interpolation at y, x for channel ch for a
    /// color-image. assumes rgb values are sequential for pixels
    template<typename T>
    float bicubic_interpolation(const T* im, const int& w, const int& h, const int& nc, const int& ch, const float& x, const float& y) {
        assert_pointer( im );
        int iy=int(y);
        int ix=int(x);
        assert_statement( is_inside(ix,0,w) && is_inside(iy,0,h), "coords oob" );

        if ((ix < 2) || (iy < 2) || (ix >= w-3) || (iy >= h-3))
            return (float)im[ iy*w*nc + nc*ix + ch ];

        float p = x - ix; // sub-pixel offset in the x axis
        float q = y - iy; // sub-pixel offset in the y axis
        int offset = (iy-1)*w*nc + (ix-1)*nc + ch; // position of the top-left point

        float N[16];
        for(int i = 0; i < 4; ++i) {
            N[4*i]   = im[offset];
            N[4*i+1] = im[offset +   nc];
            N[4*i+2] = im[offset + 2*nc];
            N[4*i+3] = im[offset + 3*nc];
            offset += w*nc;
        }

        // interpolate in the x direction
        float iy0 = bicubic_interpolation_1d(N[ 0], N[ 1], N[ 2], N[ 3], p);
        float iy1 = bicubic_interpolation_1d(N[ 4], N[ 5], N[ 6], N[ 7], p);
        float iy2 = bicubic_interpolation_1d(N[ 8], N[ 9], N[10], N[11], p);
        float iy3 = bicubic_interpolation_1d(N[12], N[13], N[14], N[15], p);
        // interpolate the x-axis results in the y direction
        return bicubic_interpolation_1d(iy0, iy1, iy2, iy3, q);
    }




}
