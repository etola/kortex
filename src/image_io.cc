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
#include <kortex/image_io.h>
#include <kortex/image.h>
#include <kortex/fileio.h>
#include <kortex/string.h>

#include <kortex/image_io_pnm.h>
#include <kortex/image_io_png.h>
#include <kortex/image_io_jpg.h>

namespace kortex {

    void save_binary( const string& file, const Image* img ) {
        passert_pointer( img );
        ofstream fout;
        open_or_fail( file, fout, true );
        insert_binary_stream_begin_tag( fout );
        write_bparam( fout, img->w() );
        write_bparam( fout, img->h() );
        write_bparam( fout, img->ch() );
        write_bparam( fout, (int)img->type() );
        size_t imsz = img->element_count();
        switch( img->type() ) {
        case IT_U_GRAY: write_barray( fout, img->get_row_u (0  ), imsz ); break;
        case IT_U_PRGB: write_barray( fout, img->get_row_u (0  ), imsz ); break;
        case IT_U_IRGB: write_barray( fout, img->get_row_ui(0,0), imsz ); break;
        case IT_F_GRAY: write_barray( fout, img->get_row_f (0  ), imsz ); break;
        case IT_F_PRGB: write_barray( fout, img->get_row_f (0  ), imsz ); break;
        case IT_F_IRGB: write_barray( fout, img->get_row_fi(0,0), imsz ); break;
        default: switch_fatality();
        }
        insert_binary_stream_end_tag( fout );
        fout.close();
    }

    void load_binary( const string& file, Image* img ) {
        passert_pointer( img );
        ifstream fin;
        open_or_fail( file, fin, true );
        check_binary_stream_begin_tag( fin );
        int w, h, ch, type;
        read_bparam( fin, w );
        read_bparam( fin, h );
        read_bparam( fin, ch );
        read_bparam( fin, type );
        img->create( w, h, get_image_type(type) );
        size_t imsz = img->element_count();
        switch( img->type() ) {
        case IT_U_GRAY: read_barray( fin, img->get_row_u (0  ), imsz ); break;
        case IT_U_PRGB: read_barray( fin, img->get_row_u (0  ), imsz ); break;
        case IT_U_IRGB: read_barray( fin, img->get_row_ui(0,0), imsz ); break;
        case IT_F_GRAY: read_barray( fin, img->get_row_f (0  ), imsz ); break;
        case IT_F_PRGB: read_barray( fin, img->get_row_f (0  ), imsz ); break;
        case IT_F_IRGB: read_barray( fin, img->get_row_fi(0,0), imsz ); break;
        default: switch_fatality();
        }
        check_binary_stream_end_tag( fin );
        fin.close();
    }

    void read_ibin_size( const string& file, int& w, int& h, int& nc ) {
        ifstream fin;
        open_or_fail( file, fin, true );
        check_binary_stream_begin_tag( fin );
        read_bparam( fin, w );
        read_bparam( fin, h );
        read_bparam( fin, nc );
        fin.close();
    }

    void read_image_size( const string& file, int& w, int& h, int& nc ) {
        file_exists_or_fail(file);
        switch( get_file_format(file) ) {
        case FF_PGM :
        case FF_PPM : read_pnm_size ( file, w, h, nc ); break;
        case FF_JPG : read_jpg_size ( file, w, h, nc ); break;
        case FF_PNG : read_png_size ( file, w, h, nc ); break;
        case FF_IBIN: read_ibin_size( file, w, h, nc ); break;
        default     : logman_fatal_g( "unhandled image format [%s]", get_file_extension(file).c_str() );
        }
    }

    void load_image( const string& file, Image* img) {
        file_exists_or_fail(file);
        switch( get_file_format(file) ) {
        case FF_PGM : load_pgm   ( file, img ); break;
        case FF_PPM : load_ppm   ( file, img ); break;
        case FF_JPG : load_jpg   ( file, img ); break;
        case FF_PNG : load_png   ( file, img ); break;
        case FF_IBIN: load_binary( file, img ); break;
        default     : logman_fatal_g( "unhandled image format [%s]", get_file_extension(file).c_str() );
        }
    }

    void save_image( const string& file, const Image* img) {
        switch( get_file_format(file) ) {
        case FF_PGM : save_pgm   ( file, img ); break;
        case FF_PPM : save_ppm   ( file, img ); break;
        case FF_JPG : save_jpg   ( file, img ); break;
        case FF_PNG : save_png   ( file, img ); break;
        case FF_IBIN: save_binary( file, img ); break;
        default: switch_fatality();
        }
    }



}

