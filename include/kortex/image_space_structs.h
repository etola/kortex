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
#ifndef KORTEX_IMAGE_SPACE_STRUCTS_H
#define KORTEX_IMAGE_SPACE_STRUCTS_H

#include <vector>

namespace kortex {

    enum ColorMapType { CMT_LINEAR, CMT_POWER, CMT_LOG };
    enum RangeType    { RT_PERCENT, RT_VALUE, RT_SUM };
    enum HistogramType { HT_LINEAR =0, // sets xtags uniformly between lo-hi
                         HT_UNIFORM=1  // sets xtags such that the bins have
                                       // almost same number of elements.
    };


    class Image;

    class Histogram {
    public:
        Histogram();
        Histogram( const int& n_bins_ );
        void create( const int& n_bins_ );
        void reset();

        void init_xtags( const Image* img, const HistogramType& ht );
        void build ( const Image* img, const HistogramType& ht );
        int  bin_id( const float& v ) const;

        /// returns a value for which percent% of all pixels are smaller than it.
        float get_threshold_value( const float& percent ) const;

    private:
        HistogramType htype;

        int   n_bins;
        std::vector<int> bins;
        std::vector<float> xtags;
    };

    struct ColorMap {
    public:
        ColorMapType cm_type;
        RangeType     r_type;

        float min_percent;
        float max_percent;
        float minv;
        float maxv;
        float pow_scale;

        bool  invert;        // invert color space q = 255-p is applied
        ColorMap();
    };

    void find_min_max_range( const ColorMap& cmap, const Histogram* hist, float& minv, float& maxv );


}

#endif
