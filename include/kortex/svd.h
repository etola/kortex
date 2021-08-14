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
#ifndef KORTEX_SVD_H
#define KORTEX_SVD_H

#include <kortex/mem_unit.h>
#include <kortex/kmatrix.h>

namespace kortex {

    class SVD {
    public:
        SVD();
        ~SVD();

        void set_Sd(int d, double val);

        void release();

        void decompose(const double* A, int nr, int nc, int nld, bool compute_u, bool compute_vt);
        void decompose(const double* A, int nr, int nc, bool compute_u, bool compute_vt) {
            decompose(A, nr, nc, nc, compute_u, compute_vt);
        }
        void decompose( const KMatrix& A, bool compute_u, bool compute_vt ) {
            decompose( A(), A.h(), A.w(), A.w(), compute_u, compute_vt );
        }

        void combine( double* A, int lda ) const;

        double pseudo_inverse(double* iA, int ldia) const;

        const double* Sd() const { return m_Sd(); }
        const double* Vt() const { return m_Vt(); }
        const double* U () const { return m_U();  }

        const KMatrix& mU () const { return m_U;  }
        const KMatrix& mVt() const { return m_Vt; }
        const KMatrix& mSd() const { return m_Sd; }

        void print() const;

        double condition_number() const {
            static const double tolerance = 1e-10;
            double sd_max = m_Sd[    0];
            double sd_min = m_Sd[m_d-1];
            double cond = ( sd_min<tolerance ) ? sd_max/tolerance : sd_max/sd_min;
            return cond;
        }

    private:
        void init();

        MemUnit m_memory;

        int  m_r;
        int  m_c;
        int  m_d;
        bool m_compute_u;
        bool m_compute_vt;

        KMatrix m_A;
        KMatrix m_U;
        KMatrix m_Sd;
        KMatrix m_Vt;

        void   allocate();
        void   set_params(int nr, int nc, bool compute_U, bool compute_VT);
        size_t req_mem( int nr, int nc, bool compute_U, bool compute_VT ) const;
        void   combine( const KMatrix& U, const KMatrix& Sd, const KMatrix& Vt,
                        double* A, int r, int c, int lda ) const;
    };

}

#endif
