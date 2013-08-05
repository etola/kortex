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
#include <kortex/mem_unit.h>
#include <kortex/check.h>
#include <kortex/mem_manager.h>

#include <cstring> // memcpy

namespace kortex {

    MemUnit::MemUnit() {
        init_();
    }

    MemUnit::MemUnit(const size_t& init_cap) {
        init_();
        resize(init_cap);
    }

    MemUnit::~MemUnit() {
        deallocate();
    }

    void MemUnit::init_() {
        m_buffer = NULL;
        m_cap    = 0;
        m_owner  = true;
    }

    void MemUnit::resize(const size_t& new_cap) {
        if( m_cap < new_cap ) {
            passert_statement( is_owner(), "cannot resize array when not owner of memory" );
            deallocate();
            uchar* buf = NULL;
            kortex::allocate( buf, new_cap );
            set_buffer( buf, new_cap, true );
        }
    }

    void MemUnit::expand(const size_t& new_cap) {
        if( m_cap >= new_cap ) return;
        passert_statement( is_owner(), "cannot expand array when not owner of memory" );
        MemUnit* new_unit = new MemUnit( new_cap );
        memcpy( new_unit->get_buffer(), this->get_buffer(), sizeof(*m_buffer)*this->capacity() );
        this->swap( new_unit );
        new_unit->deallocate();
        delete new_unit;
    }

    void MemUnit::deallocate() {
        if( is_owner() )
            kortex::deallocate( m_buffer );
        init_();
    }

    void MemUnit::set_buffer(uchar* new_buff, const size_t& new_cap, const bool& new_own_buff) {
        deallocate();
        m_buffer = new_buff;
        m_cap    = new_cap;
        m_owner  = new_own_buff;
    }

    void MemUnit::swap(MemUnit* unit) {
        std::swap(m_buffer,   unit->m_buffer);
        std::swap(m_cap,      unit->m_cap);
        std::swap(m_owner,    unit->m_owner);
    }

    void MemUnit::borrow(MemUnit* unit) {
        deallocate();
        m_buffer  = unit->m_buffer;
        m_cap     = unit->m_cap;
        m_owner   = false;
    }


}

