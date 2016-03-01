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

#include <vector>
using std::vector;

namespace kortex {

    class Histogram {
    private:
        vector<int> m_bins;
        float       m_min;
        float       m_max;
        float       m_bin_step;

    public:
        Histogram();
        Histogram( float mn_val, float mx_val, int num_bins ) {
            reset( mn_val, mx_val, num_bins );
        }

        void clear_bins();
        void reset( float mn_val, float mx_val, int num_bins );
        void insert( const float& val );

        void compute( const vector<float>& arr );

        int  bin_id( const float& val ) const;

        int  n_bins() const { return m_bins.size(); }

        void print() const;
    };

}

#endif
