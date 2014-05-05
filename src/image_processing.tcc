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

        int   x0  = (int)floor( x );
        int   y0  = (int)floor( y );
        int   x1  = x0+1;
        int   y1  = y0+1;

        float alfa = x0 - x;
        float beta = y0 - y;

        if( alfa < 0.0001f ) {
            alfa = 0.0f;
            y1   = y0;
        }
        if( beta < 0.0001f ) {
            beta = 0.0f;
            x1   = x0;
        }
        assert_statement( is_inside(x0,0,w) && is_inside(x1,0,w), "coords oob" );
        assert_statement( is_inside(y0,0,h) && is_inside(y1,0,h), "coords oob" );

        const T* row0 = img + y0*w*nc+c;
        const T* row1 = img + y1*w*nc+c;

        double ab = alfa * beta;

        x1 = nc*x1;
        x0 = nc*x0;

        return (alfa - ab) * row0[x1] + (1.0-beta)*(1.0-alfa) * row0[x0] + ab * row1[x1] + (beta-ab) * row1[x0];
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
