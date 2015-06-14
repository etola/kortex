// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2014 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_SORTING_H
#define KORTEX_SORTING_H

#include <vector>

namespace kortex {

    void sort_ascending ( std::vector<float>& arr);
    void sort_descending( std::vector<float>& arr );

    void sort_ascending ( std::vector<int>& arr);
    void sort_descending( std::vector<int>& arr );

}

#endif
