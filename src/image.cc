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
#include <kortex/image.h>
#include <kortex/image_processing.h>
#include <kortex/image_conversion.h>
#include <kortex/image_io.h>
#include <kortex/check.h>
#include <kortex/fileio.h>

#include <cstring>

namespace kortex {

    void Image::init_() {
        m_w            = 0;
        m_h            = 0;
        m_ch           = 0;
        m_type         = IT_U_GRAY;
        m_channel_type = ITC_PIXEL;
        m_data_i       = NULL;
        m_data_u       = NULL;
        m_data_f       = NULL;
        m_data_u16     = NULL;
        m_wrapper      = false;
    }

    Image::Image() {
        init_();
    }

    Image::Image( int w, int h, ImageType type ) {
        init_();
        create( w, h, type );
    }

    Image::Image( const Image& img ) {
        init_();
        if( img.is_empty() )
            return;
        this->copy( &img );
    }

    Image& Image::operator=( const Image& p ) {
        this->copy( &p );
        return *this;
    }

    void Image::create( int w, int h, ImageType type ) {
        passert_statement( w*h>0, "will not create null image" );
        size_t sz = req_mem( w, h, type );
        if( m_wrapper ) {
            passert_statement( (m_w==w) && (m_h==h) && (type==m_type), "cannot change the attributes of a wrapper image" );
            return;
        }
        m_memory.resize( sz );
        switch( image_precision(type) ) {
        case TYPE_UCHAR  : m_data_u   = (uchar   *) m_memory.get_buffer(); break;
        case TYPE_FLOAT  : m_data_f   = (float   *) m_memory.get_buffer(); break;
        case TYPE_INT    : m_data_i   = (int     *) m_memory.get_buffer(); break;
        case TYPE_UINT16 : m_data_u16 = (uint16_t*) m_memory.get_buffer(); break;

        default        : switch_fatality();
        }
        m_w    = w;
        m_h    = h;
        m_type = type;
        m_ch   = image_no_channels( type );
        m_channel_type = image_channel_type( type );
    }

    void Image::release() {
        m_memory.deallocate();
        init_();
    }

    Image::~Image() {
        release();
    }

    size_t Image::req_mem( int w, int h, ImageType type ) {
        return size_t(w) * size_t(h) * image_pixel_size(type);
    }

    size_t Image::req_mem( const Image* img ) {
        return req_mem( img->w(), img->h(), img->type() );
    }

    size_t Image::mem_usage() const {
        return req_mem( this );
    }

    void Image::convert( ImageType im_type ) {
        if( m_type == im_type ) return;
        if( m_wrapper ) logman_fatal("cannot convert wrapper image");
        Image new_image;
        new_image.create( m_w, m_h, im_type );
        convert_image( this, &new_image );
        this->copy( &new_image );
    }

    void Image::swap( Image* img ) {
        passert_pointer( img );
        passert_statement( !m_wrapper && !img->is_wrapper(), "cannot swap wrapper image" );
        std::swap( m_w            , img->m_w            );
        std::swap( m_h            , img->m_h            );
        std::swap( m_ch           , img->m_ch           );
        std::swap( m_type         , img->m_type         );
        std::swap( m_channel_type , img->m_channel_type );
        std::swap( m_data_i       , img->m_data_i       );
        std::swap( m_data_u       , img->m_data_u       );
        std::swap( m_data_f       , img->m_data_f       );
        std::swap( m_data_u16     , img->m_data_u16     );
        m_memory.swap( &(img->m_memory) );
    }

    void Image::copy(const Image* img) {
        passert_pointer( img );
        passert_statement( img != this, "cannot copy self" );
        create( img->w(), img->h(), img->type() );
        size_t imsz = size_t(m_w) * size_t(m_h) * size_t(m_ch);
        switch( image_precision(m_type) ) {
        case TYPE_UCHAR  : memcpy( m_data_u, img->m_data_u, sizeof(*m_data_u)* imsz ); break;
        case TYPE_FLOAT  : memcpy( m_data_f, img->m_data_f, sizeof(*m_data_f)* imsz ); break;
        case TYPE_INT    : memcpy( m_data_i, img->m_data_i, sizeof(*m_data_i)* imsz ); break;
        case TYPE_UINT16 : memcpy( m_data_u16, img->m_data_u16, sizeof(*m_data_u16)* imsz ); break;
        default          : switch_fatality();
        }
    }

    void Image::zero() {
        size_t im_sz = size_t(m_w) * size_t(m_h) * size_t(m_ch);
        switch( precision() ) {
        case TYPE_UCHAR  : memset( m_data_u, 0, sizeof(*m_data_u)*im_sz ); break;
        case TYPE_FLOAT  : memset( m_data_f, 0, sizeof(*m_data_f)*im_sz ); break;
        case TYPE_INT    : memset( m_data_i, 0, sizeof(*m_data_i)*im_sz ); break;
        case TYPE_UINT16 : memset( m_data_u16, 0, sizeof(*m_data_u16)*im_sz ); break;
        default          : switch_fatality();
        }
    }

    void Image::set( const float& v ) {
        passert_statement( !is_empty(), "empty image" );
        passert_type( IT_F_GRAY );
        for( int y=0; y<m_h; y++ ) {
            float* row = get_row_f( y );
            for( int x=0; x<m_w; x++ ) {
                row[x] = v;
            }
        }
    }

    void Image::set( const uchar& v ) {
        passert_statement( !is_empty(), "empty image" );
        passert_type( IT_U_GRAY );
        for( int y=0; y<m_h; y++ ) {
            uchar* row = get_row_u( y );
            for( int x=0; x<m_w; x++ ) {
                row[x] = v;
            }
        }
    }

