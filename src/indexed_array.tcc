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
#ifndef KORTEX_INDEXED_ARRAY_TCC
#define KORTEX_INDEXED_ARRAY_TCC

#include <kortex/indexed_array.h>

namespace kortex {

    template <typename T>
    void IndexedArray<T>::clear() {
        m_map.clear();
    }

    template <typename T>
    void IndexedArray<T>::get_keys( vector<T>& keys ) const {
        keys.clear();
        keys.reserve( m_map.size() );
        for( auto it=m_map.begin(); it!=m_map.end(); it++ ) {
            keys.push_back( (*it).first );
        }
    }

    template <typename T>
    int  IndexedArray<T>::size() const {
        return m_map.size();
    }

    template <typename T>
    bool IndexedArray<T>::is_present( const int& p ) const {
        return bool( m_map.find(p) == m_map.end() );
    }

    template <typename T>
    bool IndexedArray<T>::get( int p, vector<T>& val ) const {
        auto it = m_map.find( p );
        if( it != m_map.end() ) {
            val = (*it).second;
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    void IndexedArray<T>::insert( int p, const T& val ) {
        m_map[p].push_back(val);
    }

    template <typename T>
    void IndexedArray<T>::remove( int p ) {
        m_map.erase( p );
    }


}

#endif
