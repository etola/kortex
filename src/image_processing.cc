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

#include <kortex/image_processing.h>
#include <kortex/types.h>
#include <kortex/image.h>
#include <kortex/filter.h>
#include <kortex/mem_manager.h>
#include <kortex/math.h>
#include <kortex/color.h>

#include "image_processing.tcc"
#include <limits>

namespace kortex {

    template float bilinear_interpolation(const uchar* img, const int& w, const int& h, const int& nc, const int& c,  const float& x, const float& y);
    template float bilinear_interpolation(const float* img, const int& w, const int& h, const int& nc, const int& c,  const float& x, const float& y);
    template float  bicubic_interpolation(const uchar* im,  const int& w, const int& h, const int& nc, const int& ch, const float& x, const float& y);
    template float  bicubic_interpolation(const float* im,  const int& w, const int& h, const int& nc, const int& ch, const float& x, const float& y);

    bool image_min_max( const Image* img,
                        const int& xmin, const int& ymin,
                        const int& xmax, const int& ymax,
                        float& min_v, float& max_v ) {
        passert_pointer( img );
        img->passert_type( IT_F_GRAY );

        int xs = std::max( std::min( xmin, xmax ), 0        );
        int xe = std::min( std::max( xmin, xmax ), img->w() );
        int ys = std::max( std::min( ymin, ymax ), 0        );
        int ye = std::min( std::max( ymin, ymax ), img->h() );

        min_v =  std::numeric_limits<float>::max();
        max_v = -std::numeric_limits<float>::max();
        for( int y=ys; y<ye; y++ ) {
            const float* row = img->get_row_f(y);
            for( int x=xs; x<xe; x++ ) {
                const float& v = row[x];
                if( is_a_number(v) ) {
                    min_v = std::min(v,min_v);
                    max_v = std::max(v,max_v);
                }
            }
        }
        if( (min_v ==  std::numeric_limits<float>::max())  ||
            (max_v == -std::numeric_limits<float>::max()) ) {
            return false;
        }
        return true;
    }


    void filter_hv( const Image* img, const float* kernel, const int& ksz, Image* out ) {
        assert_pointer( img && out && kernel );
        assert_pointer_size( ksz );
        assert_statement( !img->is_empty(), "image is empty" );
        passert_statement( out->type() == img->type(), "image types not agree" );
        passert_statement( img->w() == out->w() && img->h() == out->h(),
                           "image in/out has different sizes");
        img->passert_type( IT_F_GRAY | IT_F_IRGB );

        switch( img->type() ) {
        case IT_F_GRAY:
            filter_hv( img->get_row_f(0), img->w(), img->h(), kernel, ksz, out->get_row_f(0) );
            break;
        case IT_F_IRGB: {
            for( int c=0; c<3; c++ ) {
                const Image* sch = img->get_channel_wrapper( c );
                Image      * dch = out->get_channel_wrapper( c );
                filter_hv( sch, kernel, ksz, dch );
                delete sch;
                delete dch;
            }
        } break;
        default: switch_fatality();
        }
    }

    void filter_hv_par( const Image* img, const float* kernel, const int& ksz, Image* out ) {
        assert_pointer( img && out && kernel );
        assert_pointer_size( ksz );
        assert_statement( !img->is_empty(), "image is empty" );
        passert_statement( out->type() == img->type(), "image types not agree" );
        passert_statement( img->w() == out->w() && img->h() == out->h(),
                           "image in/out has different sizes");
        img->passert_type( IT_F_GRAY | IT_F_IRGB ); // supporting these types
                                                    // for now
        switch( img->type() ) {
        case IT_F_GRAY:
            filter_hv_par( img->get_row_f(0), img->w(), img->h(), kernel, ksz, out->get_row_f(0) );
            break;
        case IT_F_IRGB: {
            for( int c=0; c<3; c++ ) {
                const Image* sch = img->get_channel_wrapper( c );
                Image      * dch = out->get_channel_wrapper( c );
                filter_hv_par( sch, kernel, ksz, dch );
                delete sch;
                delete dch;
            }
        } break;
        default: switch_fatality();
        }
    }

    int  filter_size( const float& sigma ) {
        passert_statement( sigma > 0.0f, "sigma should be positive" );
        const int gauss_truncate = 4;
        int fsz = 2.0 * gauss_truncate * sigma + 1.0;
        if( fsz%2 == 0 ) fsz++;
        if( fsz < 3 ) fsz = 3;
        return fsz;
    }

