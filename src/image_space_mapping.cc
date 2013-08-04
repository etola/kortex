// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See licence.txt file for licence information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
//
// ---------------------------------------------------------------------------

#include <kortex/image_space_mapping.h>
#include <kortex/image_processing.h>
#include <kortex/image.h>
#include <kortex/check.h>
#include <kortex/color.h>

namespace kortex {

    void image_map_linear(const Image* img, float& min_v, float& max_v, Image* out ) {
        passert_pointer( img && out );
        img->passert_type( IT_F_GRAY );

        float scale = 1.0f;
        if( min_v == -1.0f && max_v == -1.0f ) {
            if( !image_min_max( img, 0, 0, img->w(), img->h(), min_v, max_v ) ) {
                logman_error("min max range for the image could not be found");
                min_v = 0.0f;
                max_v = 0.0f;
                scale = 1.0f;
            }
        } else {
            scale = 255.0f / ( max_v - min_v );
        }

        int h = img->h();
        int w = img->w();
        out->create( w, h, IT_U_GRAY );
        out->zero();
        for( int y=0; y<h; y++ ) {
            const float* srow = img->get_row_f(y);
            uchar      * orow = out->get_row_u(y);
            for( int x=0; x<w; x++ ) {
                const float& v = srow[x];
                if( is_a_number(v) )
                    orow[x] = cast_to_gray_range( (v-min_v)*scale );
            }
        }
    }

    void image_power(const Image* img, const float& pscale, Image* out ) {
        passert_pointer( img && out );
        img->passert_type( IT_F_GRAY );

        int h = img->h();
        int w = img->w();
        out->create( w, h, IT_F_GRAY );
        out->zero();
        for( int y=0; y<h; y++ ) {
            const float* srow = img->get_row_f(y);
            float      * orow = out->get_row_f(y);
            for( int x=0; x<w; x++ ) {
                const float& v = srow[x];
                if( is_a_number(v) )
                    orow[x] = pow(v,pscale);
            }
        }
    }

    void image_log(const Image* img, Image* out ) {
        passert_pointer( img && out );
        img->passert_type( IT_F_GRAY );

        int h = img->h();
        int w = img->w();
        out->create( w, h, IT_F_GRAY );
        out->zero();
        for( int y=0; y<h; y++ ) {
            const float* srow = img->get_row_f(y);
            float      * orow = out->get_row_f(y);
            for( int x=0; x<w; x++ ) {
                const float& v = srow[x];
                if( is_a_number(v) ) orow[x] = log(v);
                else                 orow[x] = v;
            }
        }
    }

    void image_map( const Image* img, const ColorMap& cm, Image* out ) {

        Histogram hist(1000);
        float minv=0.0f, maxv=0.0f;
        Image tmp;
        switch( cm.cm_type ) {
        case CMT_LINEAR:
            hist.build( img, HT_UNIFORM );
            find_min_max_range( cm, &hist, minv, maxv );
            image_map_linear(img, minv, maxv, out );
            break;
        case CMT_POWER:
            image_power(img, cm.pow_scale, &tmp);
            hist.build( &tmp, HT_UNIFORM );
            find_min_max_range( cm, &hist, minv, maxv );
            image_map_linear(&tmp, minv, maxv, out );
            break;
        case CMT_LOG:
            image_log(img, &tmp);
            hist.build( &tmp, HT_LINEAR );
            find_min_max_range( cm, &hist, minv, maxv );
            image_map_linear(&tmp, minv, maxv, out );
            break;
        }
    }

}
