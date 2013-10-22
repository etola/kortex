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

#include <kortex/kmatrix.h>
#include <kortex/matrix.h>

#include <kortex/check.h>
#include <kortex/svd.h>
#include <kortex/math.h>
#include <kortex/fileio.h>

#include <kortex/message.h>

#include <limits>
#include <iomanip>
#include <cstring>

using namespace std;

namespace kortex {

    KMatrix::~KMatrix() {
        if( m_wrapped ) return;
        m_memory.deallocate();
        m_buffer = NULL;
        h = 0;
        w = 0;
    }

    void KMatrix::init( int nr, int nc ) {
        if( m_wrapped ) {
            passert_statement( nr*nc == h*w, "matrix wrapper invalid dimensions" );
            h = nr;
            w = nc;
        } else {
            h = nr;
            w = nc;
            m_memory.resize( h*w*sizeof(double) );
            m_buffer = (double*)m_memory.get_buffer();
        }
    }

    KMatrix::KMatrix() {
        h = 0;
        w = 0;
        m_buffer = NULL;
        m_wrapped = false;
    }

    KMatrix::KMatrix( int nr, int nc ) {
        m_wrapped = false;
        m_buffer = NULL;
        init( nr, nc );
    }

    KMatrix::KMatrix( const double* data, int nr, int nc ) {
        assert_statement( nr > 0 && nc > 0, "invalid matrix size" );
        assert_pointer( data );
        m_wrapped = false;
        m_buffer = NULL;
        init( nr, nc );
        memcpy( m_buffer, data, sizeof(*m_buffer)*nr*nc );
    }

    KMatrix::KMatrix( const KMatrix& rhs ) {
        m_wrapped = false;
        m_buffer = NULL;
        init( rhs.h, rhs.w );
        memcpy( m_buffer, rhs(), sizeof(double)*h*w );
    }

    KMatrix& KMatrix::operator= ( const KMatrix& rhs ) {
        clone( rhs );
        return *this;
    }

    void KMatrix::reshape( int nh, int nw ) {
        if( h*w != nh*nw )
            logman_fatal("new size is not compatible with reshape");
        h = nh;
        w = nw;
    }

    void KMatrix::init( const double* data ) {
        assert_statement( h*w>0, "init matrix dimensions" );
        memcpy( m_buffer, data, sizeof(*m_buffer)*h*w );
    }

    void KMatrix::wrap( double* data, int nr, int nc ) {
        h = nr;
        w = nc;
        m_buffer = data;
        m_wrapped = true;
        m_memory.deallocate();
    }

    void KMatrix::unwrap() {
        if( !m_wrapped ) return;
        m_memory.resize( h*w*sizeof(double) );
        memcpy( m_memory.get_buffer(), m_buffer, sizeof(*m_buffer)*h*w );
        m_buffer = (double*) m_memory.get_buffer();
        m_wrapped = false;
    }

    void KMatrix::init3( const double& a0, const double& a1, const double& a2 ) {
        assert_statement( h*w == 3, "invalid matrix dimensions" );
        m_buffer[0] = a0;
        m_buffer[1] = a1;
        m_buffer[2] = a2;
    }

    void KMatrix::init33( const double& a0, const double& a1, const double& a2,
                           const double& a3, const double& a4, const double& a5,
                           const double& a6, const double& a7, const double& a8 ) {
        assert_statement( is_square() && h==3, "invalid matrix dimensions" );
        m_buffer[0] = a0;
        m_buffer[1] = a1;
        m_buffer[2] = a2;
        m_buffer[3] = a3;
        m_buffer[4] = a4;
        m_buffer[5] = a5;
        m_buffer[6] = a6;
        m_buffer[7] = a7;
        m_buffer[8] = a8;
    }

    void KMatrix::identity() {
        assert_statement( h > 0, "uninit matrix" );
        mat_identity( m_buffer, h, w );
    }

    void KMatrix::zero() {
        assert_statement( h*w > 0, "uninit matrix" );
        memset( m_buffer, 0, sizeof(*m_buffer)*h*w );
    }

