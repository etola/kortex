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

#include <algorithm>
#include <cstring>

#include <kortex/defs.h>
#include <kortex/check.h>

#ifdef DEBUG
#define assert_matrix_compat(op,ra,ca,rb,cb)   { assert_statement_g( matrix_compatibility(op,ra,ca,rb,cb), "incompatible matrices [op %s] [A: %d %d] [B: %d %d]", mat_op_string(op).c_str(), ra, ca, rb, cb ); }
#define assert_matrix_size(op,ra,ca,rb,cb,csz) { assert_statement_g( mat_out_size(op,ra,ca,rb,cb) == csz, "insufficient matrix size for [op %s] [A: %d %d] [B: %d %d] [Csz %d]", mat_op_string(op).c_str(), ra, ca, rb, cb, csz ); }
#define assert_matrix_init(A,ra,ca) { assert_matrix_init_(A,ra,ca); }
#else
#define assert_matrix_compat(op,ra,ca,rb,cb) { }
#define assert_matrix_size(op,ra,ca,rb,cb,csz) {  }
#define assert_matrix_init(A,ra,ca) { }
#endif

#define passert_matrix_compat(op,ra,ca,rb,cb)   { passert_statement_g( matrix_compatibility(op,ra,ca,rb,cb), "incompatible matrices [op %s] [%d %d] [%d %d]", mat_op_string(op).c_str(), ra, ca, rb, cb ); }
#define passert_matrix_size(op,ra,ca,rb,cb,csz) { passert_statement_g( mat_out_size(op,ra,ca,rb,cb) == csz, "insufficient matrix size for [op %s] [A: %d %d] [B: %d %d] [Csz %d]", mat_op_string(op).c_str(), ra, ca, rb, cb, csz ); }

namespace kortex {

    enum MatrixOperation {
        MO_MUL,      // A*B
        MO_T_MUL,    // trans(A)*B
        MO_MUL_T,    // A*trans(B)
        MO_I_MUL,    // inv(A)*B
        MO_PI_MUL,   // pseudo_inv(A)*B
        MO_ELEM      // element-wise operation
    };

    bool matrix_compatibility( const MatrixOperation& op,
                               const int& nra, const int& nca,
                               const int& nrb, const int& ncb );

    int mat_out_size( const MatrixOperation& op,
                      const int& nra, const int& nca,
                      const int& nrb, const int& ncb );

    string mat_op_string( const MatrixOperation& op );

    void assert_matrix_init_( const double* A, int nra, int nca );

//
// single mat ops
//
    double  mat_det_2( const double* A, int nra );
    double  mat_det_3( const double* A, int nra );
    double  mat_trace( const double* A, int nra, int nca );

    void    mat_zero    ( double* A, int nra, int nca );
    void    mat_identity( double* A, int nra, int nca );

    void    mat_cross_form( const double* v, int vsz, double* A, int asz );
    void    mat_cross_form( double x, double y, double z, double* A, int asz );

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

    bool    mat_inv_2( const double* A, int nra, double* iA, int nria, double inversion_threshold );
    bool    mat_inv_3( const double* A, int nra,          double* iA, int nria, double inversion_threshold );
    bool    mat_inv  ( const double* A, int nra, int nca, double* iA, int nria, int ncia );

    inline bool mat_inv_3( const double A[9], double iA[9] ) {
        return mat_inv_3( A, 3, iA, 3, 0.0 );
    }

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

    bool mat_inv_mat_mat( const double* A, int nra, int nca,
                          const double* B, int nrb, int ncb,
                          const double* C, int nrc, int ncc,
                          double* D, int dsz );


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

    int     mat_print_line( char* cstr, int csz, const double* A, int asz, const char* mat_name );

    /// dst[ dr:dr+srsz, dc:dc+scsz ] = src[ sr:sr+srsz, sc:sc+scsz ]
    void mat_copy( const double* src, int nrs, int ncs,
                   int sr, int sc, int srsz, int scsz,
                   double* dst, int nrd, int ncd,
                   int dr, int dc );

    /// B = A
    void mat_copy( const double* A, int asz, double* B, int bsz );

    bool mat_is_upper_hessenberg( const double* A, int nra, int nca );

    /// AtA = A' * A : computes only upper part
    void mat_trans_mat_upper( const double* A, int ar,  int ac, double* AtA, int ata_sz );

    /// makes matrix symmetric by copying keep_upper [true: upper->lower, false:
    /// lower->upper];
    void mat_sym( double* A, int nra, int nca, bool keep_upper );

//
// some specialized functions
//
    bool    mat_solve_Ax_b_3( const double* A, int nra, int nca,
                              const double* b, int bsz,
                              double      * x, int xsz );

