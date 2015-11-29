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
#ifndef KORTEX_OBJECT_CACHE_TCC
#define KORTEX_OBJECT_CACHE_TCC

#include <kortex/check.h>
#include <kortex/object_cache.h>

namespace kortex {

    template<typename T>
    void ObjectCache<T>::set_cache_size( int n_max_object_number ) {
        assert_statement( n_max_object_number > 1, "too small cache" );
        m_max_object_number = n_max_object_number;
        m_objects.resize( n_max_object_number );
        clear_cache();
    }


    template<typename T>
    const T* ObjectCache<T>::get_object( int fidx ) const {
        int cidx = get_cache_index( fidx );
        if( cidx == -1 ) return NULL;
        assert_boundary( cidx, 0, (int)m_objects.size() );
        return &(m_objects[cidx].obj);
    }

    template<typename T>
    T* ObjectCache<T>::get_object( int fidx ) {
        int cidx = get_cache_index( fidx );
        if( cidx == -1 ) return NULL;
        assert_boundary( cidx, 0, (int)m_objects.size() );
        return &(m_objects[cidx].obj);
    }

    template<typename T>
    bool ObjectCache<T>::is_in_cache( int file_id ) const {
        assert_boundary( file_id, 0, n_files() );
        for( unsigned i=0; i<m_objects.size(); i++ ) {
            if( m_objects[i].file_index == file_id )
                return true;
        }
        return false;
    }

    template<typename T>
    void ObjectCache<T>::load_objects( int p, int q, int r, int s, int t, int u ) {
        assert_statement( n_files(), "not initialized properly" );
        assert_boundary( p, -1, n_files() );
        assert_boundary( q, -1, n_files() );
        assert_boundary( r, -1, n_files() );
        assert_boundary( s, -1, n_files() );
        assert_boundary( t, -1, n_files() );
        assert_boundary( u, -1, n_files() );

        vector<int> to_be_loaded;
        if( is_inside(p,0,n_files()) ) to_be_loaded.push_back(p);
        if( is_inside(q,0,n_files()) ) to_be_loaded.push_back(q);
        if( is_inside(r,0,n_files()) ) to_be_loaded.push_back(r);
        if( is_inside(s,0,n_files()) ) to_be_loaded.push_back(s);
        if( is_inside(t,0,n_files()) ) to_be_loaded.push_back(t);
        if( is_inside(u,0,n_files()) ) to_be_loaded.push_back(u);

        load_objects( to_be_loaded );
    }

    template<typename T>
    void ObjectCache<T>::load_objects( const vector<int>& to_be_loaded ) {
        assert_statement( n_files(), "not initialized properly" );
        prep_cache_for_new_files( to_be_loaded );
        for( unsigned i=0; i<to_be_loaded.size(); i++ ) {
            int fidx = to_be_loaded[i];
            load_object(fidx);
        }
    }

    template<typename T>
    CacheObject<T>* ObjectCache<T>::get_empty_object() {
        for( unsigned i=0; i<m_objects.size(); i++ ) {
            if( m_objects[i].file_index == -1 )
                return &m_objects[i];
        }
        return NULL;
    }

    template<typename T>
    int ObjectCache<T>::get_cache_index( int file_index ) const {
        for( unsigned i=0; i<m_objects.size(); i++ ) {
            if( m_objects[i].file_index == file_index )
                return i;
        }
        return -1;
    }

    template<typename T>
    void ObjectCache<T>::load_object( int fidx ) {
        assert_boundary( fidx, 0, n_files() );
        if( is_in_cache(fidx) )
            return;
        CacheObject<T>* p = get_empty_object();
        passert_statement( p, "no empty object slot is available - cache is full - init with larger cache size" );
        p->file_index = fidx;
        p->obj.load( m_file_paths[fidx] );
        if( post_load_func )
            post_load_func( p->obj );
    }

    template<typename T>
    void ObjectCache<T>::clear_cache() {
        for( unsigned i=0; i<m_objects.size(); i++ )
            m_objects[i].file_index = -1;
    }

    template<typename T>
    void ObjectCache<T>::release_cache_memory() {
        for( unsigned i=0; i<m_objects.size(); i++ )
            m_objects[i].obj.release();
        clear_cache();
    }

    template<typename T>
    void ObjectCache<T>::reset_cache() {
        release_cache_memory();
        m_file_paths.clear();
    }

    template<typename T>
    int ObjectCache<T>::n_empty_cache_slots() const {
        int n_slots = 0;
        for( unsigned i=0; i<m_objects.size(); i++ ) {
            if( m_objects[i].file_index == -1 )
                n_slots++;
        }
        return n_slots;
    }

    template<typename T>
    void ObjectCache<T>::prep_cache_for_new_files( const vector<int>& to_be_loaded ) {
        passert_statement( to_be_loaded.size() <= m_max_object_number, "insufficient cache size" )
        assert_statement( has_unique_elements(to_be_loaded), "array does not have unique elements" );

        int n_new_loads = 0;
        for( unsigned i=0; i<to_be_loaded.size(); i++ ) {
            if( !is_in_cache( to_be_loaded[i] ) )
                n_new_loads++;
        }

        int n_available = n_empty_cache_slots() - n_new_loads;

        if( n_available >= 0 )
            return;

        for( unsigned i=0; i<m_objects.size(); i++ ) {
            if( is_inside( to_be_loaded,  m_objects[i].file_index ) )
                continue;
            logman_info_g( "[cidx %d] removing file %d", i, m_objects[i].file_index );
            m_objects[i].file_index = -1;
            n_available++;
            if( n_available >= 0 )
                break;
        }

    }

    template<typename T>
    void ObjectCache<T>::report_cache_state() const {
        logman_log( "cache state - begins" );
        logman_log_g( "max_object_number: %d", m_max_object_number );
        // logman_log_gvs( "files", &(m_file_paths[0]), (int)m_file_paths.size() );

        logman_log( "cache items" );
        int n_empty = 0;
        for( unsigned i=0; i<m_objects.size(); i++ ) {
            if( m_objects[i].file_index == -1 ) {
                n_empty++;
            } else {
                logman_log_g( "cache slot [% 3d] [file %s]", i, m_file_paths[ m_objects[i].file_index ].c_str() );
            }
        }
        logman_log_g( "num empty slots: %d", n_empty );
        logman_log( "cache state - ends" );
    }

}

#endif