    void KMatrix::set_row( int r, const double* row_data ) {
        mat_set_row( get_pointer(), h, w, r, row_data, w );
    }
    void KMatrix::set_row( int r, double alpha, const double* row_data ) {
        mat_set_row( get_pointer(), h, w, r, alpha, row_data, w );
    }
    void KMatrix::set_row( int r, int c, const double* row_data, int csz ) {
        mat_set_row_from_point( get_pointer(), h, w, r, c, row_data, csz );
    }
    void KMatrix::set_col( int c, const double* col_data ) {
        mat_set_col( get_pointer(), h, w, c, col_data, h );
    }
    void KMatrix::set_col( int c, double alpha, const double* col_data ) {
        mat_set_col( get_pointer(), h, w, c, alpha, col_data, h );
    }
    void KMatrix::set_col( int r, int c, const double* col_data, int rsz ) {
        mat_set_col_from_point( get_pointer(), h, w, r, c, col_data, rsz );
    }

    void KMatrix::clone( const KMatrix& rhs ) {
        passert_statement( !m_wrapped, "cannot clone to a wrapped matrix - use copy instead" );
        init( rhs.h, rhs.w );
        memcpy( m_buffer, rhs(), sizeof(*m_buffer)*rhs.h*rhs.w );
    }

    void KMatrix::copy( const KMatrix& rhs ) {
        passert_statement( (h == rhs.h) && (w==rhs.w), "invalid matrix size" );
        memcpy( m_buffer, rhs(), sizeof(*m_buffer)*rhs.h*rhs.w );
    }

    // copies  this[ dr:dr+srsz, dc:dc+srsz ] =  rhs[ sr:sr+srsz, sc:sc+scsz ]
    void KMatrix::copy( const KMatrix& rhs, int sr, int sc, int srsz, int scsz,
                         int dr, int dc ) {
        assert_statement( sr+srsz <= rhs.h, "size oob" );
        assert_statement( sc+scsz <= rhs.w, "size oob" );
        assert_statement( dr+srsz <= this->h, "size oob" );
        assert_statement( dc+scsz <= this->w, "size oob" );
        for( int r=0; r<srsz; r++ ) {
            const double* srow =   rhs.get_row( sr+r );
            double      * drow = this->get_row( dr+r );
            memcpy( drow+dc, srow+sc, sizeof(*drow)*scsz );
        }
    }

    /// copies rhs into this starting at [dr,dc]
    void KMatrix::copy( const KMatrix& rhs, int dr, int dc ) {
        assert_statement( dr+rhs.h <= this->h, "size oob" );
        assert_statement( dc+rhs.w <= this->w, "size oob" );
        for( int r=0; r<rhs.h; r++ ) {
            const double* srow = rhs.get_row(r);
            double      * drow = this->get_row( dr+r );
            memcpy( drow+dc, srow, sizeof(*drow)*rhs.w );
        }
    }
    void KMatrix::negate() {
        mat_negate( get_pointer(), h, w );
    }

    void KMatrix::transpose() {
        if( h == w ) {
            for( int y=0; y<h; y++ ) {
                for( int x=y+1; x<w; x++ ) {
                    std::swap(m_buffer[y*w+x], m_buffer[x*w+y]);
                }
            }
        } else {
            KMatrix tmp(w,h);
            double* i_buffer = tmp();
            for( int r=0; r<h; r++ ) {
                for( int c=0; c<w; c++ ) {
                    i_buffer[c*h+r] = m_buffer[r*w+c];
                }
            }
            if( m_wrapped ) {
                memcpy( m_buffer, i_buffer, sizeof(*m_buffer)*h*w );
            } else {
                m_memory.swap( &tmp.get_memory() );
                m_buffer = (double*)m_memory.get_buffer();
            }
            std::swap(w,h);
        }
    }

    double KMatrix::trace() const {
        return mat_trace( get_pointer(), h, w );
    }

    double KMatrix::det3() const {
        assert_statement( h == w && h == 3, "invalid matrix size" );
        return mat_det_3( get_pointer(), h );
    }

    void KMatrix::scale( double s ) {
        mat_scale( get_pointer(), h, w, s );
    }

    void KMatrix::scale_row( int rid, double alpha ) {
        assert_boundary( rid, 0, h );
        double* row = get_row(rid);
        for( int i=0; i<w; i++ )
            row[i] *= alpha;
    }

