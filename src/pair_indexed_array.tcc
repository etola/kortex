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
#include <kortex/fileio.h>
#include <kortex/types.h>

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

    // template< typename IndexType, typename T >
    // void PairIndexedArray<IndexType,T>::report( int mode ) const {
        // int prec = 6;
        // for( auto it=m_array.begin(); it !=m_array.end(); ++it ) {
            // switch( mode ) {
            // case 0:
                // printf( "%-5d %-5d : %s\n", it->first.first, it->first.second, interp_val( it->second, prec ).c_str() );
                // break;
            // case 1:
                // logman_log_g( "%-5d %-5d : %s", it->first.first, it->first.second, interp_val( it->second, prec ).c_str() );
                // break;
            // case 2:
                // logman_info_g( "%-5d %-5d : %s", it->first.first, it->first.second, interp_val( it->second, prec ).c_str() );
                // break;
            // default:
                // switch_fatality();
                // break;
            // }
        // }
    // }

    template< typename IndexType, typename T >
    bool PairIndexedArray<IndexType,T>::is_present( const IndexType& x, const IndexType& y ) const {
        if( m_array.find( itpair(x,y) ) == m_array.end() )
            return false;
        return true;
    }

    template< typename IndexType, typename T >
    void PairIndexedArray<IndexType,T>::remove( const IndexType& x, const IndexType& y ) {
        m_array.erase( itpair(x,y) );
    }

    template< typename IndexType, typename T >
    void PairIndexedArray<IndexType,T>::set( const IndexType& x, const IndexType& y, const T& val ) {
        m_array[ itpair(x,y) ] = val;
    }

    template< typename IndexType, typename T >
    const T& PairIndexedArray<IndexType,T>::add( const IndexType& x, const IndexType& y, const T& val ) {
        itpair p = itpair(x,y);
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

        return it->second;
    }

    template< typename IndexType, typename T >
    T PairIndexedArray<IndexType,T>::get( const IndexType& x, const IndexType& y ) const {
        assert_statement_g( is_present(x,y), "key does not exist [x,y] = [%d,%d]", x, y );
        return m_array.at( itpair(x,y) );
    }

    template< typename IndexType, typename T >
    void PairIndexedArray<IndexType,T>::export_pairs( vector< PairValue<IndexType,T> >& pairs ) const {
        pairs.clear();
        int cnt = 0;
        pairs.resize( m_array.size() );
        for( auto it=m_array.begin(); it !=m_array.end(); ++it, ++cnt ) {
            pairs[cnt].init( it->first.first, it->first.second, it->second );
        }
    }

    template< typename IndexType, typename T >
    int PairIndexedArray<IndexType,T>::filter_array( const T& th ) {
        int cnt = 0;
        for( auto it=m_array.begin(); it!=m_array.end(); ++it ) {
            if( it->second >= th )
                continue;
            m_array.erase( it );
            cnt++;
        }
        return cnt;
    }

    template< typename IndexType, typename T >
    void PairIndexedArray<IndexType,T>::save( const string& file ) const {
        int sz = this->size();
        ofstream fout;
        open_or_fail( file, fout, true );
        T t;
        write_bparam( fout, get_type(t) );
        write_bparam( fout, sz );
        for( auto it=m_array.begin(); it != m_array.end(); ++it ) {
            write_bparam( fout, it->first.first );
            write_bparam( fout, it->first.second );
            write_bparam( fout, it->second );
        }
        fout.close();
    }

    template< typename IndexType, typename T >
    void PairIndexedArray<IndexType,T>::load( const string& file ) {
        ifstream fin;
        open_or_fail( file, fin, true );

        this->clear();

        int ft = 0;
        read_bparam( fin, ft );
        T t;
        passert_statement( ft == get_type(t),
                           "file data type and passed array type is different" );
        int sz=0;
        read_bparam( fin, sz );

        int x, y;
        T   val;
        for( int i=0; i<sz; i++ ) {
            read_bparam( fin, x );
            read_bparam( fin, y );
            read_bparam( fin, val );
            this->set( x, y, val );
        }

        fin.close();
    }


}
