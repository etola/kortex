// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
//
// ---------------------------------------------------------------------------

#include <kortex/wmatrix.h>
#include <kortex/matrix.h>
#include <kortex/fileio.h>
#include <kortex/check.h>
#include <kortex/message.h>
#include <kortex/math.h>

#include <cstring>
#include <iomanip>

using std::endl;

namespace kortex {

    void WMatrix::init_() {
        m_ro_data = NULL;
        m_data    = NULL;
        m_wrapper = false;
        m_const   = false;
        nr        = 0;
        nc        = 0;
    }

    WMatrix::WMatrix() {
        init_();
    }

    WMatrix::WMatrix( const double* data, int h, int w ) {
        init_();
        m_ro_data = data;
        m_wrapper = true;
        m_const   = true;
        nr = h;
        nc = w;
    }

    WMatrix::WMatrix( double* data, int h, int w ) {
        init_();
        m_data    = data;
        m_wrapper = true;
        m_const   = false;
        nr        = h;
        nc        = w;
    }

    WMatrix::WMatrix( int h, int w ) {
        init_();
        init( h, w );
    }

    void WMatrix::copy( const WMatrix& rhs ) {
        passert_statement( !is_const(), "cannot copy onto const matrix" );
        resize( rhs.h(), rhs.w() );
        memcpy( m_data, rhs.get_pointer(), sizeof(*m_data)*rhs.size() );
    }

    void WMatrix::clone( const WMatrix& rhs ) {
        passert_statement( !is_wrapper(), "cannot clone onto wrapper matrix" );
        if( rhs.is_wrapper() ) {
            m_data    = rhs.m_data;
            m_ro_data = rhs.m_ro_data;
            m_const   = rhs.m_const;
            nr        = rhs.h();
            nc        = rhs.w();
            m_wrapper = true;
        } else {
            this->copy( rhs );
        }
    }

    void WMatrix::init( int h, int w ) {
        passert_statement( !is_wrapper(), "call release() on the wrapped matrix first" );
        m_memory.resize( req_mem(h,w) );
        m_ro_data = NULL;
        m_data    = (double*)m_memory.get_buffer();
        m_wrapper = false;
        m_const   = false;
        nr        = h;
        nc        = w;
    }

    void WMatrix::resize( int h, int w ) {
        if( is_wrapper() ) {
            passert_statement( h*w == this->size(), "wrapped matrix has insufficient space" );
            nr = h;
            nc = w;
        } else {
            init( h, w );
        }
    }

    void WMatrix::reset() {
        init_();
    }

    void WMatrix::release() {
        m_memory.deallocate();
        init_();
    }

    WMatrix::~WMatrix() {
        release();
    }

    void WMatrix::set( const double* data, int dsz ) {
        passert_statement( !is_const(), "cannot modify const matrix" );
        passert_statement( dsz == size(), "data size should be equal to the matrix size" );
        assert_pointer( data );
        memcpy( m_data, data, sizeof(*m_data)*dsz );
    }

    double* WMatrix::get_pointer() {
        passert_statement( !is_const(), "cannot return non-const pointer for const matrix" );
        assert_statement( size() != 0, "matrix has not been initialized" );
        return m_data;
    }

    const double* WMatrix::get_const_pointer() const {
        assert_statement( size() != 0, "matrix has not been initialized" );
        switch( m_const ) {
        case true : return m_ro_data;
        case false: return m_data;
        }
    }
    const double* WMatrix::get_pointer() const {
        assert_statement( size() != 0, "matrix has not been initialized" );
        switch( m_const ) {
        case true : return m_ro_data;
        case false: return m_data;
        }
    }

    double* WMatrix::get_row( int rid ) {
        assert_boundary( rid, 0, nr );
        return get_pointer() + rid * nc;
    }
    double* WMatrix::get_col( int cid ) {
        assert_boundary( cid, 0, nc );
        return get_pointer() + cid;
    }

    const double* WMatrix::get_row( int rid ) const {
        assert_boundary( rid, 0, nr );
        return get_pointer() + rid * nc;
    }
    const double* WMatrix::get_col( int cid ) const {
        assert_boundary( cid, 0, nc );
        return get_pointer() + cid;
    }

