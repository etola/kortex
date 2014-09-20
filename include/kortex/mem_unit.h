// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_MEM_UNIT_H
#define KORTEX_MEM_UNIT_H

#include <kortex/types.h>

using std::ofstream;
using std::ifstream;

namespace kortex {

    class MemUnit {
    public:
        MemUnit();
        MemUnit( const size_t& init_cap );
        ~MemUnit();

        MemUnit( const MemUnit& m );
        MemUnit& operator=( const MemUnit& mem );

        void copy( const MemUnit& m );

        /// content can be destroyed - if you want to keep the content unchanged
        /// use expand
        void resize( const size_t& new_cap );

        /// expands the size of the capacity of the memunit. copies the old
        /// content if relocation occurs.
        void expand( const size_t& new_cap );

        void deallocate();

        size_t capacity () const { return m_cap; }
        size_t is_owner () const { return m_owner; }

        const uchar* get_buffer() const { return m_buffer; }
        uchar*       get_buffer()       { return m_buffer; }

        void swap  ( MemUnit* unit );

        /// makes an alias of unit. sets owner=false
        void borrow( MemUnit* unit );

        void save( ofstream& fout ) const;
        void load( ifstream& fin  );

    private:
        void init_();
        void set_buffer(uchar* new_buff, const size_t& new_cap, const bool& new_own_buff);

        uchar* m_buffer;
        size_t m_cap;
        bool   m_owner;

    };

}

#endif
