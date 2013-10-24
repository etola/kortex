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

#include <kortex/svd.h>

#include <kortex/math.h>
#include <kortex/check.h>
#include <kortex/fileio.h>
#include <kortex/lapack_externs.h>
#include <kortex/kmatrix.h>

#include <cstring>

namespace kortex {

    size_t SVD::req_mem(int nr, int nc, bool compute_U, bool compute_VT) {
        double work;
        char jobu  = 'N'; if( compute_U  ) jobu  = 'A';
        char jobvt = 'N'; if( compute_VT ) jobvt = 'A';
        int lwork = -1;
        int info;
        int m = nr;
        int n = nc;
        dgesvd_(&jobvt, &jobu, &m, &n, 0,  &m, 0, 0, &m, 0, &n, &work, &lwork, &info);
        size_t mat_sz = 0;
        mat_sz += (nr>nc) ? nr : nc;
        if( compute_U  ) mat_sz += nr*nr;
        if( compute_VT ) mat_sz += nc*nc;
        mat_sz += nr*nc;
        return (work + mat_sz)*sizeof(double);
    }

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
        m_A = m_U = m_Sd = m_Vt = NULL;
        m_work = NULL;
        m_lwork = 0;
    }

    void SVD::set_params(int nr, int nc, bool compute_U, bool compute_VT) {
        m_r = nr;
        m_c = nc;
        m_d = (m_r>m_c ) ? m_r : m_c;
        m_compute_u  = compute_U;
        m_compute_vt = compute_VT;
        allocate();
    }

    void SVD::allocate() {
        size_t rm = req_mem(m_r, m_c, m_compute_u, m_compute_vt);
        m_memory.resize(rm);

        double* buffer = (double*)m_memory.get_buffer();

        int sz = m_memory.capacity()/sizeof(double);

        int asz = 0;
        m_Sd  = buffer;
        asz += m_d;
        if( m_compute_u ) {
            m_U = buffer + asz;
            asz += m_r*m_r;
        }
        if( m_compute_vt ) {
            m_Vt = buffer + asz;
            asz += m_c*m_c;
        }
        m_A = buffer + asz;
        asz += m_r*m_c;

        m_work  = buffer + asz;
        m_lwork = int( sz - asz );
        passert_statement( m_lwork > 0, "memory allocation failure" );
    }

    void SVD::set_mem(MemUnit* mem) {
        m_memory.deallocate();
        m_memory.swap(mem);
    }

    void SVD::set_Sd(int d, double val) {
        passert_boundary(d,0,m_d);
        m_Sd[d] = val;
    }

    void SVD::release() {
        m_memory.deallocate();
        init();
    }

    void SVD::decompose(const double* A, int nr, int nc, int nld, bool compute_u, bool compute_vt) {
        set_params(nr, nc, compute_u, compute_vt);
        for(int y=0; y<nr; y++)
            memcpy( m_A+y*nc, A+y*nld, sizeof(*m_A)*nc );

        char jobu  = 'N'; if( m_compute_u  ) jobu  = 'A';
        char jobvt = 'N'; if( m_compute_vt ) jobvt = 'A';

        int proxy_ld = nc;
        int work_sz = m_lwork;
        int info;
        int m = nr;
        int n = nc;

        // last element of m_Sd might be uninitialized even after a call to
        // _sgesvd/_dgesvd
        std::memset(m_Sd, 0, m_d*sizeof(*m_Sd));
        dgesvd_(&jobvt, &jobu, &n, &m, m_A, &proxy_ld, m_Sd, m_Vt, &n, m_U, &m, m_work, &work_sz, &info);
    }

    void SVD::combine(double* A, int lda) const {
        if( !( m_compute_u && m_compute_vt ) )
            logman_fatal("U or Vt is not computed. cannot combine back.");

        for( int i=0; i<m_r; i++ ) {
            const double* Ui = m_U+i*m_r;
            for( int j=0; j<m_c; j++ ) {
                double s = 0.0f;
                for( int k=0; k<m_r; k++ )
                    s += Ui[k] * m_Sd[k] * m_Vt[k*m_c+j];
                A[i*lda+j] = s;
            }
        }
    }

    double SVD::pseudo_inverse(double* iA, int ldia) {
        passert_statement( m_compute_u && m_compute_vt, "U or Vt is not computed. cannot combine back.");
        passert_statement( ldia>=m_r, "pseudo_inverse memory not enough");

        KMatrix Uw( m_U,  m_r, m_r ); Uw.transpose();
        KMatrix Vw( m_Vt, m_c, m_c ); Vw.transpose();

        int d = (m_r<m_c) ? m_r : m_c;

        static const double tolerance = 1e-24;

        double sd_max = m_Sd[0];
        double sd_min = m_Sd[d-1];
        double cond = ( sd_min<tolerance ) ? sd_max/tolerance : sd_max/sd_min;

        for(int i=0; i<d; i++ ) {
            if( m_Sd[i] > tolerance ) m_Sd[i] = 1.0/m_Sd[i];
            else                      m_Sd[i] = 0.0;
        }
        std::swap(m_U, m_Vt);
        std::swap(m_r, m_c);
        combine(iA, ldia);
        return cond;
    }

    void SVD::print() const {
        if( m_compute_u ) {
            KMatrix Uw( (const double*)m_U, m_r, m_r );
            Uw.print( "U" );
        } else {
            printf("U is not computed\n");
        }
        KMatrix Sw( (const double*)m_Sd, m_d, 1 );
        Sw.print("Sd");

        if( m_compute_vt ) {
            KMatrix Vtw( (const double*)m_Vt, m_c, m_c );
            Vtw.print( "Vt" );
            // matrix_print("Vt", m_Vt, m_c, m_c, false, false);
        } else {
            printf("Vt is not computed\n");
        }
    }


}
