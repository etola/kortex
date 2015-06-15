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
        double* work = (double*)mem.get_buffer();

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

    //
    // Generalized eigenvalue, eigenvector decomposition. There are more efficient
    // versions for more well-behaving ( symmetric, hessenberg... ) matrices.
    //
    // eigenvalues are stored within eval_r, eval_i (real+imaginary)
    //
    // EVEC_R/VR is DOUBLE PRECISION array, dimension (N,N). The right eigenvectors
    //   v(j) are stored one after another in the columns of VR, in the same order
    //   as their eigenvalues. If the j-th eigenvalue is real, then v(j) = VR(:,j),
    //   the j-th column of VR.  If the j-th and (j+1)-st eigenvalues form a complex
    //   conjugate pair, then v(j) = VR(:,j) + i*VR(:,j+1) and v(j+1) = VR(:,j) -
    //   i*VR(:,j+1).
    //
    // EVEC_L -> left eigenvectors of A - stored similarly as EVEC_R
    //
    bool mat_eigen( const KMatrix& A,
                    KMatrix& eval_r, KMatrix& eval_i,
                    KMatrix* evec_r, KMatrix* evec_l ) {

        assert_statement( A.is_square(), "matrix has to be square" );
        assert_statement( A.h() > 0, "empty matrix" );

        char jobvl = 'N';
        char jobvr = 'N';
        if( evec_r ) jobvr = 'V';
        if( evec_l ) jobvl = 'V';

        KMatrix Atmp;
        mat_transpose(A, Atmp);

        int n = A.h();

        eval_r.resize(n,1);
        eval_i.resize(n,1);
        if( evec_r ) evec_r->resize(n,n);
        if( evec_l ) evec_l->resize(n,n);


        double* vec_l = NULL;
        double* vec_r = NULL;
        if( evec_l ) vec_l = evec_l->get_pointer();
        if( evec_r ) vec_r = evec_r->get_pointer();

        int info = 0;
        int lwork = -1;
        double work_sz[1];
        dgeev_( &jobvl, &jobvr, &n, Atmp.get_pointer(), &n,
                eval_r.get_pointer(), eval_i.get_pointer(),
                vec_l, &n, vec_r, &n, work_sz, &lwork, &info );

        MemUnit mem;
        lwork = work_sz[0];
        mem.resize( lwork*sizeof(double) );
        double* work = (double*)mem.get_buffer();

        dgeev_( &jobvl, &jobvr, &n, Atmp.get_pointer(), &n,
                eval_r.get_pointer(), eval_i.get_pointer(),
                vec_l, &n, vec_r, &n, work, &lwork, &info );

        if( evec_l ) evec_l->transpose();
        if( evec_r ) evec_r->transpose();

        return bool(info==0);
    }

    bool mat_eigen_real( const KMatrix& A, KMatrix& eval, KMatrix* evec_r, KMatrix* evec_l ) {

        KMatrix er, ei;
        KMatrix vr, vl;

        KMatrix* pvr = NULL; if( evec_r ) pvr = &vr;
        KMatrix* pvl = NULL; if( evec_l ) pvl = &vl;
        if( !mat_eigen( A, er, ei, pvr, pvl ) )
            return false;

        if( is_all_zero( ei(), ei.size() ) ) {
            eval = er;
            if( evec_l ) evec_l->copy( vl );
            if( evec_r ) evec_r->copy( vr );
            return true;
        }

        double eps = 1e-16;
        vector<int> cols;
        for( int i=0; i<ei.size(); i++ ) {
            if( fabs(ei[i]) < eps )
                cols.push_back(i);
        }

        if( evec_l ) mat_copy_columns( vl, cols, *evec_l );
        if( evec_r ) mat_copy_columns( vr, cols, *evec_r );

        int sz = cols.size();
        eval.resize( sz, 1 );
        double      * ev  = eval.get_pointer();
        const double* per = er();
        for( unsigned int i=0; i<cols.size(); i++ ) {
            ev[i] = per[ cols[i] ];
        }
        return true;
    }

}

#endif
