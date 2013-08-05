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
#include <kortex/image_io_jpg.h>
#include <kortex/check.h>

#ifdef WITH_LIBJPEG

#include <kortex/image.h>
#include <setjmp.h>
#include <cstdlib>
extern "C" {
#include "jpeglib.h"
}

namespace kortex {

    void save_jpg(const string& file, const Image* img) {

        img->passert_type( IT_U_GRAY | IT_U_PRGB | IT_U_IRGB, file.c_str() );

        int quality = 100;
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;

        FILE * outfile;		 /* target file */
        JSAMPROW row_pointer[1]; /* pointer to JSAMPLE row[s] */
        int row_stride;		 /* physical row width in image buffer */

        /* Step 1: allocate and initialize JPEG compression object */
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);

        /* Step 2: specify data destination (eg, a file) */
        /* Note: steps 2 and 3 can be done in either order. */

        if( (outfile = fopen(file.c_str(), "wb")) == NULL )
            logman_fatal_g( "cannot open [%s]", file.c_str() );

        jpeg_stdio_dest(&cinfo, outfile);

        /* Step 3: set parameters for compression */
        cinfo.image_width  = img->w();
        cinfo.image_height = img->h();
        cinfo.input_components = img->ch(); /* color components per pixel */

        if( img->ch() == 3 )
            cinfo.in_color_space = JCS_RGB;  /* colorspace of input image */
        else if( img->ch() == 1 )
            cinfo.in_color_space = JCS_GRAYSCALE;  /* colorspace of input image */
        else {
            cinfo.in_color_space = JCS_RGB;  /* colorspace of input image */
        }
        jpeg_set_defaults(&cinfo);

        /* Now you can set any non-default parameters you wish to.
         * Here we just illustrate the use of quality (quantization table) scaling:
         */
        jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

        /* Step 4: Start compressor */
        jpeg_start_compress(&cinfo, TRUE);

        /* Step 5: while (scan lines remain to be written) */
        /*           jpeg_write_scanlines(...); */

        /* Here we use the library's state variable cinfo.next_scanline as the
         * loop counter, so that we don't have to keep track ourselves.
         * To keep things simple, we pass one scanline per call; you can pass
         * more if you wish, though.
         */
        row_stride = img->w()*img->ch(); /* JSAMPLEs per row in image_buffer */

        uchar* tmp_buffer = new uchar[row_stride];

        while (cinfo.next_scanline < cinfo.image_height) {
            /* jpeg_write_scanlines expects an array of pointers to scanlines.
             * Here the array is only one element long, but you could pass
             * more than one scanline at a time if that's more convenient.
             */
            switch( img->type() ) {
            case IT_U_GRAY:
            case IT_U_PRGB: {
                const uchar* body = img->get_row_u( (int)cinfo.next_scanline );
                row_pointer[0] = (uchar*) body;
            } break;
            case IT_U_IRGB: {
                const uchar* sr = img->get_row_ui(cinfo.next_scanline,0);
                const uchar* sg = img->get_row_ui(cinfo.next_scanline,1);
                const uchar* sb = img->get_row_ui(cinfo.next_scanline,2);
                for( int x=0; x<img->w(); x++ ) {
                    tmp_buffer[3*x+0] = sr[x];
                    tmp_buffer[3*x+1] = sg[x];
                    tmp_buffer[3*x+2] = sb[x];
                }
                row_pointer[0] = tmp_buffer;
            } break;
            default: switch_fatality();
            }
            (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }
        delete []tmp_buffer;

        /* Step 6: Finish compression */

        jpeg_finish_compress(&cinfo);
        fclose(outfile);

        /* Step 7: release JPEG compression object */

        jpeg_destroy_compress(&cinfo);

        /* And we're done! */
    }

    struct my_error_mgr {
        struct jpeg_error_mgr pub;	/* "public" fields */
        jmp_buf setjmp_buffer;	/* for return to caller */
    };

    typedef struct my_error_mgr * my_error_ptr;

    METHODDEF(void) my_error_exit (j_common_ptr cinfo) {
        /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
        my_error_ptr myerr = (my_error_ptr) cinfo->err;
        /* Always display the message. */
        /* We could postpone this until after returning, if we chose. */
        (*cinfo->err->output_message) (cinfo);
        /* Return control to the setjmp point */
        longjmp(myerr->setjmp_buffer, 1);
    }

