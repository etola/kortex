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
#ifndef KORTEX_IMAGE_H
#define KORTEX_IMAGE_H

#include <kortex/types.h>
#include <kortex/check.h>
#include <kortex/mem_unit.h>

namespace kortex {

    enum ImageType { IT_U_GRAY=1,    // uchar 1-channel
                     IT_F_GRAY=2,    // float 1-channel
                     IT_U_PRGB=4,    // uchar 3-channel pixel-ordered
                     IT_F_PRGB=8,    // float 3-channel pixel-ordered
                     IT_U_IRGB=16,   // uchar 3-channel image-ordered
                     IT_F_IRGB=32 }; // float 3-channel image-ordered

    enum ChannelType { ITC_PIXEL=1,   // pixel-ordered  [ r0g0b0 r1g1b1...]
                       ITC_IMAGE=2 }; // image-ordered  [ r0r1r2 g0g1g2...]

    inline size_t      image_no_channels  ( const ImageType& it );
    inline DataType    image_precision    ( const ImageType& it );
    inline size_t      image_pixel_size   ( const ImageType& it );
    inline ChannelType image_channel_type ( const ImageType& it );
    inline ImageType   image_type         ( DataType precision,
                                            int n_channels,
                                            ChannelType channel_type );
    inline ImageType get_image_type( int im_type );

///

    class Image {
    private:
        void init_();

        int         m_w;
        int         m_h;
        int         m_ch;
        ImageType   m_type;
        ChannelType m_channel_type;
        uchar*      m_data_u;
        float*      m_data_f;
        MemUnit     m_memory;

    public:
        Image();
        Image(int w, int h, ImageType type);
        Image(const Image& img);

        void create( int w, int h, ImageType type );

        ~Image();
        void release();

        static size_t req_mem( int w, int h, ImageType type );
        static size_t req_mem( const Image* img );
        size_t mem_usage() const;

        int         w ()           const { return m_w;                     }
        int         h ()           const { return m_h;                     }
        int         ch()           const { return m_ch;                    }
        ImageType   type()         const { return m_type;                  }
        ChannelType channel_type() const { return m_channel_type;          }
        DataType    precision()    const { return image_precision(m_type); }
        bool        is_empty()     const { return !(m_w*m_h);              }
        size_t      pixel_count()  const { return size_t(m_w)*size_t(m_h)*size_t(m_ch); }

        bool is_inside( int x, int y ) const {
            return kortex::is_inside(x,0,m_w)
                && kortex::is_inside(y,0,m_h);
        }
        bool is_inside_margin( float x, float y, int margin ) const {
            return kortex::is_inside(x,margin,m_w-margin)
                && kortex::is_inside(y,margin,m_h-margin);
        }
        bool is_inside_margin( int x, int y, int margin ) const {
            return kortex::is_inside(x,margin,m_w-margin)
                && kortex::is_inside(y,margin,m_h-margin);
        }

        void convert( ImageType im_type );
        void swap(       Image* img );
        void copy( const Image* img );

        void zero();
        void set( const float& v );
        void set( const uchar& v );

        //
        // get image channels
        //
        uchar      * get_channel_u( int cid );
        const uchar* get_channel_u( int cid ) const;
        float      * get_channel_f( int cid );
        const float* get_channel_f( int cid ) const;

        //
        // easy access get/set functions - use the row pointers for performance
        // critical stuff
        //

        // 1-channel get
        float getf( int x0, int y0 ) const;
        uchar getu( int x0, int y0 ) const;
        float get ( int x0, int y0 ) const; // accesses the pixel val of
                                            // whatever type. use for
                                            // convenience - not efficient at
                                            // all.

        float get_bilinear  (const float& x0, const float& y0) const;
        float get_bilinear_u(const float& x0, const float& y0) const;
        float get_bilinear_f(const float& x0, const float& y0) const;
        float get_bicubic   (const float& x0, const float& y0) const;
        float get_bicubic_u (const float& x0, const float& y0) const;
        float get_bicubic_f (const float& x0, const float& y0) const;

        // 3-channel get
        void  get ( int x0, int y0, float& r, float& g, float& b ) const;
        void  get ( int x0, int y0, uchar& r, uchar& g, uchar& b ) const;

