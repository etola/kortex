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
    bool image_min_max( const Image& img,
                        const int& xmin, const int& ymin,
                        const int& xmax, const int& ymax,
                        float& min_v, float& max_v );

    inline bool image_min_max( const Image& img, float& min_v, float& max_v ) {
        return image_min_max( img, -1, -1, -1, -1, min_v, max_v );
    }

    bool abs_image_min_max( const Image& img,
                            const int& xmin, const int& ymin,
                            const int& xmax, const int& ymax,
                            float& min_v, float& max_v );

    inline bool abs_image_min_max( const Image& img, float& min_v, float& max_v ) {
        return abs_image_min_max( img, -1, -1, -1, -1, min_v, max_v );
    }


    void        filter_hv( const Image& img, const float* kernel, const int& ksz, Image& out );
    inline void filter_hv( Image& img, const float* kernel, const int& ksz ) {
        filter_hv( img, kernel, ksz, img );
    }
    void filter_hv_par( const Image& img, const float* kernel, const int& ksz, Image& out );

    inline void filter_hv( const Image& img, const float* kernel, const int& ksz, const bool& run_parallel, Image& out ) {
        switch( run_parallel ) {
        case false: filter_hv    ( img, kernel, ksz, out ); break;
        case true : filter_hv_par( img, kernel, ksz, out ); break;
        }
    }
    inline void filter_hv( Image& img, const float* kernel, const int& ksz, const bool& run_parallel ) {
        filter_hv( img, kernel, ksz, run_parallel, img );
    }

    void filter_hor ( const Image& img, const float* kernel, const int& ksz, Image& out );
    inline void filter_hor( Image& img, const float* kernel, const int& ksz ) {
        filter_hor( img, kernel, ksz, img );
    }
    void filter_hor_par( const Image& img, const float* kernel, const int& ksz, Image& out );

    void filter_ver ( const Image& img, const float* kernel, const int& ksz, Image& out );
    inline void filter_ver( Image& img, const float* kernel, const int& ksz ) {
        filter_ver( img, kernel, ksz, img );
    }
    void filter_ver_par( const Image& img, const float* kernel, const int& ksz, Image& out );


    void filter_gaussian    ( const Image& img, const float& sigma, Image& out );
    void filter_gaussian_par( const Image& img, const float& sigma, Image& out );
    inline void filter_gaussian( Image& img, const float& sigma ) {
        filter_gaussian( img, sigma, img );
    }
    inline void filter_gaussian_par( Image& img, const float& sigma ) {
        filter_gaussian_par( img, sigma, img );
    }
    inline void filter_gaussian( const Image& img, const float& sigma, const bool& run_parallel, Image& out ) {
        switch( run_parallel ) {
        case false: filter_gaussian    ( img, sigma, out ); break;
        case true : filter_gaussian_par( img, sigma, out ); break;
        }
    }

    void combine_horizontally(const Image& im0, const Image& im1, Image& out);
    void combine_vertically  (const Image& im0, const Image& im1, Image& out);

    void flip_image_ver( Image& img );
    void flip_image_hor( Image& img );

    void image_threshold( const Image& img, float th, Image& out );
    inline void image_threshold( Image& img, float th ) {
        image_threshold( img, th, img );
    }

    ///
    /// assumes image_threshold is called first over mask.
    ///
    /// er_size : how many pixels to erode
    void erode_mask( Image& mask, int er_size );

    void image_resize_coarse( const Image& src, const int& nw, const int& nh, Image& dst );
    void image_resize_fine  ( const Image& src, const int& nw, const int& nh, Image& dst );

    void image_resize_coarse( const Image& src, int max_img_dim, Image& dst );
    void image_resize_fine  ( const Image& src, int max_img_dim, Image& dst );

    void image_subtract    ( const Image& im0, const Image& im1, Image& out );
    void image_subtract_par( const Image& im0, const Image& im1, Image& out );
    inline void image_subtract( const Image& im0, const Image& im1, bool run_parallel, Image& out ) {
        switch( run_parallel ) {
        case false: image_subtract    ( im0, im1, out ); break;
        case true : image_subtract_par( im0, im1, out ); break;
        }
    }

    /// adds v to every pixel
    void image_add( const Image& img, float v, Image& out );
    inline void image_add( Image& img, float v ) {
        image_add( img, v, img );
    }

    void image_add    ( const Image& im0, const Image& im1, Image& out );
    void image_add_par( const Image& im0, const Image& im1, Image& out );
    inline void image_add( const Image& im0, const Image& im1, bool run_parallel, Image& out ) {
        switch( run_parallel ) {
        case false: image_add    ( im0, im1, out ); break;
        case true : image_add_par( im0, im1, out ); break;
        }
    }

    /// r = p/q for q>1e-6 else 0
    void image_divide    ( const Image& p, const Image& q, Image& r );
    void image_divide_par( const Image& p, const Image& q, Image& r );
    inline void image_divide( const Image& p, const Image& q, bool run_parallel, Image& r ) {
        switch( run_parallel ) {
        case false: image_divide    ( p, q, r ); break;
        case true : image_divide_par( p, q, r ); break;
        }
    }


    /// r = p*q
    void image_multiply    ( const Image& p, const Image& q, Image& r );
    void image_multiply_par( const Image& p, const Image& q, Image& r );
    inline void image_multiply( const Image& p, const Image& q, bool run_parallel, Image& r ) {
        switch( run_parallel ) {
        case false: image_multiply    ( p, q, r ); break;
        case true : image_multiply_par( p, q, r ); break;
        }
    }

    /// o = p*q + r
    void image_multiply_add    ( const Image& p, const Image& q, const Image& r, Image& o );
    void image_multiply_add_par( const Image& p, const Image& q, const Image& r, Image& o );
    inline void image_multiply_add( const Image& p, const Image& q, const Image& r, bool run_parallel, Image& o ) {
        switch( run_parallel ) {
        case false: image_multiply_add    ( p, q, r, o ); break;
        case true : image_multiply_add_par( p, q, r, o ); break;
        }
    }

    /// q = s*p
    void image_scale    ( const Image& p, float s, Image& q );
    void image_scale_par( const Image& p, float s, Image& q );
    inline void image_scale( const Image& p, float s, bool run_parallel, Image& q ) {
        switch( run_parallel ) {
        case false: image_scale    ( p, s, q ); break;
        case true : image_scale_par( p, s, q ); break;
        }
    }

    /// checks whether p has values of either 0.0f or 1.0f
    bool is_binarized( const Image& p );

    /// checks whether p has values in the range [0.0f 1.0f]
    bool is_normalized( const Image& p );

    /// checks whether p and q are non-zero for the same pixel
    bool does_overlap( const Image& p, const Image& q );

    void init_gaussian_weight_mask( Image& mask );
    void init_linear_weight_mask  ( Image& mask );

    void image_color_invert( Image& img );

    template <typename T>
    float bilinear_interpolation(const T* img, const int& w, const int& h, const int& nc, const int& c,  const float& x, const float& y);

    template <typename T>
    float  bicubic_interpolation(const T* im,  const int& w, const int& h, const int& nc, const int& ch, const float& x, const float& y);

    int  filter_size( const float& sigma );

    /// maps the src image to 0.0 -> 1.0 range linearly
    void image_linearize( const Image& src, Image& dst );
    inline void image_linearize( Image& img ) {
        image_linearize( img, img );
    }

    /// standard = true normalizes with 255 -> false normalizes according to the max value
    void image_normalize( const Image& src, bool standard, Image& dst );
    inline void image_normalize( Image& img, bool standard ) {
        image_normalize( img, standard, img );
    }
    inline void image_normalize( Image& img ) {
        image_normalize( img, true, img );
    }


    void image_unnormalize( const Image& src, Image& dst );
    inline void image_unnormalize( Image& img ) {
        image_unnormalize( img, img );
    }

    /// computes per pixel image gradient magnitude
    void image_gradient_magnitude( const Image& src, bool run_parallel, Image& mag );

    /// stretches image info such that its minv->0.0f maxv->255.0f. if minv,maxv
    /// specified as 0.0f 0.0f range is extracted from the source image.
    void image_stretch( const Image& src, float minv, float maxv, Image& out );

    /// computes the absolute value image
    void image_abs( const Image& img, bool run_parallel, Image& out );
    inline void image_abs( Image& img, bool run_parallel ) {
        image_abs( img, run_parallel, img );
    }

    void image_negate( const Image& img, Image& out );
    inline void image_negate( Image& img ) {
        image_negate( img, img );
    }

    void image_clip_lower( const Image& src, float min_v, bool run_parallel, Image& out );
    inline void image_clip_lower( Image& src, float min_v, bool run_parallel ) {
        image_clip_lower( src, min_v, run_parallel, src );
    }
    inline void image_clip_lower_par( Image& src, float min_v ) {
        image_clip_lower( src, min_v, true, src );
    }

    void image_clip( const Image& src, float min_v, float max_v, bool run_parallel, Image& out );
    inline void image_clip( Image& src, float min_v, float max_v, bool run_parallel ) {
        image_clip( src, min_v, max_v, run_parallel, src );
    }
    inline void image_clip_par( Image& src, float min_v, float max_v ) {
        image_clip( src, min_v, max_v, true, src );
    }


    // gtype can be one of [ "simple", "sobel", "prewitt" ]
    void image_gradient( const Image& img, const char* gtype, Image& gx, Image& gy );

    void image_gradient_prewitt( const Image& img, Image& gx, Image& gy );
    void image_gradient_sobel  ( const Image& img, Image& gx, Image& gy );
    void image_gradient_simple (const float* im, int w, int h, float* dx, float* dy);
    void image_gradient_simple ( const Image& img, Image& gx, Image& gy );

    // set nb pixels of the boundary to 0.0f
    void image_reset_boundary( Image& img, int nb );


}

#endif
