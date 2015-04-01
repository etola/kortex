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
#include <kortex/mem_manager.h>
#include <kortex/check.h>
#include <kortex/defs.h>

namespace kortex {

    const static size_t simd_alignment = 2*sizeof(double);

    /// creates a memory segment such that the returned memory address is
    /// divisible by 'alignment'. 'alignment' is intended to be 16, 32, 64
    void* aligned_malloc( const size_t& number_of_bytes, const size_t& alignment ) {
        passert_boundary    ( alignment, 0, 256 );
        passert_pointer_size( number_of_bytes   );
        size_t offset = (alignment-1) + sizeof( void* );
        void*   ptr = (void*)malloc( number_of_bytes + offset );
        void** aptr = (void**)( ((size_t)ptr+offset) & ~(alignment-1) );
        aptr[-1] = ptr;
        return aptr;
    }

    void aligned_free( void* aptr ) {
        void* ptr = ( (void**)aptr )[-1];
        free(ptr);
    }

    void* allocate(const size_t& sz) {
        void* ptr = NULL;

#if defined( __GNUC__ ) && defined( WITH_SSE )
        const int ret = posix_memalign(&ptr, simd_alignment, sz);
        if (ret != 0) ptr = NULL;
#else
        ptr = (void*)malloc(sz);
#endif
        return ptr;
    }

    void deallocate( void *ptr ) {
        free((void*)ptr);
        ptr = NULL;
    }

    void allocate( int*& ptr, const size_t& n_elem ) {
        passert_pointer_size( n_elem );
        passert_statement( ptr == NULL, "passed non-NULL pointer" );
        size_t memsz = n_elem * sizeof(*ptr);
        ptr = (int*)( allocate(memsz) );
        if( ptr == NULL ) logman_fatal_g( "cannot allocate memory [%f]", n_elem/GB );
    }
    void allocate( float*& ptr, const size_t& n_elem ) {
        passert_pointer_size( n_elem );
        passert_statement( ptr == NULL, "passed non-NULL pointer" );
        size_t memsz = n_elem * sizeof(*ptr);
        ptr = (float*)( allocate(memsz) );
        if( ptr == NULL ) logman_fatal_g( "cannot allocate memory [%f]", n_elem/GB );
    }
    void allocate( double*& ptr, const size_t& n_elem ) {
        passert_pointer_size( n_elem );
        passert_statement( ptr == NULL, "passed non-NULL pointer" );
        size_t memsz = n_elem * sizeof(*ptr);
        ptr = (double*)( allocate(memsz) );
        if( ptr == NULL ) logman_fatal_g( "cannot allocate memory [%f]", n_elem/GB );
    }
    void allocate( uchar*& ptr, const size_t& n_elem ) {
        passert_pointer_size( n_elem );
        passert_statement( ptr == NULL, "passed non-NULL pointer" );
        size_t memsz = n_elem * sizeof(*ptr);
        ptr = (uchar*)( allocate(memsz) );
        if( ptr == NULL ) logman_fatal_g( "cannot allocate memory [%f]", n_elem/GB );
    }
    void allocate( uint64_t*& ptr, const size_t& n_elem ) {
        passert_pointer_size( n_elem );
        passert_statement( ptr == NULL, "passed non-NULL pointer" );
        size_t memsz = n_elem * sizeof(*ptr);
        ptr = (uint64_t*)( allocate(memsz) );
        if( ptr == NULL ) logman_fatal_g( "cannot allocate memory [%f]", n_elem/GB );
    }

    void deallocate( uint64_t*& ptr ) { deallocate( (void*) ptr ); ptr = NULL; }
    void deallocate(      int*& ptr ) { deallocate( (void*) ptr ); ptr = NULL; }
    void deallocate(    float*& ptr ) { deallocate( (void*) ptr ); ptr = NULL; }
    void deallocate(   double*& ptr ) { deallocate( (void*) ptr ); ptr = NULL; }
    void deallocate(    uchar*& ptr ) { deallocate( (void*) ptr ); ptr = NULL; }


}