    /// z = Ax + y
    inline void mat_Axpy_3( const double A[9], const double x[3], const double y[3], double z[3] ) {
        for( int r=0; r<3; r++ ) {
            const double* ar = A + 3*r;
            z[r] = ar[0]*x[0] + ar[1]*x[1] + ar[2]*x[2] + y[r];
        }
    }


    /// decompose matrix A so that U is an upper triangular matrix and R is an
    /// orthonormal matrix and A = U*R
    bool rq_givens_decomposition_3( double A[9], double U[9], double R[9] );

    inline void mat_vec_3( const double* A, const double* x, double* y ) {
        mat_mat( A, 3, 3, x, 3, 1, y, 3 );
    }

    inline void mat_trans_vec_3( const double* A, const double* x, double* y ) {
        mat_trans_mat( A, 3, 3, x, 3, 1, y, 3 );
    }

    inline void mat_mat_3( const double* A, const double* B, double* C ) {
        mat_mat( A, 3, 3, B, 3, 3, C, 9 );
    }

    inline void mat_trans_mat_mat_3( const double* A, const double* B, const double* C, double* D ) {
        mat_trans_mat_mat( A, 3, 3, B, 3, 3, C, 3, 3, D, 9 );
    }

    inline void mat_minus_mat_3( const double* A, const double* B, double* C ) {
        mat_minus_mat( A, 3, 3, B, 3, 3, C, 3, 3 );
    }

    inline void mat_plus_mat_3( const double* A, const double* B, double* C ) {
        mat_plus_mat( A, 3, 3, B, 3, 3, C, 3, 3 );
    }

    /// C = inv(A)*B
    bool mat_inv_mat_3( const double A[9], const double B[9], double C[9] );

    /// C = A*inv(B)
    bool mat_mat_inv_3( const double A[9], const double B[9], double C[9] );

    /// D = inv(A) * B * C
    bool mat_inv_mat_mat_3( const double A[9], const double B[3], const double C[3], double D[9] );

    /// D = A * B * inv(C)
    bool mat_mat_mat_inv_3( const double A[9], const double B[9], const double C[9], double D[9] );

    inline void mat_mat_mat_3( const double A[9], const double B[9], const double C[9], double D[9] ) {
        mat_mat_mat( A, 3, 3, B, 3, 3, C, 3, 3, D, 9 );
    }

    inline void mat_transpose_3( double A[9] ) {
        std::swap( A[1], A[3] );
        std::swap( A[2], A[6] );
        std::swap( A[5], A[7] );
    }

    inline void mat_transpose_3( const double A[9], double At[9] ) {
        assert_noalias_p( A, At );
        memcpy( At, A, sizeof(*A)*9 );
        mat_transpose_3( At );
    }

    inline void vec_negate( const double* a, int asz, double* b, int bsz ) {
        assert_pointer( a );
        assert_pointer( b );
        assert_pointer_size( asz );
        assert_pointer_size( bsz );
        assert_statement( asz == bsz, "size mismatch" );
        for( int i=0; i<asz; i++ )
            b[i] = -a[i];
    }

    inline void vec_negate( double* a, int asz ) {
        vec_negate( a, asz, a, asz );
    }

    inline void vec_minus_vec_3( const double a[3], const double b[3], double c[3] ) {
        c[0] = a[0]-b[0];
        c[1] = a[1]-b[1];
        c[2] = a[2]-b[2];
    }

    inline void vec_copy_3( const double src[3], double dst[3] ) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
    }

    inline double vec_norm_3( const double X[3] ) {
        return sqrt( X[0]*X[0] + X[1]*X[1] + X[2]*X[2] );
    }

    inline void vec_scale_3( double X[3], double alpha ) {
        X[0] *= alpha;
        X[1] *= alpha;
        X[2] *= alpha;
    }

    inline void   vec_normalize_3( double X[3] ) {
        double nrm = vec_norm_3( X );
        if( nrm < NRM_EPS ) {
            X[0] = X[1] = X[2] = 0.0;
        } else {
            nrm = 1.0/nrm;
            vec_scale_3( X, nrm );
        }
    }

    inline void mat_vec_concat_hor_3( const double r[9], const double t[3], double rt[12] ) {
        mat_copy( r, 3, 3, 0, 0, 3, 3, rt, 3, 4, 0, 0 );
        mat_copy( t, 3, 1, 0, 0, 3, 1, rt, 3, 4, 0, 3 );
    }
    inline void mat_vec_deconcat_hor_3( const double rt[12], double r[3], double t[3] ) {
        mat_copy( rt, 3, 4, 0, 0, 3, 3, r, 3, 3, 0, 0 );
        mat_copy( rt, 3, 4, 0, 3, 3, 1, t, 3, 1, 0, 0 );
    }

    /// A[rid, :] *= alpha
    void mat_scale_row_3( double* A, int rid, double alpha );

    void mat_add_3( const double* A, double* sA );

}

#endif
