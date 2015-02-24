#ifdef WITH_EIGEN

#include <kortex/eigen_conversion.h>
namespace kortex {

    size_t SVD::req_mem( int nr, int nc, bool compute_U, bool compute_VT ) const {
        return 0;
    }

    void SVD::decompose(const double* A, int nr, int nc, int nld, bool compute_u, bool compute_vt) {
        set_params(nr, nc, compute_u, compute_vt);
        Eigen::MatrixXd tA;
        convert_mat( A, nr, nc, nld, tA );

        unsigned int svd_opts = 0;
        if( compute_u  ) svd_opts = svd_opts | Eigen::ComputeFullU;
        if( compute_vt ) svd_opts = svd_opts | Eigen::ComputeFullV;

        Eigen::JacobiSVD<Eigen::MatrixXd> svd( tA, svd_opts );
        if( compute_u ) {
            convert_mat( svd.matrixU(), m_U  );
        }
        if( compute_vt ) {
            convert_mat( svd.matrixV(), m_Vt );
            m_Vt.transpose();
        }

        for( int i=0; i<m_d; i++ )
            m_Sd.set( i, 0, svd.singularValues()[i] );
    }

}

#endif

