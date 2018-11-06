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

#include <map>
using std::map;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <utility>
using std::pair;

namespace kortex {

    // // std::hash does not have an implementation to hash pair<T1,T2> - below is
    // // a simple workaround
    typedef std::pair<int,int> ipair;
    // struct ipair_hash {
        // std::size_t operator ()( const ipair &p ) const {
            // auto h1 = std::hash<int>{}(p.first);
            // auto h2 = std::hash<int>{}(p.second);
            // // works but not a very good way of combining hashes
            // // return h1 ^ h2;
            // return 6501*h1+h2;
        // }
    // };

    template< typename IndexType, typename T >
    struct PairValue {
        IndexType id0;
        IndexType id1;
        T   val;
        void init( const IndexType& id0_, const IndexType& id1_, const T& val_ ) {
            id0 = id0_;
            id1 = id1_;
            val = val_;
        }
    };
    template< typename IndexType, typename T >
    inline bool pair_value_cmp_l( const PairValue<IndexType,T>& l,
                                  const PairValue<IndexType,T>& r ) {
        return l.val > r.val;
    }
    template< typename IndexType, typename T >
    inline bool pair_value_cmp_s( const PairValue<IndexType,T>& l,
                                  const PairValue<IndexType,T>& r ) {
        return l.val < r.val;
    }
    template< typename IndexType, typename T >
    void sort_ascending( vector< PairValue<IndexType, T> >& arr ) {
        sort( arr.begin(), arr.end(), pair_value_cmp_s<IndexType,T> );
    }
    template< typename IndexType, typename T >
    void sort_descending( vector< PairValue<IndexType, T> >& arr ) {
        sort( arr.begin(), arr.end(), pair_value_cmp_l<IndexType,T> );
    }


    template< typename IndexType, typename T >
    class PairIndexedArray {
    public:

        typedef std::pair<IndexType,IndexType> itpair;

        bool is_present( const IndexType& x, const IndexType& y ) const;

        void clear() { m_array.clear(); }

        void remove( const IndexType& x, const IndexType& y );

        void     set( const IndexType& x, const IndexType& y, const T& val );
        const T& add( const IndexType& x, const IndexType& y, const T& val );
        T        get( const IndexType& x, const IndexType& y ) const;

        // void report( int mode = 0 ) const;

        void export_pairs( vector< PairValue<IndexType,T> >& pairs ) const;

        // void reserve( const int& n_samples );

        int  size() const { return (int)m_array.size(); }

        int filter_array( const T& th );

        typename std::map<itpair,T>::iterator begin() { return m_array.begin(); }
        typename std::map<itpair,T>::iterator end  () { return m_array.end();   }
        typename std::map<itpair,T>::const_iterator begin() const { return m_array.begin(); }
        typename std::map<itpair,T>::const_iterator end  () const { return m_array.end();   }

        void load( const string& file );
        void save( const string& file ) const;

    private:
        // unordered_map< ipair, T, ipair_hash > m_array;

        std::map< itpair, T > m_array;
    };

}

#endif
