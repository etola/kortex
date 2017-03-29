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
#ifndef KORTEX_HISTOGRAM_H
#define KORTEX_HISTOGRAM_H

#include <cstdlib>
#include <vector>
using std::vector;

namespace kortex {

    class Histogram {
    private:
        vector<int> m_bins;
        float       m_min;
        float       m_max;
        float       m_bin_step;
        int         m_n_samples;

    public:
        Histogram();
        Histogram( float mn_val, float mx_val, int num_bins ) {
            reset( mn_val, mx_val, num_bins );
        }

        const vector<int>& get_bins() const { return m_bins; }

        void clear_bins();
        void reset( float mn_val, float mx_val, int num_bins );
        void insert( const float& val );

        void compute( const vector<float>& arr );
        void compute( const vector<float>& arr, const float& min_value_th );
        void compute( const float* arr, const size_t& narr, const float& min_value_th );

        int  bin_id( const float& val ) const;

        int  n_bins() const { return m_bins.size(); }

        int  bin_value( int bid ) const;
        int  max_value() const;

        /// returns the approximate value of the percentage point. it returns the
        /// closest lower bound of the bin which surpasses percentage of samples;
        float approximate_value( const float& percentage ) const;

        void print() const;

        int   integrate_till( const int& bid ) const;

    };

}

#endif
