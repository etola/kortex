// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2016 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
// web   : http://www.aurvis.com
//
// ---------------------------------------------------------------------------

#include <kortex/check.h>
#include <kortex/string.h>

namespace kortex {

    string interp_val( const float& val, const int& prec ) {
        return num2str(val,prec);
    }
    string interp_val( const double& val, const int& prec ) {
        return num2str(val,prec);
    }
    string interp_val( const int& val, const int& prec ) {
        return num2str(val);
    }

    // template< typename T >
    // void PairIndexedArray<T>::reserve( const int& n_samples ) {
        // m_array.reserve( n_samples );
    // }

    template< typename T >
    void PairIndexedArray<T>::report( int mode ) const {

        int prec = 6;

        for( auto it=m_array.begin(); it !=m_array.end(); ++it ) {
            switch( mode ) {
            case 0:
                printf( "%-5d %-5d : %s\n", it->first.first, it->first.second, interp_val( it->second, prec ).c_str() );
                break;
            case 1:
                logman_log_g( "%-5d %-5d : %s", it->first.first, it->first.second, interp_val( it->second, prec ).c_str() );
                break;
            case 2:
                logman_info_g( "%-5d %-5d : %s", it->first.first, it->first.second, interp_val( it->second, prec ).c_str() );
                break;
            default:
                switch_fatality();
                break;
            }
        }
    }

    template< typename T >
    bool PairIndexedArray<T>::is_present( const int& x, const int& y ) const {
        if( m_array.find( ipair(x,y) ) == m_array.end() )
            return false;
        return true;
    }

    template< typename T >
    void PairIndexedArray<T>::remove( const int& x, const int& y ) {
        m_array.erase( ipair(x,y) );
    }

    template< typename T >
    void PairIndexedArray<T>::set( const int& x, const int& y, const T& val ) {
        m_array[ ipair(x,y) ] = val;
    }

    template< typename T >
    void PairIndexedArray<T>::add( const int& x, const int& y, const T& val ) {
        ipair p = ipair(x,y);
        auto it = m_array.find( p );
        if( it == m_array.end() ) {
            m_array[ p ] = val;
        } else {
            it->second += val;
        }
        // if( is_present( x,y ) )
            // m_array[ p ] += val;
        // else
            // m_array[ p ] = val;
    }

    template< typename T >
    T PairIndexedArray<T>::get( const int& x, const int& y ) const {
        assert_statement_g( is_present(x,y), "key does not exist [x,y] = [%d,%d]", x, y );
        return m_array.at( ipair(x,y) );
    }

    template< typename T >
    void PairIndexedArray<T>::export_pairs( vector< PairValue<T> >& pairs ) {
        pairs.clear();
        int cnt = 0;
        pairs.resize( m_array.size() );
        for( auto it=m_array.begin(); it !=m_array.end(); ++it, ++cnt ) {
            pairs[cnt].init( it->first.first, it->first.second, it->second );
        }
    }


}

