#include <kortex/eigen_conversion.h>

#include <Eigen/Eigenvalues>

#include <iostream>

namespace kortex {

    int matrix_invert_g_lu( const double* A, int ar, double* iA ) {
        passert_pointer( A && iA );
        assert_pointer_size( ar );
        Eigen::MatrixXd m;
        convert_mat( A, ar, ar, ar, m );
        int asz = ar*ar;
        convert_mat( m.inverse(), iA, asz );
        return !is_valid_array(iA,asz);
    }

    //
    // Least Square Solvers
    //
    void lsq_solver_svd( const double* A, int ar, int ac, int lda,
                         const double* B, int bc, int ldb,
                         double* x, int xsz ) {
        Eigen::MatrixXd eA; convert_mat( A, ar, ac, lda, eA );
        Eigen::MatrixXd eB; convert_mat( B, ar, bc, ldb, eB );
        Eigen::MatrixXd eX = eA.jacobiSvd( Eigen::ComputeThinU | Eigen::ComputeThinV ).solve(eB);
        convert_mat( eX, x, xsz );
        assert_array( "EIGEN - SVD LSQ", x, xsz );
    }

    void lsq_solver_cholesky( const double* A, int ar, int lda,
                              const double* B, int bc, int ldb,
                              double* x, int xsz ) {
        assert_statement( is_symmetric( A, ar, ar ), "A is not symmetric - call lsq_solver_svd" );
        Eigen::MatrixXd eA; convert_mat( A, ar, ar, lda, eA );
        Eigen::MatrixXd eB; convert_mat( B, ar, bc, ldb, eB );
        Eigen::MatrixXd eX = eA.ldlt().solve(eB);
        convert_mat( eX, x, xsz );
        assert_array( "EIGEN - CHOLESKY LSQ", x, xsz );
    }

    void lsq_solver_svd( const KMatrix& A, const KMatrix& B, KMatrix& x ) {
        x.resize( A.w(), B.w() );
        lsq_solver_svd( A(), A.h(), A.w(), A.w(), B(), B.w(), B.w(), x.get_pointer(), x.size() );
    }


    void lsq_solver_cholesky( const KMatrix& A, const KMatrix& B, KMatrix& x ) {
        x.resize( A.w(), B.w() );
        lsq_solver_cholesky( A(), A.h(), A.w(), B(), B.w(), B.w(), x.get_pointer(), x.size() );
    }

    //
    //
    //
    bool find_eigenvalues( const KMatrix& m, vector<double>& eig_real, vector<double>& eig_imag ) {
        eig_real.clear();
        eig_imag.clear();

        passert_statement( m.is_square(), "matrix needs to be square" );
        Eigen::MatrixXd em;
        convert_mat( m, em );
        Eigen::EigenSolver<Eigen::MatrixXd> esolver(em);

        auto eigs = esolver.eigenvalues();

        int n_eigs = eigs.rows();
        for( int i=0; i<n_eigs; i++ ) {
            std::complex<double> r = eigs.col(0)[i];
            eig_real.push_back( r.real() );
            eig_imag.push_back( r.imag() );
        }

        return true;
    }



}
