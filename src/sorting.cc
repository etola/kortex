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
#include <algorithm>
#include <kortex/sorting.h>

using std::vector;

namespace kortex {

    //
    //
    //
    inline bool double_cmp_l(const double& l, const double& r) { return l > r; }
    inline bool double_cmp_s(const double& l, const double& r) { return l < r; }
    void sort_ascending( vector<double>& arr) {
        std::sort( arr.begin(), arr.end(), double_cmp_s );
    }

    void sort_descending( vector<double>& arr ) {
        std::sort( arr.begin(), arr.end(), double_cmp_l );
    }

    //
    //
    //
    inline bool float_cmp_l(const float& l, const float& r) { return l > r; }
    inline bool float_cmp_s(const float& l, const float& r) { return l < r; }
    void sort_ascending( vector<float>& arr) {
        std::sort( arr.begin(), arr.end(), float_cmp_s );
    }

    void sort_descending( vector<float>& arr ) {
        std::sort( arr.begin(), arr.end(), float_cmp_l );
    }

    //
    //
    //
    inline bool int_cmp_l(const int& l, const int& r) { return l > r; }
    inline bool int_cmp_s(const int& l, const int& r) { return l < r; }
    void sort_ascending( vector<int>& arr) {
        std::sort( arr.begin(), arr.end(), int_cmp_s );
    }
    void sort_descending( vector<int>& arr ) {
        std::sort( arr.begin(), arr.end(), int_cmp_l );
    }

    //
    //
    //
    inline bool uint16_cmp_l(const uint16_t& l, const uint16_t& r) { return l > r; }
    inline bool uint16_cmp_s(const uint16_t& l, const uint16_t& r) { return l < r; }
    void sort_ascending( vector<uint16_t>& arr) {
        std::sort( arr.begin(), arr.end(), uint16_cmp_s );
    }
    void sort_descending( vector<uint16_t>& arr ) {
        std::sort( arr.begin(), arr.end(), uint16_cmp_l );
    }

    //
    //
    //
    inline bool uint32_cmp_l(const uint32_t& l, const uint32_t& r) { return l > r; }
    inline bool uint32_cmp_s(const uint32_t& l, const uint32_t& r) { return l < r; }
    void sort_ascending( vector<uint32_t>& arr) {
        std::sort( arr.begin(), arr.end(), uint32_cmp_s );
    }
    void sort_descending( vector<uint32_t>& arr ) {
        std::sort( arr.begin(), arr.end(), uint32_cmp_l );
    }

}
