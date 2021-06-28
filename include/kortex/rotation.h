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

#include <kortex/defs.h>
#include <kortex/math.h>
#include <cmath>

namespace kortex {

    // rotates normal in referential frame to the world referential assuming R
    // is the rotation matrix of the referential.
    inline void normal_local_to_world( const double* R, const float lnormal[3], float wnormal[3] ) {
        wnormal[0] = (float)R[0] * lnormal[0] + (float)R[3] * lnormal[1] + (float)R[6] * lnormal[2];
        wnormal[1] = (float)R[1] * lnormal[0] + (float)R[4] * lnormal[1] + (float)R[7] * lnormal[2];
        wnormal[2] = (float)R[2] * lnormal[0] + (float)R[5] * lnormal[1] + (float)R[8] * lnormal[2];
    }

    // rotates normal in the world referential to referential frame assuming R
    // is the rotation matrix of the referential.
    inline void normal_world_to_local( const double* R, const float wnormal[3], float lnormal[3] ) {
        lnormal[0] = (float)R[0] * wnormal[0] + (float)R[1] * wnormal[1] + (float)R[2] * wnormal[2];
        lnormal[1] = (float)R[3] * wnormal[0] + (float)R[4] * wnormal[1] + (float)R[5] * wnormal[2];
        lnormal[2] = (float)R[6] * wnormal[0] + (float)R[7] * wnormal[1] + (float)R[8] * wnormal[2];
    }

    /// computes the rotation matrix that rotates na to nb
    void rotate_normal_to_normal( const double* na, const double* nb, double* Rab );

    void  axisangle_to_quaternion( const double* aa, double* q );
    void quaternion_to_rotation  ( const double* q,  double* R );
    void  axisangle_to_rotation  ( const double* aa, double* R );

    void rotation_matrix_around_z( const float & angle_in_degrees, float  R[9] );
    void rotation_matrix_around_z( const double& angle_in_degrees, double R[9] );

    void euler_to_rotation( double theta, double phi, double psi, double * R );

    // roll : around x (angles in degree)
    // pitch: around y
    // yaw  : around z
    void roll_pitch_yaw_to_rotation( double roll, double pitch, double yaw, double R[9] );

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

    //
    //
    //
    void angle_to_normal( const float& a_in_rad, const float& b_in_rad, float normal[3] );
    void angle_to_normal( const float& a_in_rad, const float& b_in_rad, double normal[3] );
    void normal_to_angle( const double n[3], float& a_in_rad, float& b_in_rad );
    void normal_to_angle( const float  n[3], float& a_in_rad, float& b_in_rad );

    template<typename T1, typename T2>
    double angle_between_normals( const T1 n0[3], const T2 n1[3] ) {
        return std::acos( std::max( std::min( (double)dot3(n0,n1), (double)1.0 ), -1.0 ) ) * DEGREES;
    }

