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

#include <kortex/matrix.h>
#include <kortex/check.h>
#include <kortex/linear_algebra.h>
#include <kortex/mem_manager.h>

#include <cstring>

#define MAT_EPS 1e-36

#ifdef DEBUG
#define assert_matrix_compat(op,ra,ca,rb,cb)   { assert_statement_g( matrix_compatibility(op,ra,ca,rb,cb), "incompatible matrices [op %s] [A: %d %d] [B: %d %d]", mat_op_string(op).c_str(), ra, ca, rb, cb ); }
#define assert_matrix_size(op,ra,ca,rb,cb,csz) { assert_statement_g( mat_out_size(op,ra,ca,rb,cb) == csz, "insufficient matrix size for [op %s] [A: %d %d] [B: %d %d] [Csz %d]", mat_op_string(op).c_str(), ra, ca, rb, cb, csz ); }
#define assert_matrix_init(A,ra,ca) { assert_matrix_init_(A,ra,ca); }
#else
#define assert_matrix_compat(op,ra,ca,rb,cb) { }
#define assert_matrix_size(op,ra,ca,rb,cb,csz) {  }
#define assert_matrix_init(A,ra,ca) { }
#endif

#define passert_matrix_compat(op,ra,ca,rb,cb)   { passert_statement_g( matrix_compatibility(op,ra,ca,rb,cb), "incompatible matrices [op %s] [%d %d] [%d %d]", mat_op_string(op).c_str(), ra, ca, rb, cb ); }
#define passert_matrix_size(op,ra,ca,rb,cb,csz) { passert_statement_g( mat_out_size(op,ra,ca,rb,cb) == csz, "insufficient matrix size for [op %s] [A: %d %d] [B: %d %d] [Csz %d]", mat_op_string(op).c_str(), ra, ca, rb, cb, csz ); }

namespace kortex {

    enum MatrixOperation {
        MO_MUL,      // A*B
        MO_T_MUL,    // trans(A)*B
        MO_MUL_T,    // A*trans(B)
        MO_I_MUL,    // inv(A)*B
        MO_PI_MUL    // pseudo_inv(A)*B
    };

    bool matrix_compatibility( const MatrixOperation& op,
                               const int& nra, const int& nca,
                               const int& nrb, const int& ncb ) {
        switch( op ) {
        case MO_MUL   : return (nca == nrb);
        case MO_T_MUL : return (nra == nrb);
        case MO_MUL_T : return (nca == ncb);
        case MO_I_MUL : return (nra == nca) && ( nra == nrb);
        case MO_PI_MUL: return (nra == nrb);
        default       : switch_fatality();
        }
    }

    int mat_out_size( const MatrixOperation& op,
                      const int& nra, const int& nca,
                      const int& nrb, const int& ncb ) {
        assert_number( nra*nca );
        assert_number( nrb*ncb );
        switch( op ) {
        case MO_MUL   : return nra*ncb;
        case MO_T_MUL : return nca*ncb;
        case MO_MUL_T : return nra*nrb;
        case MO_I_MUL : return nra*ncb;
        case MO_PI_MUL: return nca*ncb;
        default       : switch_fatality();
        }
    }

    string mat_op_string( const MatrixOperation& op ) {
        switch( op ) {
        case MO_MUL   : return string("A*B"); break;
        case MO_T_MUL : return string("trans(A)*B"); break;
        case MO_MUL_T : return string("A*trans(B)"); break;
        case MO_I_MUL : return string("inv(A)*B"); break;
        case MO_PI_MUL: return string("pseudo_inv(A)*B"); break;
        default       : switch_fatality();
        }
    }

    void assert_matrix_init_( const double* A, int nra, int nca ) {
        assert_pointer( A );
        assert_statement_g( is_a_number( nra ) &&
                            is_a_number( nca ) &&
                            (nra>0) && (nca>0),
                            "invalid matrix size [%d %d]", nra, nca );
    }

//
// single mat ops
//
    double mat_det_3( const double* A, int nra ) {
        assert_pointer( A );
        assert_statement_g( nra == 3, "invalid sized matrix [%d]", nra );
        return
            A[0] * ( A[4]*A[8] - A[5]*A[7] ) -
            A[1] * ( A[3]*A[8] - A[5]*A[6] ) +
            A[2] * ( A[3]*A[7] - A[4]*A[6] );
    }

