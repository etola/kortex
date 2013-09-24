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
#ifndef KORTEX_MINMAX_H
#define KORTEX_MINMAX_H

#include <vector>
using std::vector;

namespace kortex {

    int min( const float* arr, const int& narr, float& min_val );
    int max( const float* arr, const int& narr, float& max_val );

    int min( const vector<float>& arr, float& min_val );
    int max( const vector<float>& arr, float& max_val );

    void find_2_minima( const float* cost, int np, int rad, int matches[2] );
}

#endif
