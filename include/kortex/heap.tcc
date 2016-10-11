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

#ifndef KORTEX_HEAP_TCC
#define KORTEX_HEAP_TCC

#include <cfloat>
#include <kortex/check.h>
#include "heap.h"

namespace kortex {

    template<typename HData>
    Heap<HData>::Heap() {
        m_sz   = 0;
        m_cap  = 0;
        m_type = HT_ASCENDING;
    }
    template<typename HData>
    Heap<HData>::~Heap() {
        m_node_ptrs.clear();
    }

    template<typename HData>
    void Heap<HData>::init( const size_t& init_cap, const HeapType& type ) {
        m_sz   = 0;
        m_cap  = 0;
        m_type = type;

        reserve( init_cap+1 );

        switch( m_type ) {
        case HT_DESCENDING: m_sentinel.heap_val =  DBL_MAX; cmp = heap_max; break;
        case HT_ASCENDING : m_sentinel.heap_val = -DBL_MAX; cmp = heap_min; break;
        }
        m_node_ptrs[0] = &m_sentinel;
    }

    template<typename HData>
    void Heap<HData>::reserve( const size_t& new_cap ) {
        m_cap = new_cap;
        m_node_ptrs.resize( new_cap );
    }

    template<typename HData>
    void Heap<HData>::release() {
        m_node_ptrs.clear();
        m_cap   = 0;
        m_sz    = 0;
    }

    template<typename HData>
    void Heap<HData>::insert( HNode<HData>* node ) {
        assert_pointer( cmp );
        m_sz++;
        if( is_full() ) {
            printf( "resizing heap! %d\n", (int)m_cap*2 );
            reserve( 2*m_cap );
        }
        m_node_ptrs[m_sz] = node;
        m_node_ptrs[m_sz]->heap_idx = m_sz;
        upheap(m_sz);
    }

    template<typename HData>
    void Heap<HData>::upheap( size_t k ) {
        assert_pointer( cmp );
        HNode<HData>* v = m_node_ptrs[k];

        while( (*cmp)( v, m_node_ptrs[k>>1] ) ) {
            this->swap( m_node_ptrs[k], m_node_ptrs[k>>1] );
            k >>= 1;
        }
    }

    template<typename HData>
    void Heap<HData>::swap( HNode<HData>* &p1, HNode<HData>* &p2 ) {
        std::swap( p1, p2 );
        std::swap( p1->heap_idx, p2->heap_idx );
    }

    template<typename HData>
    void Heap<HData>::downheap( size_t k ) {
        assert_pointer( cmp );

        size_t j;
        HNode<HData>* v = m_node_ptrs[k];

        while( k <= m_sz/2 ) {
            j = k << 1;
            if( j < m_sz && (*cmp)( m_node_ptrs[j+1], m_node_ptrs[j] ) )
                j++;
            if( (*cmp)( v, m_node_ptrs[j] ) )
                break;

            this->swap( m_node_ptrs[k], m_node_ptrs[j] );
            // m_node_ptrs[k] = m_node_ptrs[j];
            // m_node_ptrs[k]->heap_idx = k;
            k = j;
        }
        // this->swap( m_node_ptrs[k], v );
        // m_node_ptrs[k] = v;
        // m_node_ptrs[k]->heap_idx = k;
    }

    template<typename HData>
    HNode<HData>* Heap<HData>::pop( size_t k ) {
        assert_statement( k <= m_sz && k > 0, "oob" );
        HNode<HData>* node_k = m_node_ptrs[k];
        assert_statement( node_k->heap_idx == k, "invalid node" );
        if( m_sz > 0 ) {
            m_node_ptrs[k] = m_node_ptrs[m_sz];
            m_node_ptrs[k]->heap_idx = k;
            m_sz--;
            update(k);
        }
        return node_k;
    }

    template<typename HData>
    HNode<HData>* Heap<HData>::pop( HNode<HData>* node ) {
        assert_pointer( node );
        return pop( node->heap_idx );
    }

    template<typename HData>
    HNode<HData>* Heap<HData>::pop() {
        if( m_sz == 0 ) return NULL;
        HNode<HData>* top = m_node_ptrs[1];
        if( m_sz > 0 ) {
            m_node_ptrs[1] = m_node_ptrs[m_sz];
            m_node_ptrs[1]->heap_idx = 1;
            m_sz--;
            downheap(1);
        }

        return top;
    }

    template<typename HData>
    HNode<HData>* Heap<HData>::pop_last() {
        if( m_sz == 0 ) return NULL;
        HNode<HData>* last = m_node_ptrs[m_sz];
        m_sz--;
        return last;
    }

    template<typename HData>
    HNode<HData>* Heap<HData>::peek( size_t k ) {
        if( m_sz == 0 ) return NULL;
        assert_statement( k>=1 && k<=m_sz, "invalid idx to peek" );
        return m_node_ptrs[k];
    }

    template<typename HData>
    HNode<HData>* Heap<HData>::peek() {
        if( m_sz == 0 ) return NULL;
        return m_node_ptrs[1];
    }

    template<typename HData>
    void Heap<HData>::update( size_t k ) {
        assert_statement( k>0 && k<=m_sz, "invalid idx to update" );
        if( check_heap_condition(k) )
            upheap(k);
        else
            downheap(k);
    }

    template<typename HData>
    void Heap<HData>::update( HNode<HData>* node ) {
        assert_pointer( node );
        update( node->heap_idx );
    }

    template<typename HData>
    bool Heap<HData>::check_heap_condition( size_t k ) {
        assert_pointer( cmp );

        HNode<HData>* v = m_node_ptrs[k];
        assert_statement_g( v->heap_idx == k, "invalid node! [k %d]", k );

        if( 2*k+2 <= m_sz ) {
            if( (*cmp)( v, m_node_ptrs[2*k+1] ) &&
                (*cmp)( v, m_node_ptrs[2*k+2] ) )
                return true;
            else
                return false;
        } else if( 2*k+1 <= m_sz ) {
            if( (*cmp)( v, m_node_ptrs[2*k+1] ) )
                return true;
            return false;
        }
        return true;
    }

    template<typename HData>
    HNode<HData>* Heap<HData>::end() {
        if( m_sz == 0 ) return NULL;
        return m_node_ptrs[0];
    }

    template<typename HData>
    bool Heap<HData>::is_heap_healthy() const {
        for( size_t i=0; i<m_sz; i++ ) {
            if( m_node_ptrs[i]->heap_idx != i ) {
                logman_error_g( "heap error: node [%d]", i );
                return false;
            }
        }
        return true;
    }


}

#endif


