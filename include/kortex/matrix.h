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
#ifndef KORTEX_MATRIX_H
#define KORTEX_MATRIX_H

namespace kortex {

//
// single mat ops
//
    double  mat_det_3( const double* A, int nra );
    double  mat_trace( const double* A, int nra, int nca );

    void    mat_zero    ( double* A, int nra, int nca );
    void    mat_identity( double* A, int nra, int nca );
    void    mat_cross_form( const double* v, int vsz, double* A, int asz );

    // A[ r, : ] <-- rdata
    void    mat_set_row( double* A, int nra, int nca, int rid, const double* rdata, int rdsz );
    // A[ r, : ] <-- alpha * rdata
    void    mat_set_row( double* A, int nra, int nca, int rid, double alpha, const double* rdata, int rdsz );

    // A[ r, c:c+rdsz] <-- rdata
    void    mat_set_row_from_point( double* A, int nra, int nca, int r0, int c0, const double* rdata, int rdsz );

    void    mat_set_col( double* A, int nra, int nca, int cid, const double* cdata, int cdsz );
    void    mat_set_col( double* A, int nra, int nca, int cid, double alpha, const double* cdata, int cdsz );
    void    mat_set_col_from_point( double* A, int nra, int nca, int r0, int c0,
                                    const double* cdata, int cdsz );

    void    mat_transpose( const double* A, int nra, int nca, double* At, int atsz );

    void    mat_negate   ( double* A, int nra, int nca );
    void    mat_scale    ( double* A, int nra, int nca, double s );
    double  mat_normalize( double* A, int nra, int nca );

    // returns the frobenius norm
    double  mat_norm( const double* A, int nra, int nca );
    double  mat_norm_sq( const double* A, int nra, int nca );

    bool    mat_inv_3( const double* A, int nra,          double* iA, int nria, double inversion_threshold );
    bool    mat_inv  ( const double* A, int nra, int nca, double* iA, int nria, int ncia );

    double  mat_pseudo_inv( const double* A, int nra, int nca, double* iA, int nria, int ncia );

//
// multiple mat ops
//
    void    mat_mat( const double* A, int nra, int nca,
                     const double* B, int nrb, int ncb,
                     double* C, int csz );

    void    mat_mat_trans( const double* A, int nra, int nca,
                           const double* B, int nrb, int ncb,
                           double* C, int csz );

    void    mat_trans_mat( const double* A, int nra, int nca,
                           const double* B, int nrb, int ncb,
                           double* C, int csz );

    void    mat_mat_mat( const double* A, int nra, int nca,
                         const double* B, int nrb, int ncb,
                         const double* C, int nrc, int ncc,
                         double      * D, int dsz );

    void    mat_mat_mat_trans( const double* A, int nra, int nca,
                               const double* B, int nrb, int ncb,
                               const double* C, int nrc, int ncc,
                               double      * D, int dsz );

    void    mat_trans_mat_mat( const double* A, int nra, int nca,
                               const double* B, int nrb, int ncb,
                               const double* C, int nrc, int ncc,
                               double      * D, int dsz );

    void    mat_minus_mat( const double* A, int nra, int nca,
                           const double* B, int nrb, int ncb,
                           double      * C, int nrc, int ncc );

    void    mat_plus_mat( const double* A, int nra, int nca,
                          const double* B, int nrb, int ncb,
                          double      * C, int nrc, int ncc );

    void    mat_mat_elem( const double* A, int nra, int nca,
                          const double* B, int nrb, int ncb,
                          double      * C, int nrc, int ncc );

    void    mat_ABAt( const double* A, int nra, int nca,
                      const double* B, int nrb, int ncb,
                      double      * C, int csz );

//
//
//
    bool    mat_solve_Ax_b_3( const double* A, int nra, int nca,
                              const double* b, int bsz,
                              double      * x, int xsz );

//
// some specialized functions
//

    inline void mat_vec_3( const double* A, const double* x, double* y ) {
        mat_mat( A, 3, 3, x, 3, 1, y, 3 );
    }

    inline void mat_trans_vec_3( const double* A, const double* x, double* y ) {
        mat_trans_mat( A, 3, 3, x, 3, 1, y, 3 );
    }

    inline void mat_mat_3( const double* A, const double* B, double* C ) {
        mat_mat( A, 3, 3, B, 3, 3, C, 9 );
    }

    inline void mat_mat_mat_3( const double A[9], const double B[9], const double C[9], double D[9] ) {
        mat_mat( A, 3, 3, B, 3, 3, C, 3, 3, D, 9 );
    }

}

#endif
