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
#ifndef KORTEX_KVECTOR_H
#define KORTEX_KVECTOR_H

#include <cstring>
#include <kortex/check.h>

namespace kortex {

    template <typename T, int N> class KVector;

    typedef KVector<int,2> Vec2i;
    typedef KVector<int,3> Vec3i;
    typedef KVector<int,4> Vec4i;

    typedef KVector<float,2> Vec2f;
    typedef KVector<float,3> Vec3f;
    typedef KVector<float,4> Vec4f;

    typedef KVector<double,2> Vec2d;
    typedef KVector<double,3> Vec3d;
    typedef KVector<double,4> Vec4d;

    template<typename T, int N>
    class KVector {
    public:
        KVector() {};
        explicit KVector( const T* vals );
        explicit KVector( const T& val  );

        KVector( const KVector<T,N>& rhs );

        KVector( const T& v0, const T& v1 );                           // init first 3 elems
        KVector( const T& v0, const T& v1, const T& v2 );              // init first 3 elems
        KVector( const T& v0, const T& v1, const T& v2, const T& v3 ); // init first 4 elems

        KVector( const T* a, int asz );

        void set( const T* vals );
        void set( const T& v0, const T& v1 );
        void set( const T& v0, const T& v1, const T& v2 );
        void set( const T& v0, const T& v1, const T& v2, const T& v3 );

        int dim() const { return N; }

        T            dot  ( const KVector<T,N>& rhs ) const;
        KVector<T,N> cross( const KVector<T,N>& rhs ) const;

        T norm() const;
        T square_norm() const;

        T normalize();

        void zero();
        void negate() {
            for( int i=0; i<N; i++ )
                m_v[i] = -m_v[i];
        }
        void scale( const double& s ) {
            for( int i=0; i<N; i++ )
                m_v[i] *= static_cast<T>(s);
        }
        void add( const double* a, const int& asz ) {
            assert_statement( asz == N, "invalid sized add vector" );
            for( int i=0; i<asz; i++ )
                m_v[i] += static_cast<T>(a[i]);
        }

        void print( const char* str ) const {
            if( str )
                printf( "%s ", str );
            printf( "%f %f %f\n", m_v[0], m_v[1], m_v[2] );
        }

    public:
        T&          operator[]( int i )       { assert_boundary(i,0,N); return m_v[i]; }
        const T&    operator[]( int i ) const { assert_boundary(i,0,N); return m_v[i]; }

        T*       operator()()       { return m_v; }
        const T* operator()() const { return m_v; }

        KVector<T,N> square() const;

        KVector<T,N> operator*( const T& s ) const;
        KVector<T,N> operator/( const T& s ) const;
        KVector<T,N> operator-( const KVector<T,N>& rhs ) const;
        KVector<T,N> operator+( const KVector<T,N>& rhs ) const;

        void operator+=( const KVector<T,N>& rhs );
        void operator-=( const KVector<T,N>& rhs );

        KVector<T,N>& operator=( const KVector<T,N>& rhs );

        T&       x()       { assert_statement( 0<N, "dimension does not exist" ); return m_v[0]; }
        const T& x() const { assert_statement( 0<N, "dimension does not exist" ); return m_v[0]; }
        T&       y()       { assert_statement( 1<N, "dimension does not exist" ); return m_v[1]; }
        const T& y() const { assert_statement( 1<N, "dimension does not exist" ); return m_v[1]; }
        T&       z()       { assert_statement( 2<N, "dimension does not exist" ); return m_v[2]; }
        const T& z() const { assert_statement( 2<N, "dimension does not exist" ); return m_v[2]; }
        T&       w()       { assert_statement( 3<N, "dimension does not exist" ); return m_v[3]; }
        const T& w() const { assert_statement( 3<N, "dimension does not exist" ); return m_v[3]; }

    private:
        T m_v[N];
    };

//
//
//
    template<typename T, int N>
    KVector<T,N>::KVector( const T* vals ) {
        std::copy( vals, vals+N, m_v );
    }

    template<typename T, int N>
    KVector<T,N>::KVector( const T& val  ) {
        std::fill( m_v, m_v+N, val );
    }

    template<typename T, int N>
    KVector<T,N>::KVector( const T& v0, const T& v1 ) {
        m_v[0] = v0;
        m_v[1] = v1;
    }

    template<typename T, int N>
    KVector<T,N>::KVector( const T& v0, const T& v1, const T& v2 ) {
        m_v[0] = v0;
        m_v[1] = v1;
        m_v[2] = v2;
    }

    template<typename T, int N>
    KVector<T,N>::KVector( const T& v0, const T& v1, const T& v2, const T& v3 ) {
        m_v[0] = v0;
        m_v[1] = v1;
        m_v[2] = v2;
        m_v[3] = v3;
    }

    template<typename T, int N>
    KVector<T,N>::KVector( const T* a, int asz ) {
        assert_statement( N == asz, "invalid dimension" );
        for( int i=0; i<N; i++ )
            m_v[i] = a[i];
    }