    void Image::set( const int& v ) {
        passert_statement( !is_empty(), "empty image" );
        passert_type( IT_I_GRAY );
        for( int y=0; y<m_h; y++ ) {
            int* row = get_row_i( y );
            for( int x=0; x<m_w; x++ ) {
                row[x] = v;
            }
        }
    }

    void Image::set( const uint16_t& v ) {
        passert_statement( !is_empty(), "empty image" );
        passert_type( IT_J_GRAY );
        for( int y=0; y<m_h; y++ ) {
            uint16_t* row = get_row_u16( y );
            for( int x=0; x<m_w; x++ ) {
                row[x] = v;
            }
        }
    }


    //
    // get image channels
    //
    uchar      * Image::get_channel_u( int cid ) {
        assert_type( IT_U_GRAY | IT_U_IRGB );
        assert_boundary( cid, 0, m_ch );
        return m_data_u + cid*m_h*m_w;
    }
    const uchar* Image::get_channel_u( int cid ) const {
        assert_type( IT_U_GRAY | IT_U_IRGB );
        assert_boundary( cid, 0, m_ch );
        return m_data_u + cid*m_h*m_w;
    }
    float      * Image::get_channel_f( int cid ) {
        assert_type( IT_F_GRAY | IT_F_IRGB );
        assert_boundary( cid, 0, m_ch );
        return m_data_f + cid*m_h*m_w;
    }
    const float* Image::get_channel_f( int cid ) const {
        assert_type( IT_F_GRAY | IT_F_IRGB );
        assert_boundary( cid, 0, m_ch );
        return m_data_f + cid*m_h*m_w;
    }
    int        * Image::get_channel_i( int cid ) {
        assert_type( IT_I_GRAY );
        assert_boundary( cid, 0, m_ch );
        return m_data_i + cid*m_h*m_w;
    }
    const int  * Image::get_channel_i( int cid ) const {
        assert_type( IT_I_GRAY );
        assert_boundary( cid, 0, m_ch );
        return m_data_i + cid*m_h*m_w;
    }
    const uint16_t* Image::get_channel_u16( int cid ) const {
        assert_type( IT_J_GRAY );
        assert_boundary( cid, 0, m_ch );
        return m_data_u16 + cid*m_h*m_w;
    }

    //
    // row pointers
    //
    int* Image::get_row_i ( int y0 ) { // use for int gray
        assert_type( IT_I_GRAY );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_i + y0 * m_w * m_ch;
    }
    uint16_t* Image::get_row_u16( int y0 ) { // use for int gray
        assert_type( IT_J_GRAY );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_u16 + y0 * m_w * m_ch;
    }
    uchar* Image::get_row_u ( int y0 ) { // use for u gray, prgb
        assert_type( IT_U_GRAY | IT_U_PRGB );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_u + y0 * m_w * m_ch;
    }
    float* Image::get_row_f ( int y0 ) { // use for f gray, prgb
        assert_type( IT_F_GRAY | IT_F_PRGB );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_f + y0 * m_w * m_ch;
    }
    uchar* Image::get_row_ui( int y0, int cid ) { // cid'th channel y0'th row
        assert_type( IT_U_IRGB | IT_U_GRAY );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_u + (cid * m_h + y0) * m_w;
    }
    float* Image::get_row_fi( int y0, int cid ) { // cid'th channel y0'th row
        assert_type( IT_F_IRGB | IT_F_GRAY );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_f + (cid * m_h + y0) * m_w;
    }

    const int* Image::get_row_i ( int y0 ) const { // use for u gray, prgb
        assert_type( IT_I_GRAY );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_i + y0 * m_w * m_ch;
    }
    const uint16_t* Image::get_row_u16( int y0 ) const { // use for u gray, prgb
        assert_type( IT_J_GRAY );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_u16 + y0 * m_w * m_ch;
    }
    const uchar* Image::get_row_u ( int y0 ) const { // use for u gray, prgb
        assert_type( IT_U_GRAY | IT_U_PRGB );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_u + y0 * m_w * m_ch;
    }
    const float* Image::get_row_f ( int y0 ) const { // use for f gray, prgb
        assert_type( IT_F_GRAY | IT_F_PRGB );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_f + y0 * m_w * m_ch;
    }
    const uchar* Image::get_row_ui( int y0, int cid ) const { // cid'th channel y0'th row
        assert_type( IT_U_IRGB | IT_U_GRAY );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_u + (cid * m_h + y0) * m_w;
    }
    const float* Image::get_row_fi( int y0, int cid ) const { // cid'th channel y0'th row
        assert_type( IT_F_IRGB | IT_F_GRAY );
        assert_statement_g( kortex::is_inside(y0,0,m_h), "[y0 %d] oob", y0 );
        return m_data_f + (cid * m_h + y0) * m_w;
    }



    // 1-channel get
    float Image::getf( int x0, int y0 ) const {
        assert_type  ( IT_F_GRAY );
        assert_statement_g(is_inside(x0,y0), "[x %d] [y %d] oob", x0, y0);
        return m_data_f[ y0*m_w+x0 ];
    }

