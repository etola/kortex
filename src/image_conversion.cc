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

#include <kortex/color.h>
#include <kortex/image_conversion.h>

namespace kortex {

    void urgb_to_gray( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        assert_noalias_p( src, dst );
        src->passert_type( IT_U_PRGB | IT_U_IRGB );
        dst->passert_type( IT_F_GRAY | IT_U_GRAY | IT_I_GRAY );
        passert_statement( src->w() == dst->w(), "image dimensions do not agree" );
        passert_statement( src->h() == dst->h(), "image dimensions do not agree" );

        DataType dprec = dst->precision();
        int w = src->w();
        int h = src->h();
        switch( src->type() ) {
        case IT_U_PRGB:
#pragma omp parallel for
            for( int y=0; y<h; y++ ) {
                const uchar* srow = src->get_row_u(y);
                switch( dprec ) {
                case TYPE_UCHAR : {
                    uchar* drow = dst->get_row_u(y);
                    for( int x=0; x<w; x++ ) {
                        const uchar* srx = srow + 3*x;
                        drow[x] = rgb_to_gray_u(srx[0], srx[1], srx[2] );
                    }
                } break;
                case TYPE_FLOAT : {
                    float* drow = dst->get_row_f(y);
                    for( int x=0; x<w; x++ ) {
                        const uchar* srx = srow + 3*x;
                        drow[x] = rgb_to_gray_f(srx[0], srx[1], srx[2] );
                    }
                } break;
                case TYPE_INT : {
                    int* drow = dst->get_row_i(y);
                    for( int x=0; x<w; x++ ) {
                        const uchar* srx = srow + 3*x;
                        drow[x] = rgb_to_gray_u(srx[0], srx[1], srx[2] );
                    }
                } break;

                default : switch_fatality();
                }
            }
            break;
        case IT_U_IRGB:
#pragma omp parallel for
            for(int y=0; y<h; y++ ) {
                const uchar* sr = src->get_row_ui(y,0);
                const uchar* sg = src->get_row_ui(y,1);
                const uchar* sb = src->get_row_ui(y,2);
                switch( dprec ) {
                case TYPE_UCHAR : {
                    uchar* drow = dst->get_row_u(y);
                    for( int x=0; x<w; x++ )
                        drow[x] = rgb_to_gray_u( sr[x], sg[x], sb[x] );
                } break;
                case TYPE_FLOAT : {
                    float* drow = dst->get_row_f(y);
                    for( int x=0; x<w; x++ )
                        drow[x] = rgb_to_gray_f( sr[x], sg[x], sb[x] );
                } break;
                case TYPE_INT   : {
                    int  * drow = dst->get_row_i(y);
                    for( int x=0; x<w; x++ )
                        drow[x] = rgb_to_gray_u( sr[x], sg[x], sb[x] );
                } break;
                default: switch_fatality();
                }
            } break;
        default: switch_fatality();
        }
    }

    void frgb_to_gray( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        assert_noalias_p( src, dst );
        src->passert_type( IT_F_PRGB | IT_F_IRGB );
        dst->passert_type( IT_F_GRAY | IT_U_GRAY );
        passert_statement( src->w() == dst->w(), "image dimensions do not agree" );
        passert_statement( src->h() == dst->h(), "image dimensions do not agree" );

        DataType dprec = dst->precision();
        int w = src->w();
        int h = src->h();
        switch( src->type() ) {
        case IT_F_PRGB:
#pragma omp parallel for
            for( int y=0; y<h; y++ ) {
                const float* srow = src->get_row_f(y);
                switch( dprec ) {
                case TYPE_UCHAR : {
                    uchar* drow = dst->get_row_u(y);
                    for( int x=0; x<w; x++ ) {
                        const float* srx = srow + 3*x;
                        drow[x] = rgb_to_gray_u(srx[0], srx[1], srx[2] );
                    }
                } break;
                case TYPE_FLOAT : {
                    float* drow = dst->get_row_f(y);
                    for( int x=0; x<w; x++ ) {
                        const float* srx = srow + 3*x;
                        drow[x] = rgb_to_gray_f(srx[0], srx[1], srx[2] );
                    }
                } break;
                case TYPE_INT : {
                    int* drow = dst->get_row_i(y);
                    for( int x=0; x<w; x++ ) {
                        const float* srx = srow + 3*x;
                        drow[x] = rgb_to_gray_u(srx[0], srx[1], srx[2] );
                    }
                } break;

                default : switch_fatality();
                }
            }
            break;
        case IT_F_IRGB:
#pragma omp parallel for
            for(int y=0; y<h; y++ ) {
                const float* sr = src->get_row_fi(y,0);
                const float* sg = src->get_row_fi(y,1);
                const float* sb = src->get_row_fi(y,2);
                switch( dprec ) {
                case TYPE_UCHAR : {
                    uchar* drow = dst->get_row_u(y);
                    for( int x=0; x<w; x++ )
                        drow[x] = rgb_to_gray_u( sr[x], sg[x], sb[x] );
                } break;
                case TYPE_FLOAT : {
                    float* drow = dst->get_row_f(y);
                    for( int x=0; x<w; x++ )
                        drow[x] = rgb_to_gray_f( sr[x], sg[x], sb[x] );
                } break;
                case TYPE_INT : {
                    int  * drow = dst->get_row_i(y);
                    for( int x=0; x<w; x++ )
                        drow[x] = rgb_to_gray_u( sr[x], sg[x], sb[x] );
                } break;
                default: switch_fatality();
                }
            } break;
        default: switch_fatality();
        }
    }

