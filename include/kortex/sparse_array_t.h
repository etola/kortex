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
//
//  Sparse Array supporting A[ key ] = val type operations. If you want to have
//  an array as a data value, check IndexedArray.
//
#ifndef KORTEX_SPARSE_ARRAY_F_H
#define KORTEX_SPARSE_ARRAY_F_H

#include <unordered_map>
using std::unordered_map;

#include <kortex/keyed_value.h>

namespace kortex {

    template< typename KeyType, typename DataType >
    class SparseArrayT {
    private:
        unordered_map<KeyType, DataType> m_data;

    public:
        void clear();

        bool check( const KeyType& key ) const; // returns true if key exists

        void add( const KeyType& key, const DataType& val );
        void set( const KeyType& key, const DataType& val );

        void inc( const KeyType& key ); // increment
        void dec( const KeyType& key ); // decrement

        void zero( const KeyType& key );

        DataType val( const KeyType& key ) const; // read value - asserts that the key with id exists

        int  size() const { return (int)m_data.size(); }

        /// returns the item with the minimum value. returns false if no element
        /// exists
        bool min_item( KeyType& key_id, DataType& val ) const;

        /// returns the item with the minimum value. returns false if no element
        /// exists
        bool max_item( KeyType& key_id, DataType& val ) const;

        void extract_keys( vector<KeyType>& keys ) const;
        void extract( vector<KeyType>& keys, vector<DataType>& vals ) const;

        void extract( vector< KeyedValue<KeyType,DataType> >& items ) const;

        // /// returns the minimum and maximum id. use for finding range of the
        // /// keys.
        // bool min_max_keys( KeyType& min_key_id, KeyType& max_key_id ) const;

        // 0: printf
        // 1: logman_log
        // 2: logman_info
        void report( int mode=0 ) const;
    };

    typedef SparseArrayT<int  ,  int> SparseArrayII;
    typedef SparseArrayT<float,  int> SparseArrayFI;

}

#endif
