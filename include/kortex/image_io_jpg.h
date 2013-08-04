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
#ifndef KORTEX_IMAGE_IO_JPG_H
#define KORTEX_IMAGE_IO_JPG_H

#include <string>
using std::string;

namespace kortex {

    class Image;

    void save_jpg( const string& file, const Image* img );
    void load_jpg( const string& file, Image* img );
    void read_jpg_size(const string& file, int &w, int &h, int &nc );

}

#endif
