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
#include <kortex/indexed_types.h>
#include <algorithm>

using namespace std;

namespace kortex {

    inline bool ifloat_cmp_l(const ifloat& l, const ifloat& r) { return l.val > r.val; }
    inline bool ifloat_cmp_s(const ifloat& l, const ifloat& r) { return l.val < r.val; }

    void sort_ascending( vector<ifloat>& arr) {
        sort( arr.begin(), arr.end(), ifloat_cmp_s );
    }

    void sort_descending( vector<ifloat>& arr ) {
        sort( arr.begin(), arr.end(), ifloat_cmp_l );
    }

}

