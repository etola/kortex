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

#include <cstring>

#include <kortex/check.h>
#include <kortex/svd.h>

#include <kortex/linear_algebra.h>

namespace kortex {

    void mat_null( const double* A, int ar, int ac, double* v, int vsz ) {
        assert_pointer( A );
        assert_statement( vsz == ac, "insufficient vsz" );
        SVD svd;
        svd.decompose(A, ar, ac, false, true );
        const double* vt = svd.Vt();
        memcpy( v, vt+ac*(ac-1), sizeof(*v)*ac );
    }

    double matrix_pseudo_invert_g_svd( const double* A, int ar, int ac, double* iA ) {
        SVD svd;
        svd.decompose( A, ar, ac, true, true );
        double cond = svd.pseudo_inverse( iA, ar );
        return cond;
    }

//
}

#if defined(WITH_EIGEN) && defined(WITH_LAPACK)
could not define both lapack and eigen!
#endif

#ifdef WITH_EIGEN
#include "linear_algebra_eigen.cc"
#endif

#ifdef WITH_LAPACK
#include "linear_algebra_lapack.cc"
#endif