    void KMatrix::scale_col( int cid, double alpha ) {
        assert_boundary( cid, 0, w );
        double* col = get_col(cid);
        for( int i=0; i<h; i++ )
            col[i*w] *= alpha;
    }

    void KMatrix::set_row_3( int rid, const double& a, const double& b, const double& c ) {
        assert_statement( w==3, "invalid matrix type" );
        assert_pointer( m_buffer );
        assert_boundary( rid, 0, h );
        double* row = get_row(rid);
        row[0] = a;
        row[1] = b;
        row[2] = c;
    }

    void KMatrix::set_col_3( int cid, const double& a, const double& b, const double& c ) {
        assert_statement( h==3, "invalid matrix type" );
        assert_pointer( m_buffer );
        assert_boundary( cid, 0, w );
        double* col = get_col(cid);
        col[  0] = a;
        col[  w] = b;
        col[2*w] = c;
    }

    double KMatrix::norm() const {
        // returns the frobenius norm
        return mat_norm( get_pointer(), h, w );
    }
    double KMatrix::squared_norm() const {
        return mat_norm_sq( get_pointer(), h, w );
    }
    double KMatrix::normalize() {
        return mat_normalize( get_pointer(), h, w );
    }

    void KMatrix::print(const char* str) const {
        matrix_print(str, m_buffer, h, w, false, true);
    }

    void KMatrix::save( const string& str ) const {
        ofstream fout;
        open_or_fail( str, fout );
        save( fout, "X" );
        fout.close();
    }

    void KMatrix::save( ofstream& fout, const char* name ) const {
        fout<< name << " " << h << " " << w << " ";
        for( int y=0; y<h; y++ ) {
            for( int x=0; x<w; x++ ) {
                fout<< std::setprecision(24);
                fout<< m_buffer[ y*w+x ]<<" ";
            }
        }
        fout << endl;
        check_file_stream_error( fout );
    }

    void KMatrix::save_binary( ofstream& fout ) const {
        write_bparam( fout, h );
        write_bparam( fout, w );
        write_bparam( fout, m_buffer, h*w );
    }

    void KMatrix::load_binary( ifstream& fin ) {
        int th, tw;
        read_bparam( fin, th );
        read_bparam( fin, tw );
        init( th, tw );
        read_bparam( fin, m_buffer, th*tw );
    }

    void KMatrix::load( const string& str ) {
        ifstream fin;
        open_or_fail( str, fin );
        load( fin, "X" );
        fin.close();
    }

    void KMatrix::load( ifstream& fin, const char* name ) {
        string stmp;
        read_string( fin, stmp, name );
        fin >> h >> w ;
        if( h <= 0 || w <= 0 || h > 10240 || w > 10240 ) {
            logman_fatal_g("invalid matrix dimensions [%d %d]", h, w );
        }
        init( h, w );
        for( int y=0; y<h; y++ ) {
            for( int x=0; x<w; x++ ) {
                fin >> m_buffer[ y*w+x ];
            }
        }
        check_file_stream_error( fin );
    }

//
//
//

    /// element-by-element multiplication C = A.*B
    void mat_mat_elem( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        assert_statement( (A.h==B.h) && (A.w==B.w), "invalid matrix size" );
        C.init( A.h, A.w );
        mat_mat_elem( A(), A.h, A.w, B(), B.h, B.w, C(), C.h, C.w );
    }

    void mat_mat( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        C.init( A.h, B.w );
        mat_mat( A(), A.h, A.w, B(), B.h, B.w, C(), C.size() );
    }

    void mat_mat_trans( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        C.init( A.h, B.h );
        mat_mat_trans( A(), A.h, A.w, B(), B.h, B.w, C(), C.size() );
    }

    void mat_trans_mat( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        assert_statement( A.h == B.h, "invalid matrix dimensions" );
        C.init( A.w, B.w );
        mat_trans_mat( A(), A.h, A.w, B(), B.h, B.w, C(), C.size() );
    }

    void mat_mat_mat_trans( const KMatrix& A, const KMatrix& B, const KMatrix& C,
                            KMatrix& D ) {
        D.init( A.h, C.h );
        mat_mat_mat_trans( A(), A.h, A.w, B(), B.h, B.w, C(), C.h, C.w, D(), D.size() );
    }

