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
#ifndef KORTEX_LINEAR_ALGEBRA_H
#define KORTEX_LINEAR_ALGEBRA_H

#include <kortex/kmatrix.h>

namespace kortex {

    /// computes the right null vector of A - returns only 1 null
    /// vector. possible to return multiple ones by checking a tolerance... see
    /// null implementation of octave
    void mat_null( const double* A, int ar, int ac, double* v, int vsz );

    inline void mat_null_3( const double A[9], double v[3] ) {
        mat_null( A, 3, 3, v, 3 );
    }

    int matrix_invert_g_lu( const double* A, int ar, double* iA );

    double matrix_pseudo_invert_g_svd( const double* A, int ar, int ac, double* iA );

    // Least Square Solvers

    /// A: ar x ac -> lda = ac if A is not a sub-matrix
    /// B: ar x bc -> ldb = bc if B is not a sub-matrix
    void lsq_solver_svd( const double* A, int ar, int ac, int lda,
                         const double* B, int bc, int ldb,
                         double* x, int xsz );
    void lsq_solver_svd( const KMatrix& A, const KMatrix& B, KMatrix& x );

    /// A needs to be symmetric
    /// A is ar x ar -> lda = ar if A is not a sub-matrix
    /// B is ar x bc -> ldb = ar if B is not a sub-matrix
    void lsq_solver_cholesky( const double* A, int ar, int lda,
                              const double* B, int bc, int ldb,
                              double* x, int xsz );
    void lsq_solver_cholesky( const KMatrix& A, const KMatrix& B, KMatrix& x );

    bool find_eigenvalues( const KMatrix& M, vector<double>& eig_real, vector<double>& eig_imag );

    inline bool find_eigenvalues( const double* M, int nr, vector<double>& eig_real, vector<double>& eig_imag ) {
        KMatrix wM( M, nr, nr );
        return find_eigenvalues( wM, eig_real, eig_imag );
    }

    ///
    /// Generalized eigenvalue, eigenvector decomposition. There are more efficient
    /// versions for more well-behaving ( symmetric, hessenberg... ) matrices.
    ///
    /// eigenvalues are stored within eval_r, eval_i (real+imaginary)
    ///
    /// EVEC_R/VR is DOUBLE PRECISION array, dimension (N,N). The right eigenvectors
    ///   v(j) are stored one after another in the columns of VR, in the same order
    ///   as their eigenvalues. If the j-th eigenvalue is real, then v(j) = VR(:,j),
    ///   the j-th column of VR.  If the j-th and (j+1)-st eigenvalues form a complex
    ///   conjugate pair, then v(j) = VR(:,j) + i*VR(:,j+1) and v(j+1) = VR(:,j) -
    ///   i*VR(:,j+1).
    ///
    /// EVEC_L -> left eigenvectors of A - stored similarly as EVEC_R
    ///
    bool mat_eigen( const KMatrix& A, KMatrix& eval_r, KMatrix& eval_i,
                    KMatrix* evec_r, KMatrix* evec_l );

    /// extracts the real eigenvalues and eigenvectors
    bool mat_eigen_real( const KMatrix& A, KMatrix& eval, KMatrix* evec_r, KMatrix* evec_l=NULL );


}

#endif
