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
#ifndef KORTEX_SPARSE_ARRAY_H
#define KORTEX_SPARSE_ARRAY_H

#include <kortex/indexed_types.h>

#include <unordered_map>
using std::unordered_map;

namespace kortex {

    class SparseArray {
    private:
        unordered_map<int, int> m_data;

    public:
        void clear();

        bool check( const int& key ) const; // returns true if key exists

        void set( const int& key, const int& val );
        void inc( const int& key ); // increment
        void dec( const int& key ); // decrement

        void zero( const int& key );

        int  val( const int& key ) const; // read value - asserts that the key with id exists

        int  size() const { return m_data.size(); }

        /// returns the minimum and maximum id. use for finding range of the
        /// keys.
        bool min_max_keys( int& min_key_id, int& max_key_id ) const;

        /// returns the item with the minimum value. returns false if no element
        /// exists
        bool min_item( int& key_id, int& val ) const;

        /// returns the item with the minimum value. returns false if no element
        /// exists
        bool max_item( int& key_id, int& val ) const;

        void print() const;

        void extract( vector<iint>& items ) const;
    };


}

#endif