    double mat_trace( const double* A, int nra, int nca ) {
        assert_matrix_init(A, nra, nca);
        int d = std::min(nra, nca);
        double t=0.0;
        for( int i=0; i<d; i++ )
            t += A[ i*nca+i ];
        return t;
    }

    void mat_zero( double* A, int nra, int nca ) {
        assert_matrix_init( A, nra, nca );
        memset( A, 0, sizeof(*A)*nra*nca );
    }
    void mat_identity( double* A, int nra, int nca ) {
        assert_matrix_init( A, nra, nca );
        memset( A, 0, sizeof(*A)*nra*nca );
        int d = std::min(nra, nca);
        for( int i=0; i<d; i++ )
            A[ i*nca+i ] = 1.0;
    }

    void mat_cross_form( const double* v, int vsz,
                         double* A, int asz ) {
        assert_pointer( v && A );
        assert_statement( vsz == 3, "invalid number of inputs" );
        assert_statement( asz == 9, "A size not enough" );
        A[0] =     0; A[1] = -v[2]; A[2] =  v[1];
        A[3] =  v[2]; A[4] =     0; A[5] = -v[0];
        A[6] = -v[1]; A[7] =  v[0]; A[8] =     0;
    }

    void mat_set_row( double* A, int nra, int nca, int rid, const double* rdata, int rdsz ) {
        assert_matrix_init( A, nra, nca );
        assert_boundary( rid, 0, nra );
        assert_pointer( rdata );
        assert_statement_g( rdsz == nca, "invalid rdsz [%d]", rdsz );
        memcpy( A + rid*nca, rdata, sizeof(*A)*nca );
    }

    // A[ r, : ] <-- alpha * rdata
    void mat_set_row( double* A, int nra, int nca, int rid, double alpha, const double* rdata, int rdsz ) {
        assert_matrix_init( A, nra, nca );
        assert_boundary( rid, 0, nra );
        assert_pointer( rdata );
        assert_statement_g( rdsz == nca, "invalid rdsz [%d]", rdsz );
        assert_number( alpha );
        double* arow = A + rid * nca;
        for( int i=0; i<nca; i++ ) {
            arow[i] = alpha * rdata[i];
        }
    }

    // A[ r, c:c+rdsz] <-- rdata
    void mat_set_row_from_point( double* A, int nra, int nca,
                                 int r0, int c0,
                                 const double* rdata, int rdsz ) {
        assert_matrix_init( A, nra, nca );
        assert_boundary( r0, 0, nra );
        assert_boundary( c0, 0, nca );
        assert_boundary( c0+rdsz, 0, nca );
        assert_pointer( rdata );
        assert_number( rdsz );
        double* arow = A + r0*nca + c0;
        memcpy( arow, rdata, sizeof(*A)*rdsz );
    }

    void mat_set_col( double* A, int nra, int nca, int cid, const double* cdata, int cdsz ) {
        assert_matrix_init( A, nra, nca );
        assert_boundary( cid, 0, nca );
        assert_pointer( cdata );
        assert_statement_g( cdsz == nra, "invalid cdsz [%d]", cdsz );
        double *acol = A + cid;
        for( int i=0; i<nra; i++ )
            acol[i*nca] = cdata[i];
    }

    void mat_set_col( double* A, int nra, int nca, int cid, double alpha, const double* cdata, int cdsz ) {
        assert_matrix_init( A, nra, nca );
        assert_boundary( cid, 0, nca );
        assert_pointer( cdata );
        assert_statement_g( cdsz == nra, "invalid cdsz [%d]", cdsz );
        assert_number( alpha );
        double *acol = A + cid;
        for( int i=0; i<nra; i++ )
            acol[i*nca] = alpha * cdata[i];
    }

    void mat_set_col_from_point( double* A, int nra, int nca, int r0, int c0,
                                 const double* cdata, int cdsz ) {
        assert_matrix_init( A, nra, nca );
        assert_boundary( r0, 0, nra );
        assert_boundary( c0, 0, nca );
        assert_boundary( r0+cdsz, 0, nra );
        assert_pointer( cdata );
        assert_number( cdsz );
        double* acol = A + r0*nca + c0;
        for( int i=0; i<cdsz; i++ )
            acol[i*nca] = cdata[i];
    }

