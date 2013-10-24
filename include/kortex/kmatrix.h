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
//
// a wrapper class for most matrix operations
//
#ifndef KORTEX_WMATRIX_H
#define KORTEX_WMATRIX_H

#include <kortex/mem_unit.h>
#include <kortex/matrix.h>
#include <kortex/check.h>

#include <fstream>

using std::ifstream;
using std::ofstream;


namespace kortex {

    class KMatrix {
    public:
        KMatrix();
        KMatrix( int h, int w );

        /// wraps around data - no change possible
        KMatrix( const double* data, int h, int w );
        /// wraps around data - possible to change
        KMatrix( double* data, int h, int w );

        /// copies the matrix contents - creates memory and copies the
        /// contents. see clone() also
        void copy ( const KMatrix& rhs );
        /// makes an exact copy of rhs. copies pointers and not data if rhs is a
        /// wrapped matrix
        void clone( const KMatrix& rhs );

        /// creates/owns itself memory - not possible to call for already
        /// wrapped initializations. call release() first for that situations.
        void init( int h, int w );

        /// resizes the matrix dimensions. for wrapped matrices, before and
        /// after size() should be the same - otherwise fatals out.
        void resize( int h, int w );

        /// sets the matrix data - does not do any initializations for memory !
        /// if this is a fresh matrix - call init(h,w) before. data size 'dsz'
        /// should be equivalent to nr*nc
        void set( const double* data, int dsz );

        ~KMatrix();

        /// sets the state of the matrix to init_  and deallocates any memory. see reset()
        void release();
        /// sets the state of the matrix to init_ but does not deallocate memory. see release()
        void reset();

        double      * get_pointer();
        double      * get_row( int rid );
        double      * get_col( int cid );

        const double* get_const_pointer() const;

        const double* get_pointer()      const;
        const double* get_row( int rid ) const;
        const double* get_col( int cid ) const;

        // convenience access
        double      * operator() ()       { return get_pointer(); }
        const double* operator() () const { return get_pointer(); }
        const double& operator() (int y0, int x0) const {
            assert_boundary( y0, 0, nr );
            assert_boundary( x0, 0, nc );
            return get_pointer()[ y0*nc+x0 ];
        }
        void set( int y0, int x0, double v ) {
            assert_statement( !is_const(), "cannot modify const matrix" );
            get_pointer()[ y0*nc+x0 ] = v;
        }

        /// copies  this[ dr:dr+srsz, dc:dc+srsz ] =  rhs[ sr:sr+srsz, sc:sc+scsz ]
        void copy( const KMatrix& rhs, int sr, int sc, int srsz, int scsz, int dr, int dc );
        void copy( const KMatrix& rhs, int dr, int dc );

        void   transpose();

        void   identity();
        void   zero   ();
        void   negate ();
        double normalize();

        double trace  () const;
        double det3   () const;
        double norm   () const;
        double norm_sq() const;

        void scale_row( int rid, double alpha );
        void scale_col( int cid, double alpha );
        void scale    ( double s );

        void print( const char* str ) const;

        void save( const char* file ) const;
        void load( const char* file );

        void save( ofstream& fout, const char* mat_name ) const;
        void load( ifstream& fin,  const char* mat_name );

        void save_binary( ofstream& fout ) const;
        void load_binary( ifstream& fin );


        int  h          () const { return nr;          }
        int  w          () const { return nc;          }
        int  size       () const { return nr*nc;       }
        bool is_wrapper () const { return m_wrapper;   }
        bool is_const   () const { return m_const;     }
        bool is_square  () const { return bool(nr==nc);}
    private:
        const double* m_ro_data; // read only data pointer
        double      * m_data;    // data pointer

        MemUnit m_memory;
        bool    m_wrapper;
        bool    m_const;
        int     nr;
        int     nc;

        void init_();

        size_t req_mem(int h, int w) const { return sizeof(*m_data)*h*w; }
    };

///
/// mat-ops
///

    inline bool mat_inv( const KMatrix& A, KMatrix& iA ) {
        assert_statement( A.h() == A.w(), "matrix must be square - call pseudo_inv instead" );
        iA.resize( A.h(), A.w() );
        return mat_inv( A.get_pointer(),   A.h(),  A.w(),
                        iA.get_pointer(), iA.h(), iA.w() );
    }

    inline void mat_mat( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        C.resize( A.h(), B.w() );
        mat_mat( A.get_pointer(), A.h(), A.w(),
                 B.get_pointer(), B.h(), B.w(),
                 C.get_pointer(), C.size() );
    }

    inline void mat_mat_trans( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        C.resize( A.h(), B.h() );
        mat_mat_trans( A.get_pointer(), A.h(), A.w(),
                       B.get_pointer(), B.h(), B.w(),
                       C.get_pointer(), C.size() );
    }

    inline void mat_trans_mat( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        C.resize( A.w(), B.w() );
        mat_trans_mat( A.get_pointer(), A.h(), A.w(),
                       B.get_pointer(), B.h(), B.w(),
                       C.get_pointer(), C.size() );
    }

    inline void mat_mat_mat_trans( const KMatrix& A, const KMatrix& B, const KMatrix& C,
                                   KMatrix& D ) {
        D.resize( A.h(), C.h() );
        mat_mat_mat_trans( A.get_pointer(), A.h(), A.w(),
                           B.get_pointer(), B.h(), B.w(),
                           C.get_pointer(), C.h(), C.w(),
                           D.get_pointer(), D.size() );
    }

