// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2015 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
// web   : http://www.aurvis.com
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_GEOMETRY_H
#define KORTEX_GEOMETRY_H

#include <vector>
#include <kortex/kvector.h>

namespace kortex {

    using std::vector;

    //
    // Plane
    //

    /// computes the plane equation fitting to the points. returning value is
    /// the curvature estimate for the points.
    float  compute_plane( const vector<const float *> pnts, float  plane[4] );
    double compute_plane( const vector<const double*> pnts, double plane[4] );
    float compute_plane ( const vector< Vec3f >& pnts, float plane[4] );

    double point_to_plane_distance( const double X[3], const double plane[4] );
    float  point_to_plane_distance( const float  X[3], const float  plane[4] );

    /// finds the closest point on the given plane and returns its distance
    float closest_point_on_plane( const float point[3], const float plane[4], float q[3] );


}

#endif
