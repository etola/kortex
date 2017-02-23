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

    // static const double  canonical_xd  [] = {  1.0,  0.0,  0.0 };
    // static const double  canonical_yd  [] = {  0.0,  1.0,  0.0 };
    // static const double  canonical_zd  [] = {  0.0,  0.0,  1.0 };
    // static const double  canonical_xd_r[] = { -1.0,  0.0,  0.0 };
    // static const double  canonical_yd_r[] = {  0.0, -1.0,  0.0 };
    // static const double  canonical_zd_r[] = {  0.0,  0.0, -1.0 };

    // rotates normal in referential frame to the world referential assuming R
    // is the rotation matrix of the referential.
    inline void normal_local_to_world( const double* R, const float lnormal[3], float wnormal[3] ) {
        wnormal[0] = R[0] * lnormal[0] + R[3] * lnormal[1] + R[6] * lnormal[2];
        wnormal[1] = R[1] * lnormal[0] + R[4] * lnormal[1] + R[7] * lnormal[2];
        wnormal[2] = R[2] * lnormal[0] + R[5] * lnormal[1] + R[8] * lnormal[2];
    }

    // rotates normal in the world referential to referential frame assuming R
    // is the rotation matrix of the referential.
    inline void normal_world_to_local( const double* R, const float wnormal[3], float lnormal[3] ) {
        lnormal[0] = R[0] * wnormal[0] + R[1] * wnormal[1] + R[2] * wnormal[2];
        lnormal[1] = R[3] * wnormal[0] + R[4] * wnormal[1] + R[5] * wnormal[2];
        lnormal[2] = R[6] * wnormal[0] + R[7] * wnormal[1] + R[8] * wnormal[2];
    }

    /// computes the rotation matrix that rotates na to nb
    void rotate_normal_to_normal( const double* na, const double* nb, double* Rab );

    void  axisangle_to_quaternion( const double* aa, double* q );
    void quaternion_to_rotation  ( const double* q,  double* R );
    void  axisangle_to_rotation  ( const double* aa, double* R );

    void rotation_matrix_around_z( const float & angle_in_degrees, float  R[9] );
    void rotation_matrix_around_z( const double& angle_in_degrees, double R[9] );

    void euler_to_rotation( double theta, double phi, double psi, double * R );

    // Extracting Euler Angles from a Rotation Matrix - returns in degrees
    void rotation_to_euler( const  double* R, double& theta, double &phi, double& psi );

    // azimuth and elevation is in DEGREES
    // azimuth  : angle from the positive x-axis to the normal [ -180 , 180 ]
    // elevation: angle from the xy-plane to the normal        [ - 90 ,  90 ]
    void azel_to_cartesian( double az, double el, double n[3] );
    void cartesian_to_azel( const double n[3], double& az, double& el );

    void construct_local_coordinate_frame(const float * z_normal, float * new_u, float * new_v );
    void construct_local_coordinate_frame(const double* z_normal, double* new_u, double* new_v );
    void rotation_to_az_el_zeta( const double R[9], double& az, double& el, double& zeta );

    /// az, el, zeta given in degrees
    void az_el_zeta_to_rotation( const double& az, const double& el, const double& zeta,
                                 double R[9] );

    void rotation_matrix_frame_error( const double* R_ref, const double* R_obs,
                                      double& e_boresight, double& e_inplain );

    /// constructs the rotation matrix looking at z_dir - applies an in plane rotation as well.
    void rotation_matrix( const double z_dir[3], const double& in_plane_in_degrees, double R[9] );
    /// front_az, front_el sets the z-direction of the camera frame.
    /// up_az, up_el specify up vector (y-direction)'s azimuth and elevation
    void rotation_matrix( double front_az, double front_el, double up_az, double up_el, double R[9] );

    /// computes the normal that is looking back towards an observer
    ///
    ///                            |
    ///      ref o------------<-n--X-->n_front (az,el)
    ///                         -  |
    ///
    void compute_reverse_normal( double az, double el, double n[3] );

    /// casts the r matrix to the closest rotation matrix - returns uncertainty.
    double enforce_rotation_matrix( double r[9] );

}

#endif
