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
//
// [ int key, vector<T> ] map manager.
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_INDEXED_ARRAY_H
#define KORTEX_INDEXED_ARRAY_H

#include <map>
#include <vector>

using std::map;
using std::vector;

namespace kortex {

    template <typename T>
    class IndexedArray {
    public:
        IndexedArray() {}

        void clear();
        int  size () const;

        bool get   ( int p, vector<T>& val ) const;
        void insert( int p, const T& val );
        void remove( int p );

        void get_keys( vector<T>& keys ) const;
    private:
        map<int, vector<T> > m_map;
    };

    typedef IndexedArray<int>   IndexedArrayI;
    typedef IndexedArray<float> IndexedArrayF;


}

#endif
