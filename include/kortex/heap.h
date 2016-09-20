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
//
// A heap implementation that can be used as a priority queue. supports
// ascending and descending sort. you need to define a HData object and use it
// to initialize the HNode's you'll insert to the heap. Heap only allocates
// memory to store the pointers to the HNode objects.
//
#ifndef KORTEX_HEAP_H
#define KORTEX_HEAP_H

#include <kortex/mem_unit.h>

namespace kortex {

    template<typename HData>
    class HNode {
    public:
        HNode() {
            heap_idx = 0;
            heap_val = 0;
            orig_idx = 0;
            data     = NULL;
        }

        size_t heap_idx;
        double heap_val;

        size_t orig_idx;

        HData* data;
    };

    template<typename HData>
    class Heap {
    public:
        enum HeapType { HT_ASCENDING, HT_DESCENDING };

    private:
        size_t m_cap; // maximum number of nodes heap can use
        size_t m_sz;  // current number of nodes

        HeapType m_type;
        MemUnit  m_memory;

        HNode<HData>**  m_nodes;
        HNode<HData>    m_sentinel;

    public:

        Heap();
        ~Heap();

        void init( const size_t& init_cap, const HeapType& type );

        void insert( HNode<HData>* node );

        HNode<HData>* peek();
        HNode<HData>* peek( size_t k );

        HNode<HData>* pop();
        HNode<HData>* pop( size_t k );
        HNode<HData>* pop( HNode<HData>* node );
        HNode<HData>* pop_last();

        void update( size_t k );
        void update( HNode<HData>* node );

        HNode<HData>* end();

        void clear() { m_sz = 0; }

        size_t capacity() const { return m_cap;         }
        size_t size    () const { return m_sz;          }
        bool   is_empty() const { return m_sz == 0;     }
        bool   is_full () const { return m_sz == m_cap; }

        static size_t req_mem( const size_t& cap ) {
            return ( cap+1 ) * sizeof( HNode<HData>* );
        }

        void set_mem( MemUnit& mem ) {
            m_memory.deallocate();
            m_memory.swap( &mem );
        }

        void borrow_mem( MemUnit& mem ) {
            m_memory.deallocate();
            m_memory.borrow( &mem );
        }

        void release();
        bool is_heap_healthy() const;

    private:

        void   upheap( size_t k );
        void downheap( size_t k );

        void reserve( const size_t& new_cap );

        bool check_heap_condition( size_t k ) const;

        static inline bool heap_min( HNode<HData>* &p1, HNode<HData>* &p2 ) {
            assert_pointer(p1);
            assert_pointer(p2);
            return p1->heap_val < p2->heap_val;
        }
        static inline bool heap_max( HNode<HData>* &p1, HNode<HData>* &p2 ) {
            assert_pointer(p1);
            assert_pointer(p2);
            return p1->heap_val > p2->heap_val;
        }

        bool (*cmp)( HNode<HData>* &p1, HNode<HData>* &p2 );
    };

}

#endif