    void filter_gaussian( const Image* img, const float& sigma, Image* out ) {
        assert_pointer( img && out );
        assert_statement( !img->is_empty(), "image is empty" );
        passert_statement( out->type() == img->type(), "image types not agree" );
        passert_statement( img->w() == out->w() && img->h() == out->h(),
                           "image in/out has different sizes");
        int sz = filter_size(sigma);
        float* sfilter = NULL;
        allocate(sfilter, sz);
        gaussian_1d(sfilter, sz, 0, sigma);
        filter_hv( img, sfilter, sz, out );
        deallocate( sfilter );
    }

    void filter_gaussian_par( const Image* img, const float& sigma, Image* out ) {
        assert_pointer( img && out );
        assert_statement( !img->is_empty(), "image is empty" );
        passert_statement( out->type() == img->type(), "image types not agree" );
        passert_statement( img->w() == out->w() && img->h() == out->h(),
                           "image in/out has different sizes");
        int sz = filter_size(sigma);
        float* sfilter = NULL;
        allocate(sfilter, sz);
        gaussian_1d(sfilter, sz, 0, sigma);
        filter_hv_par( img, sfilter, sz, out );
        deallocate( sfilter );
    }

    void subtract( const Image* im0, const Image* im1, Image* out ) {
        assert_pointer( im0 && im1 && out );
        im0->assert_type( IT_F_GRAY );
        im1->assert_type( IT_F_GRAY );
        int w = im0->w();
        int h = im0->h();
        passert_statement( (im1->w() == w) && (im1->h()==h), "dimension mismatch" );
        passert_statement( im0->type() == im1->type(), "image type mismatch" );
        out->create(w,h,im0->type());
        for( int y=0; y<h; y++ ) {
            const float*  row0 = im0->get_row_f(y);
            const float*  row1 = im1->get_row_f(y);
            float      * drow  = out->get_row_f(y);
            for( int x=0; x<w; x++ ) {
                drow[x] = row0[x]-row1[x];
            }
        }
    }

