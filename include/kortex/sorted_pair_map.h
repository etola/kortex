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
// stores a map for coupled integers. this could be used to store adjacency
// information or an edge-map. integer-couple is stored in a directed manner
// from the lower integer to the higher one i.e. (f0,f1) and (f1,f0) cannot
// co-exist; it is stored as ( min(f0,f1), max(f0,f1) ). it does not allow the
// pair to have equal ints.
//
// ---------------------------------------------------------------------------
//
// this should use unordered_map for storing elements but I dont want to write
// the hash function...
//
#ifndef KORTEX_PAIR_MAP_H
#define KORTEX_PAIR_MAP_H

#include <map>
#include <set>
#include <vector>

using std::map;
using std::vector;

namespace kortex {

    class SortedPair {
    public:
        SortedPair() {
            m_f0 = 0;
            m_f1 = 0;
        }
        SortedPair( int f0, int f1 ) {
            if( f0<f1 ) { m_f0=f0; m_f1=f1; }
            else        { m_f0=f1; m_f1=f0; }
        }
        void set( int f0, int f1 ) {
            m_f0 = std::min(f0,f1);
            m_f1 = std::max(f0,f1);
        }
        bool operator<( const SortedPair& rhs ) const {
            if( m_f0 != rhs.m_f0 ) { return (m_f0<rhs.m_f0); }
            else                   { return (m_f1<rhs.m_f1); }
        }
        int f0() const { return m_f0; }
        int f1() const { return m_f1; }
    private:
        int m_f0, m_f1;
    };

    class SortedPairMap {
    public:
        SortedPairMap() {}
        void clear() { m_map.clear(); }

        void insert( int f0, int f1 );
        void remove( int f0, int f1 );
        bool find  ( int f0, int f1 ) const;

        int  size() const { return m_map.size(); }

        // exports the pairs present in the map
        void export_pairs( std::vector<SortedPair> &fpairs ) const;

        // exports neighbourhood information storing each faces' neighbour as a list
        void export_neighbourhood( int n_faces, vector< vector<int> >& face_neighbourhood ) const;

        /// computes a list of all the ids that have a pair that includes fid
        void export_as_list( std::map< int,  std::set< int > >& sample_links ) const;

    private:
        map<SortedPair, bool> m_map;
    };




}

#endif
