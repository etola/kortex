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

    bool image_min_max( const Image& img,
                        const int& xmin, const int& ymin,
                        const int& xmax, const int& ymax,
                        float& min_v, float& max_v ) {
        img.passert_type( IT_F_GRAY | IT_U_GRAY );

        int xs, xe, ys, ye;
        if( xmin == xmax && ymin == ymax && xmin == xmax && xmin == -1 ) {
            xs = 0;
            ys = 0;
            xe = img.w();
            ye = img.h();
        } else {
            xs = std::max( std::min( xmin, xmax ), 0        );
            xe = std::min( std::max( xmin, xmax ), img.w() );
            ys = std::max( std::min( ymin, ymax ), 0        );
            ye = std::min( std::max( ymin, ymax ), img.h() );
        }

        min_v =  std::numeric_limits<float>::max();
        max_v = -std::numeric_limits<float>::max();

        switch( img.type() ) {
        case IT_F_GRAY:
            for( int y=ys; y<ye; y++ ) {
                const float* row = img.get_row_f(y);
                for( int x=xs; x<xe; x++ ) {
                    const float& v = row[x];
                    if( is_a_number(v) ) {
                        min_v = std::min(v,min_v);
                        max_v = std::max(v,max_v);
                    }
                }
            }
            break;
        case IT_U_GRAY:
            for( int y=ys; y<ye; y++ ) {
                const uchar* row = img.get_row_u(y);
                for( int x=xs; x<xe; x++ ) {
                    float v = static_cast<float>(row[x]);
                    if( is_a_number(v) ) {
                        min_v = std::min(v,min_v);
                        max_v = std::max(v,max_v);
                    }
                }
            }
            break;
        default: switch_fatality();
        }

        if( (min_v ==  std::numeric_limits<float>::max())  ||
            (max_v == -std::numeric_limits<float>::max()) ) {
            return false;
        }
        return true;
    }

    bool abs_image_min_max( const Image& img,
                            const int& xmin, const int& ymin,
                            const int& xmax, const int& ymax,
                            float& min_v, float& max_v ) {
        img.passert_type( IT_F_GRAY );

        int xs, xe, ys, ye;
        if( xmin == xmax && ymin == ymax && xmin == xmax && xmin == -1 ) {
            xs = 0;
            ys = 0;
            xe = img.w();
            ye = img.h();
        } else {
            xs = std::max( std::min( xmin, xmax ), 0        );
            xe = std::min( std::max( xmin, xmax ), img.w() );
            ys = std::max( std::min( ymin, ymax ), 0        );
            ye = std::min( std::max( ymin, ymax ), img.h() );
        }

        min_v =  std::numeric_limits<float>::max();
        max_v = -std::numeric_limits<float>::max();

        for( int y=ys; y<ye; y++ ) {
            const float* row = img.get_row_f(y);
            for( int x=xs; x<xe; x++ ) {
                const float& v = fabs(row[x]);
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


    // allows img out to be point to the same mem location -> therefore passerts
    // that out image is mem-allocated.
    void filter_hv( const Image& img, const float* kernel, const int& ksz, Image& out ) {
        assert_pointer( kernel );
        assert_pointer_size( ksz );
        assert_statement( !img.is_empty(), "image is empty" );
        passert_statement( out.type() == img.type(), "image types not agree" );
        passert_statement( check_dimensions(img, out), "dimension mismatch" );
        img.passert_type( IT_F_GRAY | IT_F_IRGB );

        switch( img.type() ) {
        case IT_F_GRAY:
            filter_hv( img.get_row_f(0), img.w(), img.h(), kernel, ksz, out.get_row_f(0) );
            break;
        case IT_F_IRGB: {
            for( int c=0; c<3; c++ ) {
                const Image* sch = img.get_channel_wrapper( c );
                Image      * dch = out.get_channel_wrapper( c );
                filter_hv( *sch, kernel, ksz, *dch );
                delete sch;
                delete dch;
            }
        } break;
        default: switch_fatality();
        }
    }

    // allows img out to be point to the same mem location -> therefore passerts
    // that out image is mem-allocated.
    void filter_hor( const Image& img, const float* kernel, const int& ksz, Image& out ) {
        assert_pointer( kernel );
        assert_pointer_size( ksz );
        assert_statement( !img.is_empty(), "empty image" );
        passert_statement( out.type() == img.type(), "image types not agree" );
        passert_statement( check_dimensions(img, out), "dimension mismatch" );
        img.passert_type( IT_F_GRAY | IT_F_IRGB );

        switch( img.type() ) {
        case IT_F_GRAY:
            filter_hor( img.get_row_f(0), img.w(), img.h(), kernel, ksz, out.get_row_f(0) );
            break;
        case IT_F_IRGB: {
            for( int c=0; c<3; c++ ) {
                const Image* sch = img.get_channel_wrapper( c );
                Image      * dch = out.get_channel_wrapper( c );
                filter_hor( *sch, kernel, ksz, *dch );
                delete sch;
                delete dch;
            }
        } break;
        default: switch_fatality();
        }
    }


    // allows img out to be point to the same mem location -> therefore passerts
    // that out image is mem-allocated.
    void filter_hor_par( const Image& img, const float* kernel, const int& ksz, Image& out ) {
        assert_pointer( kernel );
        assert_pointer_size( ksz );
        assert_statement( !img.is_empty(), "empty image" );
        passert_statement( out.type() == img.type(), "image types not agree" );
        passert_statement( check_dimensions(img, out), "dimension mismatch" );
        img.passert_type( IT_F_GRAY | IT_F_IRGB );

        switch( img.type() ) {
        case IT_F_GRAY:
            filter_hor_par( img.get_row_f(0), img.w(), img.h(), kernel, ksz, out.get_row_f(0) );
            break;
        case IT_F_IRGB: {
            for( int c=0; c<3; c++ ) {
                const Image* sch = img.get_channel_wrapper( c );
                Image      * dch = out.get_channel_wrapper( c );
                filter_hor_par( *sch, kernel, ksz, *dch );
                delete sch;
                delete dch;
            }
        } break;
        default: switch_fatality();
        }
    }

    // allows img out to be point to the same mem location -> therefore passerts
    // that out image is mem-allocated.
    void filter_ver( const Image& img, const float* kernel, const int& ksz, Image& out ) {
        assert_pointer( kernel );
        assert_pointer_size( ksz );
        assert_statement( !img.is_empty(), "empty image" );
        passert_statement( out.type() == img.type(), "image types not agree" );
        passert_statement( check_dimensions(img, out), "dimension mismatch" );
        img.passert_type( IT_F_GRAY | IT_F_IRGB );

        switch( img.type() ) {
        case IT_F_GRAY:
            filter_ver( img.get_row_f(0), img.w(), img.h(), kernel, ksz, out.get_row_f(0) );
            break;
        case IT_F_IRGB: {
            for( int c=0; c<3; c++ ) {
                const Image* sch = img.get_channel_wrapper( c );
                Image      * dch = out.get_channel_wrapper( c );
                filter_ver( *sch, kernel, ksz, *dch );
                delete sch;
                delete dch;
            }
        } break;
        default: switch_fatality();
        }
    }

    // allows img out to be point to the same mem location -> therefore passerts
    // that out image is mem-allocated.
    void filter_ver_par( const Image& img, const float* kernel, const int& ksz, Image& out ) {
        assert_pointer( kernel );
        assert_pointer_size( ksz );
        assert_statement( !img.is_empty(), "empty image" );
        passert_statement( out.type() == img.type(), "image types not agree" );
        passert_statement( check_dimensions(img, out), "dimension mismatch" );
        img.passert_type( IT_F_GRAY | IT_F_IRGB );

        switch( img.type() ) {
        case IT_F_GRAY:
            filter_ver_par( img.get_row_f(0), img.w(), img.h(), kernel, ksz, out.get_row_f(0) );
            break;
        case IT_F_IRGB: {
            for( int c=0; c<3; c++ ) {
                const Image* sch = img.get_channel_wrapper( c );
                Image      * dch = out.get_channel_wrapper( c );
                filter_ver_par( *sch, kernel, ksz, *dch );
                delete sch;
                delete dch;
            }
        } break;
        default: switch_fatality();
        }
    }



    // allows img out to be point to the same mem location -> therefore passerts
    // that out image is mem-allocated.
    void filter_hv_par( const Image& img, const float* kernel, const int& ksz, Image& out ) {
        assert_pointer( kernel );
        assert_pointer_size( ksz );
        assert_statement( !img.is_empty(), "image is empty" );
        passert_statement( out.type() == img.type(), "image types not agree" );
        passert_statement( check_dimensions(img, out), "dimension mismatch" );
        img.passert_type( IT_F_GRAY | IT_F_IRGB ); // supporting these types
                                                    // for now
        switch( img.type() ) {
        case IT_F_GRAY:
            filter_hv_par( img.get_row_f(0), img.w(), img.h(), kernel, ksz, out.get_row_f(0) );
            break;
        case IT_F_IRGB: {
            for( int c=0; c<3; c++ ) {
                const Image* sch = img.get_channel_wrapper( c );
                Image      * dch = out.get_channel_wrapper( c );
                filter_hv_par( *sch, kernel, ksz, *dch );
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

    // allows img out to be point to the same mem location -> therefore passerts
    // that out image is mem-allocated.
    void filter_gaussian( const Image& img, const float& sigma, Image& out ) {
        assert_statement( !img.is_empty(), "image is empty" );
        passert_statement( check_dimensions(img, out), "dimension mismatch" );
        passert_statement( out.type() == img.type(), "image types not agree" );
        int sz = filter_size(sigma);
        float* sfilter = NULL;
        allocate(sfilter, sz);
        gaussian_1d( sfilter, sz, 0, sigma );
        filter_hv( img, sfilter, sz, out );
        deallocate( sfilter );
    }

    // allows img out to be point to the same mem location -> therefore passerts
    // that out image is mem-allocated.
    void filter_gaussian_par( const Image& img, const float& sigma, Image& out ) {
        assert_statement( !img.is_empty(), "image is empty" );
        passert_statement( check_dimensions(img, out), "dimension mismatch" );
        passert_statement( out.type() == img.type(), "image types not agree" );
        int sz = filter_size(sigma);
        float* sfilter = NULL;
        allocate(sfilter, sz);
        gaussian_1d( sfilter, sz, 0, sigma );
        filter_hv_par( img, sfilter, sz, out );
        deallocate( sfilter );
    }

    void combine_horizontally(const Image& im0, const Image& im1, Image& out) {
        passert_statement( im0.precision() == im1.precision(), "image precisions are different" );
        passert_statement( im0.ch() == im1.ch(), "image channels different" );

        int w0 = im0.w();
        int h0 = im0.h();
        int w1 = im1.w();
        int h1 = im1.h();
        int w = w0+w1;
        int h = std::max( h0, h1 );

        out.create(w, h, im0.type());
        out.zero();
        out.copy_from_region( &im0, 0, 0, w0, h0, 0,  0 );
        out.copy_from_region( &im1, 0, 0, w1, h1, w0, 0 );
    }

    void combine_vertically(const Image& im0, const Image& im1, Image& out) {
        passert_statement( im0.precision() == im1.precision(), "image precisions are different" );
        passert_statement( im0.ch() == im1.ch(), "image channels different" );

        int w0 = im0.w();
        int h0 = im0.h();
        int w1 = im1.w();
        int h1 = im1.h();
        int w = std::max(w0, w1);
        int h = h0+h1;

        out.create(w, h, im0.type());
        out.zero();
        out.copy_from_region( &im0, 0, 0, w0, h0, 0, 0  );
        out.copy_from_region( &im1, 0, 0, w1, h1, 0, h0 );
    }

    void flip_image_ver( Image& img ) {

        img.passert_type( IT_F_GRAY | IT_U_GRAY );
        int w = img.w();
        int h = img.h();

        switch( img.precision() ) {
        case TYPE_FLOAT: {
            for( int y=0; y<h/2; y++ ) {
                float* urow = img.get_row_f(y);
                float* drow = img.get_row_f(h-y-1);
                for( int x=0; x<w; x++ ) {
                    std::swap( urow[x], drow[x] );
                }
            }
        } break;
        case TYPE_UCHAR: {
            for( int y=0; y<h/2; y++ ) {
                uchar* urow = img.get_row_u(y);
                uchar* drow = img.get_row_u(h-y-1);
                for( int x=0; x<w; x++ ) {
                    std::swap( urow[x], drow[x] );
                }
            }
        } break;
        default: switch_fatality();
        }
    }

    void flip_image_hor( Image& img ) {
        img.passert_type( IT_F_GRAY );
        int w = img.w();
        int h = img.h();

        switch( img.precision() ) {
        case TYPE_FLOAT: {
            for( int y=0; y<h; y++ ) {
                float* row = img.get_row_f(y);
                for( int x=0; x<w/2; x++ )
                    std::swap( row[x], row[w-x-1] );
            }
        } break;
        case TYPE_UCHAR : {
            for( int y=0; y<h; y++ ) {
                uchar* row = img.get_row_u(y);
                for( int x=0; x<w/2; x++ )
                    std::swap( row[x], row[w-x-1] );
            }
        } break;
        default: switch_fatality();
        }
    }

    void image_color_invert( Image& img ) {
        img.passert_type( IT_U_GRAY );
        int w = img.w();
        int h = img.h();
        for( int y=0; y<h; y++ ) {
            uchar* row = img.get_row_u(y);
            for( int x=0; x<w; x++ ) {
                row[x] = 255-row[x];
            }
        }
    }

    void image_threshold( const Image& img, float th, Image& msk ) {
        passert_statement( check_dimensions(img, msk), "dimension mismatch" );
        img.passert_type( IT_F_GRAY );
        msk.passert_type( IT_F_GRAY );
        int pc = img.pixel_count();
        const float* irow = img.get_row_f(0);
        float      * orow = msk.get_row_f(0);
        for( int x=0; x<pc; x++ ) {
            if( irow[x] > th ) orow[x] = 1.0f;
            else               orow[x] = 0.0f;
        }
    }

    void image_resize_coarse_rgb( const Image& src, const int& nw, const int& nh, Image& dst ) {

        passert_statement( nw > 0 && nh > 0, "invalid new image size" );
        src.passert_type( IT_U_PRGB | IT_U_IRGB | IT_F_IRGB | IT_F_PRGB );

        dst.create( nw, nh, src.type() );
        dst.zero();

        float ratioy = src.h() / (float)nh;
        float ratiox = src.w() / (float)nw;

        DataType dtype = image_precision( src.type() );

        float r, g, b;
        for( int y=0; y<nh; y++ ) {
            float ny = y*ratioy;
            if( ny >= src.h()-1 ) ny = src.h()-1;
            for( int x=0; x<nw; x++ ) {
                float nx = x*ratiox;
                if( nx >= src.w()-1 ) nx = src.w()-1;
                src.get_bilinear(nx, ny, r, g, b);
                switch( dtype ) {
                case TYPE_FLOAT: dst.set(x, y, r, g, b); break;
                case TYPE_UCHAR: {
                    uchar ur = cast_to_gray_range( r );
                    uchar ug = cast_to_gray_range( g );
                    uchar ub = cast_to_gray_range( b );
                    dst.set(x, y, ur, ug, ub);
                } break;
                default: switch_fatality();
                }
            }
        }
    }

    void image_resize_coarse_g( const Image& src, const int& nw, const int& nh, Image& dst ) {
        passert_statement( nw > 0 && nh > 0, "invalid new image size" );
        src.passert_type( IT_U_GRAY | IT_F_GRAY );

        dst.create( nw, nh, src.type() );
        dst.zero();

        float ratioy = src.h() / (float)nh;
        float ratiox = src.w() / (float)nw;

        DataType dtype = image_precision( src.type() );

        for( int y=0; y<nh; y++ ) {
            float ny = y*ratioy;
            if( ny >= src.h()-1 ) ny = src.h()-1;
            for( int x=0; x<nw; x++ ) {
                float nx = x*ratiox;
                if( nx >= src.w()-1 ) nx = src.w()-1;
                float v = src.get_bilinear(nx, ny);
                switch( dtype ) {
                case TYPE_FLOAT: dst.set(x, y, v); break;
                case TYPE_UCHAR: {
                    uchar uv = cast_to_gray_range( v );
                    dst.set(x, y, uv);
                } break;
                default: switch_fatality();
                }
            }
        }
    }

    void image_resize_coarse( const Image& src, const int& nw, const int& nh, Image& dst ) {
        switch( src.ch() ) {
        case 1: image_resize_coarse_g  ( src, nw, nh, dst ); break;
        case 3: image_resize_coarse_rgb( src, nw, nh, dst ); break;
        default: switch_fatality();
        }
    }

    void image_resize_fine_g( const Image& src, const int& nw, const int& nh, Image& dst ) {
        passert_statement( nw > 0 && nh > 0, "invalid new image size" );
        src.passert_type( IT_U_GRAY | IT_F_GRAY );

        dst.create( nw, nh, src.type() );
        dst.zero();

        float ratioy = src.h() / (float)nh;
        float ratiox = src.w() / (float)nw;

        DataType dtype = image_precision( src.type() );

        for( int y=0; y<nh; y++ ) {
            float ny = y*ratioy;
            if( ny >= src.h()-1 ) ny = src.h()-1;
            for( int x=0; x<nw; x++ ) {
                float nx = x*ratiox;
                if( nx >= src.w()-1 ) nx = src.w()-1;
                if( !src.is_inside_margin(nx, ny, 2) ) continue;
                float v = src.get_bicubic(nx, ny);
                switch( dtype ) {
                case TYPE_FLOAT: dst.set(x, y, v); break;
                case TYPE_UCHAR: {
                    uchar uv = cast_to_gray_range( v );
                    dst.set(x, y, uv);
                } break;
                default: switch_fatality();
                }
            }
        }
    }

    void image_resize_fine_rgb( const Image& src, const int& nw, const int& nh, Image& dst ) {
        passert_statement( nw > 0 && nh > 0, "invalid new image size" );
        src.passert_type( IT_U_PRGB | IT_U_IRGB | IT_F_IRGB | IT_F_PRGB );

        dst.create( nw, nh, src.type() );
        dst.zero();

        float ratioy = src.h() / (float)nh;
        float ratiox = src.w() / (float)nw;

        DataType dtype = image_precision( src.type() );

        float r, g, b;
        for( int y=0; y<nh; y++ ) {
            float ny = y*ratioy;
            for( int x=0; x<nw; x++ ) {
                float nx = x*ratiox;
                if( !src.is_inside_margin(nx, ny, 2) ) continue;
                src.get_bicubic(nx, ny, r, g, b);
                switch( dtype ) {
                case TYPE_FLOAT: dst.set(x, y, r, g, b); break;
                case TYPE_UCHAR: {
                    uchar ur = cast_to_gray_range( r );
                    uchar ug = cast_to_gray_range( g );
                    uchar ub = cast_to_gray_range( b );
                    dst.set(x, y, ur, ug, ub);
                } break;
                default: switch_fatality();
                }
            }
        }
    }


    void image_resize_fine( const Image& src, const int& nw, const int& nh, Image& dst ) {
        switch( src.ch() ) {
        case 1: image_resize_fine_g  ( src, nw, nh, dst ); break;
        case 3: image_resize_fine_rgb( src, nw, nh, dst ); break;
        default: switch_fatality();
        }
    }

    void resize_image_coarse( const Image& img, int max_img_dim, Image& rimg ) {
        int nw = img.w();
        int nh = img.h();
        double scr = 1.0;
        if( max_img_dim != 0 && ( std::max(nh,nw) != max_img_dim ) ) {
            if( nw >= nh ) scr = img.w() / double(max_img_dim);
            else           scr = img.h() / double(max_img_dim);
            nw  = img.w() / scr;
            nh  = img.h() / scr;
        }
        image_resize_coarse( img, nw, nh, rimg );
    }

    void resize_image_fine  ( const Image& img, int max_img_dim, Image& rimg ) {
        int nw = img.w();
        int nh = img.h();
        double scr = 1.0;
        if( max_img_dim != 0 && ( std::max(nh,nw) != max_img_dim ) ) {
            if( nw >= nh ) scr = img.w() / double(max_img_dim);
            else           scr = img.h() / double(max_img_dim);
            nw  = img.w() / scr;
            nh  = img.h() / scr;
        }
        image_resize_fine( img, nw, nh, rimg );
    }


    void image_to_gradient( const Image& img, Image& dx, Image& dy ) {
        img.passert_type( IT_F_GRAY );
        int w = img.w();
        int h = img.h();
        dx.create( w, h, IT_F_GRAY );
        dy.create( w, h, IT_F_GRAY );
        image_to_gradient( img.get_row_f(0), w, h, dx.get_row_f(0), dy.get_row_f(0) );
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

    void image_subtract( const Image& im0, const Image& im1, Image& out ) {
        passert_statement( check_dimensions(im0,im1), "dimension mismatch" );
        passert_statement( check_dimensions(im0,out), "dimension mismatch" );
        im0.passert_type( IT_F_GRAY );
        im1.passert_type( IT_F_GRAY );
        out.passert_type( IT_F_GRAY );

        int w = im0.w();
        int h = im0.h();
        for( int y=0; y<h; y++ ) {
            const float* row0 = im0.get_row_f(y);
            const float* row1 = im1.get_row_f(y);
            float      * drow = out.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                drow[x] = row0[x]-row1[x];
            }
        }
    }

    void image_subtract_par( const Image& im0, const Image& im1, Image& out ) {
        passert_statement( check_dimensions(im0,im1), "dimension mismatch" );
        passert_statement( check_dimensions(im0,out), "dimension mismatch" );
        im0.passert_type( IT_F_GRAY );
        im1.passert_type( IT_F_GRAY );
        out.passert_type( IT_F_GRAY );

        int w = im0.w();
        int h = im0.h();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const float* row0 = im0.get_row_f(y);
            const float* row1 = im1.get_row_f(y);
            float      * drow = out.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                drow[x] = row0[x]-row1[x];
            }
        }
    }


    void image_add( const Image& im0, const Image& im1, Image& out ) {
        passert_statement( check_dimensions(im0,im1), "dimension mismatch" );
        passert_statement( check_dimensions(im0,out), "dimension mismatch" );
        im0.passert_type( IT_F_GRAY );
        im1.passert_type( IT_F_GRAY );
        out.passert_type( IT_F_GRAY );

        int w = im0.w();
        int h = im0.h();
        for( int y=0; y<h; y++ ) {
            const float* row0 = im0.get_row_f(y);
            const float* row1 = im1.get_row_f(y);
            float      * drow = out.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                drow[x] = row0[x]+row1[x];
            }
        }
    }

    void image_add_par( const Image& im0, const Image& im1, Image& out ) {
        passert_statement( check_dimensions(im0,im1), "dimension mismatch" );
        passert_statement( check_dimensions(im0,out), "dimension mismatch" );
        im0.passert_type( IT_F_GRAY );
        im1.passert_type( IT_F_GRAY );
        out.passert_type( IT_F_GRAY );

        int w = im0.w();
        int h = im0.h();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const float* row0 = im0.get_row_f(y);
            const float* row1 = im1.get_row_f(y);
            float      * drow = out.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                drow[x] = row0[x]+row1[x];
            }
        }
    }


    /// r = p/q for q(i,j) > 1e-6
    void image_divide( const Image& p, const Image& q, Image& r ) {
        assert_statement( check_dimensions(p,q), "dimension mismatch" );
        assert_statement( check_dimensions(p,r), "dimension mismatch" );
        p.assert_type( IT_F_GRAY );
        q.assert_type( IT_F_GRAY );
        r.assert_type( IT_F_GRAY );
        int h = p.h();
        int w = p.w();
        for( int y=0; y<h; y++ ) {
            const float* prow = p.get_row_f(y);
            const float* qrow = q.get_row_f(y);
            float      * rrow = r.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                if( fabs(qrow[x]) < 1e-6 ) {
                    rrow[x] = 0.0f;
                } else {
                    rrow[x] = prow[x]/qrow[x];
                }
            }
        }
    }

    /// r = p/q for q(i,j) > 1e-6
    void image_divide_par( const Image& p, const Image& q, Image& r ) {
        assert_statement( check_dimensions(p,q), "dimension mismatch" );
        assert_statement( check_dimensions(p,r), "dimension mismatch" );
        p.assert_type( IT_F_GRAY );
        q.assert_type( IT_F_GRAY );
        r.assert_type( IT_F_GRAY );
        int h = p.h();
        int w = p.w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const float* prow = p.get_row_f(y);
            const float* qrow = q.get_row_f(y);
            float      * rrow = r.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                if( fabs(qrow[x]) < 1e-6 ) {
                    rrow[x] = 0.0f;
                } else {
                    rrow[x] = prow[x]/qrow[x];
                }
            }
        }
    }

    // r = p*q
    void image_multiply    ( const Image& p, const Image& q, Image& r ) {
        assert_statement( check_dimensions(p,q), "dimension mismatch" );
        assert_statement( check_dimensions(p,r), "dimension mismatch" );
        p.assert_type( IT_F_GRAY );
        q.assert_type( IT_F_GRAY );
        r.assert_type( IT_F_GRAY );
        int h = p.h();
        int w = p.w();
        for( int y=0; y<h; y++ ) {
            const float* prow = p.get_row_f(y);
            const float* qrow = q.get_row_f(y);
            float      * rrow = r.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                rrow[x] = prow[x]*qrow[x];
            }
        }
    }

    // r = p*q
    void image_multiply_par( const Image& p, const Image& q, Image& r ) {
        assert_statement( check_dimensions(p,q), "dimension mismatch" );
        assert_statement( check_dimensions(p,r), "dimension mismatch" );
        p.assert_type( IT_F_GRAY );
        q.assert_type( IT_F_GRAY );
        r.assert_type( IT_F_GRAY );
        int h = p.h();
        int w = p.w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const float* prow = p.get_row_f(y);
            const float* qrow = q.get_row_f(y);
            float      * rrow = r.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                rrow[x] = prow[x]*qrow[x];
            }
        }
    }

    /// r = o + p*q
    void image_multiply_add( const Image& p, const Image& q, const Image& r, Image& o ) {
        assert_statement( check_dimensions(p,q), "dimension mismatch" );
        assert_statement( check_dimensions(p,r), "dimension mismatch" );
        assert_statement( check_dimensions(p,o), "dimension mismatch" );
        p.assert_type( IT_F_GRAY );
        q.assert_type( IT_F_GRAY );
        r.assert_type( IT_F_GRAY );
        o.assert_type( IT_F_GRAY );
        int h = p.h();
        int w = p.w();
        for( int y=0; y<h; y++ ) {
            const float* prow = p.get_row_f(y);
            const float* qrow = q.get_row_f(y);
            const float* rrow = r.get_row_f(y);
            float      * orow = o.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                orow[x] = rrow[x] + prow[x]*qrow[x];
            }
        }
    }


    /// r = o + p*q
    void image_multiply_add_par( const Image& p, const Image& q, const Image& r, Image& o ) {
        assert_statement( check_dimensions(p,q), "dimension mismatch" );
        assert_statement( check_dimensions(p,r), "dimension mismatch" );
        assert_statement( check_dimensions(p,o), "dimension mismatch" );
        p.assert_type( IT_F_GRAY );
        q.assert_type( IT_F_GRAY );
        r.assert_type( IT_F_GRAY );
        o.assert_type( IT_F_GRAY );
        int h = p.h();
        int w = p.w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const float* prow = p.get_row_f(y);
            const float* qrow = q.get_row_f(y);
            const float* rrow = r.get_row_f(y);
            float      * orow = o.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                orow[x] = rrow[x] + prow[x]*qrow[x];
            }
        }
    }

    /// q = s * p
    void image_scale( const Image& p, float s, Image& q ) {
        assert_statement( check_dimensions(p,q), "dimension mismatch" );
        p.assert_type( IT_F_GRAY );
        q.assert_type( IT_F_GRAY );
        int h = p.h();
        int w = p.w();
        for( int y=0; y<h; y++ ) {
            const float* prow = p.get_row_f(y);
            float      * qrow = q.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                qrow[x] = s * prow[x];
            }
        }
    }

    /// q = s * p
    void image_scale_par( const Image& p, float s, Image& q ) {
        assert_statement( check_dimensions(p,q), "dimension mismatch" );
        p.assert_type( IT_F_GRAY );
        q.assert_type( IT_F_GRAY );
        int h = p.h();
        int w = p.w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const float* prow = p.get_row_f(y);
            float      * qrow = q.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                qrow[x] = s * prow[x];
            }
        }
    }


    /// checks whether p has values of either 0.0f or 1.0f
    bool is_binarized( const Image& p ) {
        assert_statement( !p.is_empty(), "passed empty image" );
        p.assert_type( IT_F_GRAY );
        int sz = p.pixel_count();
        const float* prow = p.get_row_f(0);
        for( int i=0; i<sz; i++ ) {
            if( prow[i] == 0.0f ) continue;
            if( prow[i] == 1.0f ) continue;
            return false;
        }
        return true;
    }

    /// checks whether p has values in the range [0.0f 1.0f]
    bool is_normalized( const Image& p ) {
        assert_statement( !p.is_empty(), "passed empty image" );
        p.assert_type( IT_F_GRAY );
        int sz = p.pixel_count();
        const float* prow = p.get_row_f(0);
        for( int i=0; i<sz; i++ ) {
            if( prow[i] < 0.0f ) return false;
            if( prow[i] > 1.0f ) return false;
        }
        return true;
    }


    /// checks whether p and q are non-zero for the same pixel
    bool does_overlap( const Image& p, const Image& q ) {
        assert_statement( check_dimensions(p,q), "dimension mismatch" );
        p.assert_type( IT_F_GRAY );
        q.assert_type( IT_F_GRAY );
        int w = p.w();
        int h = p.h();
        for( int y=0; y<h; y++ ) {
            const float* prow = p.get_row_f(y);
            const float* qrow = q.get_row_f(y);
            for( int x=0; x<w; x++ ) {
                if( (prow[x] != 0.0f)  && ( qrow[x] != 0.0f ) ) {
                    return true;
                }
            }
        }
        return false;
    }


    void init_gaussian_weight_mask( Image& mask ) {
        assert_statement( !mask.is_empty(), "passed empty mask" );
        mask.assert_type( IT_F_GRAY );

        int w = mask.w();
        int h = mask.h();
        int d = std::max(w,h)/2+1;
        vector<float> exp_vals(d, 0.0f);
        float sigma = d / 2.0f;
        float v = sqrt(254.0f); // 1.0f / ( sqrt(PI2) * sigma );
        for( int i=0; i<d; i++ ) {
            exp_vals[i] = v * exp( -0.5f * i*i / sigma / sigma );
        }

#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            float* mrow = mask.get_row_f(y);
            int yd = fabs(y-h/2);
            assert_statement_g( yd>=0 && yd < d, "[oob %d %d]", yd, d );
            float ey = exp_vals[ yd ];
            for( int x=0; x<w; x++ ) {
                int xd = fabs(x-w/2);
                assert_statement_g( xd>=0 && xd < d, "[oob %d %d]", xd, d );
                mrow[x] = ey * exp_vals[ xd ] + 1.0f;
            }
        }
    }

    void init_linear_weight_mask( Image& mask ) {
        assert_statement( !mask.is_empty(), "passed empty mask" );
        mask.assert_type( IT_F_GRAY );
        int w = mask.w();
        int h = mask.h();

#pragma omp parallel for
        for(int y=0; y<h; y++) {
            float* mrow = mask.get_row_f(y);
            float y_w = 1.0 - fabs(2.0f*float(y)/(h-1.0f)-1.0f);
            for( int x=0; x<w; x++ ) {
                float x_w = 1.0 - fabs(2.0f*float(x)/(w-1.0f)-1.0f);
                mrow[x] = 254.0f * (y_w*x_w) + 1.0f;
            }
        }
    }

    ///
    /// assumes image_threshold is called first over mask.
    ///
    /// er_size : how many pixels to erode
    void erode_mask( Image& mask, int er_size ) {
        mask.assert_type( IT_F_GRAY );
        assert_statement( !mask.is_empty(), "passed empty image" );
        assert_statement( is_binarized( mask ), "passed image is not binarized" );

        // how much pixels to erode
        float kernel[255];
        int ksz = 2*er_size+1;
        assert_statement( ksz <= 255, "buffer overflow" );
        for(int i=0; i<ksz; i++)
            kernel[i] = 1.0f;
        filter_hv( mask, kernel, ksz, mask );
        image_threshold( mask, ksz*ksz-1 );
    }


    /// maps the src image to 0.0 -> 1.0 range linearly. assumes the original
    /// image is of type IT_F_GRAY
    void image_linearize( const Image& src, Image& dst ) {
        src.assert_type( IT_F_GRAY );
        dst.assert_type( IT_F_GRAY );
        assert_statement( check_dimensions(src,dst), "dimension mismatch" );
        assert_statement( !src.is_empty(), "empty image" );

        float mins, maxs;
        image_min_max( src, 0, 0, src.w(), src.h(), mins, maxs );

        float srange = maxs - mins;
        if( fabs(srange ) < 1e-8 ) {
            logman_warning_g( "image range is too low [%f %f]", mins, maxs );
            dst.zero();
            return;
        }
        float isrange = 1.0f/srange;

        const float* sptr = src.get_row_f(0);
        float      * dptr = dst.get_row_f(0);
        int pc = src.pixel_count();
        for( int i=0; i<pc; i++ ) {
            dptr[i] = ( sptr[i] - mins ) * isrange;
        }
    }

    void image_normalize( const Image& src, Image& dst ) {
        src.assert_type( IT_F_GRAY );
        dst.assert_type( IT_F_GRAY );
        assert_statement( check_dimensions(src,dst), "dimension mismatch" );
        image_scale( src, 1.0f/255.0f, dst );
    }

    void image_unnormalize( const Image& src, Image& dst ) {
        src.assert_type( IT_F_GRAY );
        dst.assert_type( IT_F_GRAY );
        assert_statement( check_dimensions(src,dst), "dimension mismatch" );
        image_scale( src, 255.0f, dst );
    }

    /// computes per pixel image gradient magnitude
    void image_gradient_magnitude( const Image& src, bool run_parallel, Image& mag ) {
        src.assert_type( IT_F_GRAY );
        assert_statement( !src.is_empty(), "empty image" );
        assert_noalias( src, mag );

        mag.create( src.w(), src.h(), IT_F_GRAY );
        mag.zero();

        Image dx, dy;
        image_to_gradient( src, dx, dy );

        const float* xptr = dx.get_row_f(0);
        const float* yptr = dy.get_row_f(0);
        float      * mptr = mag.get_row_f(0);
        int pc = src.pixel_count();
        switch( run_parallel ) {
        case false: {
            for( int i=0; i<pc; i++ ) {
                mptr[i] = sqrt( sq( xptr[i] ) + sq( yptr[i] ) );
            }
        } break;
        case true: {
#pragma omp parallel for
            for( int i=0; i<pc; i++ ) {
                mptr[i] = sqrt( sq( xptr[i] ) + sq( yptr[i] ) );
            }
        } break;
        }
    }

    void image_stretch( const Image& src, float minv, float maxv, Image& out ) {
        src.assert_type( IT_F_GRAY );

        float scale = 1.0f;
        if( minv == 0.0f && maxv == 0.0f ) {
            if( !image_min_max( src, 0, 0, src.w(), src.h(), minv, maxv ) ) {
                logman_error("min max range for the image could not be found");
                minv = 0.0f;
                maxv = 0.0f;
                scale = 1.0f;
            } else {
                scale = 255.0f / ( maxv - minv );
            }
        } else {
            scale = 255.0f / ( maxv - minv );
        }

        out.create( src.w(), src.h(), IT_U_GRAY );
        out.zero();

        for( int y=0; y<src.h(); y++ ) {
            const float* srow = src.get_row_f(y);
            uchar      * orow = out.get_row_u(y);
            for( int x=0; x<src.w(); x++ ) {
                const float& v = srow[x];
                if( is_a_number(v) )
                    orow[x] = cast_to_gray_range( (v-minv)*scale );
            }
        }


    }


}
