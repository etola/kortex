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

namespace kortex {

    class SVD {
    public:
        SVD();
        ~SVD();

        static size_t req_mem(int nr, int nc, bool compute_U, bool compute_VT);

        void set_params(int nr, int nc, bool compute_U, bool compute_VT);
        void set_mem(MemUnit* mem);
        void set_Sd(int d, double val);

        void release();

        void decompose(const double* A, int nr, int nc, int nld, bool compute_u, bool compute_vt);
        void decompose(const double* A, int nr, int nc, bool compute_u, bool compute_vt) {
            decompose(A, nr, nc, nc, compute_u, compute_vt);
        }

        void combine(double* A, int lda) const;

        double pseudo_inverse(double* iA, int ldia); // destroys svd temp
                                                     // content -> cannot call
                                                     // combine afterwards
        void print() const;

        const double* Sd() const { return m_Sd; }
        const double* Vt() const { return m_Vt; }
        const double* U () const { return m_U;  }

    private:
        void init();
        void allocate();

        MemUnit m_memory;

        int m_r;
        int m_c;
        int m_d;
        bool m_compute_u;
        bool m_compute_vt;

        double* m_A;
        double* m_U;
        double* m_Sd;
        double* m_Vt;
        double* m_work;
        int     m_lwork;
    };

}

#endif
