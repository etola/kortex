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
#include <kortex/kmatrix.h>
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


    void rotation_matrix_around_z( const double& angle_in_degrees, KMatrix& R ) {
        R.init(3,3);
        double in_plane = angle_in_degrees * RADIANS;
        R[0] = cos(in_plane);  R[1] = -sin(in_plane); R[2] = 0;
        R[3] = sin(in_plane);  R[4] =  cos(in_plane); R[5] = 0;
        R[6] = 0;              R[7] =  0;             R[8] = 1;
    }


    void euler_to_rotation( double theta, double phi, double psi, KMatrix& R ) {
        theta *= RADIANS;
        phi   *= RADIANS;
        psi   *= RADIANS;

        double c,s;
        c = cos(theta); s = sin(theta);
        KMatrix Rx(3,3);
        Rx.init33( 1, 0, 0, 0, c, -s, 0, s, c );

        c = cos(phi); s = sin(phi);
        KMatrix Ry(3,3);
        Ry.init33( c, 0, s, 0, 1, 0, -s, 0, c );

        c = cos(psi); s = sin(psi);
        KMatrix Rz(3,3);
        Rz.init33( c, -s, 0, s, c, 0, 0, 0, 1 );

        mat_mat_mat( Rx, Ry, Rz, R );
    }

    // Extracting Euler Angles from a Rotation Matrix - returns in degrees
    // Mike Day, Insomniac Games
    void rotation_to_euler( const KMatrix& R, double& theta, double& phi, double& psi ) {
        assert_statement( R.w == 3 && R.h == 3, "matrix should be 3x3" );
        theta = atan2( R(1,2), R(2,2) );
        double c2 = sqrt( sq( R(0,0) ) + sq( R(0,1) ) );
        phi = atan2( -R(0,2), c2 );
        double s1 = sin( theta );
        double c1 = cos( theta );
        psi = atan2( s1*R(2,0)-c1*R(1,0) , c1*R(1,1)-s1*R(2,1) );
        theta *= -DEGREES;
        phi   *= -DEGREES;
        psi   *= -DEGREES;
    }

}