    void  rgb_to_gray( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        assert_noalias_p( src, dst );
        switch( src->precision() ) {
        case TYPE_UCHAR: urgb_to_gray( src, dst ); break;
        case TYPE_FLOAT: frgb_to_gray( src, dst ); break;
        default        : switch_fatality();
        }
    }

    /// converts IT_U_PRGB <-> IT_U_IRGB
    void convert_pixel_order_uu( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        assert_noalias_p( src, dst );
        src->assert_type( IT_U_IRGB | IT_U_PRGB );
        dst->assert_type( IT_U_IRGB | IT_U_PRGB );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            uchar r,g,b;
            for( int x=0; x<w; x++ ) {
                src->get(x, y, r, g, b);
                dst->set(x, y, r, g, b);
            }
        }
    }

    /// converts IT_F_PRGB <-> IT_F_IRGB
    void convert_pixel_order_ff( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        assert_noalias_p( src, dst );
        src->assert_type( IT_F_IRGB | IT_F_PRGB );
        dst->assert_type( IT_F_IRGB | IT_F_PRGB );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            float r,g,b;
            for( int x=0; x<w; x++ ) {
                src->get(x, y, r, g, b);
                dst->set(x, y, r, g, b);
            }
        }
    }

    /// converts IT_U_[IP]RGB <-> IT_F_[IP]RGB
    void convert_pixel_order_uf( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        assert_noalias_p( src, dst );
        src->assert_type( IT_U_PRGB | IT_U_IRGB );
        dst->assert_type( IT_F_PRGB | IT_F_IRGB );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            uchar r,g,b;
            for( int x=0; x<w; x++ ) {
                src->get(x, y, r, g, b);
                dst->set(x, y, float(r), float(g), float(b));
            }
        }
    }

    /// converts IT_F_[IP]RGB <-> IT_U_[IP]RGB
    void convert_pixel_order_fu( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        assert_noalias_p( src, dst );
        src->assert_type( IT_F_PRGB | IT_F_IRGB );
        dst->assert_type( IT_U_PRGB | IT_U_IRGB );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            float r,g,b;
            for( int x=0; x<w; x++ ) {
                src->get(x, y, r, g, b);
                dst->set( x, y,
                          cast_to_gray_range(r),
                          cast_to_gray_range(g),
                          cast_to_gray_range(b) );
            }
        }
    }

    void convert_pixel_order( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        assert_noalias_p( src, dst );
        src->assert_type( IT_F_PRGB | IT_F_IRGB | IT_U_PRGB | IT_U_IRGB );
        dst->assert_type( IT_F_PRGB | IT_F_IRGB | IT_U_PRGB | IT_U_IRGB );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        if( src->type() == dst->type() ) {
            dst->copy( src );
            return;
        }
        DataType stype = src->precision();
        DataType dtype = dst->precision();
        switch( stype ) {
        case TYPE_UCHAR:
            switch( dtype ) {
            case TYPE_UCHAR: convert_pixel_order_uu(src,dst); break;
            case TYPE_FLOAT: convert_pixel_order_uf(src,dst); break;
            default: switch_fatality();
            }
            break;
        case TYPE_FLOAT:
            switch( dtype ) {
            case TYPE_UCHAR: convert_pixel_order_fu(src,dst); break;
            case TYPE_FLOAT: convert_pixel_order_ff(src,dst); break;
            default: switch_fatality();
            }
            break;
        default: switch_fatality();
        }
    }

    void gray_to_gray_fu( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        src->assert_type( IT_F_GRAY );
        dst->assert_type( IT_U_GRAY );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const float* srow = src->get_row_f(y);
            uchar*       drow = dst->get_row_u(y);
            for( int x=0; x<w; x++ )
                drow[x] = cast_to_gray_range( srow[x] );
        }
    }

    void gray_to_gray_fu16( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        src->passert_type( IT_F_GRAY );
        dst->passert_type( IT_J_GRAY );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const float* srow = src->get_row_f(y);
            uint16_t*    drow = dst->get_row_u16(y);
            for( int x=0; x<w; x++ ) {
                drow[x] = static_cast<uint16_t>( std::min( 65535.0f, std::max(0.0f, srow[x]+0.5f) ) );
            }
        }
    }

    void gray_to_gray_u16f(const Image* src, Image* dst) {
        assert_pointer( src && dst );
        src->passert_type( IT_J_GRAY );
        dst->passert_type( IT_F_GRAY );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const uint16_t* srow = src->get_row_u16(y);
            float*          drow = dst->get_row_f(y);
            for( int x=0; x<w; x++ ) {
                drow[x] = static_cast<float>(srow[x]);
            }
        }
    }

    void gray_to_gray_fi( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        src->assert_type( IT_F_GRAY );
        dst->assert_type( IT_I_GRAY );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const float* srow = src->get_row_f(y);
            int  *       drow = dst->get_row_i(y);
            for( int x=0; x<w; x++ )
                drow[x] = static_cast<int>( srow[x]+0.5f );
        }
    }

    void gray_to_gray_uf( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        src->assert_type( IT_U_GRAY );
        dst->assert_type( IT_F_GRAY );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const uchar* srow = src->get_row_u(y);
            float*       drow = dst->get_row_f(y);
            for( int x=0; x<w; x++ )
                drow[x] = static_cast<float>( srow[x] );
        }
    }
    void gray_to_gray_ui( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        src->assert_type( IT_U_GRAY );
        dst->assert_type( IT_I_GRAY );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const uchar* srow = src->get_row_u(y);
            int  *       drow = dst->get_row_i(y);
            for( int x=0; x<w; x++ )
                drow[x] = static_cast<int>( srow[x] );
        }
    }

    void gray_to_gray_if( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        src->assert_type( IT_I_GRAY );
        dst->assert_type( IT_F_GRAY );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const int  * srow = src->get_row_i(y);
            float*       drow = dst->get_row_f(y);
            for( int x=0; x<w; x++ )
                drow[x] = static_cast<float>( srow[x] );
        }
    }
    void gray_to_gray_iu( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        src->assert_type( IT_I_GRAY );
        dst->assert_type( IT_U_GRAY );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();
#pragma omp parallel for
        for( int y=0; y<h; y++ ) {
            const int  * srow = src->get_row_i(y);
            uchar*       drow = dst->get_row_u(y);
            for( int x=0; x<w; x++ )
                drow[x] = cast_to_gray_range( srow[x] );
        }
    }


    void gray_to_rgb( const Image* src, Image* dst ) {
        assert_pointer( src && dst );
        src->assert_type( IT_U_GRAY | IT_F_GRAY | IT_I_GRAY );
        dst->assert_type( IT_U_IRGB | IT_U_PRGB | IT_F_IRGB | IT_F_PRGB );
        passert_statement( src-> w() == dst-> w(), "image dimensions do not agree" );
        passert_statement( src-> h() == dst-> h(), "image dimensions do not agree" );
        int h = src->h();
        int w = src->w();

        DataType dtype = dst->precision();

        switch( src->precision() ) {
        case TYPE_UCHAR:
#pragma omp parallel for
            for( int y=0; y<h; y++ ) {
                const uchar* srow = src->get_row_u(y);
                for( int x=0; x<w; x++ ) {
                    const uchar& u = srow[x];
                    switch( dtype ) {
                    case TYPE_FLOAT: {
                        float f = static_cast<float>(u);
                        dst->set( x, y, f, f, f );
                    } break;
                    case TYPE_UCHAR:
                        dst->set( x, y, u, u, u );
                    break;
                    default: switch_fatality();
                    }
                }
            }
            break;
        case TYPE_FLOAT:
#pragma omp parallel for
            for( int y=0; y<h; y++ ) {
                const float* srow = src->get_row_f(y);
                for( int x=0; x<w; x++ ) {
                    const float& f = srow[x];
                    switch( dtype ) {
                    case TYPE_FLOAT:
                        dst->set( x, y, f, f, f );
                        break;
                    case TYPE_UCHAR: {
                        uchar u = cast_to_gray_range(f);
                        dst->set( x, y, u, u, u );
                    } break;
                    default: switch_fatality();
                    }
                }
            }
            break;

        case TYPE_INT:
#pragma omp parallel for
            for( int y=0; y<h; y++ ) {
                const int* srow = src->get_row_i(y);
                for( int x=0; x<w; x++ ) {
                    const int& v = srow[x];
                    switch( dtype ) {
                    case TYPE_FLOAT: {
                        float f = static_cast<float>(v);
                        dst->set( x, y, f, f, f );
                    } break;
                    case TYPE_UCHAR: {
                        uchar u = static_cast<uchar>(v);
                        dst->set( x, y, u, u, u );
                    } break;
                    default: switch_fatality();
                    }
                }
            }
            break;

        default: switch_fatality();
        }
    }

    void convert_image( const Image& src, ImageType type, Image& dst ) {
        dst.create( src.w(), src.h(), type );
        convert_image( &src, &dst );
    }

    void convert_image( const Image* src, Image* dst ) {
        passert_pointer( src && dst );
        passert_noalias_p( src, dst );

        if( src->type() == dst->type() ) {
            dst->copy( src );
            return;
        }
        ImageType dtype = dst->type();

        switch( src->type() ) {
        case IT_U_IRGB:
            switch( dtype ) {
            case IT_F_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_U_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_I_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_F_PRGB: convert_pixel_order( src, dst ); break;
            case IT_U_PRGB: convert_pixel_order( src, dst ); break;
            case IT_F_IRGB: convert_pixel_order( src, dst ); break;
            case IT_U_IRGB: switch_fatality();
            default       : switch_fatality();
            } break;
        case IT_U_PRGB:
            switch( dtype ) {
            case IT_F_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_U_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_I_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_F_PRGB: convert_pixel_order( src, dst ); break;
            case IT_F_IRGB: convert_pixel_order( src, dst ); break;
            case IT_U_IRGB: convert_pixel_order( src, dst ); break;
            case IT_U_PRGB: switch_fatality();
            default       : switch_fatality();
            } break;
        case IT_F_IRGB:
            switch( dtype ) {
            case IT_F_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_U_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_I_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_F_PRGB: convert_pixel_order( src, dst ); break;
            case IT_U_PRGB: convert_pixel_order( src, dst ); break;
            case IT_U_IRGB: convert_pixel_order( src, dst ); break;
            case IT_F_IRGB: switch_fatality();
            default       : switch_fatality();
            } break;
        case IT_F_PRGB:
            switch( dtype ) {
            case IT_F_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_U_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_I_GRAY:         rgb_to_gray( src, dst ); break;
            case IT_U_PRGB: convert_pixel_order( src, dst ); break;
            case IT_F_IRGB: convert_pixel_order( src, dst ); break;
            case IT_U_IRGB: convert_pixel_order( src, dst ); break;
            case IT_F_PRGB: switch_fatality();
            default       : switch_fatality();
            } break;

        case IT_U_GRAY:
            switch( dtype ) {
            case IT_F_GRAY: gray_to_gray_uf(src, dst); break;
            case IT_I_GRAY: gray_to_gray_ui(src, dst); break;
            case IT_F_PRGB: gray_to_rgb(src, dst);     break;
            case IT_U_PRGB: gray_to_rgb(src, dst);     break;
            case IT_F_IRGB: gray_to_rgb(src, dst);     break;
            case IT_U_IRGB: gray_to_rgb(src, dst);     break;
            case IT_U_GRAY: switch_fatality();
            default       : switch_fatality();
            } break;
        case IT_F_GRAY:
            switch( dtype ) {
            case IT_U_GRAY: gray_to_gray_fu(src, dst); break;
            case IT_J_GRAY: gray_to_gray_fu16(src, dst); break;
            case IT_I_GRAY: gray_to_gray_fi(src, dst); break;
            case IT_F_PRGB: gray_to_rgb(src, dst);     break;
            case IT_U_PRGB: gray_to_rgb(src, dst);     break;
            case IT_F_IRGB: gray_to_rgb(src, dst);     break;
            case IT_U_IRGB: gray_to_rgb(src, dst);     break;
            case IT_F_GRAY: switch_fatality();
            default       : switch_fatality();
            } break;

        case IT_I_GRAY:
            switch( dtype ) {
            case IT_U_GRAY: gray_to_gray_iu(src, dst); break;
            case IT_F_GRAY: gray_to_gray_if(src, dst); break;

            case IT_F_PRGB: gray_to_rgb(src, dst);     break;
            case IT_U_PRGB: gray_to_rgb(src, dst);     break;
            case IT_F_IRGB: gray_to_rgb(src, dst);     break;
            case IT_U_IRGB: gray_to_rgb(src, dst);     break;

            case IT_I_GRAY: switch_fatality();
            default       : switch_fatality();
            } break;

        case IT_J_GRAY:
            switch( dtype ) {
            case IT_F_GRAY: gray_to_gray_u16f(src, dst); break;
            default       : switch_fatality();
            } break;

        default: switch_fatality();
        }
    }




}