    uchar Image::getu( int x0, int y0 ) const {
        assert_type  ( IT_U_GRAY );
        assert_statement_g(is_inside(x0,y0), "[x %d] [y %d] oob", x0, y0);
        return m_data_u[ y0*m_w+x0 ];
    }
    int   Image::geti( int x0, int y0 ) const {
        assert_type  ( IT_I_GRAY );
        assert_statement_g(is_inside(x0,y0), "[x %d] [y %d] oob", x0, y0);
        return m_data_i[ y0*m_w+x0 ];
    }
    uint16_t Image::getu16( int x0, int y0 ) const {
        assert_type  ( IT_J_GRAY );
        assert_statement_g(is_inside(x0,y0), "[x %d] [y %d] oob", x0, y0);
        return m_data_u16[ y0*m_w+x0 ];
    }


    float Image::get( int x0, int y0 ) const {
        assert_type( IT_U_GRAY | IT_F_GRAY );
        switch( m_type ) {
        case IT_U_GRAY: return static_cast<float>(m_data_u[ y0*m_w+x0 ]); break;
        case IT_F_GRAY: return m_data_f[ y0*m_w+x0 ]; break;
        case IT_I_GRAY: return static_cast<float>(m_data_i[ y0*m_w+x0 ]); break;
        case IT_J_GRAY: return static_cast<float>(m_data_u16[ y0*m_w+x0 ]); break;
        default       : switch_fatality();
        }
    }

    float Image::get_bilinear ( const float& x0, const float& y0 ) const {
        switch( m_type ) {
        case IT_U_GRAY: return get_bilinear_u(x0, y0); break;
        case IT_F_GRAY: return get_bilinear_f(x0, y0); break;
        default       : logman_fatal("invalid type for this function"); break;
        }
    }
    float Image::get_bicubic( const float& x0, const float& y0 ) const {
        switch( m_type ) {
        case IT_U_GRAY: return get_bicubic_u(x0, y0); break;
        case IT_F_GRAY: return get_bicubic_f(x0, y0); break;
        default       : logman_fatal("invalid type for this function"); break;
        }
    }
    float Image::get_bilinear_u( const float& x0, const float& y0 ) const {
        assert_type  ( IT_U_GRAY );
        return bilinear_interpolation( m_data_u, m_w, m_h, 1, 0, x0, y0 );
    }
    float Image::get_bilinear_f( const float& x0, const float& y0 ) const {
        assert_type  ( IT_F_GRAY );
        return bilinear_interpolation( m_data_f, m_w, m_h, 1, 0, x0, y0 );
    }
    float Image::get_bicubic_u( const float& x0, const float& y0 ) const {
        assert_type  ( IT_U_GRAY );
        assert_statement_g(is_inside_margin(x0,y0,2), "[x %f] [y %f] oob", x0, y0);
        return bicubic_interpolation( m_data_u, m_w, m_h, 1, 0, x0, y0 );
    }
    float Image::get_bicubic_f( const float& x0, const float& y0 ) const {
        assert_type  ( IT_F_GRAY );
        assert_statement_g(is_inside_margin(x0,y0,2), "[x %f] [y %f] oob", x0, y0);
        return bicubic_interpolation( m_data_f, m_w, m_h, 1, 0, x0, y0 );
    }

    void Image::get( int x0, int y0, uchar& r, uchar& g, uchar& b ) const {
        assert_pointer( m_data_u );
        assert_statement_g( is_inside(x0,y0), "[x %d] [y %d] out of bounds", x0, y0 );
        assert_statement( ch() == 3, "no_channels should be 3" );
        assert_statement( precision() == TYPE_UCHAR, "invalid precision type" );
        size_t shft = 0;
        r = g = b = 0;
        switch( m_channel_type ) {
        case ITC_PIXEL:
            shft = y0 * m_w * m_ch + x0*m_ch;
            r = m_data_u[ shft   ];
            g = m_data_u[ shft+1 ];
            b = m_data_u[ shft+2 ];
            break;
        case ITC_IMAGE:
            shft = y0*m_w+x0;
            r = m_data_u[ shft             ];
            g = m_data_u[ shft + m_w*m_h   ];
            b = m_data_u[ shft + m_w*m_h*2 ];
            break;
        default: switch_fatality();
        }
    }

    void Image::get( int x0, int y0, float& r, float& g, float& b ) const {
        assert_pointer( m_data_f );
        assert_statement_g( is_inside(x0,y0), "[x %d] [y %d] out of bounds", x0, y0 );
        assert_statement( ch() == 3, "no_channels should be 3" );
        assert_statement( precision() == TYPE_FLOAT, "invalid precision type" );
        size_t shft = 0;
        r = g = b = 0.0f;
        switch( m_channel_type ) {
        case ITC_PIXEL:
            shft = y0 * m_w * m_ch + x0*m_ch;
            r = m_data_f[ shft   ];
            g = m_data_f[ shft+1 ];
            b = m_data_f[ shft+2 ];
            break;
        case ITC_IMAGE:
            shft = y0*m_w+x0;
            r = m_data_f[ shft             ];
            g = m_data_f[ shft + m_w*m_h   ];
            b = m_data_f[ shft + m_w*m_h*2 ];
            break;
        default: switch_fatality();
        }
    }

    void Image::get_bilinear   (const float& x0, const float& y0, float& r, float& g, float& b) const {
        switch( m_type ) {
        case IT_U_PRGB: get_bilinear_up(x0, y0, r, g, b); break;
        case IT_U_IRGB: get_bilinear_ui(x0, y0, r, g, b); break;
        case IT_F_PRGB: get_bilinear_fp(x0, y0, r, g, b); break;
        case IT_F_IRGB: get_bilinear_fi(x0, y0, r, g, b); break;
        default       : logman_fatal("invalid type for this function"); break;
        }
    }
    void Image::get_bilinear_up( const float& x0, const float& y0, float& r, float& g, float& b ) const {
        assert_type( IT_U_PRGB );
        passert_statement_g( x0>=0 && x0<=m_w-1, "pixel oob [%f %f]", x0, y0 );
        passert_statement_g( y0>=0 && y0<=m_h-1, "pixel oob [%f %f]", x0, y0 );
        r = bilinear_interpolation( m_data_u, m_w, m_h, m_ch, 0, x0, y0 );
        g = bilinear_interpolation( m_data_u, m_w, m_h, m_ch, 1, x0, y0 );
        b = bilinear_interpolation( m_data_u, m_w, m_h, m_ch, 2, x0, y0 );
    }
    void Image::get_bilinear_ui( const float& x0, const float& y0, float& r, float& g, float& b ) const {
        assert_type( IT_U_IRGB );
        passert_statement_g( x0>=0 && x0<=m_w-1, "pixel oob [%f %f]", x0, y0 );
        passert_statement_g( y0>=0 && y0<=m_h-1, "pixel oob [%f %f]", x0, y0 );
        const uchar* channel = NULL;
        channel = get_channel_u(0); r = bilinear_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
        channel = get_channel_u(1); g = bilinear_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
        channel = get_channel_u(2); b = bilinear_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
    }
    void Image::get_bilinear_fp( const float& x0, const float& y0, float& r, float& g, float& b ) const {
        assert_type( IT_F_PRGB );
        passert_statement_g( x0>=0 && x0<=m_w-1, "pixel oob [%f %f]", x0, y0 );
        passert_statement_g( y0>=0 && y0<=m_h-1, "pixel oob [%f %f]", x0, y0 );
        r = bilinear_interpolation( m_data_f, m_w, m_h, 3, 0, x0, y0 );
        g = bilinear_interpolation( m_data_f, m_w, m_h, 3, 1, x0, y0 );
        b = bilinear_interpolation( m_data_f, m_w, m_h, 3, 2, x0, y0 );
    }
    void Image::get_bilinear_fi( const float& x0, const float& y0, float& r, float& g, float& b ) const {
        assert_type( IT_F_IRGB );
        passert_statement_g( x0>=0 && x0<=m_w-1, "pixel oob [%f %f]", x0, y0 );
        passert_statement_g( y0>=0 && y0<=m_h-1, "pixel oob [%f %f]", x0, y0 );
        const float* channel = NULL;
        channel = get_channel_f(0); r = bilinear_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
        channel = get_channel_f(1); g = bilinear_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
        channel = get_channel_f(2); b = bilinear_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
    }

    void Image::get_bicubic   (const float& x0, const float& y0, float& r, float& g, float& b) const {
        switch( m_type ) {
        case IT_U_PRGB: get_bicubic_up(x0, y0, r, g, b); break;
        case IT_U_IRGB: get_bicubic_ui(x0, y0, r, g, b); break;
        case IT_F_PRGB: get_bicubic_fp(x0, y0, r, g, b); break;
        case IT_F_IRGB: get_bicubic_fi(x0, y0, r, g, b); break;
        default       : logman_fatal("invalid type for this function"); break;
        }
    }
    void Image::get_bicubic_up( const float& x0, const float& y0, float& r, float& g, float& b ) const {
        assert_type( IT_U_PRGB );
        passert_statement( is_inside_margin(x0,y0,2), "pixel oob" );
        r = bicubic_interpolation( m_data_u, m_w, m_h, m_ch, 0, x0, y0 );
        g = bicubic_interpolation( m_data_u, m_w, m_h, m_ch, 1, x0, y0 );
        b = bicubic_interpolation( m_data_u, m_w, m_h, m_ch, 2, x0, y0 );
    }
    void Image::get_bicubic_ui( const float& x0, const float& y0, float& r, float& g, float& b ) const {
        assert_type( IT_U_IRGB );
        passert_statement( is_inside_margin(x0,y0,2), "pixel oob" );
        const uchar* channel = NULL;
        channel = get_channel_u(0); r = bicubic_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
        channel = get_channel_u(1); g = bicubic_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
        channel = get_channel_u(2); b = bicubic_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
    }
    void Image::get_bicubic_fp( const float& x0, const float& y0, float& r, float& g, float& b ) const {
        assert_type( IT_F_PRGB );
        passert_statement( is_inside_margin(x0,y0,2), "pixel oob" );
        r = bicubic_interpolation( m_data_f, m_w, m_h, 3, 0, x0, y0 );
        g = bicubic_interpolation( m_data_f, m_w, m_h, 3, 1, x0, y0 );
        b = bicubic_interpolation( m_data_f, m_w, m_h, 3, 2, x0, y0 );
    }
    void Image::get_bicubic_fi( const float& x0, const float& y0, float& r, float& g, float& b ) const {
        assert_type( IT_F_IRGB );
        passert_statement( is_inside_margin(x0,y0,2), "pixel oob" );
        const float* channel = NULL;
        channel = get_channel_f(0); r = bicubic_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
        channel = get_channel_f(1); g = bicubic_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
        channel = get_channel_f(2); b = bicubic_interpolation( channel, m_w, m_h, 1, 0, x0, y0 );
    }

    void Image::add( const int& x0, const int& y0, const float& v ) {
        assert_type( IT_F_GRAY );
        assert_statement_g( is_inside(x0,y0), "xy %d %d oob", x0, y0 );
        m_data_f[ y0*m_w + x0 ] += v;
    }