    void mat_mat_mat( const KMatrix& A, const KMatrix& B, const KMatrix& C,
                      KMatrix& D ) {
        D.init( A.h, C.w );
        mat_mat_mat( A(), A.h, A.w, B(), B.h, B.w, C(), C.h, C.w, D(), D.size() );
    }

    void mat_trans_mat_mat( const KMatrix& A, const KMatrix& B, const KMatrix& C,
                            KMatrix& D ) {
        D.init( A.w, C.w );
        mat_trans_mat_mat( A(), A.h, A.w, B(), B.h, B.w, C(), C.h, C.w, D(), D.size() );
    }

    void mat_minus_mat( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        C.init( A.h, A.w );
        mat_minus_mat( A(), A.h, A.w, B(), B.h, B.w, C(), C.h, C.w );
    }

    void mat_plus_mat( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        C.init( A.h, A.w );
        mat_plus_mat( A(), A.h, A.w, B(), B.h, B.w, C(), C.h, C.w );
    }

    void cross_matrix( const KMatrix& b, KMatrix& Bx ) {
        assert_statement( b.size() == 3, "invalid matrix b" );
        Bx.init( 3, 3 );
        mat_cross_form( b(), 3, Bx(), Bx.size() );
    }

    void mat_row_add( const KMatrix& A, int arid, double alpha,
                      const KMatrix& B, int brid, double beta,
                      KMatrix& C, int crid ) {
        int nc = A.w;
        passert_statement( (nc == B.w) && (nc == C.w), "invalid matrix sizes" );
        passert_boundary( arid, 0, A.h );
        passert_boundary( brid, 0, B.h );
        passert_boundary( crid, 0, C.h );
        const double* arow = A.get_row(arid);
        const double* brow = B.get_row(brid);
        double      * crow = C.get_row(crid);
        for( int i=0; i<nc; i++ ) {
            crow[i] = alpha * arow[i] + beta * brow[i];
        }
    }

    void mat_ABAt ( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        C.init( A.h, A.h );
        mat_ABAt( A(), A.h, A.w, B(), B.h, B.w, C(), C.size() );
    }

    bool mat_inv_3( const KMatrix& A, KMatrix& iA, double inversion_threshold  ) {
        assert_statement( A.is_square() && A.h == 3, "invalid matrix" );
        iA.init(3,3);
        return mat_inv_3( A(), A.h, iA(), iA.h, inversion_threshold );
    }

    /// solves the Ax=b by computing the (brute)inverse of the A matrix for 3d
    /// matrices. returns false if inverse computation fails...
    bool mat_solve_Ax_b_3( const KMatrix& A, const KMatrix& b, KMatrix& x ) {
        assert_statement( A.is_square() && A.h == 3, "invalid matrix size" );
        assert_statement( b.size() == 3, "invalid size" );
        x.init(3,1);
        return mat_solve_Ax_b_3( A(), A.h, A.w, b(), b.size(), x(), x.size() );
    }

    void mat_concat_hor( const KMatrix& A, const KMatrix& B, KMatrix& cAB ) {
        assert_statement( A.h==B.h, "matrix dimensions invalid" );
        cAB.init( A.h, A.w + B.w );
        cAB.copy( A, 0, 0, A.h, A.w, 0, 0   );
        cAB.copy( B, 0, 0, B.h, B.w, 0, A.w );
    }

    void mat_concat_ver( const KMatrix& A, const KMatrix& B, KMatrix& cAB ) {
        assert_statement( A.w==B.w, "matrix dimensions invalid" );
        cAB.init( A.h+B.h, A.w );
        cAB.copy( A, 0, 0, A.h, A.w,   0, 0 );
        cAB.copy( B, 0, 0, B.h, B.w, A.h, 0 );
    }

    /// decomposes AB so that AB = [A | B];
    void mat_deconcat_hor( const KMatrix& AB, KMatrix& A, KMatrix& B ) {
        assert_statement( (AB.h==A.h) && (AB.h==B.h), "invalid dimensions" );
        assert_statement( AB.w == A.w + B.w, "invalid dimensions" );
        A.copy( AB, 0, 0,   A.h, A.w, 0, 0 );
        B.copy( AB, 0, A.w, B.h, B.w, 0, 0 );
    }

