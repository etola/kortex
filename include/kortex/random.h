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
#ifndef KORTEX_RANDOM_H
#define KORTEX_RANDOM_H

#include <vector>
#include <kortex/random_generator.h>

namespace kortex {

    // void initialize_random_seed();

    // // http://www.eternallyconfuzzled.com/arts/jsw_art_rand.aspx
    // /// generates a seed to feed to srand. better than time(NULL)
    // unsigned int time_seed();

    double  uniform_sample();
    double  normal_sample();

    /// selects no_samples random in [minval maxval). returns false if samples cannot be selected
    bool select_random_samples(const int& minval, const int& maxval, const int& no_samples, int *selected_samples);

    void select_prosac_like_random_samples(const int& prosac_iter, const int& selection_limit, const int& no_samples_to_select,
                                           int* selected_samples);

    void random_permutation( int n, std::vector<int>& perm );

}

#endif
