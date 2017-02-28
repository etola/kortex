// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2017 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
// web   : http://www.aurvis.com
//
// ---------------------------------------------------------------------------

#include <kortex/check.h>
#include <kortex/sorted_pair_map.h>

namespace kortex {

    void SortedPairMap::insert( int f0, int f1 ) {
        assert_statement( f0 != f1, "invalid operation" );
        if( this->find(f0,f1) ) return;
        m_map[ SortedPair(f0,f1) ] = true;
    }

    void SortedPairMap::remove( int f0, int f1 ) {
        m_map.erase( SortedPair(f0,f1) );
    }

    bool SortedPairMap::find( int f0, int f1 ) const {
        return bool( m_map.find( SortedPair(f0,f1) ) != m_map.end() );
    }

    // exports the pairs present in the map
    void SortedPairMap::export_pairs( std::vector<SortedPair> &fpairs ) const {
        for( auto it=m_map.begin() ; it != m_map.end(); ++it )
            fpairs.push_back( (*it).first );
    }

    // exports neighbourhood information storing each faces' neighbour as a list
    void SortedPairMap::export_neighbourhood( int n_faces, vector< vector<int> >& face_neighbourhood ) const {
        face_neighbourhood.clear();
        face_neighbourhood.resize( n_faces );
        for( auto it=m_map.begin() ; it != m_map.end(); ++it ) {
            const SortedPair& fp = (*it).first;
            assert_boundary( fp.f0(), 0, n_faces );
            assert_boundary( fp.f1(), 0, n_faces );
            face_neighbourhood[fp.f0()].push_back(fp.f1());
            face_neighbourhood[fp.f1()].push_back(fp.f0());
        }
    }


    /// computes a list of all the ids that have a pair that includes fid
    void SortedPairMap::export_as_list( std::map< int,  std::set< int > >& sample_links ) const {
        sample_links.clear();
        for( auto it=m_map.begin(); it != m_map.end(); ++it ) {
            const SortedPair& fp = (*it).first;
            sample_links[ fp.f0() ].insert( fp.f1() );
            sample_links[ fp.f1() ].insert( fp.f0() );
        }
    }


}
