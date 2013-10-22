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

}

#endif
