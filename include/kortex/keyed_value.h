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
#ifndef KORTEX_KEYED_VALUE_H
#define KORTEX_KEYED_VALUE_H

#include <algorithm>
#include <vector>

#include <kortex/string.h>

using std::vector;
using std::sort;

namespace kortex {

    template<typename KeyType, typename DataType >
    struct KeyedValue {
        KeyType  key;
        DataType val;
        KeyedValue( const KeyType& key_, const DataType& val_ ) : key(key_), val(val_) {}
        KeyedValue() {
            in_value( in_str(0).c_str(), key );
            val = DataType(0);
        }
        void print() const {
            printf( "[key %s] [val %s]\n",
                    in_str(key).c_str(),
                    in_str(val).c_str() );
        }
    };

    typedef KeyedValue<int,float> ifloat;
    typedef KeyedValue<int,int  > iint;
    typedef KeyedValue<string,int> sint;

    template<typename KeyType, typename DataType >
    void print( const vector< KeyedValue<KeyType,DataType> >& arr ) {
        for( unsigned i=0; i<arr.size(); i++ )
            arr[i].print();
    }

    template< typename KeyType, typename DataType >
    void export_keys( const vector< KeyedValue<KeyType,DataType> >& arr,
                      vector< KeyType >& keys ) {
        int asz = arr.size();
        keys.resize( asz );
        for( int i=0; i<asz; i++ ) {
            keys[i] = arr[i].key;
        }
    }

    template< typename KeyType, typename DataType >
    void export_values( const vector< KeyedValue<KeyType,DataType> >& arr,
                        vector< KeyType >& values ) {
        int asz = arr.size();
        values.resize( asz );
        for( int i=0; i<asz; i++ ) {
            values[i] = arr[i].val;
        }
    }

    template< typename KeyType, typename DataType >
    void initialize( const vector<KeyType>& keys,
                     const vector<DataType>& values,
                     vector< KeyedValue<KeyType,DataType> >& arr ) {
        assert_statement( keys.size() == values.size(),
                          "incompatible array sizes" );
        int asz = keys.size();
        arr.resize( asz );
        for( int i=0; i<asz; i++ ) {
            arr[i].key = keys[i];
            arr[i].val = values[i];
        }
    }

    template<typename DataType >
    void initialize( const vector<DataType>& values,
                     vector< KeyedValue<int,DataType> >& arr ) {
        int asz = values.size();
        arr.resize( asz );
        for( int i=0; i<asz; i++ ) {
            arr[i].key = i;
            arr[i].val = values[i];
        }
    }

    //
    // value sort
    //
    template< typename KeyType, typename DataType >
    inline bool keyed_type_cmp_l( const KeyedValue<KeyType,DataType>& l,
                                  const KeyedValue<KeyType,DataType>& r ) {
        return l.val > r.val;
    }
    template< typename KeyType, typename DataType >
    inline bool keyed_type_cmp_s( const KeyedValue<KeyType,DataType>& l,
                                  const KeyedValue<KeyType,DataType>& r ) {
        return l.val < r.val;
    }

    template< typename KeyType, typename DataType >
    void sort_ascending( vector< KeyedValue<KeyType,DataType> >& arr ) {
        sort( arr.begin(), arr.end(), keyed_type_cmp_s<KeyType,DataType> );
    }
    template< typename KeyType, typename DataType >
    void sort_descending( vector< KeyedValue<KeyType,DataType> >& arr ) {
        sort( arr.begin(), arr.end(), keyed_type_cmp_l<KeyType,DataType> );
    }

    //
    // key sort
    //
    template< typename KeyType, typename DataType >
    inline bool keyed_type_cmp_key_l( const KeyedValue<KeyType,DataType>& l,
                                      const KeyedValue<KeyType,DataType>& r ) {
        return l.key > r.key;
    }
    template< typename KeyType, typename DataType >
    inline bool keyed_type_cmp_key_s( const KeyedValue<KeyType,DataType>& l,
                                      const KeyedValue<KeyType,DataType>& r ) {
        return l.key < r.key;
    }

    template< typename KeyType, typename DataType >
    void sort_ascending_key( vector< KeyedValue<KeyType,DataType> >& arr ) {
        sort( arr.begin(), arr.end(), keyed_type_cmp_key_s<KeyType,DataType> );
    }
    template< typename KeyType, typename DataType >
    void sort_descending_key( vector< KeyedValue<KeyType,DataType> >& arr ) {
        sort( arr.begin(), arr.end(), keyed_type_cmp_key_l<KeyType,DataType> );
    }

    //
    // Double Key
    //
    template<typename KeyType, typename DataType >
    struct DoubleKeyedValue {
        KeyType  x;
        KeyType  y;
        DataType val;
        DoubleKeyedValue( const KeyType& key0_, const KeyType& key1_, const DataType& val_ ) :
            x(key0_), y(key1_), val(val_) {}
        DoubleKeyedValue() {
            in_value( in_str(-1).c_str(), x );
            in_value( in_str(-1).c_str(), y );
            val = DataType(0);
        }
        void print() const {
            printf( "[key0 %s] [key1 %s] [val %s]\n",
                    in_str(x).c_str(),
                    in_str(y).c_str(),
                    in_str(val).c_str() );
        }
    };

    template< typename KeyType, typename DataType >
    inline bool dkeyed_type_cmp_l(const DoubleKeyedValue<KeyType,DataType>& l,
                                  const DoubleKeyedValue<KeyType,DataType>& r) {
        return l.val > r.val;
    }

    template< typename KeyType, typename DataType >
    inline bool dkeyed_type_cmp_s(const DoubleKeyedValue<KeyType,DataType>& l,
                                  const DoubleKeyedValue<KeyType,DataType>& r) {
        return l.val < r.val;
    }

    template< typename KeyType, typename DataType >
    void sort_ascending( vector< DoubleKeyedValue<KeyType,DataType> >& arr ) {
        sort( arr.begin(), arr.end(), dkeyed_type_cmp_s<KeyType,DataType> );
    }

    template< typename KeyType, typename DataType >
    void sort_descending( vector< DoubleKeyedValue<KeyType,DataType> >& arr ) {
        sort( arr.begin(), arr.end(), dkeyed_type_cmp_l<KeyType,DataType> );
    }


}

#endif
