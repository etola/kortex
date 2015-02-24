#ifdef WITH_LAPACK

#include <kortex/lapack_externs.h>
#include <kortex/mem_manager.h>

namespace kortex {

    int matrix_invert_g_lu( const double* A, int ar, double* iA ) {
        passert_pointer( A && iA );
        assert_pointer_size( ar );

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
            logman_info("LU Decomposition failed");
            return 1;
        }
        dgetri_(&ar, iA, &ar, ipiv, workspace, &lwork, &info );

        deallocate(workspace);
        deallocate(ipiv);

        return info;
    }

    void lsq_solver_svd( const KMatrix& A, const KMatrix& B, KMatrix& x ) {
        assert_statement( A.h() == B.h(), "invalid matrices" );

        KMatrix A_co( A.h(), A.w() );
        row_to_column_order( A, A_co );

        int m    = A.h();
        int n    = A.w();
        int nrhs = B.w();
        int lda  = A.h();
        int ldb  = std::max(m,n);

        KMatrix B_co( ldb, nrhs );
        row_to_column_order( B, B_co );

        x.init( n, nrhs );
        x.zero();

        int    rank;
        double rcond = -1;
        double work;
        int    lwork = -1;
        int    info;
        dgelss_( &m, &n, &nrhs, NULL, &lda, NULL, &ldb,
                 NULL, &rcond, &rank, &work, &lwork, &info );

        if( info ) {
            logman_warning( "dgelss_ failure to compute req memory" );
            return;
        }
        lwork = (int)work;
        MemUnit memory;
        memory.resize( (lwork + std::min(m,n))*sizeof(double) );
        double* dmem = (double*)memory.get_buffer();

        double* S = dmem + lwork;

        dgelss_( &m, &n, &nrhs, A_co.get_pointer(), &lda, B_co.get_pointer(), &ldb,
                 S, &rcond, &rank, dmem, &lwork, &info );
        if( info ) {
            logman_error_g( "dgelss_ cannot solve system [%d]", info );
            return;
        }

        column_to_row_order( B_co, x );
        assert_array( "BLAS - SVD LSQ", x(), x.size() );
    }

    void lsq_solver_cholesky( const KMatrix& A, const KMatrix& B, KMatrix& x ) {
        assert_statement( is_symmetric( A(), A.h(), A.w() ), "A is not symmetric - call lsq_solver_svd" );
        assert_statement( A.h() == B.h(), "invalid matrices" );

        int info;
        char uplo = 'U';

        KMatrix B_co( B.h(), B.w() );
        row_to_column_order( B, B_co );

        int n    = A.h();
        int nrhs = B.w();
        int lda  = A.w();
        int ldb  = B.h();

        KMatrix ta( A );
        dposv_( &uplo, &n, &nrhs, ta.get_pointer(), &lda, B_co.get_pointer(), &ldb, &info );

        if( info ) {
            logman_error_g( "dposv failure [%d]", info);
        }

        x.init( B_co.h(), B_co.w() );
        column_to_row_order( B_co, x );
        assert_array( "BLAS - Cholesky LSQ", x(), x.size() );
    }

    void lsq_solver_svd( const double* A, int ar, int ac, int lda,
                         const double* B, int bc, int ldb,
                         double* x, int xsz ) {
        KMatrix tA; initialize( A, ar, ac, lda, tA );
        KMatrix tB; initialize( B, ar, bc, ldb, tB );
        KMatrix tx;
        lsq_solver_svd( tA, tB, tx );
        initialize( tx, x, xsz );
    }

    void lsq_solver_cholesky( const double* A, int ar, int lda,
                              const double* B, int bc, int ldb,
                              double* x, int xsz ) {
        KMatrix tA; initialize( A, ar, ar, lda, tA );
        KMatrix tB; initialize( B, ar, bc, ldb, tB );
        KMatrix tx;
        lsq_solver_cholesky( tA, tB, tx );
        initialize( tx, x, xsz );
    }

    bool mat_eigenvalues_upper_hessenberg( const KMatrix& H, vector<double>& eig_real, vector<double>& eig_imag ) {

        eig_real.clear();
        eig_imag.clear();

        assert_statement( H.is_square(), "matrix has to be square" );
        assert_statement( H.h() > 0, "empty matrix" );

        assert_statement( mat_is_upper_hessenberg( H(), H.h(), H.w() ), "matrix H should be upper hessenberg" );

        int m = H.h();

        MemUnit mem;
        int    lwork = m*m*11;
        mem.resize( ( 2*m + lwork ) * sizeof(double) );
        double* work = mem.get_buffer();

        KMatrix tH = H;
        tH.transpose();

        double* eigs_r = work + lwork;
        double* eigs_i = eigs_r + m;

        char job   = 'E';
        char compz = 'N';
        int    ilo = 1;
        int    ihi = m;
        double   z = 0;
        int    ldz = 1;
        int    info;

        dhseqr_(&job, &compz, &m, &ilo, &ihi, tH.get_pointer(), &m, eigs_r, eigs_i, &z, &ldz, work, &lwork, &info );

        if( info != 0 )
            return false;

        eig_real.resize( m );
        eig_imag.resize( m );
        for( int i=0; i<m; i++ ) {
            eig_real[i] = eigs_r[i];
            eig_imag[i] = eigs_i[i];
        }
        return true;
    }

    // bool mat_eigenvalues_upper_hessenberg( const double* H, int nra, double* eigs_r, double* eigs_i, int n_eigs, MemUnit& mem ) {
        // assert_pointer( H );
        // assert_pointer_size( nra );
        // assert_pointer( eigs_r );
        // assert_pointer( eigs_i );
        // assert_statement_g( n_eigs >= nra, "eigenvalue array size insufficient [%f/%f]", n_eigs, nra );
        // assert_statement( mat_is_upper_hessenberg( H, nra, nra ), "matrix H should be upper hessenberg" );
        // mem.resize( sizeof(*H) * ( nra*nra + nra*11 ) );
        // double * A    = (double*)( mem.get_buffer() );
        // double * work = A + nra*nra;
        // int     lwork = nra*11;
        // mat_transpose( H, nra, nra, A, nra*nra );
        // char job   = 'E';
        // char compz = 'N';
        // int    ilo = 1;
        // int    ihi = nra;
        // double z = 0;
        // int   ldz = 1;
        // int info;
        // dhseqr_(&job, &compz, &nra, &ilo, &ihi, A, &nra, eigs_r, eigs_i, &z, &ldz, work, &lwork, &info );
        // return bool(info==0);
    // }

    bool find_eigenvalues( const KMatrix& M, vector<double>& eig_real, vector<double>& eig_imag ) {
        return mat_eigenvalues_upper_hessenberg( M, eig_real, eig_imag );
    }

}

#endif
