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
#include <kortex/sparse_array.h>

namespace kortex {

    void SparseArray::clear() {
        m_data.clear();
    }

    bool SparseArray::check( const int& key ) const {
        if( m_data.find(key) == m_data.end() )
            return false;
        return true;
    }

    void SparseArray::set( const int& key, const int& val ) {
        m_data[key] = val;
    }

    void SparseArray::inc( const int& key ) {
        if( !check(key) )
            set( key, 1 );
        else
            m_data[key] = m_data[key] + 1;
    }

    void SparseArray::dec( const int& key ) {
        if( !check(key) )
            set( key, -1 );
        else
            m_data[key] = m_data[key] - 1;
    }

    void SparseArray::zero( const int& key ) {
        m_data[key] = 0;
    }

    int SparseArray::val( const int& key ) const {
        passert_statement_g( check(key), "requesting val for non-existing key[%d]", key );
        const int& v = m_data.at(key);
        return v;
    }

    bool SparseArray::min_max_keys( int& min_key_id, int& max_key_id ) const {

        if( size() == 0 ) return false;

        auto it = m_data.begin();
        min_key_id = it->first;
        max_key_id = it->first;
        ++it;

        for( ; it!=m_data.end(); ++it ) {
            min_key_id = std::min( min_key_id, it->first );
            max_key_id = std::max( max_key_id, it->first );
        }

        return true;
    }

    bool SparseArray::min_item( int& key_id, int& val ) const {
        if( size() == 0 ) return false;

        auto it = m_data.begin();
        key_id = it->first;
        val    = it->second;
        ++it;

        for( ; it!=m_data.end(); ++it ) {
            if( val > it->second ) {
                val    = it->second;
                key_id = it->first;
            }
        }
        return true;
    }

    bool SparseArray::max_item( int& key_id, int& val ) const {
        if( size() == 0 ) return false;

        auto it = m_data.begin();
        key_id = it->first;
        val    = it->second;
        ++it;

        for( ; it!=m_data.end(); ++it ) {
            if( val < it->second ) {
                val    = it->second;
                key_id = it->first;
            }
        }
        return true;
    }

    void SparseArray::print() const {
        for( auto it=m_data.begin(); it!=m_data.end(); ++it ) {
            printf( "% 5d : %d\n", it->first, it->second );
        }
    }

    void SparseArray::extract( vector<iint>& items ) const {
        items.clear();
        items.reserve( size() );
        for( auto it=m_data.begin(); it!=m_data.end(); ++it ) {
            items.push_back( iint(it->first,it->second) );
        }
    }


}