    /// copies  this[ dr:dr+srsz, dc:dc+srsz ] =  rhs[ sr:sr+srsz, sc:sc+scsz ]
    void WMatrix::copy( const WMatrix& rhs, int sr, int sc, int srsz, int scsz, int dr, int dc ) {
        assert_statement( sr+srsz <= rhs.h(), "size oob" );
        assert_statement( sc+scsz <= rhs.w(), "size oob" );
        assert_statement( dr+srsz <= h(), "size oob" );
        assert_statement( dc+scsz <= w(), "size oob" );
        for( int r=0; r<srsz; r++ ) {
            const double* srow =   rhs.get_row( sr+r );
            double      * drow = this->get_row( dr+r );
            memcpy( drow+dc, srow+sc, sizeof(*drow)*scsz );
        }
    }

    void WMatrix::copy( const WMatrix& rhs, int dr, int dc ) {
        assert_statement( dr+rhs.h() <= h(), "size oob" );
        assert_statement( dc+rhs.w() <= w(), "size oob" );
        for( int r=0; r<rhs.h(); r++ ) {
            const double* srow =   rhs.get_row( r    );
            double      * drow = this->get_row( dr+r );
            memcpy( drow+dc, srow, sizeof(*drow)*rhs.w() );
        }
    }

    void WMatrix::identity() {
        passert_statement( !is_const(), "cannot modify const matrix" );
        assert_statement( size() != 0, "matrix has not been initialized" );
        mat_identity( m_data, nr, nc );
    }

    void WMatrix::zero() {
        passert_statement( !is_const(), "cannot modify const wrapped matrix" );
        assert_statement( size() != 0, "matrix has not been initialized" );
        mat_zero( m_data, nr, nc );
    }

    void WMatrix::negate() {
        assert_statement( size() != 0, "matrix has not been initialized" );
        mat_negate( get_pointer(), nr, nc );
    }

    double WMatrix::normalize() {
        assert_statement( size() != 0, "matrix has not been initialized" );
        return mat_normalize( get_pointer(), nr, nc );
    }

    double WMatrix::trace() const {
        return mat_trace( get_pointer(), nr, nc );
    }

    double WMatrix::det3() const {
        passert_statement( nr == nc && nr == 3, "invalid matrix size" );
        return mat_det_3( get_pointer(), nr );
    }

    void WMatrix::transpose() {
        passert_statement( !is_const(), "cannot modify const matrix" );
        if( nr == nc ) {
            for( int y=0; y<nr; y++ ) {
                double* row = get_row(y);
                double* col = get_col(y);
                for( int x=y+1; x<nc; x++ ) {
                    std::swap( row[x], col[x*nc] );
                }
            }
        } else {
            WMatrix tmp( nc, nr );
            mat_transpose( get_pointer(), nr, nc, tmp.get_pointer(), tmp.size() );
            this->copy( tmp );
        }

    }

    void WMatrix::scale_row( int rid, double alpha ) {
        assert_boundary( rid, 0, nr );
        assert_statement( !is_const(), "cannot modify const matrix" );
        double* row = get_row(rid);
        for( int i=0; i<nc; i++ )
            row[i] *= alpha;
    }

    void WMatrix::scale( double s ) {
        assert_statement( !is_const(), "cannot modify const matrix" );
        double* data = get_pointer();
        for( int i=0; i<size(); i++ )
            data[i] *= s;
    }

    void WMatrix::scale_col( int cid, double alpha ) {
        assert_boundary( cid, 0, nc );
        assert_statement( !is_const(), "cannot modify const matrix" );
        double* col = get_col(cid);
        for( int i=0; i<nr; i++ )
            col[i*nc] *= alpha;
    }

    // frobenius norm
    double WMatrix::norm() const {
        return mat_norm( get_pointer(), nr, nc );
    }

    double WMatrix::norm_sq() const {
        return mat_norm_sq( get_pointer(), nr, nc );
    }

    void WMatrix::print( const char* str ) const {
        matrix_print( str, get_pointer(), nr, nc, false, true );
    }

    void WMatrix::save( const char* file ) const {
        ofstream fout;
        open_or_fail( file, fout );
        save( fout, "X" );
        fout.close();
    }
    void WMatrix::save( ofstream& fout, const char* mat_name ) const {
        fout << mat_name << " " << nr << " " << nc << " ";
        for( int r=0; r<nr; r++ ) {
            const double* row = get_row( r );
            for( int c=0; c<nc; c++ ) {
                fout << std::setprecision(24);
                fout << row[c] << " ";
            }
        }
        fout << endl;
        check_file_stream_error( fout );
    }

