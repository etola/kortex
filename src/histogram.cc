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

#include <kortex/histogram.h>
#include <kortex/check.h>

namespace kortex {

    void Histogram::reset( float mn_val, float mx_val, int num_bins ) {
        assert_statement( num_bins > 1, "invalid n_bins" );
        m_min = mn_val;
        m_max = mx_val;
        m_bin_step = ( mx_val - mn_val ) / num_bins;
        m_bins.clear();
        m_bins.resize( num_bins, 0 );
    }
    void Histogram::clear_bins() {
        int nbins = m_bins.size();
        m_bins.clear();
        m_bins.resize( nbins, 0 );
    }

    int  Histogram::bin_id( const float& val ) const {
        if( val >= m_max-m_bin_step ) return n_bins()-1;
        if( val <= m_min            ) return 0;
        int bid = (val-m_min)/m_bin_step;
        assert_statement_g( bid >= 0 && bid < n_bins(),
                            "oob [%d/%d] [val %f] [mn/mx %f %f]",
                            bid, n_bins(), val, m_min, m_max );
        return bid;
    }
    void Histogram::insert( const float& val ) {
        int bid = bin_id( val );
        m_bins[bid]++;
    }
    void Histogram::print() const {
        static const int bufsz = 2560;
        int  nstr = 0;
        char buf[bufsz];
        for( int i=0; i<=n_bins(); i++ )
            nstr += sprintf( buf+nstr, " %6.2f", m_min + i *m_bin_step );
        logman_log( buf );
        nstr = 0;
        for( int i=0; i<n_bins(); i++ )
            nstr += sprintf( buf+nstr, " % 6d", m_bins[i] );
        logman_log( buf );
    }
    void Histogram::compute( const vector<float>& arr ) {
        clear_bins();
        for( unsigned i=0; i<arr.size(); i++ ) {
            this->insert( arr[i] );
        }
    }

}
