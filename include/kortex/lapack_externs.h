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
#ifndef KORTEX_LAPACK_EXTERNS_H
#define KORTEX_LAPACK_EXTERNS_H

extern "C" {
    // svd
    void dgesvd_(char* jobu, char* jobvt, int* m, int* n,
                 double* A, int* lda,
                 double* S, double* U, int* ldu, double* VT, int* ldvt,
                 double* work, int* lwork,
                 int* info);

    // LU decomposition
    void dgetrf_(int* m, int* n, double* A, int* lda, int* ipiv, int* info );

    // Inversion using LU Decomposition
    void dgetri_(int* n, double* A, int* lda, int* ipiv, double* work, int* lwork, int* info );

    // LSQ LDU Solver
    int dgesv_(int *n, int *nrhs, double *a, int *lda, int *ipiv, double *b, int *ldb, int *info);

    // LSQ cholesky solver
    int dposv_(char *uplo, int *n, int *nrhs, double *a, int *lda, double *b, int *ldb, int *info);

    // LSQ symmetric solver
    int dsysv_(char *uplo, int *n, int *nrhs, double *a, int *lda, int *ipiv, double *b, int *ldb,
               double *work, int *lwork, int *info);

    // LSQ QR solver
    int dgels_(char *trans, int *m, int *n, int *nrhs, double *a, int *lda, double *b, int *ldb,
               double *work, int *lwork, int *info);

    // LSQ SVD solver
    int dgelss_(int *m, int *n, int *nrhs, double *a, int *lda, double *b, int *ldb,
                double *s, double *rcond, int *rank, double *work, int *lwork, int *info);

    // computes the eigenvalues of a hessenberg matrix H and optionally the
    // matrices T and Z from the Schur decomposition H= Z T Z* where T is upper
    // triangular (Schur form of H), and Z is the unitary/orthogonal matrix
    // whose columns are the Schure vectors z_i.
    // A = Q H Q* = (QZ) T (QZ)*
    void dhseqr_(char  * job,      // 'E': only eigenvalues,
                                   // 'S': Schur form T is required,
                 char  * compz,    // 'N': no Schur vectors are computed
                                   // 'I': Schur vectors of H are computed (z is initialized inside)
                                   // 'V': Schur vectors of A are computed
                 int   * n,        // order of the matrixh H (n>=0)
                 int   * ilo,      // if A is balaced with ?gebal then ilo,ihi are retrieved from ?gebal
                 int   * ihi,      // otherwise ilo=1, ihi=n
                 double* H,        // nxn upper Hessenberg matrix
                 int   * ldh,      // leading dimension of H
                 double* wr,       // contains the real part of the eigenvalues     - dimension at least max(1,n)
                 double* wi,       // contains the imaginary part of the eigenvalues
                 double* z,        // compz=='V' -> z must contain the matrix Q from the reduction to Hessenberg form
                                   // compz=='I'/'N' -> z need not be set
                 int   * ldz,      // leading dimension of z
                 double* work,     // at least max(1,n)
                 int   * lwork,    // dimension of array work 11*n maybe required for optimum perf
                 int   * info);    // info==0: successful
}

#endif
