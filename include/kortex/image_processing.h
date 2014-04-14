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
#ifndef KORTEX_IMAGE_PROCESSING_H
#define KORTEX_IMAGE_PROCESSING_H

namespace kortex {

    class Image;

    /// finds the [min,max] value range for the image region defined by
    /// [xmin,ymin]->[xmax,ymax] ; NAN safe
    bool image_min_max( const Image* img,
                        const int& xmin, const int& ymin,
                        const int& xmax, const int& ymax,
                        float& min_v, float& max_v );

    void        filter_hv( const Image* img, const float* kernel, const int& ksz, Image* out );
    inline void filter_hv( Image* img, const float* kernel, const int& ksz ) {
        filter_hv( img, kernel, ksz, img );
    }
    void filter_hv_par( const Image* img, const float* kernel, const int& ksz, Image* out );

    inline void filter_hv( const Image* img, const float* kernel, const int& ksz, const bool& run_parallel, Image* out ) {
        switch( run_parallel ) {
        case true : filter_hv_par( img, kernel, ksz, out ); break;
        case false: filter_hv    ( img, kernel, ksz, out ); break;
        }
    }

    void filter_gaussian    ( const Image* img, const float& sigma, Image* out );
    void filter_gaussian_par( const Image* img, const float& sigma, Image* out );
    inline void filter_gaussian( Image* img, const float& sigma ) {
        filter_gaussian( img, sigma, img );
    }
    inline void filter_gaussian_par( Image* img, const float& sigma ) {
        filter_gaussian_par( img, sigma, img );
    }
    inline void filter_gaussian( const Image* img, const float& sigma, const bool& run_parallel, Image* out ) {
        switch( run_parallel ) {
        case true : filter_gaussian_par( img, sigma, out ); break;
        case false: filter_gaussian    ( img, sigma, out ); break;
        }
    }

    void combine_horizontally(const Image* im0, const Image* im1, Image* out);
    void combine_vertically  (const Image* im0, const Image* im1, Image* out);

    void flip_image_ver( Image* img );
    void flip_image_hor( Image* img );

    void image_threshold( Image* img, float th );
    void image_to_gradient(const float* im, int w, int h, float* dx, float* dy);

    void image_resize_coarse( const Image* src, const int& nw, const int& nh, Image* dst );

    void image_scale( const Image* im, const float& scale, const bool& run_parallel, Image* out );

    void image_subtract    ( const Image* im0, const Image* im1, Image* out );
    void image_subtract_par( const Image* im0, const Image* im1, Image* out );
    inline void image_subtract( const Image* im0, const Image* im1, bool run_parallel, Image* out ) {
        switch( run_parallel ) {
        case true : image_subtract    ( im0, im1, out ); break;
        case false: image_subtract_par( im0, im1, out ); break;
        }
    }

    void image_add    ( const Image* im0, const Image* im1, Image* out );
    void image_add_par( const Image* im0, const Image* im1, Image* out );
    inline void image_add( const Image* im0, const Image* im1, bool run_parallel, Image* out ) {
        switch( run_parallel ) {
        case true : image_add    ( im0, im1, out ); break;
        case false: image_add_par( im0, im1, out ); break;
        }
    }


    void image_color_invert( Image* img );

    template <typename T>
    float bilinear_interpolation(const T* img, const int& w, const int& h, const int& nc, const int& c,  const float& x, const float& y);

    template <typename T>
    float  bicubic_interpolation(const T* im,  const int& w, const int& h, const int& nc, const int& ch, const float& x, const float& y);


}

#endif
