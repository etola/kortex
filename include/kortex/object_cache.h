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
#ifndef KORTEX_OBJECT_CACHE_H
#define KORTEX_OBJECT_CACHE_H

#include <vector>
using std::vector;

namespace kortex {

    template<typename T>
    struct CacheObject {
        int  file_index;
        T    obj;
    };

    template<typename T>
    class ObjectCache {
    public:

        ObjectCache( const vector<string>& file_paths, int n_max_object_number ) {
            m_max_object_number = n_max_object_number;
            m_objects.resize( m_max_object_number );
            m_file_paths = file_paths;
            clear_cache();
        }

        int n_files() const { return m_file_paths.size(); }

        /// loads specified objects into cache. indices are regarding the order
        /// of the m_file_paths.
        void load_objects( int p, int q=-1, int r=-1, int s=-1, int t=-1, int u=-1 );

        /// load objects marked with true.
        void load_objects( const vector<int>& to_be_loaded );

        /// returns true if file with file_id is present in cache
        bool is_in_cache( int file_id ) const;

        /// marks all cache slots as free. does not release memory
        void clear_cache();

        /// marks all cache slots as free and removes object memories
        void release_cache_memory();

        /// returns the number of the empty cache spots
        int n_empty_cache_slots() const;

        /// returns a pointer to the cached object
        const T* get_object( int fidx ) const;
        T      * get_object( int fidx ) ;

        void report_cache_state() const;

    private:
        int                      m_max_object_number;
        vector< CacheObject<T> > m_objects;
        vector<string          > m_file_paths;

        /// preps cache for new file load - removes cache items if not enough
        /// empty slots are present
        void         prep_cache_for_new_files( const vector<int>& to_be_loaded );

        /// returns a pointer to an empty cache spot - returns null if none
        /// exists
        CacheObject<T>* get_empty_object();

        /// returns the cache index of the file with file_index. returns -1 if
        /// non-existent
        int          get_cache_index( int file_index ) const;

        /// loads object to the cache with the file index identifier
        void         load_object( int fidx );

    };



}

#endif
