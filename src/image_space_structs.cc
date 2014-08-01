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
#include <kortex/image_space_structs.h>
#include <kortex/image.h>
#include <kortex/image_processing.h>
#include <limits>
#include <algorithm>

namespace kortex {

    ColorMap::ColorMap() {
        cm_type       = CMT_LINEAR;
        r_type        = RT_PERCENT;
        invert        = false;
        min_percent   =   0.0f;
        max_percent   = 100.0f;
        pow_scale     = 1.0f;
        minv          = -std::numeric_limits<float>::max(); // uses the actual min of the image
        maxv          =  std::numeric_limits<float>::max(); // uses the actual max of the image
    }

    void find_min_max_range( const ColorMap& cmap, const Histogram* hist, float& minv, float& maxv ) {
        switch( cmap.r_type ) {
        case RT_PERCENT:
            minv = hist->get_threshold_value( cmap.min_percent );
            maxv = hist->get_threshold_value( cmap.max_percent );
            break;
        case RT_VALUE:
            minv = cmap.minv;
            maxv = cmap.maxv;
            break;
        case RT_SUM:
            minv = hist->get_threshold_value( cmap.min_percent ) + cmap.minv;
            maxv = hist->get_threshold_value( cmap.max_percent ) + cmap.maxv;
            break;
        }
        if( minv == maxv ) {
            logman_warning_g("[minv %f] [maxv %f] is found same...", minv, maxv);
        }
    }

//
//
//
    Histogram::Histogram() {
        n_bins = 50;
        bins.resize (n_bins,0   );
        xtags.resize(n_bins,0.0f);
    }

    Histogram::Histogram( const int& n_bins_ ) {
        create(n_bins_);
    }

    void Histogram::reset() {
        bins.clear();
        bins.resize( n_bins, 0 );
        xtags.clear();
        xtags.resize(n_bins, 0.0f);
    }

    void Histogram::create( const int& n_bins_ ) {
        n_bins = n_bins_;
        reset();
    }

    int Histogram::bin_id( const float& v ) const {
        assert_statement( n_bins > 0, "init n_bins" );
        int bid = 0;
        for( int i=0; i<n_bins; i++ ) {
            if( xtags[i] > v ) break;
            bid++;
        }
        bid = std::max( 0, std::min( bid-1, n_bins-1 ) );
        return bid;
    }

    void Histogram::init_xtags( const Image* img, const HistogramType& ht ) {

        img->assert_type( IT_F_GRAY | IT_U_GRAY );

        int h = img->h();
        int w = img->w();

        float lo, hi;

        switch( ht ) {
        case HT_LINEAR: {
            if( !image_min_max( *img, 0, 0, w, h, lo, hi ) ) {
                hi = 1.0f;
                lo = 0.0f;
            }
            for( int b=0; b<n_bins; b++ ) {
                float v = float(b)*(hi-lo)/(n_bins-1) + lo;
                xtags[b] = v;
            }
        } break;
        case HT_UNIFORM: {
            std::vector<float> pix_vals;
            pix_vals.reserve(h*w);
            for( int y=0; y<h; y++ ) {
                for( int x=0; x<w; x++ ) {
                    float v = img->get(x,y);
                    if( is_a_number(v) )
                        pix_vals.push_back(v);
                }
            }
            std::sort( pix_vals.begin(), pix_vals.end() );
            int   n_pixs           = pix_vals.size();
            float n_pixels_per_bin = n_pixs / float(n_bins);

            for( int b=0; b<n_bins; b++ ) {
                int k = std::min( n_pixs, int(b*n_pixels_per_bin + 0.5f) );
                xtags[b] = pix_vals[k];
            }
        } break;
        }
    }

    void Histogram::build( const Image* img, const HistogramType& ht ) {
        passert_pointer( img );
        img->passert_type( IT_F_GRAY | IT_U_GRAY );
        passert_statement( n_bins != 0, "initialize the histogram first" );

        reset();
        init_xtags( img, ht );

        int h = img->h();
        int w = img->w();
        switch( img->type() ) {
        case IT_F_GRAY:
            for( int y=0; y<h; y++ ) {
                const float* row = img->get_row_f(y);
                for( int x=0; x<w; x++ ) {
                    const float& v = row[x];
                    if( !is_a_number(v) ) continue;
                    int bid = bin_id( v );
                    bins[bid]++;
                }
            }
            break;
        case IT_U_GRAY:
            for( int y=0; y<h; y++ ) {
                const uchar* row = img->get_row_u(y);
                for( int x=0; x<w; x++ ) {
                    float v = static_cast<float>(row[x]);
                    if( !is_a_number(v) ) continue;
                    int bid = bin_id( v );
                    bins[bid]++;
                }
            }
            break;
        default: switch_fatality();
        }
    }

    /// returns a value for which percent% of all pixels are smaller than it.
    float Histogram::get_threshold_value( const float& percent ) const {
        passert_boundary( percent, 0.0f, 100.01f );
        int nactive = 0;
        for( int n=0; n<n_bins; n++ )
            nactive += bins[n];

        int llim = 0;
        float vp = 0.0f;
        for( int n=0; n<n_bins; n++ ) {
            vp = vp + bins[n];
            if( 100.0f*vp/nactive < percent )
                llim++;
            else
                break;
        }
        if( 100.0f*vp/nactive > percent+0.1f ) llim--;
        llim = std::min( n_bins-1, std::max(0,llim) );
        return xtags[ llim ];
    }


}

