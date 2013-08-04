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
#ifndef KORTEX_IMAGE_IO_PNM_H
#define KORTEX_IMAGE_IO_PNM_H

#include <string>
using std::string;

namespace kortex {

    class Image;

    int read_pnm_size( const string& file, int &w, int &h, int &nc );

    void load_pgm(const string& file, Image* img);
    void load_ppm(const string& file, Image* img);

    void save_pgm(const string& file, const Image* img);
    void save_ppm(const string& file, const Image* img);


}

#endif
