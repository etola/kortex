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

#ifdef WITH_LAPACK

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
    //
    // DGESV computes the solution to a real system of linear equations
    //     A * X = B,
    //  where A is an N-by-N matrix and X and B are N-by-NRHS matrices.
    //
    //  The LU decomposition with partial pivoting and row interchanges is
    //  used to factor A as
    //     A = P * L * U,
    //  where P is a permutation matrix, L is unit lower triangular, and U is
    //  upper triangular.  The factored form of A is then used to solve the
    //  system of equations A * X = B.
    void dgesv_(int *n, int *nrhs, double *a, int *lda, int *ipiv, double *b, int *ldb, int *info);
    void sgesv_(int* n, int* nrhs, float  *a, int *lda, int *ipiv, float  *b, int* ldb, int* info );

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


    //  DSPOSV computes the solution to a real system of linear equations
    //     A * X = B,
    //  where A is an N-by-N symmetric positive definite matrix and X and B
    //  are N-by-NRHS matrices.
    //
    //  DSPOSV first attempts to factorize the matrix in SINGLE PRECISION
    //  and use this factorization within an iterative refinement procedure
    //  to produce a solution with DOUBLE PRECISION normwise backward error
    //  quality (see below). If the approach fails the method switches to a
    //  DOUBLE PRECISION factorization and solve.
    //
    //  The iterative refinement is not going to be a winning strategy if
    //  the ratio SINGLE PRECISION performance over DOUBLE PRECISION
    //  performance is too small. A reasonable strategy should take the
    //  number of right-hand sides and the size of the matrix into account.
    //  This might be done with a call to ILAENV in the future. Up to now, we
    //  always try iterative refinement.
    //
    //  The iterative refinement process is stopped if
    //      ITER > ITERMAX
    //  or for all the RHS we have:
    //      RNRM < SQRT(N)*XNRM*ANRM*EPS*BWDMAX
    //  where
    //      o ITER is the number of the current iteration in the iterative
    //        refinement process
    //      o RNRM is the infinity-norm of the residual
    //      o XNRM is the infinity-norm of the solution
    //      o ANRM is the infinity-operator-norm of the matrix A
    //      o EPS is the machine epsilon returned by DLAMCH('Epsilon')
    //  The value ITERMAX and BWDMAX are fixed to 30 and 1.0D+00
    //  respectively.
    int sposv_( char* uplo, int* n, int* nrhs, float* a, int* lda,
                float* b, int* ldb, int* info );
    // LSQ cholesky solver
    int dposv_(char *uplo, int *n, int *nrhs, double *a, int *lda, double *b, int *ldb, int *info);


    // SSYSV computes the solution to a real system of linear equations
    // A * X = B,
    // where A is an N-by-N symmetric matrix and X and B are N-by-NRHS
    // matrices.
    //
    // The diagonal pivoting method is used to factor A as
    // A = U * D * U**T,  if UPLO = 'U', or
    // A = L * D * L**T,  if UPLO = 'L',
    // where U (or L) is a product of permutation and unit upper (lower)
    // triangular matrices, and D is symmetric and block diagonal with
    // 1-by-1 and 2-by-2 diagonal blocks.  The factored form of A is then
    // used to solve the system of equations A * X = B.
    int ssysv_( char* uplo, int* n, int* nrhs, float* a, int* lda,
                 int* ipiv, float* b, int* ldb, float* work, int* lwork, int* info );
    // LSQ symmetric solver
    int dsysv_(char *uplo, int *n, int *nrhs, double *a, int *lda, int *ipiv, double *b, int *ldb,
               double *work, int *lwork, int *info);


    // SGELS solves overdetermined or underdetermined real linear systems
    //  involving an M-by-N matrix A, or its transpose, using a QR or LQ
    //  factorization of A.  It is assumed that A has full rank.
    //
    //  The following options are provided:
    //
    //  1. If TRANS = 'N' and m >= n:  find the least squares solution of
    //     an overdetermined system, i.e., solve the least squares problem
    //                  minimize || B - A*X ||.
    //
    //  2. If TRANS = 'N' and m < n:  find the minimum norm solution of
    //     an underdetermined system A * X = B.
    //
    //  3. If TRANS = 'T' and m >= n:  find the minimum norm solution of
    //     an undetermined system A**T * X = B.
    //
    //  4. If TRANS = 'T' and m < n:  find the least squares solution of
    //     an overdetermined system, i.e., solve the least squares problem
    //                  minimize || B - A**T * X ||.
    //
    //  Several right hand side vectors b and solution vectors x can be
    //  handled in a single call; they are stored as the columns of the
    //  M-by-NRHS right hand side matrix B and the N-by-NRHS solution
    int sgels_( char* trans, int* m, int* n, int* nrhs, float* a, int* lda,
                 float* b, int* ldb, float* work, int* lwork, int* info );
    // LSQ QR solver
    int dgels_(char *trans, int *m, int *n, int *nrhs, double *a, int *lda, double *b, int *ldb,
               double *work, int *lwork, int *info);



    // SGELSS computes the minimum norm solution to a real linear least squares
    // problem:
    //
    // Minimize 2-norm(| b - A*x |).
    //
    // using the singular value decomposition (SVD) of A. A is an
    // M-by-N matrix which may be rank-deficient.
    //
    // Several right hand side vectors b and solution vectors x can
    // be handled in a single call; they are stored as the columns
    // of the M-by-NRHS right hand side matrix B and the N-by-NRHS
    // solution matrix X.
    //
    // The effective rank of A is determined by treating as zero those singular
    // values which are less than RCOND times the largest singular value.
    int sgelss_( int* m, int* n, int* nrhs, float* a, int* lda, float* b, int* ldb,
                  float* s, float* rcond, int* rank, float* work, int* lwork, int* info );
    // LSQ SVD solver
    int dgelss_(int *m, int *n, int *nrhs, double *a, int *lda, double *b, int *ldb,
                double *s, double *rcond, int *rank, double *work, int *lwork, int *info);


    // DGEEV computes for an N-by-N real nonsymmetric matrix A, the eigenvalues
    // and, optionally, the left and/or right eigenvectors.
    //
    // The right eigenvector v(j) of A satisfies
    //                  A * v(j) = lambda(j) * v(j)
    // where lambda(j) is its eigenvalue.
    // The left eigenvector u(j) of A satisfies
    //               u(j)**H * A = lambda(j) * u(j)**H
    // where u(j)**H denotes the conjugate-transpose of u(j).
    //
    // The computed eigenvectors are normalized to have Euclidean norm equal to
    // 1 and largest component real.
    void dgeev_( char* jobvl, char* jobvr, int* n, double* A, int* lda,
                 double* wr, double* wi, double* vl, int* ldvl,  double* vr, int* ldvr,
                 double* work, int* lwork, int* info );

}

#endif

#endif