    void subtract_par( const Image* im0, const Image* im1, Image* out ) {
        assert_pointer( im0 && im1 && out );
        im0->assert_type( IT_F_GRAY );
        im1->assert_type( IT_F_GRAY );
        int w = im0->w();
        int h = im0->h();
        passert_statement( (im1->w() == w) && (im1->h()==h), "dimension mismatch" );
        passert_statement( im0->type() == im1->type(), "image type mismatch" );
        out->create(w,h,im0->type());
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const float*  row0 = im0->get_row_f(y);
            const float*  row1 = im1->get_row_f(y);
            float      * drow  = out->get_row_f(y);
            for( int x=0; x<w; x++ ) {
                drow[x] = row0[x]-row1[x];
            }
        }
    }

    void combine_horizontally(const Image* im0, const Image* im1, Image* out) {
        passert_pointer( im0 && im1 && out );
        passert_statement( im0->precision() == im1->precision(), "image precisions are different" );
        passert_statement( im0->ch() == im1->ch(), "image channels different" );

        int w0 = im0->w();
        int h0 = im0->h();
        int w1 = im1->w();
        int h1 = im1->h();
        int w = w0+w1;
        int h = std::max( h0, h1 );

        out->create(w, h, im0->type());
        out->zero();
        out->copy_from_region(im0, 0, 0, w0, h0, 0, 0);
        out->copy_from_region(im1, 0, 0, w1, h1, w0, 0);
    }

    void combine_vertically(const Image* im0, const Image* im1, Image* out) {
        passert_pointer( im0 && im1 && out );
        passert_statement( im0->precision() == im1->precision(), "image precisions are different" );
        passert_statement( im0->ch() == im1->ch(), "image channels different" );

        int w0 = im0->w();
        int h0 = im0->h();
        int w1 = im1->w();
        int h1 = im1->h();
        int w = std::max(w0, w1);
        int h = h0+h1;

        out->create(w, h, im0->type());
        out->zero();
        out->copy_from_region(im0, 0, 0, w0, h0, 0, 0);
        out->copy_from_region(im1, 0, 0, w1, h1, 0, h0);
    }

    void flip_image_ver( Image* img ) {
        assert_pointer( img );
        img->passert_type( IT_F_GRAY | IT_U_GRAY );
        int w = img->w();
        int h = img->h();

        switch( img->precision() ) {
        case TYPE_FLOAT: {
            for( int y=0; y<h/2; y++ ) {
                float* urow = img->get_row_f(y);
                float* drow = img->get_row_f(h-y-1);
                for( int x=0; x<w; x++ ) {
                    std::swap( urow[x], drow[x] );
                }
            }
        } break;
        case TYPE_UCHAR: {
            for( int y=0; y<h/2; y++ ) {
                uchar* urow = img->get_row_u(y);
                uchar* drow = img->get_row_u(h-y-1);
                for( int x=0; x<w; x++ ) {
                    std::swap( urow[x], drow[x] );
                }
            }
        } break;
        default: switch_fatality();
        }
    }

    void flip_image_hor( Image* img ) {
        passert_pointer( img );
        img->passert_type( IT_F_GRAY );
        int w = img->w();
        int h = img->h();

        switch( img->precision() ) {
        case TYPE_FLOAT: {
            for( int y=0; y<h; y++ ) {
                float* row = img->get_row_f(y);
                for( int x=0; x<w/2; x++ )
                    std::swap( row[x], row[w-x-1] );
            }
        } break;
        case TYPE_UCHAR : {
            for( int y=0; y<h; y++ ) {
                uchar* row = img->get_row_u(y);
                for( int x=0; x<w/2; x++ )
                    std::swap( row[x], row[w-x-1] );
            }
        } break;
        default: switch_fatality();
        }
    }

    void image_color_invert( Image* img ) {
        passert_pointer( img );
        img->passert_type( IT_U_GRAY );
        int w = img->w();
        int h = img->h();
        for( int y=0; y<h; y++ ) {
            uchar* row = img->get_row_u(y);
            for( int x=0; x<w; x++ ) {
                row[x] = 255-row[x];
            }
        }
    }

    void image_threshold( Image* img, float th ) {
        assert_pointer( img );
        img->passert_type( IT_F_GRAY );
        int w = img->w();
        int h = img->h();
        for( int y=0; y<h; y++ ) {
            float* row = img->get_row_f(y);
            for( int x=0; x<w; x++ ) {
                if( row[x] > th ) row[x] = 1.0f;
                else              row[x] = 0.0f;
            }
        }
    }

    void image_resize_coarse_rgb( const Image* src, const int& nw, const int& nh, Image* dst ) {
        passert_pointer( src && dst );
        passert_statement( nw > 0 && nh > 0, "invalid new image size" );
        src->assert_type( IT_U_PRGB | IT_U_IRGB );

        dst->create( nw, nh, src->type() );
        dst->zero();

        float ratioy = src->h() / (float)nh;
        float ratiox = src->w() / (float)nw;

        DataType dtype = image_precision( src->type() );

        float r, g, b;
        for( int y=0; y<nh; y++ ) {
            float ny = y*ratioy;
            for( int x=0; x<nw; x++ ) {
                float nx = x*ratiox;
                src->get_bilinear(nx, ny, r, g, b);
                switch( dtype ) {
                case TYPE_FLOAT: dst->set(x, y, r, g, b); break;
                case TYPE_UCHAR: {
                    uchar ur = cast_to_gray_range( r );
                    uchar ug = cast_to_gray_range( g );
                    uchar ub = cast_to_gray_range( b );
                    dst->set(x, y, ur, ug, ub);
                } break;
                default: switch_fatality();
                }
            }
        }
    }

    void image_resize_coarse_g( const Image* src, const int& nw, const int& nh, Image* dst ) {
        passert_pointer( src && dst );
        passert_statement( nw > 0 && nh > 0, "invalid new image size" );
        src->assert_type( IT_U_GRAY | IT_F_GRAY );

        dst->create( nw, nh, src->type() );
        dst->zero();

        float ratioy = src->h() / (float)nh;
        float ratiox = src->w() / (float)nw;

        DataType dtype = image_precision( src->type() );

        for( int y=0; y<nh; y++ ) {
            float ny = y*ratioy;
            for( int x=0; x<nw; x++ ) {
                float nx = x*ratiox;
                float v = src->get_bilinear(nx, ny);
                switch( dtype ) {
                case TYPE_FLOAT: dst->set(x, y, v); break;
                case TYPE_UCHAR: {
                    uchar uv = cast_to_gray_range( v );
                    dst->set(x, y, uv);
                } break;
                default: switch_fatality();
                }
            }
        }
    }

    void image_resize_coarse( const Image* src, const int& nw, const int& nh, Image* dst ) {
        passert_pointer( src && dst );
        switch( src->ch() ) {
        case 1: image_resize_coarse_g  ( src, nw, nh, dst ); break;
        case 3: image_resize_coarse_rgb( src, nw, nh, dst ); break;
        default: switch_fatality();
        }
    }


    void image_to_gradient(const float* im, int w, int h, float* dx, float* dy) {
        assert_pointer( im && dx && dy );
        passert_statement_g( is_positive_number(w), "[w %d] should be positive", w );
        passert_statement_g( is_positive_number(h), "[h %d] should be positive", h );

        // x=1:w-1; y=1:h-1
        for( int y=1; y<h-1; y++ ) {
            const float* imy  = im + y*w;
            const float* imyn = im + (y+1)*w;
            const float* imyp = im + (y-1)*w;
            float* dxr = dx + y*w;
            float* dyr = dy + y*w;
            for( int x=1; x<w-1; x++ ) {
                dxr[x] = imy [x+1] - imy [x-1];
                dyr[x] = imyp[x  ] - imyn[x  ];
            }
        }

        // x=0; y=1:h-1
        for( int y=1; y<h-1; y++ ) {
            dx[ y*w ] = 2.0 * ( im[y*w+1]-im[y*w] );
            dy[ y*w ] = im[ (y-1)*w ] - im[ (y+1)*w ];
        }

        // x=w-1; y=1:h-1
        for( int y=1; y<h-1; y++ ) {
            dx[ y*w+w-1 ] = 2.0 * ( im[y*w+w-1]-im[y*w+w-2] );
            dy[ y*w+w-1 ] =       ( im[(y-1)*w+w-1]-im[(y+1)*w+w-1] );
        }

        // x=1:w-1; y=0
        for( int x=1; x<w-1; x++ ) {
            dx[x] = ( im[x+1] - im[x-1] );
            dy[x] = 2.0 * ( im[x] - im[w+x] );
        }

        // x=1:w-1; y=h-1
        for( int x=1; x<w-1; x++ ) {
            dx[ (h-1)*w + x ] = im[ x+1 ] - im[ x-1 ];
            dy[ (h-1)*w + x ] = 2.0 * ( im[ (h-2)*w + x ] - im[ (h-1)*w + x ] );
        }

        // x=0 y=0
        dx[0] = 2.0 * ( im[1] - im[0] );
        dy[0] = 2.0 * ( im[0] - im[w] );

        // x=0 y=h-1
        dx[ (h-1)*w ] = 2.0 * ( im[ (h-1)*w+1 ] - im[ (h-1)*w ] );
        dy[ (h-1)*w ] = 2.0 * ( im[ (h-2)*w   ] - im[ (h-1)*w ] );

        // x=w-1 y=0
        dx[ w-1 ] = 2.0 * ( im[ w-1 ] - im[   w-2 ] );
        dx[ w-1 ] = 2.0 * ( im[ w-1 ] - im[ 2*w-1 ] );

        // x=w-1 y=h-1
        dx[ (h-1)*w + w-1 ] = 2.0 * ( im[ (h-1)*w + w-1 ] - im[ (h-1)*w + w-2 ] );
        dy[ (h-1)*w + w-1 ] = 2.0 * ( im[ (h-2)*w + w-1 ] - im[ (h-1)*w + w-1 ] );
    }

    void image_scale( const Image* im, const float& scale, const bool& run_parallel, Image* out ) {
        passert_pointer( im && out );
        im->passert_type( IT_F_GRAY );
        int w = im->w();
        int h = im->h();
        passert_statement( out->w() == w && out->h() == h, "image dimension mismatch" );

        switch( run_parallel ) {
        case true:
#pragma omp parallel for
            for( int y=0; y<h; y++ ) {
                const float* srow =  im->get_row_f(y);
                float*       drow = out->get_row_f(y);
                for( int x=0; x<w; x++ ) {
                    drow[x] = srow[x] * scale;
                }
            }
            break;
        case false:
            for( int y=0; y<h; y++ ) {
                const float* srow =  im->get_row_f(y);
                float*       drow = out->get_row_f(y);
                for( int x=0; x<w; x++ ) {
                    drow[x] = srow[x] * scale;
                }
            }
            break;
        }
    }

    void image_subtract( const Image* im0, const Image* im1, Image* out ) {
        passert_pointer( im0 && im1 && out );
        im0->passert_type( IT_F_GRAY );
        int w = im0->w();
        int h = im0->h();
        passert_statement( (im1->w() == w) && (im1->h()==h), "dimension mismatch" );
        passert_statement( im0->type() == im1->type(), "image type mismatch" );

        out->create(w,h,im0->type());
        for( int y=0; y<h; y++ ) {
            const float*  row0 = im0->get_row_f(y);
            const float*  row1 = im1->get_row_f(y);
            float      * drow  = out->get_row_f(y);
            for( int x=0; x<w; x++ ) {
                drow[x] = row0[x]-row1[x];
            }
        }
    }

    void image_subtract_par( const Image* im0, const Image* im1, Image* out ) {
        passert_pointer( im0 && im1 && out );
        im0->passert_type( IT_F_GRAY );
        int w = im0->w();
        int h = im0->h();
        passert_statement( (im1->w() == w) && (im1->h()==h), "dimension mismatch" );
        passert_statement( im0->type() == im1->type(), "image type mismatch" );

        out->create(w,h,im0->type());
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const float*  row0 = im0->get_row_f(y);
            const float*  row1 = im1->get_row_f(y);
            float      * drow  = out->get_row_f(y);
            for( int x=0; x<w; x++ ) {
                drow[x] = row0[x]-row1[x];
            }
        }
    }



}
