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
#ifndef KORTEX_TYPES_H
#define KORTEX_TYPES_H

#include <cstdint>  // uint16_t
#include <cstdlib>  // size_t
#include <iostream> // cerr

#ifndef uchar
typedef unsigned char uchar;
#endif

namespace kortex {

    enum DataType { TYPE_CHAR,  TYPE_FLOAT, TYPE_DOUBLE, TYPE_INT,
                    TYPE_UCHAR, TYPE_UINT16, TYPE_SIZE_T };

    inline DataType get_pointer_type( const int     * p ) { return TYPE_INT;    }
    inline DataType get_pointer_type( const char    * p ) { return TYPE_CHAR;   }
    inline DataType get_pointer_type( const uchar   * p ) { return TYPE_UCHAR;  }
    inline DataType get_pointer_type( const float   * p ) { return TYPE_FLOAT;  }
    inline DataType get_pointer_type( const double  * p ) { return TYPE_DOUBLE; }
    inline DataType get_pointer_type( const uint16_t* p ) { return TYPE_UINT16; }
    inline DataType get_pointer_type( const size_t  * p ) { return TYPE_SIZE_T; }

    inline size_t get_data_byte_size( const DataType& type ) {
        switch( type ) {
        case TYPE_CHAR   : return sizeof(char);
        case TYPE_FLOAT  : return sizeof(float);
        case TYPE_DOUBLE : return sizeof(double);
        case TYPE_INT    : return sizeof(int);
        case TYPE_UCHAR  : return sizeof(uchar);
        case TYPE_UINT16 : return sizeof(uint16_t);
        case TYPE_SIZE_T : return sizeof(size_t);
        default          :
            std::cerr<<"unhandled type\n";
            exit(99);
        }
        return 0;
    }

}

#endif
