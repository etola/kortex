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

#include <kortex/linear_algebra.h>
#include <kortex/lapack_externs.h>
#include <kortex/check.h>
#include <kortex/mem_manager.h>
#include <kortex/mem_unit.h>
#include <kortex/svd.h>
#include <kortex/matrix.h>

#include <cstring>

namespace kortex {

    int matrix_invert_g_lu( const double* A, int ar, double* iA ) {
        passert_pointer( A && iA );

        memcpy( iA, A, sizeof(*iA)*ar*ar );

        int m = ar;
        double work = 0;
        int lwork = -1;
        int info;
        dgetri_(&m, NULL, &m, NULL, &work, &lwork, &info );
        lwork = (int)work;

        double* workspace = NULL;
        allocate( workspace, lwork );

        int* ipiv = NULL;
        allocate( ipiv, ar );
        dgetrf_(&ar, &ar, iA, &ar, ipiv, &info );
        if( info != 0 ) {
            logman_warning("LU Decomposition failed");
            return 1;
        }
        dgetri_(&ar, iA, &ar, ipiv, workspace, &lwork, &info );

        deallocate(workspace);
        deallocate(ipiv);

        return info;
    }

    double matrix_pseudo_invert_g_svd( const double* A, int ar, int ac, double* iA ) {
        SVD svd;
        svd.decompose( A, ar, ac, true, true );
        double cond = svd.pseudo_inverse( iA, ar );
        return cond;
    }

    // Least Square Solvers
    int lsq_solver_LDU(double* A, int n, int lda,
                       double* B, int nrhs, int ldb, int* pivot_info) {
        int info;
        dgesv_(&n, &nrhs, A, &lda, pivot_info, B, &ldb, &info);
        return info;
    }

    int lsq_solver_cholesky(double* A, int n, int lda,
                            double* B, int nrhs, int ldb, bool upper) {
        int info;
        char uplo = (upper ? 'U' : 'L');
        dposv_(&uplo, &n, &nrhs, A, &lda, B, &ldb, &info);
        return info;
    }

    int lsq_solver_symmetric_lwork(int n, int lda, int nrhs, int ldb, bool upper) {
        int     info;
        char        uplo = (upper ? 'U' : 'L');
        double* A = NULL;
        double* B = NULL;
        int*       pivot_info = NULL;
        double  work;
        int     lwork = -1;
        dsysv_(&uplo, &n, &nrhs, A, &lda, pivot_info, B, &ldb, &work, &lwork, &info);
        return !info ? (int)work : 1;
    }
    int lsq_solver_symmetric(double* A, int n, int lda,
                             double* B, int nrhs, int ldb, int* pivot_info, bool upper, MemUnit* memory) {
        int info;
        char uplo = (upper ? 'U' : 'L');
        int lwork = lsq_solver_symmetric_lwork(n, lda, nrhs, ldb, upper);
        memory->resize( lwork*sizeof(*A) );
        double * work = (double*)memory->get_buffer();
        dsysv_(&uplo, &n, &nrhs, A, &lda, pivot_info, B, &ldb, work, &lwork, &info);
        return info;
    }


    int lsq_solver_QR_lwork(int nr, int nc, int lda, int nrhs, int ldb, bool transp_A) {
        int     info;
        char        trans = (transp_A ? 'T' : 'N');
        double* A = NULL;
        double* B = NULL;
        double  work;
        int        lwork = -1;
        dgels_(&trans, &nr, &nc, &nrhs, A, &lda, B, &ldb, &work, &lwork, &info);
        return !info ? (int)work : 1;
    }
    int lsq_solver_QR(double* A, int nr, int nc, int lda, double* B, int nrhs, int ldb,
                      bool transp_A, MemUnit* memory) {
        int info;
        char trans = (transp_A ? 'T' : 'N');
        int lwork = lsq_solver_QR_lwork(nr, nc, lda, nrhs, ldb, transp_A);
        memory->resize( lwork*sizeof(*A) );
        double* work = (double*)memory->get_buffer();
        dgels_(&trans, &nr, &nc, &nrhs, A, &lda, B, &ldb, work, &lwork, &info);
        return info;
    }

    int lsq_solver_SVD_lwork(int nr, int nc, int lda, int nrhs, int ldb) {
        int     info;
        int     rank;
        double* A = NULL;
        double* B = NULL;
        double* sing_val = NULL;
        double  rcond = -1.0;
        double  work;
        int        lwork = -1;
        dgelss_(&nr, &nc, &nrhs, A, &lda, B, &ldb,
                sing_val, &rcond, &rank, &work, &lwork, &info);
        return !info ? (int)work : 1;
    }
    int lsq_solver_SVD(double* A, int nr, int nc, int lda,
                       double* B, int nrhs, int ldb,
                       double* sing_val, double rcond, int& rank,
                       MemUnit* memory) {
        int info;
        int lwork = lsq_solver_SVD_lwork(nr, nc, lda, nrhs, ldb);
        memory->resize( lwork*sizeof(*A) );
        double* work = (double*)memory->get_buffer();
        dgelss_(&nr, &nc, &nrhs, A, &lda, B, &ldb,
                sing_val, &rcond, &rank, work, &lwork, &info);
        return info;
    }

    int lsq_solve_QR_lwork( int nr, int nc, int lda, int nrhs, int ldb, bool transp_A ) {
        int     info;
        char        trans = (transp_A ? 'T' : 'N');
        double* A = NULL;
        double* B = NULL;
        double  work;
        int        lwork = -1;
        dgels_(&trans, &nr, &nc, &nrhs, A, &lda, B, &ldb, &work, &lwork, &info);
        return !info ? (int)work : 1;
    }

    int lapack_lsq_solve_QR(double* A, int nr, int nc, int lda,
                            double* B, int nrhs, int ldb, bool transp_A,
                            MemUnit* memory) {

        int info;
        int lwork = lsq_solver_QR_lwork(nr, nc, lda, nrhs, ldb, transp_A);
        memory->resize( lwork*sizeof(*A) );
        double* work = (double*)memory->get_buffer();
        char trans = (transp_A ? 'T' : 'N');
        dgels_(&trans, &nr, &nc, &nrhs, (double*)A, &lda, (double*)B, &ldb, (double*)work, &lwork, &info);
        return info;
    }


//
    bool mat_eigenvalues_upper_hessenberg( const double* H, int nra, double* eigs_r, double* eigs_i, int n_eigs, MemUnit& mem ) {

        assert_pointer( H );
        assert_pointer_size( nra );
        assert_pointer( eigs_r );
        assert_pointer( eigs_i );
        assert_statement_g( n_eigs >= nra, "eigenvalue array size insufficient [%f/%f]", n_eigs, nra );
        assert_statement( mat_is_upper_hessenberg( H, nra, nra ), "matrix H should be upper hessenberg" );

        mem.resize( sizeof(*H) * ( nra*nra + nra*11 ) );
        double * A    = (double*)( mem.get_buffer() );
        double * work = A + nra*nra;
        int     lwork = nra*11;

        mat_transpose( H, nra, nra, A, nra*nra );

        char job   = 'E';
        char compz = 'N';
        int    ilo = 1;
        int    ihi = nra;
        double z = 0;
        int   ldz = 1;

        int info;
        dhseqr_(&job, &compz, &nra, &ilo, &ihi, A, &nra, eigs_r, eigs_i, &z, &ldz, work, &lwork, &info );

        return bool(info==0);
    }



}