        void  get_bilinear   (const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bilinear_up(const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bilinear_ui(const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bilinear_fp(const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bilinear_fi(const float& x0, const float& y0, float& r, float& g, float& b) const;

        void  get_bicubic    (const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bicubic_up (const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bicubic_ui (const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bicubic_fp (const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bicubic_fi (const float& x0, const float& y0, float& r, float& g, float& b) const;

        //
        float get_grad_x( const int& x0, const int &y0 ) const;
        float get_grad_y( const int& x0, const int &y0 ) const;


        // 1-channel set
        void  set( const int& x0, const int& y0, const float& v );
        void  set( const int& x0, const int& y0, const uchar& v );
        void  set( const int& x0, const int& y0, const int& hsz, const uchar& v );
        void  set( const int& x0, const int& y0, const int& hsz, const float& v );

        // 3-channel set
        void  set ( const int& x0, const int& y0, const float& r, const float& g, const float& b );
        void  set ( const int& x0, const int& y0, const uchar& r, const uchar& g, const uchar& b );

        void  set ( const int& x0, const int& y0, const int& hsz, const uchar& r, const uchar& g, const uchar& b );
        void  set ( const int& x0, const int& y0, const int& hsz, const float& r, const float& g, const float& b );


        /// copies the src region [ (sx0,sy0) -> (sx0+rw, sy0+rh) ] to the
        /// 'this' region starting from (dx0,dy0)
        void copy_from_region(const Image* src,
                              const int& sx0, const int& sy0, const int& rw,  const int& rh,
                              const int& dx0, const int& dy0);

        //
        // row pointers
        //
        uchar* get_row_u ( int y0 ); // use for u gray, prgb
        float* get_row_f ( int y0 ); // use for f gray, prgb
        uchar* get_row_ui( int y0, int cid ); // cid'th channel y0'th row
        float* get_row_fi( int y0, int cid ); // cid'th channel y0'th row

        const uchar* get_row_u ( int y0 ) const; // use for u gray, prgb
        const float* get_row_f ( int y0 ) const; // use for f gray, prgb
        const uchar* get_row_ui( int y0, int cid ) const; // cid'th channel y0'th row
        const float* get_row_fi( int y0, int cid ) const; // cid'th channel y0'th row


        //
        // do not forget to delete the pointer!
        Image      * get_channel_wrapper( int cid );
        const Image* get_channel_wrapper( int cid ) const;

        //
        // io
        //
        void save( const string& file ) const;
        void load( const string& file );

        //
        //
        //
        bool is_maximum( const int& x0, const int& y0, const int& wnd_rad, const float& v0 ) const;
        bool is_minimum( const int& x0, const int& y0, const int& wnd_rad, const float& v0 ) const;


        //
        // assrtions
        //
        void assert_type( int type ) const {
            assert_statement( m_type & type, "invalid image type" );
        }
        void passert_type( int type, const char* msg = NULL ) const {
            if( msg ) {
                passert_statement_g( m_type & type, "invalid image type [%s]", msg );
            } else {
                passert_statement  ( m_type & type, "invalid image type" );
            }
        }
    };









///
    size_t      image_no_channels ( const ImageType& it ) {
        switch( it ) {
        case IT_U_GRAY: return 1;
        case IT_F_GRAY: return 1;
        case IT_U_PRGB: return 3;
        case IT_F_PRGB: return 3;
        case IT_U_IRGB: return 3;
        case IT_F_IRGB: return 3;
        default       : switch_fatality();
        }
        return 0;
    }
    DataType    image_precision   ( const ImageType& it ) {
        switch( it ) {
        case IT_U_GRAY:
        case IT_U_PRGB:
        case IT_U_IRGB: return TYPE_UCHAR;
        case IT_F_GRAY:
        case IT_F_PRGB:
        case IT_F_IRGB: return TYPE_FLOAT;
        default       : switch_fatality();
        }
        return TYPE_UCHAR;
    }
    size_t      image_pixel_size  ( const ImageType& it ) {
        return image_no_channels(it) * get_data_byte_size( image_precision(it) );
    }
    ChannelType image_channel_type( const ImageType& it ) {
        switch( it ) {
        case IT_U_GRAY:
        case IT_F_GRAY:
        case IT_U_PRGB:
        case IT_F_PRGB: return ITC_PIXEL;
        case IT_U_IRGB:
        case IT_F_IRGB: return ITC_IMAGE;
        default       : switch_fatality();
        }
    }
    ImageType   image_type        ( DataType precision,
                                    int n_channels,
                                    ChannelType channel_type ) {
        switch( n_channels ) {
        case 1:
            switch( precision ) {
            case TYPE_UCHAR: return IT_U_GRAY;
            case TYPE_FLOAT: return IT_F_GRAY;
            default        : switch_fatality();
            } break;
        case 3:
            switch( precision ) {
            case TYPE_UCHAR:
                switch( channel_type ) {
                case ITC_PIXEL: return IT_U_PRGB;
                case ITC_IMAGE: return IT_U_IRGB;
                default       : switch_fatality();
                } break;
            case TYPE_FLOAT:
                switch( channel_type ) {
                case ITC_PIXEL: return IT_F_PRGB;
                case ITC_IMAGE: return IT_F_IRGB;
                default       : switch_fatality();
                } break;
            default: switch_fatality();
            } break;
        default: switch_fatality();
        }
    }
    ImageType get_image_type( int im_type ) {
        switch( im_type ) {
        case 1 : return IT_U_GRAY;
        case 2 : return IT_F_GRAY;
        case 4 : return IT_U_PRGB;
        case 8 : return IT_F_PRGB;
        case 16: return IT_U_IRGB;
        case 32: return IT_F_IRGB;
        default: switch_fatality();
        }
    }


}

#endif