    void Image::add( const int& x0, const int& y0, const float& r, const float& g, const float& b ) {
        assert_type( IT_F_PRGB || IT_F_IRGB );
        assert_statement_g( is_inside(x0,y0), "xy %d %d oob", x0, y0 );

        size_t shft = 0;
        switch( m_channel_type ) {
        case ITC_PIXEL:
            shft = (y0*m_w + x0)*m_ch;
            m_data_f[ shft   ] += r;
            m_data_f[ shft+1 ] += g;
            m_data_f[ shft+2 ] += b;
            break;
        case ITC_IMAGE:
            shft = y0*m_w + x0;
            m_data_f[ shft             ] += r;
            m_data_f[ shft + m_w*m_h   ] += g;
            m_data_f[ shft + m_w*m_h*2 ] += b;
            break;
        default: switch_fatality();
        }
    }



    // 1-channel set
    void Image::set ( const int& x0, const int& y0, const float& v ) {
        assert_type( IT_F_GRAY );
        assert_statement_g( is_inside(x0,y0), "[x0 %d] [y0 %d] oob", x0, y0 );
        m_data_f[ y0*m_w + x0 ] = v;
    }
    void Image::set ( const int& x0, const int& y0, const uchar& v ) {
        assert_type( IT_U_GRAY );
        assert_statement_g( is_inside(x0,y0), "[x0 %d] [y0 %d] oob", x0, y0 );
        m_data_u[ y0*m_w + x0 ] = v;
    }
    void Image::set ( const int& x0, const int& y0, const int  & v ) {
        assert_type( IT_I_GRAY );
        assert_statement_g( is_inside(x0,y0), "[x0 %d] [y0 %d] oob", x0, y0 );
        m_data_i[ y0*m_w + x0 ] = v;
    }
    void Image::set ( const int& x0, const int& y0, const uint16_t  & v ) {
        assert_type( IT_J_GRAY );
        assert_statement_g( is_inside(x0,y0), "[x0 %d] [y0 %d] oob", x0, y0 );
        m_data_u16[ y0*m_w + x0 ] = v;
    }


    /// sets a patch centered at (x,y) with half_width = hsz to v
    void Image::set( const int& x0, const int& y0, const int& hsz, const float& v ) {
        assert_type( IT_F_GRAY );
        for(int y=y0-hsz; y<=y0+hsz; y++) {
            if( y<0 || y>=m_h ) continue;
            float* row_y = get_row_f(y);
            for(int x=x0-hsz; x<=x0+hsz; x++) {
                if( x<0 || x>=m_w ) continue;
                row_y[x] = v;
            }
        }
    }
    /// sets a patch centered at (x,y) with half_width = hsz to v
    void Image::set( const int& x0, const int& y0, const int& hsz, const uchar& v ) {
        assert_type( IT_U_GRAY );
        for(int y=y0-hsz; y<=y0+hsz; y++) {
            if( y<0 || y>=m_h ) continue;
            uchar* row_y = get_row_u(y);
            for(int x=x0-hsz; x<=x0+hsz; x++) {
                if( x<0 || x>=m_w ) continue;
                row_y[x] = v;
            }
        }
    }
    /// sets a patch centered at (x,y) with half_width = hsz to v
    void Image::set( const int& x0, const int& y0, const int& hsz, const int & v ) {
        assert_type( IT_I_GRAY );
        for(int y=y0-hsz; y<=y0+hsz; y++) {
            if( y<0 || y>=m_h ) continue;
            int* row_y = get_row_i(y);
            for(int x=x0-hsz; x<=x0+hsz; x++) {
                if( x<0 || x>=m_w ) continue;
                row_y[x] = v;
            }
        }
    }

    /// sets a patch centered at (x,y) with half_width = hsz to v
    void Image::set( const int& x0, const int& y0, const int& hsz, const uint16_t & v ) {
        assert_type( IT_J_GRAY );
        for(int y=y0-hsz; y<=y0+hsz; y++) {
            if( y<0 || y>=m_h ) continue;
            uint16_t* row_y = get_row_u16(y);
            for(int x=x0-hsz; x<=x0+hsz; x++) {
                if( x<0 || x>=m_w ) continue;
                row_y[x] = v;
            }
        }
    }



    // 3-channel set
    void Image::set( const int& x0, const int& y0, const uchar& r, const uchar& g, const uchar& b ) {
        assert_type( IT_U_PRGB | IT_U_IRGB );
        assert_statement_g( is_inside(x0,y0), "[x %d] [y %d] out of bounds", x0, y0 );
        size_t shft = 0;
        switch( m_channel_type ) {
        case ITC_PIXEL:
            shft = (y0*m_w + x0)*m_ch;
            m_data_u[ shft   ] = r;
            m_data_u[ shft+1 ] = g;
            m_data_u[ shft+2 ] = b;
            break;
        case ITC_IMAGE:
            shft = y0*m_w + x0;
            m_data_u[ shft             ] = r;
            m_data_u[ shft + m_w*m_h   ] = g;
            m_data_u[ shft + m_w*m_h*2 ] = b;
            break;
        default: switch_fatality();
        }
    }
    void Image::set( const int& x0, const int& y0, const float& r, const float& g, const float& b ) {
        assert_type( IT_F_PRGB | IT_F_IRGB );
        assert_statement_g( is_inside(x0,y0), "[x %d] [y %d] out of bounds", x0, y0 );
        size_t shft = 0;
        switch( m_channel_type ) {
        case ITC_PIXEL:
            shft = (y0*m_w + x0)*m_ch;
            m_data_f[ shft   ] = r;
            m_data_f[ shft+1 ] = g;
            m_data_f[ shft+2 ] = b;
            break;
        case ITC_IMAGE:
            shft = y0*m_w + x0;
            m_data_f[ shft             ] = r;
            m_data_f[ shft + m_w*m_h   ] = g;
            m_data_f[ shft + m_w*m_h*2 ] = b;
            break;
        default: switch_fatality();
        }
    }

