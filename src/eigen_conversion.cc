// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2015 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
// web   : http://www.aurvis.com
//
// ---------------------------------------------------------------------------
#include <kortex/eigen_conversion.h>

#ifdef WITH_EIGEN

namespace kortex {

    void convert_mat( const Eigen::MatrixXd& m, kortex::KMatrix& km ) {
        assert_statement( m.IsRowMajor, "eigen matrix should be row major" );
        assert_statement( m.size() != 0, "matrix has 0 dims" );
        km.init( m.rows(), m.cols() );
        int sz = km.size();
        const double*  m_ =  m.data();
        double      * km_ = km.get_pointer();
        for( int i=0; i<sz; i++ )
            km_[i] = m_[i];
    }
    void convert_mat( const kortex::KMatrix& km, Eigen::MatrixXd& m ) {
        assert_statement( m.IsRowMajor, "eigen matrix should be row major" );
        assert_statement( km.size() != 0, "matrix has 0 dims" );
        m.resize( km.h(), km.w() );
        int sz = km.size();
        double      *  m_ =  m.data();
        const double* km_ = km();
        for( int i=0; i<sz; i++ )
            m_[i] = km_[i];
    }
    void convert_mat( const double* A, int nr, int nc, int nld, Eigen::MatrixXd& m ) {
        assert_statement( m.IsRowMajor, "eigen matrix should be row major" );
        assert_statement( nr*nc != 0, "matrix has 0 dims" );

        m.resize( nr, nc );
        double* m_ = m.data();
        for( int y=0; y<nr; y++ ) {
            const double* ar = A  + y*nld;
            double      * mr = m_ + y*nc ;
            for( int x=0; x<nc; x++ )
                mr[x] = ar[x];
        }
    }
    void convert_mat( const Eigen::MatrixXd& m, double* A, int asz ) {
        assert_statement( m.size() == asz, "buffer size not enough" );
        assert_statement( m.size() != 0, "matrix has 0 dims" );
        const double* m_ = m.data();
        for( int i=0; i<asz; i++ )
            A[i] = m_[i];
    }

}

#endif