    void mat_negate( double* A, int nra, int nca ) {
        assert_matrix_init( A, nra, nca );
        int asz = nra * nca;
        for( int r=0; r<asz; r++ )
            A[r] = -A[r];
    }

    void mat_transpose( const double* A, int nra, int nca,
                        double* At, int atsz ) {
        assert_matrix_init( A, nra, nca );
        assert_statement( atsz == nra*nca, "invalid mat size" );
        assert_statement( A != At, "aliasing not allowed" );
        for( int r=0; r<nra; r++ ) {
            const double* arow = A + r*nca;
            for( int c=0; c<nca; c++ ) {
                At[ c*nra+r ] = arow[c];
            }
        }
    }

    void mat_scale( double* A, int nra, int nca, double s ) {
        assert_matrix_init( A, nra, nca );
        assert_number( s );
        int asz = nra * nca;
        for( int i=0; i<asz; i++ )
            A[i] *= s;
    }

    double mat_norm( const double* A, int nra, int nca ) {
        assert_matrix_init( A, nra, nca );
        double nrm = 0.0;
        int asz = nra*nca;
        for( int i=0; i<asz; i++ )
            nrm += A[i]*A[i];
        return sqrt( nrm );
    }

    double mat_norm_sq( const double* A, int nra, int nca ) {
        assert_matrix_init( A, nra, nca );
        double nrm = 0.0;
        int asz = nra*nca;
        for( int i=0; i<asz; i++ )
            nrm += A[i]*A[i];
        return nrm;
    }

    double mat_normalize( double* A, int nra, int nca ) {
        assert_matrix_init( A, nra, nca );
        double nrm = mat_norm( A, nra, nca );
        if( nrm < MAT_EPS ) {
            logman_warning_g( "matrix norm too small [%e]", nrm );
            nrm = MAT_EPS;
        }
        mat_scale( A, nra, nca, 1.0/nrm );
        return nrm;
    }

    inline double det_2(double a00, double a01, double a10, double a11) {
        return a00*a11 - a01*a10;
    }

    bool mat_inv_3( const double* A, int nra,
                    double* iA, int nria,
                    double inversion_threshold ) {
        assert_pointer( A && iA );
        assert_statement( nra == 3 && nria == 3, "invalid matrix size" );

        double d = mat_det_3( A, nra );
        if( inversion_threshold == 0.0 ) {
            inversion_threshold = MAT_EPS;
        }
        if( fabs(d) < inversion_threshold ) {
            d = 0.0;
            mat_zero( iA, nria, nria );
            return false;
        }
        double id = 1.0 / d;
        iA[0] =  det_2( A[4], A[5], A[7], A[8] ) * id;
        iA[1] = -det_2( A[1], A[2], A[7], A[8] ) * id;
        iA[2] =  det_2( A[1], A[2], A[4], A[5] ) * id;
        iA[3] = -det_2( A[3], A[5], A[6], A[8] ) * id;
        iA[4] =  det_2( A[0], A[2], A[6], A[8] ) * id;
        iA[5] = -det_2( A[0], A[2], A[3], A[5] ) * id;
        iA[6] =  det_2( A[3], A[4], A[6], A[7] ) * id;
        iA[7] = -det_2( A[0], A[1], A[6], A[7] ) * id;
        iA[8] =  det_2( A[0], A[1], A[3], A[4] ) * id;
        return true;
    }

    bool mat_inv( const double* A, int nra, int nca,
                  double* iA, int nria, int ncia ) {
        assert_pointer( A && iA );
        assert_statement( is_a_number(nra) && (nra==nca), "invalid matrix size" );
        assert_statement( is_a_number(nria) && (nria==ncia) && (nra==ncia), "invalid matrix size" );
        memcpy( iA, A, sizeof(*iA)*nra*nra );
        int info = matrix_invert_g_lu( A, nra, iA );
        return (info==0);
    }

