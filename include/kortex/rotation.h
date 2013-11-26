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
#ifndef KORTEX_ROTATION_H
#define KORTEX_ROTATION_H

namespace kortex {

    /// computes the rotation matrix that rotates na to nb
    void rotate_normal_to_normal( const double* na, const double* nb, double* Rab );

    void  axisangle_to_quaternion( const double* aa, double* q );
    void quaternion_to_rotation  ( const double* q,  double* R );
    void  axisangle_to_rotation  ( const double* aa, double* R );

    class KMatrix;
    void rotation_matrix_around_z( const double& angle_in_degrees, KMatrix& R );
    void euler_to_rotation( double theta, double phi, double psi, KMatrix& R );
    void euler_to_rotation( double theta, double phi, double psi, double * R );

    // Extracting Euler Angles from a Rotation Matrix - returns in degrees
    void rotation_to_euler( const KMatrix& R, double& theta, double& phi, double& psi );
    void rotation_to_euler( const  double* R, double& theta, double &phi, double& psi );



}

#endif
