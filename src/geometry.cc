// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2017 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
// web   : http://www.aurvis.com
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_GEOMETRY_CC
#define KORTEX_GEOMETRY_CC

#include <kortex/geometry.h>
#include <kortex/math.h>
#include <kortex/svd.h>

namespace kortex {

    void compute_mean( const vector<const double*> pnts, double mean[3] ) {
        assert_statement( pnts.size() > 0, "invalid number of points" );
        memset( mean, 0, sizeof(*mean)*3 );
        int n_pnts = (int)pnts.size();
        for( int p=0; p<n_pnts; p++ ) {
            const double* X = pnts[p];
            assert_pointer( X );
            mean[0] += X[0];
            mean[1] += X[1];
            mean[2] += X[2];
        }
        mean[0] /= n_pnts;
        mean[1] /= n_pnts;
        mean[2] /= n_pnts;
    }
    void compute_mean( const vector<const float*> pnts, float mean[3] ) {
        assert_statement( pnts.size() > 0, "invalid number of points" );
        memset( mean, 0, sizeof(*mean)*3 );
        int n_pnts = (int)pnts.size();
        for( int p=0; p<n_pnts; p++ ) {
            const float* X = pnts[p];
            assert_pointer( X );
            mean[0] += X[0];
            mean[1] += X[1];
            mean[2] += X[2];
        }
        mean[0] /= static_cast<float>(n_pnts);
        mean[1] /= static_cast<float>(n_pnts);
        mean[2] /= static_cast<float>(n_pnts);
    }

    /// computes the plane equation fitting to the points. returning value is
    /// the curvature estimate for the points.
    double compute_plane( const vector<const double*> pnts, double plane[4] ) {
        int n_pnts = (int)pnts.size();
        if( n_pnts < 4 ) {
            plane[0] = plane[1] = plane[2] = plane[3] = 0.0;
            logman_warning_g( "not enough points [%d] to estimate a plane", n_pnts );
            return -1.0;
        }
        double  mu[3];
        compute_mean( pnts, mu );

        double Cov[9];
        mat_zero( Cov, 3, 3 );

        double nX[3];
        double ppt[9];
        for( int p=0; p<n_pnts; p++ ) {
            const double* X = pnts[p];
            nX[0] = X[0]-mu[0];
            nX[1] = X[1]-mu[1];
            nX[2] = X[2]-mu[2];
            mat_mat_trans( nX, 3, 1, nX, 3, 1, ppt, 9 );
            mat_add_3( ppt, Cov );
        }

        // need to divide by number of samples to compute the actual covariance
        // but it is not necessary here as we divide singular values to each
        // other and the n_pnts will just cancel each oher.

        SVD svd;
        svd.decompose( Cov, 3, 3, true, true );

        const double* components = svd.Vt();
        memcpy( plane, components+6, sizeof(*plane)*3 );

        plane[3] = -dot3(plane, mu);

        const double* sd = svd.Sd();
        double l0 = fabs(sd[0]);
        double l1 = fabs(sd[1]);
        double l2 = fabs(sd[2]);

        return l2 / (l0+l1+l2);
    }

    float compute_plane( const vector< Vec3f >& pnts, float plane[4] ) {
        int n_pnts = (int)pnts.size();
        if( n_pnts < 4 ) {
            plane[0] = plane[1] = plane[2] = plane[3] = 0.0f;
            logman_warning_g( "not enough points [%d] to estimate a plane", n_pnts );
            return -1.0;
        }

        Vec3f mu;
        compute_mean( pnts, mu );

        double Cov[9];
        mat_zero( Cov, 3, 3 );
        double ppt[9];
        double nX[3];
        for( int p=0; p<n_pnts; p++ ) {
            const Vec3f& X = pnts[p];
            nX[0] = X[0]-mu[0];
            nX[1] = X[1]-mu[1];
            nX[2] = X[2]-mu[2];
            mat_mat_trans( nX, 3, 1, nX, 3, 1, ppt, 9 );
            mat_add_3( ppt, Cov );
        }
        SVD svd;
        svd.decompose( Cov, 3, 3, true, true );

        const double* components = svd.Vt();
        plane[0] = (float)components[6];
        plane[1] = (float)components[7];
        plane[2] = (float)components[8];
        plane[3] = -dot3(plane, mu());

        const double* sd = svd.Sd();
        double l0 = fabs(sd[0]);
        double l1 = fabs(sd[1]);
        double l2 = fabs(sd[2]);

        return float( l2 / (l0+l1+l2) );
    }

    /// computes the plane equation fitting to the points. returning value is
    /// the curvature estimate for the points.
    float compute_plane( const vector<const float*> pnts, float plane[4] ) {
        int n_pnts = (int)pnts.size();
        if( n_pnts < 4 ) {
            plane[0] = plane[1] = plane[2] = plane[3] = 0.0f;
            logman_warning_g( "not enough points [%d] to estimate a plane", n_pnts );
            return -1.0;
        }
        float mu[3];
        compute_mean( pnts, mu );

        double Cov[9];
        mat_zero( Cov, 3, 3 );

        double  nX[3];
        double ppt[9];
        for( int p=0; p<n_pnts; p++ ) {
            const float* X = pnts[p];
            assert_pointer(X);
            nX[0] = X[0]-mu[0];
            nX[1] = X[1]-mu[1];
            nX[2] = X[2]-mu[2];
            mat_mat_trans( nX, 3, 1, nX, 3, 1, ppt, 9 );
            mat_add_3( ppt, Cov );
        }

        SVD svd;
        svd.decompose( Cov, 3, 3, true, true );

        const double* components = svd.Vt();
        plane[0] = (float)components[6];
        plane[1] = (float)components[7];
        plane[2] = (float)components[8];
        plane[3] = -dot3(plane, mu);

        const double* sd = svd.Sd();
        double l0 = fabs(sd[0]);
        double l1 = fabs(sd[1]);
        double l2 = fabs(sd[2]);

        return float( l2 / (l0+l1+l2) );
    }

    double point_to_plane_distance( const double X[3], const double plane[4] ) {
        assert_statement_g( is_unit_norm_3( plane ), "plane normal is not normalized [%f]", dot3(plane,plane) );
        return dot3(plane, X) + plane[3];
    }

    float  point_to_plane_distance( const float  X[3], const float plane[4] ) {
        assert_statement_g( is_unit_norm_3( plane ), "plane normal is not normalized [%f]", dot3(plane,plane) );
        return dot3(plane, X) + plane[3];
    }

    /// finds the closest point on the given plane and returns its distance
    float closest_point_on_plane( const float point[3], const float plane[4], float q[3] ) {
        float d = point_to_plane_distance( point, plane );
        q[0] = point[0] - d*plane[0];
        q[1] = point[1] - d*plane[1];
        q[2] = point[2] - d*plane[2];
        return d;
    }

}

#endif
