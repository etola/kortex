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
#ifndef KORTEX_PAIR_INDEXED_ARRAY_H
#define KORTEX_PAIR_INDEXED_ARRAY_H

#include <unordered_map>
using std::unordered_map;

// #include <map>
// using std::map;

#include <vector>
using std::vector;

#include <utility>
using std::pair;

namespace kortex {

    // std::hash does not have an implementation to hash pair<T1,T2> - below is
    // a simple workaround
    typedef std::pair<int,int> ipair;
    struct ipair_hash {
        std::size_t operator ()( const ipair &p ) const {
            auto h1 = std::hash<int>{}(p.first);
            auto h2 = std::hash<int>{}(p.second);
            // works but not a very good way of combining hashes
            // return h1 ^ h2;
            return 6501*h1+h2;
        }
    };

    template< typename T >
    struct PairValue {
        int id0;
        int id1;
        T   val;
        void init( const int& id0_, const int& id1_, const T& val_ ) {
            id0 = id0_;
            id1 = id1_;
            val = val_;
        }
    };
    template< typename T >
    inline bool pair_value_cmp_l( const PairValue<T>& l,
                                  const PairValue<T>& r ) {
        return l.val > r.val;
    }
    template< typename T >
    inline bool pair_value_cmp_s( const PairValue<T>& l,
                                  const PairValue<T>& r ) {
        return l.val < r.val;
    }
    template< typename T >
    void sort_ascending( vector< PairValue<T> >& arr ) {
        sort( arr.begin(), arr.end(), pair_value_cmp_s<T> );
    }
    template< typename T >
    void sort_descending( vector< PairValue<T> >& arr ) {
        sort( arr.begin(), arr.end(), pair_value_cmp_l<T> );
    }


    template< typename T >
    class PairIndexedArray {
    public:

        bool is_present( const int& x, const int& y ) const;

        void remove( const int& x, const int& y );

        void set( const int& x, const int& y, const T& val );
        void add( const int& x, const int& y, const T& val );

        T    get( const int& x, const int& y ) const;

        void report( int mode = 0 ) const;

        void export_pairs( vector< PairValue<T> >& pairs );

        // void reserve( const int& n_samples );

    private:
        unordered_map< ipair, T, ipair_hash > m_array;

        // map< ipair, T > m_array;
    };

}

#endif
