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
#ifndef KORTEX_GEOMETRY_H
#define KORTEX_GEOMETRY_H

#include <vector>
using std::vector;

namespace kortex {

    struct Polynomial2d {
        vector<double> coeffs;

        int    degree() const { return coeffs.size()-1; }
        double offset() const { return coeffs[0]; }
        double eval( double x ) const;
        // returns distance to point px,py -> closest point on polynomial: x0,y0
        double distance( const double& py, const double& px, double& x0, double& y0 ) const;
        void   print() const;
    };


}

#endif
