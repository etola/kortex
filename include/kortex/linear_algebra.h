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

namespace kortex {

    class MemUnit;

    int matrix_invert_g_lu( const double* A, int ar, double* iA );

    double matrix_pseudo_invert_g_svd( const double* A, int ar, int ac, double* iA );


    // Least Square Solvers
    // ----------------------------------------------------------- The functions
    // in this file provide a C++ interface to Lapack functions. They expect
    // matrices to be in !!! COLUMN MAJOR !!!  form contrary to the rest of the
    // kortex library. This is done to preserve functionality and efficiency at
    // the same time.
    // -----------------------------------------------------------

    /// Solve the square system Ax = b for each column of B, A ~ (n x n), B ~ (n
    /// x nrhs), pivot_info (output) ~ (n x 1)
    int lsq_solver_LDU(double* A, int n, int lda,
                       double* B, int nrhs, int ldb, int* pivot_info);

    /// Solve the symmetric positive definite system Ax = b for each column of
    /// B, A ~ (n x n), B ~ (n x nrhs). If 'upper' is true then the upper
    /// triangular part of A is used, otherwise the lower triangular part.
    int lsq_solver_cholesky(double* A, int n, int lda,
                            double* B, int nrhs, int ldb, bool upper);

    /// Solve the symmetric indefinite system Ax = b for each column of B, A ~
    /// (n x n), B ~ (n x nrhs), pivot_info (output) ~ (n x 1). If 'upper' is
    /// true then the upper triangular part of A is used, otherwise the lower
    /// triangular part.
    int lsq_solver_symmetric(double* A, int n, int lda,
                             double* B, int nrhs, int ldb, int* pivot_info,
                             bool upper, MemUnit* memory);

    /// If transp A = false then solve the least squares problem using QR
    /// factorization min_x |A*x - b|_2 for each column of B, A ~ (nr x nc), B ~
    /// (nr x nrhs) If transp A = true then solve the least squares problem
    /// using LQ factorization.  min_x |At*x - b|_2 for each column of B, A ~
    /// (nr x nc), B ~ (nc x nrhs)
    int lsq_solver_QR(double* A, int nr, int nc, int lda,
                      double* B, int nrhs, int ldb,
                      bool transp_A, MemUnit* memory);

    /// Solve the least squares problem min_x |A*x - b|_2 for each column of B
    /// using SVD, A ~ (nr x nc), B ~ (nr x nrhs). The effective rank of A is
    /// determined by treating as zero the singular values smaller than the
    /// largest singular value times 'rcond'.  If 'rcond' < 0 then machine
    /// precision is used.
    int lsq_solver_SVD(double* A, int nr, int nc, int lda,
                       double* B, int nrhs, int ldb,
                       double* sing_val, double rcond, int& rank,
                       MemUnit* memory);

    /// min_x |A*x - b|_2 for each column of B, A ~ (nr x nc), B ~ (nr x nrhs)
    /// If transp A = true then solve the least squares problem using LQ
    /// factorization.  min_x |At*x - b|_2 for each column of B, A ~ (nr x nc),
    /// B ~ (nc x nrhs)
    int lapack_lsq_solve_QR(double* A, int nr, int nc, int lda,
                            double* B, int nrhs, int ldb, bool transp_A,
                            MemUnit* memory);

    // Least Square Solvers


    bool mat_eigenvalues_upper_hessenberg( const double* H, int nra, double* eigs_r, double* eigs_i, int n_eigs, MemUnit& mem );


}

#endif
