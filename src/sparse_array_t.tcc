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

#include <typeinfo>
#include <kortex/check.h>
#include <kortex/string.h>
#include <kortex/sparse_array_t.h>

namespace kortex {

    template< typename KeyType, typename DataType >
    void SparseArrayT<KeyType,DataType>::clear() {
        m_data.clear();
    }

    template< typename KeyType, typename DataType >
    bool SparseArrayT<KeyType,DataType>::check( const KeyType& key ) const {
        if( m_data.find(key) == m_data.end() )
            return false;
        return true;
    }

    template< typename KeyType, typename DataType >
    void SparseArrayT<KeyType,DataType>::set( const KeyType& key, const DataType& val ) {
        m_data[key] = val;
    }

    template< typename KeyType, typename DataType >
    void SparseArrayT<KeyType,DataType>::add( const KeyType& key, const DataType& val ) {
        if( check(key) )
            m_data[key] += val;
        else
            this->set( key, val );
    }

    template< typename KeyType, typename DataType >
    void SparseArrayT<KeyType,DataType>::inc( const KeyType& key ) {
        if( !check(key) )
            this->set( key, DataType(1) );
        else
            m_data[key] = m_data[key] + DataType(1);
    }

    template< typename KeyType, typename DataType >
    void SparseArrayT<KeyType,DataType>::dec( const KeyType& key ) {
        if( !check(key) )
            this->set( key, DataType(-1) );
        else
            m_data[key] = m_data[key] - DataType(1);
    }

    template< typename KeyType, typename DataType >
    void SparseArrayT<KeyType,DataType>::zero( const KeyType& key ) {
        m_data[key] = DataType(0);
    }

    template< typename KeyType, typename DataType >
    DataType SparseArrayT<KeyType,DataType>::val( const KeyType& key ) const {
        passert_statement_g( check(key), "requesting val for non-existing key[%s]", in_str(key).c_str() );
        return m_data.at(key);
    }

    // template< typename KeyType, typename DataType >
    // bool SparseArrayT<KeyType,DataType>::min_max_keys( KeyType& min_key_id, KeyType& max_key_id ) const {
        // if( size() == 0 ) return false;
        // auto it = m_data.begin();
        // min_key_id = it->first;
        // max_key_id = it->first;
        // ++it;
        // for( ; it!=m_data.end(); ++it ) {
            // min_key_id = std::min( min_key_id, it->first );
            // max_key_id = std::max( max_key_id, it->first );
        // }
        // return true;
    // }

    template< typename KeyType, typename DataType >
    bool SparseArrayT<KeyType,DataType>::min_item( KeyType& key_id, DataType& val ) const {
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

    template< typename KeyType, typename DataType >
    bool SparseArrayT<KeyType,DataType>::max_item( KeyType& key_id, DataType& val ) const {
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

    template< typename KeyType, typename DataType >
    void SparseArrayT<KeyType,DataType>::extract( vector<KeyType>& keys, vector<DataType>& vals ) const {
        keys.clear();
        vals.clear();
        keys.reserve( size() );
        vals.reserve( size() );
        for( auto it=m_data.begin(); it!=m_data.end(); ++it ) {
            keys.push_back( it->first  );
            vals.push_back( it->second );
        }
    }

    template< typename KeyType, typename DataType >
    void SparseArrayT<KeyType,DataType>::extract( vector< KeyedValue<KeyType,DataType> >& items ) const {
        items.clear();
        items.reserve( size() );
        for( auto it=m_data.begin(); it!=m_data.end(); ++it ) {
            items.push_back( KeyedValue<KeyType,DataType>( it->first, it->second) );
        }

    }


    template< typename KeyType, typename DataType >
    void SparseArrayT<KeyType,DataType>::extract_keys( vector<KeyType>& keys ) const {
        keys.clear();
        keys.reserve( size() );
        for( auto it=m_data.begin(); it!=m_data.end(); ++it ) {
            keys.push_back( it->first  );
        }
    }

    template<typename DataType>
    string interp_val( const DataType& v ) {
        return in_str(v);
    }
    template<>
    string interp_val( const float& val ) {
        return num2str(val,4);
    }
    template<>
    string interp_val( const int& val ) {
        return num2str(val);
    }
    template<>
    string interp_val( const string& val ) {
        return val;
    }

    template< typename KeyType, typename DataType >
    void SparseArrayT<KeyType,DataType>::report( int mode ) const {
        // 0: printf
        // 1: logman_log
        // 2: logman_info

        int key_width = 0;
        for( auto it=m_data.begin(); it!=m_data.end(); ++it ) {
            key_width = std::max( key_width, (int)in_str(it->first).size() );
        }
        key_width++;

        for( auto it=m_data.begin(); it!=m_data.end(); ++it ) {
            switch( mode ) {
            case 0:
                printf( "%-*s : %s\n", key_width, in_str(it->first).c_str(), interp_val(it->second).c_str() );
                break;
            case 1:
                logman_log_g( "%-*s : %s", key_width, in_str(it->first).c_str(), interp_val(it->second).c_str() );
                break;
            case 2:
                logman_info_g( "%-*s : %s", key_width, in_str(it->first).c_str(), interp_val(it->second).c_str() );
                break;
            default:
                switch_fatality();
            }
        }
    }

}