    const float table_sin_degree[360] BYTE_ALIGNED_16 = {
        0.00000000, 0.01745241, 0.03489950, 0.05233596, 0.06975647, 0.08715574,
        0.10452846, 0.12186934, 0.13917310, 0.15643447, 0.17364818, 0.19080900,
        0.20791169, 0.22495105, 0.24192190, 0.25881905, 0.27563736, 0.29237170,
        0.30901699, 0.32556815, 0.34202014, 0.35836795, 0.37460659, 0.39073113,
        0.40673664, 0.42261826, 0.43837115, 0.45399050, 0.46947156, 0.48480962,
        0.50000000, 0.51503807, 0.52991926, 0.54463904, 0.55919290, 0.57357644,
        0.58778525, 0.60181502, 0.61566148, 0.62932039, 0.64278761, 0.65605903,
        0.66913061, 0.68199836, 0.69465837, 0.70710678, 0.71933980, 0.73135370,
        0.74314483, 0.75470958, 0.76604444, 0.77714596, 0.78801075, 0.79863551,
        0.80901699, 0.81915204, 0.82903757, 0.83867057, 0.84804810, 0.85716730,
        0.86602540, 0.87461971, 0.88294759, 0.89100652, 0.89879405, 0.90630779,
        0.91354546, 0.92050485, 0.92718385, 0.93358043, 0.93969262, 0.94551858,
        0.95105652, 0.95630476, 0.96126170, 0.96592583, 0.97029573, 0.97437006,
        0.97814760, 0.98162718, 0.98480775, 0.98768834, 0.99026807, 0.99254615,
        0.99452190, 0.99619470, 0.99756405, 0.99862953, 0.99939083, 0.99984770,
        1.00000000, 0.99984770, 0.99939083, 0.99862953, 0.99756405, 0.99619470,
        0.99452190, 0.99254615, 0.99026807, 0.98768834, 0.98480775, 0.98162718,
        0.97814760, 0.97437006, 0.97029573, 0.96592583, 0.96126170, 0.95630476,
        0.95105652, 0.94551858, 0.93969262, 0.93358043, 0.92718385, 0.92050485,
        0.91354546, 0.90630779, 0.89879405, 0.89100652, 0.88294759, 0.87461971,
        0.86602540, 0.85716730, 0.84804810, 0.83867057, 0.82903757, 0.81915204,
        0.80901699, 0.79863551, 0.78801075, 0.77714596, 0.76604444, 0.75470958,
        0.74314483, 0.73135370, 0.71933980, 0.70710678, 0.69465837, 0.68199836,
        0.66913061, 0.65605903, 0.64278761, 0.62932039, 0.61566148, 0.60181502,
        0.58778525, 0.57357644, 0.55919290, 0.54463904, 0.52991926, 0.51503807,
        0.50000000, 0.48480962, 0.46947156, 0.45399050, 0.43837115, 0.42261826,
        0.40673664, 0.39073113, 0.37460659, 0.35836795, 0.34202014, 0.32556815,
        0.30901699, 0.29237170, 0.27563736, 0.25881905, 0.24192190, 0.22495105,
        0.20791169, 0.19080900, 0.17364818, 0.15643447, 0.13917310, 0.12186934,
        0.10452846, 0.08715574, 0.06975647, 0.05233596, 0.03489950, 0.01745241,
        0.00000000, -0.01745241, -0.03489950, -0.05233596, -0.06975647, -0.08715574,
        -0.10452846, -0.12186934, -0.13917310, -0.15643447, -0.17364818, -0.19080900,
        -0.20791169, -0.22495105, -0.24192190, -0.25881905, -0.27563736, -0.29237170,
        -0.30901699, -0.32556815, -0.34202014, -0.35836795, -0.37460659, -0.39073113,
        -0.40673664, -0.42261826, -0.43837115, -0.45399050, -0.46947156, -0.48480962,
        -0.50000000, -0.51503807, -0.52991926, -0.54463904, -0.55919290, -0.57357644,
        -0.58778525, -0.60181502, -0.61566148, -0.62932039, -0.64278761, -0.65605903,
        -0.66913061, -0.68199836, -0.69465837, -0.70710678, -0.71933980, -0.73135370,
        -0.74314483, -0.75470958, -0.76604444, -0.77714596, -0.78801075, -0.79863551,
        -0.80901699, -0.81915204, -0.82903757, -0.83867057, -0.84804810, -0.85716730,
        -0.86602540, -0.87461971, -0.88294759, -0.89100652, -0.89879405, -0.90630779,
        -0.91354546, -0.92050485, -0.92718385, -0.93358043, -0.93969262, -0.94551858,
        -0.95105652, -0.95630476, -0.96126170, -0.96592583, -0.97029573, -0.97437006,
        -0.97814760, -0.98162718, -0.98480775, -0.98768834, -0.99026807, -0.99254615,
        -0.99452190, -0.99619470, -0.99756405, -0.99862953, -0.99939083, -0.99984770,
        -1.00000000, -0.99984770, -0.99939083, -0.99862953, -0.99756405, -0.99619470,
        -0.99452190, -0.99254615, -0.99026807, -0.98768834, -0.98480775, -0.98162718,
        -0.97814760, -0.97437006, -0.97029573, -0.96592583, -0.96126170, -0.95630476,
        -0.95105652, -0.94551858, -0.93969262, -0.93358043, -0.92718385, -0.92050485,
        -0.91354546, -0.90630779, -0.89879405, -0.89100652, -0.88294759, -0.87461971,
        -0.86602540, -0.85716730, -0.84804810, -0.83867057, -0.82903757, -0.81915204,
        -0.80901699, -0.79863551, -0.78801075, -0.77714596, -0.76604444, -0.75470958,
        -0.74314483, -0.73135370, -0.71933980, -0.70710678, -0.69465837, -0.68199836,
        -0.66913061, -0.65605903, -0.64278761, -0.62932039, -0.61566148, -0.60181502,
        -0.58778525, -0.57357644, -0.55919290, -0.54463904, -0.52991926, -0.51503807,
        -0.50000000, -0.48480962, -0.46947156, -0.45399050, -0.43837115, -0.42261826,
        -0.40673664, -0.39073113, -0.37460659, -0.35836795, -0.34202014, -0.32556815,
        -0.30901699, -0.29237170, -0.27563736, -0.25881905, -0.24192190, -0.22495105,
        -0.20791169, -0.19080900, -0.17364818, -0.15643447, -0.13917310, -0.12186934,
        -0.10452846, -0.08715574, -0.06975647, -0.05233596, -0.03489950, -0.01745241
    };