    void Image::set( const int& x0, const int& y0, const int& hsz, const uchar& r, const uchar& g, const uchar& b ) {
        assert_type( IT_U_PRGB | IT_U_IRGB );
        assert_statement_g( is_inside(x0,y0), "[x %d] [y %d] out of bounds", x0, y0 );
        for( int y=y0-hsz; y<=y0+hsz; y++ ) {
            if( !kortex::is_inside(y,0,m_h) ) continue;
            for( int x=x0-hsz; x<=x0+hsz; x++ ) {
                if( !kortex::is_inside(x,0,m_w) ) continue;
                this->set( x, y, r, g, b );
            }
        }
    }

    void Image::set( const int& x0, const int& y0, const int& hsz, const float& r, const float& g, const float& b ) {
        assert_type( IT_F_PRGB | IT_F_IRGB );
        assert_statement_g( is_inside(x0,y0), "[x %d] [y %d] out of bounds", x0, y0 );
        for( int y=y0-hsz; y<=y0+hsz; y++ ) {
            if( !kortex::is_inside(y,0,m_h) ) continue;
            for( int x=x0-hsz; x<=x0+hsz; x++ ) {
                if( !kortex::is_inside(x,0,m_w) ) continue;
                this->set( x, y, r, g, b );
            }
        }
    }

    //
    //
    //
    void Image::save( const string& file ) const {
        save_image( file.c_str(), this );
    }

    void Image::load( const string& file ) {
        load_image( file.c_str(), this );
    }

    void Image::save( ofstream& fout ) const {
        insert_binary_stream_begin_tag( fout );
        write_bparam( fout, m_w );
        write_bparam( fout, m_h );
        int imt = int( m_type );
        write_bparam( fout, imt );
        write_barray( fout, m_memory.get_buffer(), req_mem( m_w, m_h, m_type ) );
        insert_binary_stream_end_tag( fout );
    }

    void Image::load( ifstream& fin  ) {
        check_binary_stream_begin_tag( fin );
        int w, h, imt;
        read_bparam( fin, w );
        read_bparam( fin, h );
        read_bparam( fin, imt );
        ImageType type = ImageType(imt);
        this->create( w, h, type );
        read_barray( fin, m_memory.get_buffer(), req_mem( w, h, type ) );
        check_binary_stream_end_tag( fin );
    }


    //
    //
    //

    void Image::copy_from_region(const Image* src,
                                 const int& sx0, const int& sy0, const int& rw,  const int& rh,
                                 const int& dx0, const int& dy0) {
        assert_pointer( src );
        assert_noalias_p( this, src );
        passert_statement_g( is_inside(dx0,dy0) && is_inside(dx0+rw-1,dy0+rh-1) ,
                             "out of bounds [ds %d %d] [de %d %d] [wh %d %d]",
                             dx0, dy0, dx0+rw-1,dy0+rh-1, m_w, m_h );
        assert_statement_g( src->is_inside(sx0,sy0), "out of bounds [%d %d]", sx0, sy0 );
        passert_statement( type() == src->type(), "image types should be the same" );

        switch( m_type ) {
        case IT_F_GRAY:
        case IT_F_PRGB:
            for( int y=0; y<rh; y++ ) {
                const float* sptr =  src->get_row_f(sy0+y) + sx0*m_ch;
                float*       dptr = this->get_row_f(dy0+y) + dx0*m_ch;
                memcpy( dptr, sptr, sizeof(*sptr)*rw*m_ch );
            }
            break;
        case IT_U_GRAY:
        case IT_U_PRGB:
            for( int y=0; y<rh; y++ ) {
                const uchar* sptr =  src->get_row_u(sy0+y) + sx0*m_ch;
                uchar*       dptr = this->get_row_u(dy0+y) + dx0*m_ch;
                memcpy( dptr, sptr, sizeof(*sptr)*rw*m_ch );
            }
            break;
        case IT_F_IRGB:
            for( int c=0; c<3; c++ ) {
                for( int y=0; y<rh; y++ ) {
                    const float* sptr =  src->get_row_fi(sy0+y, c) + sx0;
                    float*       dptr = this->get_row_fi(dy0+y, c) + dx0;
                    memcpy( dptr, sptr, sizeof(*sptr)*rw );
                }
            }
            break;
        case IT_U_IRGB:
            for( int c=0; c<3; c++ ) {
                for( int y=0; y<rh; y++ ) {
                    const uchar* sptr =  src->get_row_ui(sy0+y, c) + sx0;
                    uchar*       dptr = this->get_row_ui(dy0+y, c) + dx0;
                    memcpy( dptr, sptr, sizeof(*sptr)*rw );
                }
            }
            break;
        case IT_I_GRAY:
            for( int y=0; y<rh; y++ ) {
                const int* sptr =  src->get_row_i(sy0+y) + sx0*m_ch;
                int*       dptr = this->get_row_i(dy0+y) + dx0*m_ch;
                memcpy( dptr, sptr, sizeof(*sptr)*rw*m_ch );
            }
            break;
        case IT_J_GRAY:
            for( int y=0; y<rh; y++ ) {
                const uint16_t* sptr =  src->get_row_u16(sy0+y) + sx0*m_ch;
                uint16_t*       dptr = this->get_row_u16(dy0+y) + dx0*m_ch;
                memcpy( dptr, sptr, sizeof(*sptr)*rw*m_ch );
            }
            break;
        default: switch_fatality(); break;
        }
    }

