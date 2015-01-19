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

#include <fstream>

#include <kortex/types.h>
#include <kortex/check.h>
#include <kortex/mem_unit.h>

using std::ofstream;
using std::ifstream;

namespace kortex {

    enum ImageType { IT_U_GRAY=1,    // uchar 1-channel
                     IT_F_GRAY=2,    // float 1-channel
                     IT_U_PRGB=4,    // uchar 3-channel pixel-ordered
                     IT_F_PRGB=8,    // float 3-channel pixel-ordered
                     IT_U_IRGB=16,   // uchar 3-channel image-ordered
                     IT_F_IRGB=32,   // float 3-channel image-ordered
                     IT_I_GRAY=64 }; // int   1-channel

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
        int  *      m_data_i;
        MemUnit     m_memory;
        bool        m_wrapper;

    public:
        Image();
        Image(int w, int h, ImageType type);
        Image(const Image& img);

        Image& operator=( const Image& p );

        void create( int w, int h, ImageType type );

        ~Image();
        void release();

        static size_t req_mem( int w, int h, ImageType type );
        static size_t req_mem( const Image* img );
        size_t mem_usage() const;

        int         w ()            const { return m_w;                     }
        int         h ()            const { return m_h;                     }
        int         ch()            const { return m_ch;                    }
        ImageType   type()          const { return m_type;                  }
        ChannelType channel_type()  const { return m_channel_type;          }
        DataType    precision()     const { return image_precision(m_type); }
        bool        is_empty()      const { return !(m_w*m_h);              }
        bool        is_wrapper()    const { return m_wrapper;               }
        int         pixel_count()   const { return m_w*m_h;                 }
        size_t      element_count() const { return size_t(m_w)*size_t(m_h)*size_t(m_ch); }

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

        /// checks if image is non zero in a  ( 2*rsz+1 )^2 window. only FGRAY is implemented
        bool is_non_zero( const int& x0, const int& y0, const int& rsz ) const;

        /// checks if there is a zero in a (2*rsz+1)^2 window. only FGRAY is implemented
        bool does_contain_zero( const int& x0, const int& y0, const int& rsz ) const;

        /// converts image between all the defined types
        void convert( ImageType im_type );

        /// swaps the content completely including the memory
        void swap(       Image* img );

        /// copies image content - cannot copy itself.
        void copy( const Image* img );

        /// sets image data to zero
        void zero();

        /// sets image pixels to v
        void set( const float& v );
        void set( const uchar& v );
        void set( const int  & v );

        ///
        /// get raw pointers
        ///
        const float* get_fptr() const { return m_data_f; }
        const uchar* get_uptr() const { return m_data_u; }
        const int  * get_iptr() const { return m_data_i; }
        float      * get_fptr()       { return m_data_f; }
        uchar      * get_uptr()       { return m_data_u; }
        int        * get_iptr()       { return m_data_i; }


        ///
        /// get image channels
        ///
        uchar      * get_channel_u( int cid );
        const uchar* get_channel_u( int cid ) const;
        float      * get_channel_f( int cid );
        const float* get_channel_f( int cid ) const;
        int        * get_channel_i( int cid );
        const int  * get_channel_i( int cid ) const;

        /// 1-channel get

        ///
        /// easy access get/set functions - use the row pointers for performance
        /// critical stuff
        ///
        int   geti( int x0, int y0 ) const;
        float getf( int x0, int y0 ) const;
        uchar getu( int x0, int y0 ) const;

        /// accesses the pixel val of whatever type. use for convenience - not
        /// efficient at all.
        float get ( int x0, int y0 ) const;

        /// gets bilinearly interpolated values - image needs to be single channel
        float get_bilinear  (const float& x0, const float& y0) const;
        float get_bilinear_u(const float& x0, const float& y0) const;
        float get_bilinear_f(const float& x0, const float& y0) const;
        /// gets bicubic interpolated values - image needs to be single channel
        float get_bicubic   (const float& x0, const float& y0) const;
        float get_bicubic_u (const float& x0, const float& y0) const;
        float get_bicubic_f (const float& x0, const float& y0) const;

        // 3-channel get
        void  get ( int x0, int y0, float& r, float& g, float& b ) const;
        void  get ( int x0, int y0, uchar& r, uchar& g, uchar& b ) const;

