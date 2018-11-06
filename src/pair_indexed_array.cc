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

#include <kortex/pair_indexed_array.h>
#include "pair_indexed_array.tcc"

namespace kortex {

    template class PairIndexedArray<int,int>;
    template class PairIndexedArray<int,float>;
    template class PairIndexedArray<int,double>;

    template class PairIndexedArray<uint64_t,int>;

}