    void Image::copy_from_region_safe(const Image* src,
                                      const int& sx0, const int& sy0, const int& rw,  const int& rh,
                                      const int& dx0, const int& dy0) {
        assert_pointer( src );
        assert_noalias_p( this, src );
        passert_statement( type() == src->type(), "image types should be the same" );
        for( int y=0; y<rh; y++ ) {
            for( int x=0; x<rw; x++ ) {
                if( !src->is_inside(sx0+x,sy0+y) )
                    continue;
                if( !this->is_inside(dx0+x,dy0+y) )
                    continue;

                switch( m_type ) {
                case IT_F_GRAY:
                case IT_F_PRGB: {
                    const float* sptr =  src->get_row_f(sy0+y) + (sx0+x)*m_ch;
                    float*       dptr = this->get_row_f(dy0+y) + (dx0+x)*m_ch;
                    memcpy( dptr, sptr, sizeof(*sptr)*m_ch );
                } break;
                case IT_U_GRAY:
                case IT_U_PRGB: {
                    const uchar* sptr =  src->get_row_u(sy0+y) + (sx0+x)*m_ch;
                    uchar*       dptr = this->get_row_u(dy0+y) + (dx0+x)*m_ch;
                    memcpy( dptr, sptr, sizeof(*sptr)*m_ch );
                } break;
                case IT_F_IRGB: {
                    for( int c=0; c<3; c++ ) {
                        const float* sptr =  src->get_row_fi(sy0+y,c) + sx0 + x;
                        float*       dptr = this->get_row_fi(dy0+y,c) + dx0 + x;
                        dptr[0] = sptr[0];
                    }
                } break;
                case IT_U_IRGB: {
                    for( int c=0; c<3; c++ ) {
                        const uchar* sptr =  src->get_row_ui(sy0+y, c) + sx0 + x;
                        uchar*       dptr = this->get_row_ui(dy0+y, c) + dx0 + x;
                        dptr[0] = sptr[0];
                    }
                } break;
                case IT_I_GRAY: {
                    const int* sptr =  src->get_row_i(sy0+y) + (sx0+x)*m_ch;
                    int*       dptr = this->get_row_i(dy0+y) + (dx0+x)*m_ch;
                    memcpy( dptr, sptr, sizeof(*sptr)*m_ch );
                } break;
                case IT_J_GRAY: {
                    const uint16_t* sptr =  src->get_row_u16(sy0+y) + (sx0+x)*m_ch;
                    uint16_t*       dptr = this->get_row_u16(dy0+y) + (dx0+x)*m_ch;
                    memcpy( dptr, sptr, sizeof(*sptr)*m_ch );
                } break;
                default:
                    switch_fatality();
                    break;
                }
            }
        }
    }


    //
    //
    //

    const Image* Image::get_channel_wrapper( int cid ) const {
        passert_type( IT_U_GRAY | IT_U_IRGB | IT_F_GRAY | IT_F_IRGB );
        Image* img = new Image();
        img->m_w = m_w;
        img->m_h = m_h;
        img->m_ch = 1;
        img->m_channel_type = ITC_IMAGE;
        img->m_wrapper = true;
        switch( precision() ) {
        case TYPE_UCHAR:
            img->m_type = IT_U_GRAY;
            img->m_data_u = m_data_u + cid * m_w * m_h;
            img->m_data_f = NULL;
            break;
        case TYPE_FLOAT:
            img->m_type = IT_F_GRAY;
            img->m_data_f = m_data_f + cid * m_w * m_h;
            img->m_data_u = NULL;
            break;
        default: switch_fatality();
        }
        return img;
    }
    Image* Image::get_channel_wrapper( int cid ) {
        passert_type( IT_U_GRAY | IT_U_IRGB | IT_F_GRAY | IT_F_IRGB );
        Image* img = new Image();
        img->m_w = m_w;
        img->m_h = m_h;
        img->m_ch = 1;
        img->m_channel_type = ITC_IMAGE;
        img->m_wrapper = true;
        switch( precision() ) {
        case TYPE_UCHAR:
            img->m_type = IT_U_GRAY;
            img->m_data_u = m_data_u + cid * m_w * m_h;
            img->m_data_f = NULL;
            break;
        case TYPE_FLOAT:
            img->m_type = IT_F_GRAY;
            img->m_data_f = m_data_f + cid * m_w * m_h;
            img->m_data_u = NULL;
            break;
        default: switch_fatality();
        }
        return img;
    }

    float Image::get_grad_x( const int& x0, const int &y0 ) const {
        assert_type( IT_F_GRAY );
        const float* row = get_row_f( y0 );
        if     ( x0 >= m_w-1 ) return 2.0f * (row[m_w-1] - row[m_w-2]);
        else if( x0 <= 0     ) return 2.0f * (row[    1] - row[    0]);
        else                   return        (row[ x0+1] - row[ x0-1]);
    }

    float Image::get_grad_y( const int& x0, const int &y0 ) const {
        assert_type( IT_F_GRAY );
        passert_boundary( x0, 0, m_w );
        const float* col = m_data_f + x0;
        if     ( y0 >= m_h-1 ) return 2.0f * ( col[ (m_h-2)*m_w ] - col[ (m_h-1)*m_w ] );
        else if( y0 <= 0     ) return 2.0f * ( col[           0 ] - col[         m_w ] );
        else                   return        ( col[  (y0-1)*m_w ] - col[  (y0+1)*m_w ] );
    }

