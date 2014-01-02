// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2014 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
//
// ---------------------------------------------------------------------------

#include <kortex/geometry.h>
#include <kortex/math.h>
#include <kortex/check.h>
#include <cstdio>
#include <cfloat>

namespace kortex {

    //
    double Polynomial2d::eval( double x ) const {
        double y  = 0.0;
        double xp = 1.0;
        for( int d=0; d<(int)coeffs.size(); d++ ) {
            y += coeffs[d]*xp;
            xp *= x;
        }
        return y;
    }

    void Polynomial2d::print() const {
        for(unsigned int i=0; i<coeffs.size(); i++ )
            printf("%f ", coeffs[i]);
        printf("\n");
    }

    double Polynomial2d::distance( const double& py, const double& px, double& x0, double& y0 ) const {
        if( degree() < 0 ) {
            x0 = y0 = 0.0;
            return DBL_MAX;
        } else if( degree() == 0 ) {
            x0 = px;
            y0 = offset();
            return sqrt( sq(py-y0) );
        }

        int deg     = degree();
        int csz     = deg + 1;
        int der_deg = deg + deg-1;

        vector<double> der_coeffs( der_deg+1, 0.0);
        for( int d=0; d<=der_deg; d++ ) {
            der_coeffs[d] = 0.0;
            for( int j=0; j<=d; j++ ) {
                int i = d+1-j;
                double ci=0.0, cj=0.0;
                if     ( i==  0 ) ci = coeffs[0]-py;
                else if( i<=csz ) ci = coeffs[i];
                if     ( j==  0 ) cj = coeffs[0]-py;
                else if( j<=csz ) cj = coeffs[j];
                der_coeffs[d] += i * ci * cj;
            }
        }
        der_coeffs[0] -= px;
        der_coeffs[1] += 1.0;

        vector<double> real_roots;
        if( !find_real_roots_of_polynomial( der_coeffs, real_roots ) ) {
            logman_warning( "something is wrong..." );
            return DBL_MAX;
        }

        if( real_roots.size() == 0 ) {
            logman_warning( "something is wrong..." );
            return DBL_MAX;
        }

        double dmin = DBL_MAX;
        for( unsigned int i=0; i<real_roots.size(); i++ ) {
            double xr = real_roots[i];
            double yr = eval( xr );
            double d  = sq(py-yr) + sq(px-xr);
            if( d < dmin ) {
                dmin = d;
                x0   = xr;
                y0   = yr;
            }
        }

        return sqrt(dmin);
    }


}