    double mat_pseudo_inv( const double* A, int nra, int nca,
                           double      *iA, int nria, int ncia ) {
        assert_matrix_init( A, nra,  nca  );
        assert_matrix_init(iA, nria, ncia );
        assert_statement( (nra==ncia) && (nca==nria), "invalid matrix size" );
        return matrix_pseudo_invert_g_svd( A, nra, nca, iA );
    }

//
// multiple mat ops
//
    void mat_mat( const double* A, int nra, int nca,
                  const double* B, int nrb, int ncb,
                  double* C, int csz ) {
        assert_matrix_init( A, nra, nca );
        assert_matrix_init( B, nrb, ncb );
        assert_statement( (A!=C) && (B!=C), "aliasing not allowed" );
        assert_pointer( C );
        assert_number( csz );
        assert_matrix_compat( MO_MUL, nra, nca, nrb, ncb );
        assert_matrix_size( MO_MUL, nra, nca, nrb, ncb, csz );
        double res=0.0;
        int r, c, k;
        for( r=0; r<nra; r++ ) {
            const double* arow = A+r*nca;
            double      * crow = C+r*ncb;
            for( c=0; c<ncb; c++ ) {
                const double* bcol = B + c;
                res = 0.0;
                for( k=0; k<nca; k++ )
                    res += arow[k] * bcol[ k*ncb ];
                crow[c] = res;
            }
        }
    }

    void mat_mat_trans( const double* A, int nra, int nca,
                        const double* B, int nrb, int ncb,
                        double* C, int csz ) {
        assert_matrix_init( A, nra, nca );
        assert_matrix_init( B, nrb, ncb );
        assert_pointer( C );
        assert_statement( (A!=C) && (B!=C), "aliasing not allowed" );
        assert_number( csz );
        assert_matrix_compat( MO_MUL_T, nra, nca, nrb, ncb );
        assert_matrix_size( MO_MUL_T, nra, nca, nrb, ncb, csz );
        for( int r=0; r<nra; r++ ) {
            const double* arow = A+r*nca;
            double      * crow = C+r*nrb;
            for( int c=0; c<nrb; c++ ) {
                const double* brow = B + c*ncb;
                double res = 0.0;
                for( int k=0; k<nca; k++ )
                    res += arow[k]*brow[k];
                crow[c]=res;
            }
        }
    }

    void mat_trans_mat( const double* A, int nra, int nca,
                        const double* B, int nrb, int ncb,
                        double* C, int csz ) {
        assert_matrix_init( A, nra, nca );
        assert_matrix_init( B, nrb, ncb );
        assert_pointer( C );
        assert_statement( (A!=C) && (B!=C), "aliasing not allowed" );
        assert_number( csz );
        assert_matrix_compat( MO_T_MUL, nra, nca, nrb, ncb );
        assert_matrix_size( MO_T_MUL, nra, nca, nrb, ncb, csz );
        for( int r=0; r<nca; r++ ) {
            const double* acol = A+r;
            double      * crow = C+r*ncb;
            for( int c=0; c<ncb; c++ ) {
                const double* bcol = B + c;
                double res = 0.0;
                for( int k=0; k<nra; k++ )
                    res += acol[k*nca]*bcol[k*ncb];
                crow[c]=res;
            }
        }
    }

    void mat_mat_mat( const double* A, int nra, int nca,
                      const double* B, int nrb, int ncb,
                      const double* C, int nrc, int ncc,
                      double      * D, int dsz ) {
        assert_pointer( A && B && C );
        int tsz = mat_out_size( MO_MUL, nra, nca, nrb, ncb );
        double* T = NULL; allocate( T, tsz );
        mat_mat(A, nra, nca, B, nrb, ncb, T, tsz );
        mat_mat(T, nra, ncb, C, nrc, ncc, D, dsz );
        deallocate( T );
    }

    void mat_mat_mat_trans( const double* A, int nra, int nca,
                            const double* B, int nrb, int ncb,
                            const double* C, int nrc, int ncc,
                            double      * D, int dsz ) {
        assert_pointer( A && B && C );
        int tsz = mat_out_size( MO_MUL, nra, nca, nrb, ncb );
        double* T = NULL; allocate( T, tsz );
        mat_mat(A, nra, nca, B, nrb, ncb, T, tsz );
        mat_mat_trans( T, nra, ncb, C, nrc, ncc, D, dsz );
        deallocate( T );
    }


