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

#include "sparse_array_t.tcc"

namespace kortex {

    template class SparseArrayT<int,    int>;
    template class SparseArrayT<float,  int>;
    template class SparseArrayT<double, int>;

    template class SparseArrayT<string, float>;

}