    void read_jpg_size(const string& file, int &w, int &h, int &nc ) {
        struct jpeg_decompress_struct cinfo;
        struct my_error_mgr jerr;
        FILE * infile;		/* source file */
        if( (infile = fopen(file.c_str(), "rb")) == NULL ) {
            fprintf( stderr, "can't open %s\n", file.c_str() );
            return;
        }
        /* Step 1: allocate and initialize JPEG decompression object */
        /* We set up the normal JPEG error routines, then override error_exit. */
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = my_error_exit;
        /* Establish the setjmp return context for my_error_exit to use. */
        if (setjmp(jerr.setjmp_buffer)) {
            /* If we get here, the JPEG code has signaled an error. */
            jpeg_destroy_decompress(&cinfo);
            fclose(infile);
            return;
        }
        /* Now we can initialize the JPEG decompression object. */
        jpeg_create_decompress(&cinfo);
        /* Step 2: specify data source (eg, a file) */
        jpeg_stdio_src(&cinfo, infile);
        /* Step 3: read file parameters with jpeg_read_header() */
        (void) jpeg_read_header(&cinfo, TRUE);
        /* Step 5: Start decompressor */
        // (void) jpeg_start_decompress(&cinfo);
        w  = cinfo.image_width;
        h  = cinfo.image_height;
        nc = cinfo.num_components;
        /* Step 7: Finish decompression */
        // (void) jpeg_finish_decompress(&cinfo);
        /* Step 8: Release JPEG decompression object */
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
    }

    void load_jpg(const string& file, Image* img) {
        passert_pointer( img );

        struct jpeg_decompress_struct cinfo;
        struct my_error_mgr jerr;

        FILE * infile;		/* source file */
        JSAMPARRAY buffer;	/* Output row buffer */
        int row_stride;		/* physical row width in output buffer */

        if( (infile = fopen(file.c_str(), "rb")) == NULL )
            logman_fatal_g("cannot open [%s]", file.c_str());

        /* Step 1: allocate and initialize JPEG decompression object */

        /* We set up the normal JPEG error routines, then override error_exit. */
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = my_error_exit;
        /* Establish the setjmp return context for my_error_exit to use. */
        if (setjmp(jerr.setjmp_buffer)) {
            /* If we get here, the JPEG code has signaled an error. */
            jpeg_destroy_decompress(&cinfo);
            fclose(infile);
            logman_fatal_g("cannot load image [%s]", file.c_str());
        }
        /* Now we can initialize the JPEG decompression object. */
        jpeg_create_decompress(&cinfo);
        /* Step 2: specify data source (eg, a file) */
        jpeg_stdio_src(&cinfo, infile);
        /* Step 3: read file parameters with jpeg_read_header() */
        (void) jpeg_read_header(&cinfo, TRUE);
        /* Step 4: set parameters for decompression */
        /* we don't need to change any of the defaults set by jpeg_read_header(), so
         * we do nothing here.  */
        /* Step 5: Start decompressor */
        (void) jpeg_start_decompress(&cinfo);
        /* We may need to do some setup of our own at this point before reading
         * the data.  After jpeg_start_decompress() we have the correct scaled
         * output image dimensions available, as well as the output colormap if
         * we asked for color quantization.  In this example, we need to make an
         * output work buffer of the right size.
         */
        /* JSAMPLEs per row in output buffer */
        row_stride = cinfo.output_width * cinfo.output_components;

        int  w = cinfo.output_width;
        int  h = cinfo.output_height;
        int ch = cinfo.output_components;

        switch( ch ) {
        case 1: img->create(w, h, IT_U_GRAY); break;
        case 3: img->create(w, h, IT_U_PRGB); break;
        default: logman_fatal_g("invalid channel number [%d]", ch);
        }

        /* Make a one-row-high sample array that will go away when done with image */
        buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

        /* Step 6: while (scan lines remain to be read) */
        /*           jpeg_read_scanlines(...); */
        /* Here we use the library's state variable cinfo.output_scanline as the
         * loop counter, so that we don't have to keep track ourselves.
         */
        while (cinfo.output_scanline < cinfo.output_height) {
            /* jpeg_read_scanlines expects an array of pointers to scanlines.
             * Here the array is only one element long, but you could ask for
             * more than one scanline at a time if that's more convenient.
             */
            (void) jpeg_read_scanlines(&cinfo, buffer, 1);
            /* Assume put_scanline_someplace wants a pointer and sample count. */
            int row_number = cinfo.output_scanline-1;
            // printf("scanline : %d\n",row_number);
            uchar* row = img->get_row_u( row_number );
            switch( ch ) {
            case 1:
                for( int k=0; k<row_stride; k++ )
                    row[k] = buffer[0][k];
                break;
            case 3:
                for( int k=0; k<w; k++ ) {
                    row[3*k+0] = buffer[0][3*k+0];
                    row[3*k+1] = buffer[0][3*k+1];
                    row[3*k+2] = buffer[0][3*k+2];
                }
                break;
            }
        }
        /* Step 7: Finish decompression */
        (void) jpeg_finish_decompress(&cinfo);

        /* Step 8: Release JPEG decompression object */
        jpeg_destroy_decompress(&cinfo);

        fclose(infile);

        /* At this point you may want to check to see whether any corrupt-data
         * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
         */
    }

}

#else

namespace kortex {
    void save_jpg( const string& file, const Image* img ) {
        logman_fatal_g("libjpg is not linked with. [%s]", file.c_str() );
    }

    void load_jpg( const string& file, Image* img ) {
        logman_fatal_g("libjpg is not linked with. [%s]", file.c_str() );
    }
    void read_jpg_size(const string& file, int &w, int &h, int &nc ) {
        logman_fatal_g("libjpg is not linked with. [%s]", file.c_str() );
    }
}

#endif

