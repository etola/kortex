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

        static const double tolerance = 1e-10;
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

#if defined(WITH_EIGEN) && defined(WITH_LAPACK)
could not define both lapack and eigen!
#endif

#ifdef WITH_EIGEN
#include "svd_eigen.cc"
#endif

#ifdef WITH_LAPACK
#include "svd_lapack.cc"
#endif
