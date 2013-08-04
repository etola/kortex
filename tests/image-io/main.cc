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

#include <kortex/image.h>
#include <kortex/fileio.h>

using namespace kortex;

void display_help() {
    printf("enter test input folder\n");
}

void image_io_test( const string& test_folder  );

int main(int argc, char **argv) {

    if( argc == 1 ) {
       display_help();
       exit(0);
    }

    image_io_test( argv[1] );

}

void image_io_test( const string& test_folder  ) {

    string of = "test_out/";
    create_folder(of);

    string file =  test_folder + "/test_color0.jpg";
    // string file =  test_folder + "/test_color0.png";
    // string file =  test_folder + "/test_color0.ppm";
    // string file =  test_folder + "/test_gray0.jpg";
    // string file =  test_folder + "/test_gray0.png";
    // string file =  test_folder + "/test_gray0.pgm";

    Image img;
    img.load(file);
    img.save(of+"test_out.jpg");
    img.save(of+"test_out.ppm");
    // img.save(of+"test_out.pgm");
    img.save(of+"test_out.png");


    img.convert( IT_U_IRGB );

    Image* wch = NULL;
    wch = img.get_channel_wrapper( 0 ); wch->save( of+"channel_r.jpg"); delete wch;
    wch = img.get_channel_wrapper( 1 ); wch->save( of+"channel_g.jpg"); delete wch;
    wch = img.get_channel_wrapper( 2 ); wch->save( of+"channel_b.jpg"); delete wch;

    img.convert( IT_U_IRGB );
    const Image* timg = &img;
    const Image* cimg = timg->get_channel_wrapper(0);
    cimg->save(of+"ch_r.pgm");
    delete cimg;


    img.convert( IT_U_PRGB );
    img.save(of+"test_uprgb.jpg");
    img.save(of+"test_uprgb.ppm");
    img.save(of+"test_uprgb.png");

    img.convert( IT_U_IRGB );
    img.save(of+"test_uirgb.jpg");
    img.save(of+"test_uirgb.ppm");
    // img.save(of+"test_uirgb.png");

    img.convert( IT_F_IRGB );
    img.convert( IT_F_PRGB );
    img.convert( IT_U_PRGB );
    img.save(of+"test_fprgb.jpg");
    img.save(of+"test_fprgb.ppm");
    img.save(of+"test_fprgb.png");

    img.convert( IT_F_GRAY );
    img.convert( IT_U_GRAY );
    img.save(of+"test_fgray.jpg");
    img.save(of+"test_fgray.pgm");
    img.save(of+"test_fgray.png");

    img.convert( IT_U_PRGB );
    img.save(of+"test_3gray.jpg");
    img.save(of+"test_3gray.ppm");
    img.save(of+"test_3gray.png");

}


// Local Variables:
// mode: c++
// compile-command: "make -C ."
// End:
