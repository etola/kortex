// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See licence.txt file for licence information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_MEM_MANAGER_H
#define KORTEX_MEM_MANAGER_H

#include <kortex/types.h>
#include <kortex/check.h>

namespace kortex {

    void allocate(    int*& ptr, const size_t& n_elem );
    void allocate(  float*& ptr, const size_t& n_elem );
    void allocate( double*& ptr, const size_t& n_elem );
    void allocate(  uchar*& ptr, const size_t& n_elem );

    void deallocate(    int*& ptr );
    void deallocate(  float*& ptr );
    void deallocate( double*& ptr );
    void deallocate(  uchar*& ptr );

    enum MemoryMode { MM_16_UNALIGNED=0, MM_16_ALIGNED=1 };

    template <typename T> inline
    MemoryMode get_alignment(const T* kernel) {
        if( is_16_byte_aligned(kernel) ) return MM_16_ALIGNED;
        else                             return MM_16_UNALIGNED;
    }

    template <typename T> inline
    MemoryMode get_alignment(const T* buffer, const T* kernel) {
        return ( is_16_byte_aligned(buffer) && is_16_byte_aligned(kernel) ) ? MM_16_ALIGNED : MM_16_UNALIGNED;
    }


}

#endif
