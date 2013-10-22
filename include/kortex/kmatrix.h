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
//
//  KMatrix class will be turned into a container class shortly. Interface may
//  change a lot - use matrix.h raw routines if necessary
//
#ifndef KORTEX_KMATRIX_H
#define KORTEX_KMATRIX_H

#include <kortex/mem_unit.h>
#include <kortex/check.h>
#include <fstream>
#include <string>

using std::ofstream;
using std::ifstream;
using std::string;

namespace kortex {

    /// stores content in row-major order
    class KMatrix {
    public:
        int h;
        int w;
        KMatrix();
        KMatrix( int nr, int nc );
        KMatrix( const double* data, int nr, int nc );
        KMatrix( const KMatrix& rhs );
        ~KMatrix();

        int  size() const { return h*w; }

        void init( const double* data );
        void init( int nr, int nc );
        void init33( const double& a0, const double& a1, const double& a2,
                     const double& a3, const double& a4, const double& a5,
                     const double& a6, const double& a7, const double& a8 );
        void init3( const double& a0, const double& a1, const double& a2 );

        void wrap( double* data, int nr, int nc );
        void unwrap();

        bool is_square() const { return (h==w); }

        void identity();
        void zero();

        double      * get_pointer()       { return m_buffer; }
        const double* get_pointer() const { return m_buffer; }

        const double* get_row( int rid ) const { return m_buffer+rid*w; }
        double      * get_row( int rid )       { return m_buffer+rid*w; }
        const double* get_col( int cid ) const { return m_buffer + cid; }
        double      * get_col( int cid )       { return m_buffer + cid; }

        /// set row r with row_data - assumes row_data has at least w-elements
        void set_row( int r,               const double* row_data );
        /// set row r with alpha * row_data - assumes row_data has at least w-elements
        void set_row( int r, double alpha, const double* row_data );
        /// set the contents of the row with row_data[0:csz-1] starting from r,c
        void set_row( int r, int c, const double* row_data, int csz );

        /// set col c with col_data - assumes col_data has at least h-elements
        void set_col( int c,               const double* col_data );
        /// set col c with alpha * col_data - assumes col_data has at least h-elements
        void set_col( int c, double alpha, const double* col_data );
        /// set the contents of the col with cow_data[0:csz-1] starting from r,c
        void set_col( int r, int c, const double* col_data, int rsz );

        /// copies rhs matrix data - does not ensure sizes are same - see clone
        void copy( const KMatrix& rhs );

        /// copies  this[ dr:dr+srsz, dc:dc+srsz ] =  rhs[ sr:sr+srsz, sc:sc+scsz ]
        void copy( const KMatrix& rhs, int sr, int sc, int srsz, int scsz, int dr=0, int dc=0 );
        /// copies rhs into this starting at [dr,dc]
        void copy( const KMatrix& rhs, int dr, int dc );

        /// makes an exact copy of the rhs matrix - does ensure matrix sizes are
        /// the same - is not defined for wrapped matrices
        void clone( const KMatrix& rhs);

        void negate();
        void reshape( int nh, int nw );

        void transpose();

        void scale( double s );
        void scale_row( int rid, double alpha );
        void scale_col( int cid, double alpha );

        double trace() const;
        double det3() const;

        double*       operator() ()       { return m_buffer; }
        const double* operator() () const { return m_buffer; }
        double&       operator() (int y0, int x0)       { return m_buffer[y0*w+x0]; }
        const double& operator() (int y0, int x0) const { return m_buffer[y0*w+x0]; }

        double&       operator[] (int x)       { return m_buffer[x];       }
        const double& operator[] (int x) const { return m_buffer[x];       }

        KMatrix& operator= ( const KMatrix& rhs );

        double normalize();

        double norm() const;
        double squared_norm() const;

        MemUnit& get_memory() { return m_memory; }

        void set_row_3( int rid, const double& a, const double& b, const double& c );
        void set_col_3( int cid, const double& a, const double& b, const double& c );

        void print(const char* str) const;

        void save( const string& str ) const;
        void load( const string& str );

        void save_binary( ofstream& fout ) const;
        void load_binary( ifstream& fin  );

        void save( ofstream& str, const char* name ) const;
        void load( ifstream& str, const char* name );

    private:
        double* m_buffer;
        MemUnit m_memory;

        bool    m_wrapped;
    };

    /// element-by-element multiplication C = A.*B
    void mat_mat_elem( const KMatrix& A, const KMatrix& B, KMatrix& C );
    void mat_mat( const KMatrix& A, const KMatrix& B, KMatrix& C );
    void mat_mat_trans( const KMatrix& A, const KMatrix& B, KMatrix& C );
    void mat_trans_mat( const KMatrix& A, const KMatrix& B, KMatrix& C );
    void mat_mat_mat_trans( const KMatrix& A, const KMatrix& B, const KMatrix& C,
                            KMatrix& D );
    void mat_mat_mat( const KMatrix& A, const KMatrix& B, const KMatrix& C,
                      KMatrix& D );
    void mat_trans_mat_mat( const KMatrix& A, const KMatrix& B, const KMatrix& C,
                            KMatrix& D );
    void mat_minus_mat( const KMatrix& A, const KMatrix& B, KMatrix& C );
    void mat_plus_mat( const KMatrix& A, const KMatrix& B, KMatrix& C );
    void cross_matrix( const KMatrix& b_, KMatrix& Bx );

    // C(crid,:) = alpha * A(arid,:) + beta * B(brid,:);
    void mat_row_add( const KMatrix& A, int arid, double alpha,
                      const KMatrix& B, int brid, double beta,
                      KMatrix& C, int crid );

    /// dst(:,dc) = alpha * src(:,di);
    void mat_col_copy( const KMatrix& src, int sc, double alpha, KMatrix& dst, int dc );

    void mat_ABAt ( const KMatrix& A, const KMatrix& B, KMatrix& C );

    bool mat_inv_3( const KMatrix& A, KMatrix& iA, double inversion_threshold );

    /// solves the Ax=b by computing the (brute)inverse of the A matrix for 3d
    /// matrices. returns false if inverse computation fails...
    bool mat_solve_Ax_b_3( const KMatrix& A, const KMatrix& b, KMatrix& x );

    void mat_concat_hor( const KMatrix& A, const KMatrix& B, KMatrix& cAB );
    void mat_concat_ver( const KMatrix& A, const KMatrix& B, KMatrix& cAB );

    /// decomposes AB so that AB = [A | B];
    void mat_deconcat_hor( const KMatrix& AB, KMatrix& A, KMatrix& B );


    bool rq_givens_decomposition_3( const KMatrix& A, KMatrix& R, KMatrix& Q );

    /// c = A * x + y; x and y are column vectors.
    void mat_Ax_plus_y( const KMatrix& A, const KMatrix& x, const KMatrix& y, KMatrix& c );

    /// c = alpha * A * x + beta * y; x and y are column vectors.
    void mat_alpha_Ax_plus_beta_y( const double& alpha, const KMatrix& A, const KMatrix& x,
                                   const double& beta,  const KMatrix& y, KMatrix& c );

    double mat_pseudo_inv( const KMatrix& A, KMatrix& iA );

}

#endif