    inline void mat_mat_mat( const KMatrix& A, const KMatrix& B, const KMatrix& C,
                             KMatrix& D ) {
        D.resize( A.h(), C.w() );
        mat_mat_mat( A.get_pointer(), A.h(), A.w(),
                     B.get_pointer(), B.h(), B.w(),
                     C.get_pointer(), C.h(), C.w(),
                     D.get_pointer(), D.size() );
    }

    inline void mat_trans_mat_mat( const KMatrix& A, const KMatrix& B, const KMatrix& C,
                                   KMatrix& D ) {
        D.resize( A.w(), C.w() );
        mat_trans_mat_mat( A.get_pointer(), A.h(), A.w(),
                           B.get_pointer(), B.h(), B.w(),
                           C.get_pointer(), C.h(), C.w(),
                           D.get_pointer(), D.size() );
    }

    inline void mat_mat_elem( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        C.resize( A.h(), A.w() );
        mat_mat_elem( A.get_pointer(), A.h(), A.w(),
                      B.get_pointer(), B.h(), B.w(),
                      C.get_pointer(), C.h(), C.w() );
    }

    inline void mat_minus_mat( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        C.resize( A.h(), A.w() );
        mat_minus_mat( A.get_pointer(), A.h(), A.w(),
                       B.get_pointer(), B.h(), B.w(),
                       C.get_pointer(), C.h(), C.w() );
    }

    inline void mat_plus_mat( const KMatrix& A, const KMatrix& B, KMatrix& C ) {
        C.resize( A.h(), A.w() );
        mat_plus_mat( A.get_pointer(), A.h(), A.w(),
                      B.get_pointer(), B.h(), B.w(),
                      C.get_pointer(), C.h(), C.w() );
    }

    inline void cross_matrix( const KMatrix& b, KMatrix& Bx ) {
        assert_statement( b.size() == 3, "invalid b matrix" );
        Bx.resize( 3, 3 );
        mat_cross_form( b.get_pointer(), 3, Bx.get_pointer(), Bx.size() );
    }

    inline void mat_ABAt( const KMatrix& A, const KMatrix& B,
                          KMatrix& C ) {
        C.resize( A.h(), A.h() );
        mat_ABAt( A.get_pointer(), A.h(), A.w(),
                  B.get_pointer(), B.h(), B.w(),
                  C.get_pointer(), C.size() );
    }

    inline bool mat_inv_3( const KMatrix& A, KMatrix& iA, double inversion_threshold ) {
        assert_statement( A.is_square() && A.h() == 3, "invalid matrix" );
        iA.resize( 3, 3 );
        return mat_inv_3( A.get_pointer(), A.h(), iA.get_pointer(), A.h(),
                          inversion_threshold );
    }

    /// solves the Ax=b by computing the (brute)inverse of the A matrix for 3d
    /// matrices. returns false if inverse computation fails...
    inline bool mat_solve_Ax_b_3( const KMatrix& A, const KMatrix& b, KMatrix& x ) {
        assert_statement( A.is_square() && A.h() == 3, "invalid matrix size" );
        assert_statement( b.size() == 3, "invalid size" );
        x.resize(3,1);
        return mat_solve_Ax_b_3( A.get_pointer(), A.h(), A.w(),
                                 b.get_pointer(), b.size(),
                                 x.get_pointer(), x.size() );
    }


    inline void mat_concat_hor( const KMatrix& A, const KMatrix& B, KMatrix& cAB ) {
        assert_statement( A.h() == B.h(), "invalid matrix size" );
        cAB.resize( A.h(), A.w() + B.w() );
        cAB.copy( A, 0, 0, A.h(), A.w(), 0, 0     );
        cAB.copy( B, 0, 0, B.h(), B.w(), 0, A.w() );
    }

    inline void mat_concat_ver( const KMatrix& A, const KMatrix& B, KMatrix& cAB ) {
        assert_statement( A.w() == B.w(), "invalid matrix size" );
        cAB.init( A.h() + B.h(), A.w() );
        cAB.copy( A, 0, 0, A.h(), A.w(),     0, 0 );
        cAB.copy( B, 0, 0, B.h(), B.w(), A.h(), 0 );
    }

    /// decomposes AB so that AB = [A | B];
    inline void mat_deconcat_hor( const KMatrix& AB, KMatrix& A, KMatrix& B ) {
        assert_statement( (AB.h()==A.h()) && (AB.h()==B.h()), "invalid matrix size" );
        assert_statement( AB.w() == A.w() + B.w(), "invalid matrix size" );
        A.copy( AB, 0, 0,     A.h(), A.w(), 0, 0 );
        B.copy( AB, 0, A.w(), B.h(), B.w(), 0, 0 );
    }

    inline double mat_pseudo_inv( const KMatrix& A, KMatrix& iA ) {
        iA.resize( A.w(), A.h() );
        return mat_pseudo_inv( A.get_pointer(), A.h(), A.w(),
                               iA.get_pointer(), iA.h(), iA.w() );
    }

    void mat_row_add( const KMatrix& A, int arid, double alpha,
                      const KMatrix& B, int brid, double beta,
                      KMatrix&       C, int crid );

    void mat_Ax_plus_y( const KMatrix& A, const KMatrix& x, const KMatrix& y, KMatrix& c );

    void mat_alpha_Ax_plus_beta_y( const double& alpha, const KMatrix& A, const KMatrix& x,
                                   const double& beta,  const KMatrix& y, KMatrix& c );


    void mat_col_copy( const KMatrix& src, int sc, double alpha, KMatrix& dst, int dc );

}

#endif
