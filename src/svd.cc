// -----------------------------------------------------------
//
// This file is part of kortex
//
// Copyright (C) 2013 Engin Tola
//
// All rights reserved.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
//
// -----------------------------------------------------------

#include <kortex/check.h>
#include <kortex/kmatrix.h>
#include <kortex/svd.h>

namespace kortex {

    SVD::SVD() {
        init();
    }

    SVD::~SVD() {
        release();
    }

    void SVD::init() {
        m_r = 0;
        m_c = 0;
        m_compute_u  = false;
        m_compute_vt = false;
    }

    void SVD::set_params(int nr, int nc, bool compute_U, bool compute_VT) {
        m_r = nr;
        m_c = nc;
        m_d = (m_r<m_c) ? m_r : m_c;
        m_compute_u  = compute_U;
        m_compute_vt = compute_VT;
        allocate();
    }

    void SVD::allocate() {
        size_t rm = req_mem(m_r, m_c, m_compute_u, m_compute_vt);
        m_memory.resize(rm);

        m_A.init (m_r, m_c);
        m_Sd.init(m_d, 1);
        if( m_compute_u  ) m_U.init( m_r, m_r );
        if( m_compute_vt ) m_Vt.init( m_c, m_c );
    }

    void SVD::set_Sd( int d, double val ) {
        passert_boundary(d,0,m_d);
        m_Sd.set(d,0,val);
    }

    void SVD::release() {
        m_memory.deallocate();
        init();
    }

    void SVD::combine( double* A, int lda ) const {
        if( !( m_compute_u && m_compute_vt ) )
            logman_fatal("U or Vt is not computed. cannot combine back.");
        combine( m_U, m_Sd, m_Vt, A, m_r, m_c, lda );
    }

    // U is rxr,  Vt is cxc, Sd is max(r,c) x 1, A is rxc
    void SVD::combine( const KMatrix& U, const KMatrix& Sd, const KMatrix& Vt,
                        double* A, int r, int c, int lda ) const {

        assert_statement( U.h () == r, "invalid U" );
        assert_statement( Vt.h() == c, "invalid U" );
        assert_statement( Sd.w() == 1 && Sd.h() == std::min(r,c), "invalid Sd" );
        assert_statement( lda >= c, "invalid lda" );

        const double* sd = Sd();
        const double* vt = Vt();

        int d = (r<c) ? r : c;
        for( int i=0; i<r; i++ ) {
            const double* Ui = U.get_row(i);
            double* ai = A+i*lda;
            for( int j=0; j<c; j++ ) {
                double s = 0.0;
                for( int k=0; k<d; k++ )
                    s += Ui[k] * sd[k] * vt[k*c+j];
                ai[j] = s;
            }
        }

    }

    double SVD::pseudo_inverse( double* iA, int ldia ) const {
        passert_statement( m_compute_u && m_compute_vt, "U or Vt is not computed. cannot combine back.");
        passert_statement( ldia>=m_r, "pseudo_inverse memory not enough");

        KMatrix Uw, Vw;
        mat_transpose( m_U,  Uw );
        mat_transpose( m_Vt, Vw );

        static const double tolerance = 1e-24;
        double sd_max = m_Sd[    0];
        double sd_min = m_Sd[m_d-1];
        double cond = ( sd_min<tolerance ) ? sd_max/tolerance : sd_max/sd_min;

        assert_number( cond );

        KMatrix Sw = m_Sd;
        for(int i=0; i<m_d; i++ ) {
            if( Sw[i] > tolerance ) Sw.set(i, 0, 1.0/Sw[i]);
            else                    Sw.set(i, 0, 0.0 );
        }

        combine( Vw, Sw, Uw, iA, m_c, m_r, ldia );
        return cond;
    }

    void SVD::print() const {
        if( m_compute_u ) {
            m_U.print( "U" );
        } else {
            printf("U is not computed\n");
        }
        m_Sd.print( "Sd" );

        if( m_compute_vt ) {
            m_Vt.print( "Vt" );
        } else {
            printf("Vt is not computed\n");
        }
    }
}

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

#else

#include <kortex/lapack_externs.h>
namespace kortex {
    size_t SVD::req_mem( int nr, int nc, bool compute_U, bool compute_VT ) const {
        double work;
        char jobu  = 'N'; if( compute_U  ) jobu  = 'A';
        char jobvt = 'N'; if( compute_VT ) jobvt = 'A';
        int lwork = -1;
        int info;
        int m = nr;
        int n = nc;
        dgesvd_(&jobvt, &jobu, &m, &n, 0,  &m, 0, 0, &m, 0, &n, &work, &lwork, &info);
        return int(work)*sizeof(double);
    }

    void SVD::decompose(const double* A, int nr, int nc, int nld, bool compute_u, bool compute_vt) {

        set_params(nr, nc, compute_u, compute_vt);
        for(int y=0; y<nr; y++)
            m_A.set_row( y, A + y*nld, nc );

        char jobu  = 'N'; if( m_compute_u  ) jobu  = 'A';
        char jobvt = 'N'; if( m_compute_vt ) jobvt = 'A';

        double* work     = (double*)m_memory.get_buffer();
        int     work_sz  = m_memory.capacity()/sizeof(double);
        int     proxy_ld = nc;
        int     m = nr;
        int     n = nc;
        int     info;

        // dont forget - contents of m_A is destroyed here...
        dgesvd_( &jobvt, &jobu, &n, &m, m_A.get_pointer(), &proxy_ld,
                 m_Sd.get_pointer(), m_Vt.get_pointer(), &n, m_U.get_pointer(), &m, work, &work_sz,
                 &info );
    }
}
#endif
