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
#ifndef KORTEX_INDEXED_TYPES_H
#define KORTEX_INDEXED_TYPES_H

#include <vector>

using std::vector;

namespace kortex {

    struct ifloat {
        int   id;
        float val;
        ifloat() : id(0), val(0.0f) {}
        ifloat(int i, float v) : id(i), val(v) {}
    };

    void sort_ascending( vector<ifloat>& arr );
    void sort_descending( vector<ifloat>& arr );

}

#endif