    void mat_Ax_plus_y( const KMatrix& A, const KMatrix& x, const KMatrix& y, KMatrix& c ) {
        passert_statement( (x.w==1) && (y.w==1), "x and y should be column vectors" );
        assert_statement( (A.h==x.h) && (A.h==y.h), "invalid matrix dimensions" );
        c.init( A.h, 1 );
        for( int r=0; r<A.h; r++ ) {
            c[r] = dot( A.get_row(r), x(), A.w ) + y[r];
        }
    }

    /// c = alpha * A * x + beta * y; x and y are column vectors.
    void mat_alpha_Ax_plus_beta_y( const double& alpha, const KMatrix& A, const KMatrix& x,
                                   const double& beta,  const KMatrix& y, KMatrix& c ) {
        passert_statement( (x.w==1) && (y.w==1), "x and y should be column vectors" );
        assert_statement( (A.h==x.h) && (A.h==y.h), "invalid matrix dimensions" );
        c.init( A.h, 1 );
        for( int r=0; r<A.h; r++ ) {
            c[r] = alpha * dot( A.get_row(r), x(), A.w ) + beta * y[r];
        }

    }

    double mat_pseudo_inv( const KMatrix& A, KMatrix& iA ) {
        iA.init( A.w, A.h );
        return mat_pseudo_inv( A(), A.h, A.w, iA(), iA.h, iA.w );
    }



    /// A = R*Q:
    /// R: upper triangular, Q: rotation matrix
    bool rq_givens_decomposition_3( const KMatrix& A, KMatrix& R, KMatrix& Q ) {
        assert_statement( A.is_square() && A.h == 3, "invalid input A" );

        // set A(2,1) <-- 0 by Qx
        double r = sqrt( sq(A(2,1)) + sq( A(2,2) ) );
        double c = -A(2,2)/r;
        double s =  A(2,1)/r;

        if( !is_a_number( s*c ) ) return false;

        KMatrix Qx(3,3);
        Qx.init33( 1., 0., 0., 0., c, -s, 0., s, c );

        KMatrix AQx(3,3);
        mat_mat( A, Qx, AQx );

        // set A(2,0) <-- 0
        r = sqrt( sq(AQx(2,2)) + sq(AQx(2,0)) );
        c = AQx(2,2)/r;
        s = AQx(2,0)/r;
        if( !is_a_number( s*c ) ) return false;

        KMatrix Qy(3,3);
        Qy.init33(c, 0., s, 0., 1., 0., -s, 0., c);

        KMatrix AQxQy(3,3);
        mat_mat( AQx, Qy, AQxQy );

        // set A(1,0) <-- 0
        r = sqrt( sq(AQxQy(1,0)) + sq(AQxQy(1,1)) );
        c = -AQxQy(1,1) / r;
        s =  AQxQy(1,0) / r;
        if( !is_a_number( s*c ) ) return false;

        KMatrix Qz(3,3);
        Qz.init33( c, -s, 0., s, c, 0., 0., 0., 1. );

        mat_mat( AQxQy, Qz, R );
        mat_mat_mat( Qx, Qy, Qz, Q );

        Q.transpose();


        KMatrix Ar(3,3);
        mat_mat(R,Q,Ar);

        for(int i=0; i<9; i++) {
            if( fabs(A[i]) < 1e-15 )
                continue;
            if( fabs( (A[i]-Ar[i])/A[i] ) > 1e-3 )
                return false;
        }

        return true;
    }

    void mat_col_copy( const KMatrix& src, int sc, double alpha, KMatrix& dst, int dc ) {
        assert_boundary( dc, 0, dst.w );
        assert_boundary( sc, 0, src.w );
        passert_statement( dst.h == src.h, "dst src heights differ" );

        const double* src_col = src.get_col(sc);
        double* dst_col = dst.get_col(dc);

        for( int i=0; i<dst.h; i++ ) {
            dst_col[i*dst.w] = alpha * src_col[i*src.w];
        }
    }


}


