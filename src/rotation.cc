// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See licence.txt file for licence information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
//
// ---------------------------------------------------------------------------

#include <kortex/rotation.h>
#include <kortex/matrix.h>
#include <kortex/math.h>
#include <kortex/defs.h>
#include <kortex/check.h>

namespace kortex {

    /// computes the rotation matrix that rotates na to nb
    void rotate_normal_to_normal( const double* na, const double* nb, double* Rab ) {
        double Na[3];
        double Nb[3];
        normalize_l2norm3(na, Na);
        normalize_l2norm3(nb, Nb);

        double dot_ab = dot3(Na,Nb);

        double axis[4];
        if( 1-dot_ab < 1e-10 ) {
            axis[0] = 0.0;
            axis[1] = 0.0;
            axis[2] = 1.0;
            axis[3] = 0.0;
        } else {
            cross3(Na, Nb, axis);
            normalize_l2norm3(axis);
            axis[3] = acos( dot_ab );
        }

        axisangle_to_rotation( axis, Rab );
    }

    void axisangle_to_quaternion( const double* aa, double* q ) {
        double t = aa[3]/2;
        double s = sin( t );
        q[0] = aa[0] * s;
        q[1] = aa[1] * s;
        q[2] = aa[2] * s;
        q[3] = cos(t);
    }

    void quaternion_to_rotation( const double* q, double* R ) {
        double q1_2 = sq( q[0] );
        double q2_2 = sq( q[1] );
        double q3_2 = sq( q[2] );

        double q12 = q[0]*q[1];
        double q13 = q[0]*q[2];
        double q14 = q[0]*q[3];
        double q23 = q[1]*q[2];
        double q24 = q[1]*q[3];
        double q34 = q[2]*q[3];

        R[0] = 1-2*(q2_2+q3_2);
        R[1] = 2*(q12-q34);
        R[2] = 2*(q13+q24);
        R[3] = 2*(q12+q34);
        R[4] = 1-2*(q1_2+q3_2);
        R[5] = 2*(q23-q14);
        R[6] = 2*(q13-q24);
        R[7] = 2*(q23+q14);
        R[8] = 1-2*(q1_2+q2_2);
    }

    void axisangle_to_rotation( const double* aa, double* R ) {
        double q[4];
        axisangle_to_quaternion(aa, q);
        quaternion_to_rotation ( q, R);
    }

    void rotation_matrix_around_z( const double& angle_in_degrees, double R[9] ) {
        double in_plane = angle_in_degrees * RADIANS;
        R[0] = cos(in_plane);  R[1] = -sin(in_plane); R[2] = 0;
        R[3] = sin(in_plane);  R[4] =  cos(in_plane); R[5] = 0;
        R[6] = 0;              R[7] =  0;             R[8] = 1;
    }

    void euler_to_rotation( double theta, double phi, double psi, double R[9] ) {
        theta *= RADIANS;
        phi   *= RADIANS;
        psi   *= RADIANS;

        double c,s;
        c = cos(theta); s = sin(theta);
        double Rx [] = { 1, 0, 0, 0, c, -s, 0, s, c };

        c = cos(phi); s = sin(phi);
        double Ry [] = { c, 0, s, 0, 1, 0, -s, 0, c };

        c = cos(psi); s = sin(psi);
        double Rz[] = { c, -s, 0, s, c, 0, 0, 0, 1 };

        mat_mat_mat_3( Rx, Ry, Rz, R );
    }

    // Extracting Euler Angles from a Rotation Matrix - returns in degrees
    // Mike Day, Insomniac Games
    void rotation_to_euler( const double R[9], double& theta, double& phi, double& psi ) {
        theta = atan2( R[5], R[8] );
        double c2 = sqrt( sq( R[0] ) + sq( R[1] ) );
        phi = atan2( -R[2], c2 );
        double s1 = sin( theta );
        double c1 = cos( theta );
        psi = atan2( s1*R[6]-c1*R[3] , c1*R[4]-s1*R[7] );
        theta *= -DEGREES;
        phi   *= -DEGREES;
        psi   *= -DEGREES;
    }

    void azel_to_cartesian( double az, double el, double n[3] ) {
        az *= RADIANS;
        el *= RADIANS;
        n[0] = sin( el ) * cos( az );
        n[1] = sin( el ) * sin( az );
        n[2] = cos( el );
    }

    void cartesian_to_azel( const double n[3], double& az, double& el ) {
        double r = sqrt( sq(n[0]) + sq(n[1]) + sq(n[2]) );
        double n2 = n[2]/r;
        if     ( n2 >  1.0 ) n2 =  1.0;
        else if( n2 < -1.0 ) n2 = -1.0;
        el = acos( n2 ) * DEGREES;
        az = atan2( n[1], n[0] ) * DEGREES;
    }

    static const double  canonical_xd[] = { 1.0, 0.0, 0.0 };
    static const double  canonical_yd[] = { 0.0, 1.0, 0.0 };
    static const double  canonical_zd[] = { 0.0, 0.0, 1.0 };
    void construct_local_coordinate_frame(const double* z_normal, double* new_u, double* new_v) {
        assert_pointer( z_normal && new_u && new_v );
        passert_statement( (z_normal != new_u) && (z_normal != new_v) && (new_u != new_v),
                           "overlapping pointers not allowed" );
        assert_statement( is_unit_norm_3(z_normal), "z should be unit normed" );

        const double *tmp_n = canonical_xd;
        if( fabs(dot3(z_normal, tmp_n)) > 0.8 ) {
            tmp_n = canonical_yd;
            cross3_normalized(tmp_n, z_normal, new_u);
            cross3_normalized(z_normal, new_u, new_v);
        } else {
            cross3_normalized(z_normal, tmp_n, new_v);
            cross3_normalized(new_v, z_normal, new_u);
        }

        assert_statement( is_unit_norm_3(new_u) && is_unit_norm_3(new_v),
                          "output is not unit normed" );
    }


}

