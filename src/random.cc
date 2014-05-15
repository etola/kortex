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
#include <kortex/random.h>
#include <kortex/check.h>
#include <kortex/indexed_types.h>

#include <cstdlib>
#include <sys/timeb.h>
#include <climits>

namespace kortex {

    void initialize_random_seed() {
        srand( time_seed() );
    }

    unsigned int time_seed() {
        struct timeb one; ftime(&one);
        time_t now = (one.millitm) * (one.millitm+42) * (one.millitm+1981);
        unsigned char *p = (unsigned char *)&now;
        unsigned int seed = 0;
        for ( size_t i = 0; i < sizeof(now); i++ )
            seed = seed * ( UCHAR_MAX + 2U ) + p[i];
        return seed;
    }

    double uniform_sample() {
        return double(rand())/RAND_MAX;
    }

    bool select_random_samples(const int& minval, const int& maxval, const int& no_samples,
                               int *selected_samples) {

        assert_pointer( selected_samples );
        assert_statement_g( is_positive_number(no_samples), "[no_samples %d] must be positive", no_samples );

        int range = maxval - minval;
        if( range < no_samples ) return false;

        // if range is larger than no_samples then there's no problem
        if( range > no_samples*2 ) {
            int counter = 0;
            while( counter < no_samples ) {
                int sample = (int)(uniform_sample()*range)+minval;
                if( sample >= maxval ) continue;
                if( is_inside(selected_samples, counter, sample) )
                    continue;
                selected_samples[counter] = sample;
                counter ++;
            }
            return true;
        } else { // select which samples not to select since they are less in number
            int excluded_sample_no = range - no_samples;
            int * temp_space = new int [excluded_sample_no];

            int counter = 0;
            while( counter < excluded_sample_no ) {
                int sample = (int)(uniform_sample()*range)+minval;
                if( is_inside(temp_space, counter, sample) )
                    continue;
                temp_space[counter] = sample;
                counter ++;
            }
            counter = 0;
            for(int i=minval; i<maxval; i++) {
                if( is_inside(temp_space, excluded_sample_no, i) ) continue;
                selected_samples[counter] = i;
                counter++;
            }
            delete [] temp_space;
            return true;
        }
    }

    void select_prosac_like_random_samples(const int& prosac_iter, const int& selection_limit, const int& no_samples_to_select,
                                           int* selected_samples) {
        // implements select_from_top for use with prosac.
        assert_pointer( selected_samples );
        assert_statement_g( is_nonnegative_number(prosac_iter),       "prosac iteration must be a positive integer [%d]", prosac_iter );
        assert_statement_g( is_positive_number(no_samples_to_select), "no_samples_to_select must be > 0 [%d]", no_samples_to_select);
        assert_statement_g( is_positive_number(selection_limit),      "selection_limit must be > 0 [%d]", selection_limit);

        int   prosac_start = 32;
        float prosac_inc   = 0.5;
        int allowed_range = std::min( selection_limit, prosac_start + (int)(prosac_iter*prosac_inc) );
        assert_statement_g( no_samples_to_select < selection_limit, "[no_samples %d] [iter %d], [n_max_samples %d] [allowed range %d] > [no_samples %d]", no_samples_to_select, prosac_iter, selection_limit, allowed_range, no_samples_to_select );
        assert_statement_g( allowed_range > no_samples_to_select,   "[no_samples %d] [iter %d], [n_max_samples %d] [allowed range %d] > [no_samples %d]", no_samples_to_select, prosac_iter, selection_limit, allowed_range, no_samples_to_select );
        if( !select_random_samples(0, allowed_range, no_samples_to_select, selected_samples) )
            logman_fatal_g("could not select enough random samples [prosac_iter %d] [no_samples_to_select %d] [selection_limit %d]", prosac_iter, no_samples_to_select, selection_limit);
    }

    void random_permutation( int n, std::vector<int>& perm ) {
        assert_statement( n>0, "number of samples is not positive" );

        vector<ifloat> nums( n );
        for( int i=0; i<n; i++ ) {
            nums[i].id  = i;
            nums[i].val = uniform_sample();
        }
        sort_ascending( nums );

        perm.clear();
        perm.resize( n );
        for( int i=0; i<n; i++ )
            perm[i] = nums[i].id;
    }

}