    bool Image::is_non_zero( const int& x0, const int& y0, const int& rsz ) const {
        assert_type( IT_F_GRAY );
        for( int y=y0-rsz; y<=y0+rsz; y++ ) {
            if( y<0 || y>=m_h ) continue;
            const float* row = get_row_f(y);
            for( int x=x0-rsz; x<=x0+rsz; x++ ) {
                if( x<0 || x>=m_w ) continue;
                if( row[x] ) return true;
            }
        }
        return false;
    }
    bool Image::does_contain_zero( const int& x0, const int& y0, const int& rsz ) const {
        assert_type( IT_F_GRAY );
        for( int y=y0-rsz; y<=y0+rsz; y++ ) {
            if( y<0 || y>=m_h ) continue;
            const float* row = get_row_f(y);
            for( int x=x0-rsz; x<=x0+rsz; x++ ) {
                if( x<0 || x>=m_w ) continue;
                if( row[x] == 0.0f ) return true;
            }
        }
        return false;
    }




    bool Image::is_maximum( const int& x0, const int& y0, const int& wnd_rad, const float& v0 ) const {
        assert_type( IT_F_GRAY );
        assert_statement( wnd_rad > 0, "window should be nonnegative" );
        for( int y=y0-wnd_rad; y<=y0+wnd_rad; y++ ) {
            if( y<0 || y>=m_h ) continue;
            const float* row = get_row_f(y);
            for( int x=x0-wnd_rad; x<=x0+wnd_rad; x++ ) {
                if( x<0 || x>=m_w ) continue;
                if( row[x] > v0 ) return false;
            }
        }
        return true;
    }
    bool Image::is_maximum( const int& x0, const int& y0, const int& wnd_rad, const int& v0 ) const {
        assert_type( IT_I_GRAY );
        assert_statement( wnd_rad > 0, "window should be nonnegative" );
        for( int y=y0-wnd_rad; y<=y0+wnd_rad; y++ ) {
            if( y<0 || y>=m_h ) continue;
            const int* row = get_row_i(y);
            for( int x=x0-wnd_rad; x<=x0+wnd_rad; x++ ) {
                if( x<0 || x>=m_w ) continue;
                if( row[x] > v0 ) return false;
            }
        }
        return true;
    }


    bool Image::is_minimum( const int& x0, const int& y0, const int& wnd_rad, const float& v0 ) const {
        assert_type( IT_F_GRAY );
        assert_statement( wnd_rad > 0, "window should be nonnegative" );
        for( int y=y0-wnd_rad; y<=y0+wnd_rad; y++ ) {
            if( y<0 || y>=m_h ) continue;
            const float* row = get_row_f(y);
            for( int x=x0-wnd_rad; x<=x0+wnd_rad; x++ ) {
                if( x<0 || x>=m_w ) continue;
                if( row[x] < v0 ) return false;
            }
        }
        return true;
    }
    bool Image::is_minimum( const int& x0, const int& y0, const int& wnd_rad, const int& v0 ) const {
        assert_type( IT_F_GRAY );
        assert_statement( wnd_rad > 0, "window should be nonnegative" );
        for( int y=y0-wnd_rad; y<=y0+wnd_rad; y++ ) {
            if( y<0 || y>=m_h ) continue;
            const int* row = get_row_i(y);
            for( int x=x0-wnd_rad; x<=x0+wnd_rad; x++ ) {
                if( x<0 || x>=m_w ) continue;
                if( row[x] < v0 ) return false;
            }
        }
        return true;
    }

//
//
//
    void extract_region_patch( const Image& img, int x0, int y0, int x1, int y1, Image& patch ) {
        assert_statement_g( x0<x1 && y0<y1, "[sp %d %d] [ep %d %d]", x0, y0, x1, y1 );
        assert_statement_g( x0<img.w() && y0<img.h(), "invalid region [sp %d %d] [ep %d %d]", x0, y1, x1, y1 );
        assert_statement_g( x1>0 && y1>0, "invalid region [sp %d %d] [ep %d %d]", x0, y1, x1, y1 );
        int px = x1-x0;
        int py = y1-y0;
        patch.create( px, py, img.type() );
        patch.zero();

        int sx0=x0, sy0=y0, sx1=x1, sy1=y1;
        int dx =0,  dy = 0;

        if( x0 < 0 ) {
            sx0 = 0;
            dx  = -x0;
        }
        if( y0 < 0 ) {
            sy0 = 0;
            dy  = -y0;
        }
        sx1 = std::min( sx1, img.w() );
        sy1 = std::min( sy1, img.h() );

        patch.copy_from_region( &img, sx0, sy0, sx1-sx0, sy1-sy0, dx, dy );
    }

    void extract_centered_patch( const Image& img, int x0, int y0, int pw, int ph, Image& patch ) {
        int x1 = x0 + pw/2;
        int y1 = y0 + ph/2;
        x0 -= pw/2;
        y0 -= ph/2;
        if( x1-x0 != pw ) x1++;
        if( y1-y0 != ph ) y1++;
        assert_statement_g( (x1-x0 == pw) && (y1-y0 == ph), "[xy0 %d %d pwh %d %d -> x1 y1 %d %d]", x0, y0, pw, ph, x1, y1 );
        extract_region_patch( img, x0, y0, x1, y1, patch );
    }


}