    void mat_trans_mat_mat( const double* A, int nra, int nca,
                            const double* B, int nrb, int ncb,
                            const double* C, int nrc, int ncc,
                            double      * D, int dsz ) {
        assert_pointer( A && B && C );
        int tsz = mat_out_size( MO_T_MUL, nra, nca, nrb, ncb );
        double* T = NULL; allocate( T, tsz );
        mat_trans_mat(A, nra, nca, B, nrb, ncb, T, tsz );
        mat_mat( T, nca, ncb, C, nrc, ncc, D, dsz );
        deallocate( T );
    }

    void mat_minus_mat( const double* A, int nra, int nca,
                        const double* B, int nrb, int ncb,
                        double      * C, int nrc, int ncc ) {
        assert_matrix_init( A, nra, nca );
        assert_matrix_init( B, nrb, ncb );
        assert_matrix_init( C, nrc, ncc );
        assert_statement( (nra == nrb) && (nrb == nrc), "invalid mat size" );
        assert_statement( (nca == ncb) && (ncb == ncc), "invalid mat size" );

        for( int r=0; r<nra; r++ ) {
            const double* arow = A + r*nca;
            const double* brow = B + r*nca;
            double      * crow = C + r*nca;
            for( int c=0; c<nca; c++ ) {
                crow[c] = arow[c]-brow[c];
            }
        }
    }

    void mat_plus_mat( const double* A, int nra, int nca,
                       const double* B, int nrb, int ncb,
                       double      * C, int nrc, int ncc ) {
        assert_matrix_init( A, nra, nca );
        assert_matrix_init( B, nrb, ncb );
        assert_matrix_init( C, nrc, ncc );
        assert_statement( (nra == nrb) && (nrb == nrc), "invalid mat size" );
        assert_statement( (nca == ncb) && (ncb == ncc), "invalid mat size" );

        for( int r=0; r<nra; r++ ) {
            const double* arow = A + r*nca;
            const double* brow = B + r*nca;
            double      * crow = C + r*nca;
            for( int c=0; c<nca; c++ ) {
                crow[c] = arow[c]+brow[c];
            }
        }
    }

    void mat_mat_elem( const double* A, int nra, int nca,
                       const double* B, int nrb, int ncb,
                       double      * C, int nrc, int ncc ) {
        assert_matrix_init( A, nra, nca );
        assert_matrix_init( B, nrb, ncb );
        assert_matrix_init( C, nrc, ncc );
        assert_statement( (nra == nrb) && (nrb == nrc), "invalid mat size" );
        assert_statement( (nca == ncb) && (ncb == ncc), "invalid mat size" );

        for( int r=0; r<nra; r++ ) {
            const double* arow = A + r*nca;
            const double* brow = B + r*nca;
            double      * crow = C + r*nca;
            for( int c=0; c<nca; c++ ) {
                crow[c] = arow[c]*brow[c];
            }
        }
    }


    void mat_ABAt( const double* A, int nra, int nca,
                   const double* B, int nrb, int ncb,
                   double      * C, int csz ) {
        assert_matrix_init( A, nra, nca );
        assert_matrix_init( B, nrb, ncb );
        assert_statement( nrb == ncb, "invalid matrix size" );
        int tsz = mat_out_size( MO_MUL, nra, nca, nrb, ncb );
        double* T = NULL; allocate( T, tsz );
        mat_mat( A, nra, nca, B, nrb, ncb, T, tsz );
        mat_mat_trans( T, nra, ncb, A, nra, nca, C, csz );
        deallocate(T);
    }

    /// dst[ dr:dr+srsz, dc:dc+scsz ] = src[ sr:sr+srsz, sc:sc+scsz ]
    void mat_copy( const double* src, int nrs, int ncs,
                   int sr, int sc, int srsz, int scsz,
                   double* dst, int nrd, int ncd,
                   int dr, int dc ) {
        assert_statement( sr+srsz <= nrs, "src size oob" );
        assert_statement( sc+scsz <= ncs, "src size oob" );
        assert_statement( dr+srsz <= nrd, "dst size oob" );
        assert_statement( dc+scsz <= ncd, "dst size oob" );
        for( int r=0; r<srsz; r++ ) {
            const double* srow = src + (sr+r)*ncs;
            double      * drow = dst + (dr+r)*ncd;
            memcpy( drow+dc, srow+sc, sizeof(*drow)*scsz );
        }
    }

//
//
//