    void WMatrix::save_binary( ofstream& fout ) const {
        write_bparam( fout, nr );
        write_bparam( fout, nc );
        write_bparam( fout, get_pointer(), size() );
    }

    void WMatrix::load_binary( ifstream& fin ) {
        passert_statement( !is_const(), "cannot modify const matrix" );

        int th, tw;
        read_bparam( fin, th );
        read_bparam( fin, tw );

        if( m_wrapper ) {
            if( th*tw != size() )
                logman_fatal( "wrapped matrix has an invalid size" );
            nr = th;
            nc = tw;
            read_bparam( fin, get_pointer(), th*tw );
        } else {
            init( th, tw );
            read_bparam( fin, get_pointer(), th*tw );
        }
    }

    void WMatrix::load( const char* file ) {
        passert_statement( !is_const(), "cannot modify const matrix" );
        ifstream fin;
        open_or_fail( file, fin );
        load( fin, "X" );
        fin.close();
    }

    void WMatrix::load( ifstream& fin, const char* mat_name ) {
        passert_statement( !is_const(), "cannot modify const matrix" );
        string stmp;
        read_string( fin, stmp, mat_name );
        int th, tw;
        fin >> th >> tw;

        if( m_wrapper ) {
            if( th*tw != size() )
                logman_fatal( "wrapped matrix has invalid size" );
            nr = th;
            nc = tw;
        } else {
            init( th, tw );
        }
        for( int r=0; r<nr; r++ ) {
            double* row = get_row( r );
            for( int c=0; c<nc; c++ ) {
                fin >> row[c];
            }
        }
        check_file_stream_error( fin );
    }

//
//
//

    void mat_row_add( const WMatrix& A, int arid, double alpha,
                      const WMatrix& B, int brid, double beta,
                      WMatrix&       C, int crid ) {
        int nc = A.w();
        assert_statement( (B.w() == nc) && (C.w() == nc),
                          "invalid matrix size" );
        assert_boundary( arid, 0, A.h() );
        assert_boundary( brid, 0, B.h() );
        assert_boundary( crid, 0, C.h() );
        assert_number( alpha );
        assert_number( beta  );
        const double* arow = A.get_row( arid );
        const double* brow = B.get_row( brid );
        double      * crow = C.get_row( crid );

        for( int i=0; i<nc; i++ ) {
            crow[i] = alpha*arow[i] + beta * brow[i];
        }
    }

    void mat_Ax_plus_y( const WMatrix& A, const WMatrix& x, const WMatrix& y, WMatrix& c ) {
        assert_statement( ( x.w()==1     ) && ( y.w()==1     ), "x and y should be column vectors" );
        assert_statement( ( A.h()==x.h() ) && ( A.h()==y.h() ), "invalid matrix dimensions" );
        c.init( A.h(), 1 );
        double      * cp = c.get_pointer();
        const double* yp = y.get_pointer();
        const double* xp = x.get_pointer();
        for( int r=0; r<A.h(); r++ ) {
            cp[r] = dot( A.get_row(r), xp, A.w() ) + yp[r];
        }
    }


    /// c = alpha * A * x + beta * y; x and y are column vectors.
    void mat_alpha_Ax_plus_beta_y( const double& alpha, const WMatrix& A, const WMatrix& x,
                                   const double& beta,  const WMatrix& y, WMatrix& c ) {
        assert_statement( ( x.w()==1     ) && ( y.w()==1     ), "x and y should be column vectors" );
        assert_statement( ( A.h()==x.h() ) && ( A.h()==y.h() ), "invalid matrix dimensions" );

        c.resize( A.h(), 1 );
        double      * cp = c.get_pointer();
        const double* yp = y.get_pointer();
        const double* xp = x.get_pointer();
        for( int r=0; r<A.h(); r++ ) {
            cp[r] = alpha * dot( A.get_row(r), xp, A.w() ) + beta * yp[r];
        }

    }

    void mat_col_copy( const WMatrix& src, int sc, double alpha, WMatrix& dst, int dc ) {
        assert_boundary( dc, 0, dst.w() );
        assert_boundary( sc, 0, src.w() );
        assert_statement( dst.h() == src.h(), "dst src heights differ" );

        const double* src_col = src.get_col(sc);
        double      * dst_col = dst.get_col(dc);

        for( int i=0; i<dst.h(); i++ ) {
            dst_col[i*dst.w()] = alpha * src_col[i*src.w()];
        }
    }



}