        /// gets bilinearly interpolated values - image needs to be 3-channel
        void  get_bilinear   (const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bilinear_up(const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bilinear_ui(const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bilinear_fp(const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bilinear_fi(const float& x0, const float& y0, float& r, float& g, float& b) const;

        /// gets bicubic interpolated values - image needs to be 3-channel
        void  get_bicubic    (const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bicubic_up (const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bicubic_ui (const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bicubic_fp (const float& x0, const float& y0, float& r, float& g, float& b) const;
        void  get_bicubic_fi (const float& x0, const float& y0, float& r, float& g, float& b) const;

        /// returns a simple 2-point gradient. for full image gradient check the
        /// image_processing.h for a more efficient version
        float get_grad_x( const int& x0, const int &y0 ) const;
        float get_grad_y( const int& x0, const int &y0 ) const;


        // 1-channel set

        /// single channel set functions - not access efficient - use for convenience
        void  set( const int& x0, const int& y0, const float& v );
        void  set( const int& x0, const int& y0, const uchar& v );
        void  set( const int& x0, const int& y0, const int  & v );

        /// sets a (2*hsz+1)^2 patch with value v
        void  set( const int& x0, const int& y0, const int& hsz, const uchar& v );
        void  set( const int& x0, const int& y0, const int& hsz, const float& v );
        void  set( const int& x0, const int& y0, const int& hsz, const int  & v );

        /// 3-channel set functions - not access efficient - use for convenience
        void  set ( const int& x0, const int& y0, const float& r, const float& g, const float& b );
        void  set ( const int& x0, const int& y0, const uchar& r, const uchar& g, const uchar& b );

        /// sets a (2*hsz+1)^2 patch with value [r,g,b]
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

        /// uchar data row pointer - use for u gray, prgb
        uchar* get_row_u ( int y0 );
        /// float data row pointer - use for f gray, prgb
        float* get_row_f ( int y0 );
        /// int data row pointer - use for i gray
        int  * get_row_i ( int y0 );

        /// cid'th channel y0'th row - u gray/prgb
        uchar* get_row_ui( int y0, int cid );
        /// cid'th channel y0'th row - f gray/prgb
        float* get_row_fi( int y0, int cid );

        /// const versions
        const uchar* get_row_u ( int y0 ) const; // use for u gray, prgb
        const float* get_row_f ( int y0 ) const; // use for f gray, prgb
        const int  * get_row_i ( int y0 ) const; // use for i gray

        /// const versions
        const uchar* get_row_ui( int y0, int cid ) const; // cid'th channel y0'th row
        const float* get_row_fi( int y0, int cid ) const; // cid'th channel y0'th row


        /// generates a wrapper image for the cid'th channel - does not copy
        /// content but caller needs to delete the pointer to get rid of
        /// boilerplate mems
        Image      * get_channel_wrapper( int cid );
        const Image* get_channel_wrapper( int cid ) const;

        ///
        /// io
        ///
        void save( const string& file ) const;
        void load( const string& file );
        void save( ofstream& fout ) const;
        void load( ifstream& fin  );

        /// is the v0 value the maximum within a square patch - wnd_rad is the half-width.
        bool is_maximum( const int& x0, const int& y0, const int& wnd_rad, const float& v0 ) const;
        bool is_maximum( const int& x0, const int& y0, const int& wnd_rad, const int  & v0 ) const;
        /// is the v0 value the minimum within a square patch - wnd_rad is the half-width.
        bool is_minimum( const int& x0, const int& y0, const int& wnd_rad, const float& v0 ) const;
        bool is_minimum( const int& x0, const int& y0, const int& wnd_rad, const int  & v0 ) const;


        /// asserts the image type - disables in RELEASE mode
        void assert_type( int type ) const {
            assert_statement( m_type & type, "invalid image type" );
        }
        /// asserts the image type - persistent in DEBUG/RELEASE mode
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
        case IT_I_GRAY: return 1;
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
        case IT_I_GRAY: return TYPE_INT;
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
        case IT_I_GRAY:
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
            case TYPE_INT  : return IT_I_GRAY;
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
        case 64: return IT_I_GRAY;
        default: switch_fatality();
        }
    }

    inline bool check_dimensions( const Image* p, const Image* q ) {
        assert_pointer( p && q );
        if( p->w() != q->w() ) return false;
        if( p->h() != q->h() ) return false;
        return true;
    }

    inline bool check_dimensions( const Image& p, const Image& q ) {
        if( p.w() != q.w() ) return false;
        if( p.h() != q.h() ) return false;
        return true;
    }



}

#endif