    bool mat_solve_Ax_b_3( const double* A, int nra, int nca,
                           const double* b, int bsz,
                           double      * x, int xsz ) {
        assert_matrix_init( A, nra, nca );
        assert_pointer( x && b );
        assert_statement( nra==nca, "invalid matrix A" );
        assert_statement( bsz==xsz && xsz == 3, "invalid size" );
        double iA[9];
        if( !mat_inv_3( A, nra, iA, 3, MAT_EPS ) )
            return false;
        mat_mat( iA, 3, 3, b, bsz, 1, x, xsz );
        return true;
    }

    /// decompose matrix A so that U is an upper triangular matrix and R is an
    /// orthonormal matrix and A = U*R
    bool rq_givens_decomposition_3( double A[9], double U[9], double R[9] ) {

        // set A[7] = 0 by Rx
        double r = sqrt( A[7]*A[7] + A[8]*A[8] );
        double c = -A[8]/r;
        double s =  A[7]/r;
        if( !is_a_number(s*c) ) return false;
        double Rx[] = { 1.0, 0.0, 0.0, 0.0, c, -s, 0.0, s, c };
        double ARx[9];
        mat_mat_3( A, Rx, ARx );

        // set A[6] = 0 by Ry
        r = sqrt( ARx[8]*ARx[8] + ARx[6]*ARx[6] );
        c = -ARx[8]/r;
        s =  ARx[6]/r;
        if( !is_a_number(s*c) ) return false;
        double Ry[] = { c, 0.0, s, 0.0, 1.0, 0.0, -s, 0.0, c };
        double ARxRy[9];
        mat_mat_3( ARx, Ry, ARxRy );

        // set A[3] = 0 by Rz
        r = sqrt( ARxRy[3]*ARxRy[3] + ARxRy[4]*ARxRy[4] );
        c = -ARxRy[4]/r;
        s =  ARxRy[3]/r;
        if( !is_a_number(s*c) ) return false;
        double Rz[] = { c, -s, 0.0, s, c, 0.0, 0.0, 0.0, 1.0 };
        mat_mat_3( ARxRy, Rz, U );
        mat_mat_mat_3( Rx, Ry, Rz, R );
        mat_transpose_3( R );

        // check decomposition
        double Ar[9];
        mat_mat_3( U, R, Ar );
        for( int i=0; i<9; i++ ) {
            if( fabs(A[i]) < 1e-15 ) continue;
            if( fabs( (A[i]-Ar[i])/A[i] ) > 1e-3 )
                return false;
        }
        return true;
    }

    int     mat_print_line( char* cstr, int csz, const double* A, int asz, const char* mat_name ) {
        int cnt = 0;
        cnt  = sprintf( cstr, "[%s ", mat_name );
        for( int i=0; i<asz; i++ )
            cnt += sprintf( cstr+cnt, "%f ", A[i] );
        cnt += sprintf( cstr+cnt, "] ");
        assert_statement( cnt <= csz, "string buffer overflowed" );
        return cnt;
    }

    /// A[rid, :] *= alpha
    void mat_scale_row_3( double* A, int rid, double alpha ) {
        assert_pointer( A );
        assert_boundary( rid, 0, 3 );
        assert_number( alpha );
        for( int i=0; i<3; i++ ) {
            A[ 3*rid+i ] *= alpha;
        }
    }

    bool mat_inv_mat_3( const double A[9], const double B[9], double C[9] ) {
        assert_statement( B != C, "mem aliasing not allowed" );
        double iA[9];
        if( !mat_inv_3( A, 3, iA, 3, 0.0 ) ) return false;
        mat_mat_3( iA, B, C );
        return true;
    }

    bool mat_inv_mat_mat_3( const double A[9], const double B[9], const double C[9], double D[9] ) {
        double BC[9];
        mat_mat_3(B, C, BC);
        return mat_inv_mat_3( A, BC, D );
    }

    bool mat_is_upper_hessenberg( const double* A, int nra, int nca ) {
        assert_pointer( A );
        assert_pointer_size( nra*nca );
        if( nra != nca ) return false;
        for( int i=2; i<nra; i++ ) {
            for( int j=0; j<i-1; j++ ) {
                if( fabs(A[i*nra+j]) > MAT_EPS )
                    return false;
            }
        }
        return true;
    }




}