    const float table_cos_degree[360] BYTE_ALIGNED_16 = {
        1.00000000, 0.99984770, 0.99939083, 0.99862953, 0.99756405, 0.99619470,
        0.99452190, 0.99254615, 0.99026807, 0.98768834, 0.98480775, 0.98162718,
        0.97814760, 0.97437006, 0.97029573, 0.96592583, 0.96126170, 0.95630476,
        0.95105652, 0.94551858, 0.93969262, 0.93358043, 0.92718385, 0.92050485,
        0.91354546, 0.90630779, 0.89879405, 0.89100652, 0.88294759, 0.87461971,
        0.86602540, 0.85716730, 0.84804810, 0.83867057, 0.82903757, 0.81915204,
        0.80901699, 0.79863551, 0.78801075, 0.77714596, 0.76604444, 0.75470958,
        0.74314483, 0.73135370, 0.71933980, 0.70710678, 0.69465837, 0.68199836,
        0.66913061, 0.65605903, 0.64278761, 0.62932039, 0.61566148, 0.60181502,
        0.58778525, 0.57357644, 0.55919290, 0.54463904, 0.52991926, 0.51503807,
        0.50000000, 0.48480962, 0.46947156, 0.45399050, 0.43837115, 0.42261826,
        0.40673664, 0.39073113, 0.37460659, 0.35836795, 0.34202014, 0.32556815,
        0.30901699, 0.29237170, 0.27563736, 0.25881905, 0.24192190, 0.22495105,
        0.20791169, 0.19080900, 0.17364818, 0.15643447, 0.13917310, 0.12186934,
        0.10452846, 0.08715574, 0.06975647, 0.05233596, 0.03489950, 0.01745241,
        0.00000000, -0.01745241, -0.03489950, -0.05233596, -0.06975647, -0.08715574,
        -0.10452846, -0.12186934, -0.13917310, -0.15643447, -0.17364818, -0.19080900,
        -0.20791169, -0.22495105, -0.24192190, -0.25881905, -0.27563736, -0.29237170,
        -0.30901699, -0.32556815, -0.34202014, -0.35836795, -0.37460659, -0.39073113,
        -0.40673664, -0.42261826, -0.43837115, -0.45399050, -0.46947156, -0.48480962,
        -0.50000000, -0.51503807, -0.52991926, -0.54463904, -0.55919290, -0.57357644,
        -0.58778525, -0.60181502, -0.61566148, -0.62932039, -0.64278761, -0.65605903,
        -0.66913061, -0.68199836, -0.69465837, -0.70710678, -0.71933980, -0.73135370,
        -0.74314483, -0.75470958, -0.76604444, -0.77714596, -0.78801075, -0.79863551,
        -0.80901699, -0.81915204, -0.82903757, -0.83867057, -0.84804810, -0.85716730,
        -0.86602540, -0.87461971, -0.88294759, -0.89100652, -0.89879405, -0.90630779,
        -0.91354546, -0.92050485, -0.92718385, -0.93358043, -0.93969262, -0.94551858,
        -0.95105652, -0.95630476, -0.96126170, -0.96592583, -0.97029573, -0.97437006,
        -0.97814760, -0.98162718, -0.98480775, -0.98768834, -0.99026807, -0.99254615,
        -0.99452190, -0.99619470, -0.99756405, -0.99862953, -0.99939083, -0.99984770,
        -1.00000000, -0.99984770, -0.99939083, -0.99862953, -0.99756405, -0.99619470,
        -0.99452190, -0.99254615, -0.99026807, -0.98768834, -0.98480775, -0.98162718,
        -0.97814760, -0.97437006, -0.97029573, -0.96592583, -0.96126170, -0.95630476,
        -0.95105652, -0.94551858, -0.93969262, -0.93358043, -0.92718385, -0.92050485,
        -0.91354546, -0.90630779, -0.89879405, -0.89100652, -0.88294759, -0.87461971,
        -0.86602540, -0.85716730, -0.84804810, -0.83867057, -0.82903757, -0.81915204,
        -0.80901699, -0.79863551, -0.78801075, -0.77714596, -0.76604444, -0.75470958,
        -0.74314483, -0.73135370, -0.71933980, -0.70710678, -0.69465837, -0.68199836,
        -0.66913061, -0.65605903, -0.64278761, -0.62932039, -0.61566148, -0.60181502,
        -0.58778525, -0.57357644, -0.55919290, -0.54463904, -0.52991926, -0.51503807,
        -0.50000000, -0.48480962, -0.46947156, -0.45399050, -0.43837115, -0.42261826,
        -0.40673664, -0.39073113, -0.37460659, -0.35836795, -0.34202014, -0.32556815,
        -0.30901699, -0.29237170, -0.27563736, -0.25881905, -0.24192190, -0.22495105,
        -0.20791169, -0.19080900, -0.17364818, -0.15643447, -0.13917310, -0.12186934,
        -0.10452846, -0.08715574, -0.06975647, -0.05233596, -0.03489950, -0.01745241,
        -0.00000000, 0.01745241, 0.03489950, 0.05233596, 0.06975647, 0.08715574,
        0.10452846, 0.12186934, 0.13917310, 0.15643447, 0.17364818, 0.19080900,
        0.20791169, 0.22495105, 0.24192190, 0.25881905, 0.27563736, 0.29237170,
        0.30901699, 0.32556815, 0.34202014, 0.35836795, 0.37460659, 0.39073113,
        0.40673664, 0.42261826, 0.43837115, 0.45399050, 0.46947156, 0.48480962,
        0.50000000, 0.51503807, 0.52991926, 0.54463904, 0.55919290, 0.57357644,
        0.58778525, 0.60181502, 0.61566148, 0.62932039, 0.64278761, 0.65605903,
        0.66913061, 0.68199836, 0.69465837, 0.70710678, 0.71933980, 0.73135370,
        0.74314483, 0.75470958, 0.76604444, 0.77714596, 0.78801075, 0.79863551,
        0.80901699, 0.81915204, 0.82903757, 0.83867057, 0.84804810, 0.85716730,
        0.86602540, 0.87461971, 0.88294759, 0.89100652, 0.89879405, 0.90630779,
        0.91354546, 0.92050485, 0.92718385, 0.93358043, 0.93969262, 0.94551858,
        0.95105652, 0.95630476, 0.96126170, 0.96592583, 0.97029573, 0.97437006,
        0.97814760, 0.98162718, 0.98480775, 0.98768834, 0.99026807, 0.99254615,
        0.99452190, 0.99619470, 0.99756405, 0.99862953, 0.99939083, 0.99984770 };


}

#endif