    template<typename T, int N>
    void KVector<T,N>::set( const T& v0, const T& v1 ) {
        m_v[0] = v0;
        m_v[1] = v1;
    }
    template<typename T, int N>
    void KVector<T,N>::set( const T& v0, const T& v1, const T& v2 ) {
        m_v[0] = v0;
        m_v[1] = v1;
        m_v[2] = v2;
    }
    template<typename T, int N>
    void KVector<T,N>::set( const T& v0, const T& v1, const T& v2, const T& v3 ) {
        m_v[0] = v0;
        m_v[1] = v1;
        m_v[2] = v2;
        m_v[3] = v3;
    }

    template<typename T, int N>
    KVector<T,N>::KVector( const KVector<T,N>& rhs ) {
        std::copy( rhs(), rhs()+N, m_v );
    }

    template<typename T, int N>
    void KVector<T,N>::set( const T* vals ) {
        std::copy( vals, vals+N, m_v );
    }


    template<typename T, int N>
    T KVector<T,N>::norm() const {
        T nrm_sq = T(0.0);
        for( int i=0; i<N; i++ )
            nrm_sq += m_v[i]*m_v[i];
        return std::sqrt(nrm_sq);
    }

    template<typename T, int N>
    T KVector<T,N>::square_norm() const {
        T nrm_sq = T(0.0);
        for( int i=0; i<N; i++ )
            nrm_sq += m_v[i]*m_v[i];
        return nrm_sq;
    }

    template<typename T, int N>
    T KVector<T,N>::normalize() {
        T nrm = norm();
        T inrm = T(1)/nrm;
        for( int i=0; i<N; i++ ) {
            m_v[i] *= inrm;
            assert_number( m_v[i] );
        }
        return nrm;
    }

    template<typename T, int N>
    void KVector<T,N>::zero() {
        std::fill( m_v, m_v+N, T(0) );
    }


//
//
//
    template<typename T, int N>
    KVector<T,N>& KVector<T,N>::operator=( const KVector<T,N>& rhs ) {
        std::memcpy( m_v, rhs.m_v, sizeof(*m_v)*N );
        return *this;
    }

    template<typename T, int N>
    KVector<T,N> KVector<T,N>::operator/( const T& s ) const {
        KVector<T,N> rval;
        for( int i=0; i<N; i++ )
            rval.m_v[i] = m_v[i]/s;
        return rval;
    }

    template<typename T, int N>
    KVector<T,N> KVector<T,N>::operator*( const T& s ) const {
        KVector<T,N> rval;
        for( int i=0; i<N; i++ )
            rval.m_v[i] = m_v[i]*s;
        return rval;
    }

    template <typename T, int N>
    KVector<T,N>  KVector<T,N>::operator-( const KVector<T,N>& rhs ) const {
        KVector<T,N> rval;
        for( int i=0; i<N; i++ )
            rval.m_v[i] = m_v[i] - rhs.m_v[i];
        return rval;
    }

    template <typename T, int N>
    KVector<T,N>  KVector<T,N>::square() const {
        KVector<T,N> rval;
        for( int i=0; i<N; i++ )
            rval.m_v[i] = m_v[i]*m_v[i];
        return rval;
    }


    template <typename T, int N>
    KVector<T,N>  KVector<T,N>::operator+( const KVector<T,N>& rhs ) const {
        KVector<T,N> rval;
        for( int i=0; i<N; i++ )
            rval.m_v[i] = m_v[i] + rhs.m_v[i];
        return rval;
    }

    template <typename T, int N>
    void KVector<T,N>::operator+=( const KVector<T,N>& rhs ) {
        for( int i=0; i<N; i++ )
            m_v[i] += rhs.m_v[i];
    }

    template <typename T, int N>
    void KVector<T,N>::operator-=( const KVector<T,N>& rhs ) {
        for( int i=0; i<N; i++ )
            m_v[i] -= rhs.m_v[i];
    }

//
//
//

    template <typename T>
    KVector<T,3> cross_product( KVector<T,3> const& v1, KVector<T,3> const& v2 ) {
        return KVector<T,3>(v1[1] * v2[2] - v1[2] * v2[1],
                           v1[2] * v2[0] - v1[0] * v2[2],
                           v1[0] * v2[1] - v1[1] * v2[0]);
    }

//
//
//

    template <typename T, int N>
    KVector<T,N> KVector<T,N>::cross( KVector<T,N> const& rhs ) const {
        assert_statement( dim() == 3, "only defined for 3-vectors" );
        return cross_product(*this, rhs);
    }

    template <typename T, int N>
    T KVector<T,N>::dot( KVector<T,N> const& rhs ) const {
        T dp = T(0);
        for( int i=0; i<N; i++ )
            dp += m_v[i] * rhs.m_v[i];
        return dp;
    }

//
//
//

    // Serializing a vector to an output stream.
    template <typename T, int N>
    inline std::ostream& operator<<( std::ostream& os, const KVector<T,N>& v ) {
        for( int i=0; i < N-1; i++ )
            os << v[i] << " ";
        os << v[N-1];
        return os;
    }

    template <typename T, int N>
    void compute_mean( const vector< KVector<T,N> >& pnts, KVector<T,N>& mean_val ) {
        mean_val.zero();
        int psz = (int)pnts.size();
        assert_statement( psz != 0, "invalid op" );
        for( int i=0; i<psz; i++ )
            mean_val += pnts[i];
        mean_val.scale( 1.0/psz );
    }

}

#endif
